#include "application.h"
#include "daemon.h"
#include "log.h"
#include "env.h"
#include "config.h"

namespace qiu{

static qiu::Logger::ptr g_logger = QIU_LOG_NAME("system");

static qiu::ConfigVar<std::string>::ptr g_server_work_path = 
    qiu::Config::Lookup("server.work_path", std::string("/apps/work/qiu"), "server work path");

static qiu::ConfigVar<std::string>::ptr g_server_pid_file = 
    qiu::Config::Lookup("server.pid_file", std::string("qiu.pid"), "server pid file");


struct HttpServerConf{
    std::vector<std::string> address;
    int keepalive = 0;
    int timeout = 1000 * 2 * 60;
    std::string name;
    
    bool operator==(const HttpServerConf& v) const{
        return address == v.address
        && keepalive == v.keepalive
        && timeout == v.timeout
        && name == v.name;
    }
    bool isValid() const{
        return !address.empty();
    }
};

template<>
class LexicalCast<std::string, HttpServerConf>{
public:
    HttpServerConf operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        HttpServerConf conf;
        conf.keepalive = node["keepalive"].as<int>(conf.keepalive);
        conf.timeout = node["timeout"].as<int>(conf.timeout);
        conf.name = node["name"].as<std::string>(conf.name);
        if(node["address"].IsDefined()){
            for(size_t i = 0 ; i < node["address"].size() ; ++i){
                conf.address.push_back(node["address"][i].as<std::string>());
            }
        }
        return conf;
    }
};

template<>
class LexicalCast<HttpServerConf, std::string>{
public:
    std::string operator()(const HttpServerConf& conf){
        YAML::Node node;
        node["keepalive"] = conf.keepalive;
        node["timeout"] = conf.timeout;
        node["name"] = conf.name;
        for(auto& i : conf.address){
            node["address"].push_back(i);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

static qiu::ConfigVar<std::vector<HttpServerConf>>::ptr g_http_servers_conf =
    qiu::Config::Lookup("http_servers", std::vector<HttpServerConf>(), "http server config");

Application* Application::s_instance = nullptr;

Application::Application(){
    s_instance = this;
}

bool Application::init(int argc, char** argv){
    m_argc = argc;
    m_argv = argv;

    qiu::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    qiu::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    qiu::EnvMgr::GetInstance()->addHelp("c", "conf path default: ./conf");
    qiu::EnvMgr::GetInstance()->addHelp("p", "print help");

    if(!qiu::EnvMgr::GetInstance()->init(argc, argv)){
        qiu::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    if(qiu::EnvMgr::GetInstance()->has("p")){
        qiu::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    int run_type = 0;
    if(qiu::EnvMgr::GetInstance()->has("s")){
        run_type = 1;
    }
    if(qiu::EnvMgr::GetInstance()->has("d")){
        run_type = 2;
    }

    if(run_type == 0){
        qiu::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    std::string pidfile = g_server_work_path->getValue() 
                          + "/" + g_server_pid_file->getValue();
    if(qiu::FSUtil::IsRunningPidfile(pidfile)){
        QIU_LOG_ERROR(g_logger) << "server is running:" << pidfile;
        return false;
    }

    std::string conf_path = qiu::EnvMgr::GetInstance()->getAbsolutePath(
        qiu::EnvMgr::GetInstance()->get("c", "conf"));
    QIU_LOG_INFO(g_logger) << "load conf path:" << conf_path;
    qiu::Config::LoadFromConfDir(conf_path);

    if(!qiu::FSUtil::Mkdir(g_server_work_path->getValue())){
        QIU_LOG_FATAL(g_logger) << "create work path [" << g_server_work_path->getValue()
        << "] errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }

    return true;
}

bool Application::run(){
    bool is_daemon = qiu::EnvMgr::GetInstance()->has("d");
    return start_daemon(m_argc, m_argv, 
    std::bind(&Application::main, this, std::placeholders::_1, 
    std::placeholders::_2), is_daemon);
}

int Application::main(int argc, char** argv){
    std::string pidfile = g_server_work_path->getValue() 
                          + "/" + g_server_pid_file->getValue();
    std::ofstream ofs(pidfile);
    if(!ofs){
        QIU_LOG_ERROR(g_logger) << "open pidfile " << pidfile << " failed";
        return false;
    }
    ofs << getpid();
    
    
    return 0;
}
}