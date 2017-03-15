/*
 * Progarm Name: cfg-loader.hpp
 * Created Time: 2017-03-14 18:36:24
 * Last modified: 2017-03-15 09:58:58
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _CFG_LOADER_HPP_
#define _CFG_LOADER_HPP_

#include <string>
#include <vector>
#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"
#include <thread>
#include <sys/stat.h>
#include <time.h>

using std::string;
using std::vector;
using std::thread;

typedef struct{
    string  master_ip;
    string  master_port;
    string  log_path;
    string  plugin_python;
    string  db_sqlite;
    string  exorigin_path;
}xmlMaster_t;

typedef struct{
    string  script_ip;
    string  script_port;
    string  log_path;
    string  log_level;
}xmlPlugin_t;

class CfgLoader{
public:
    CfgLoader(string cfg_path = "");
    ~CfgLoader();
    int  getMaster_ip(string &ip, int &port);
    int  getMaster_log_path(string &log_path);
    int  getMaster_plugin_python(string &plugin_python);
    int  getMaster_db_sqlite(string &sqlite);
    int  getMaster_exorigin_path(string &exorigin);
    int  getPlugin_ip(string &ip, int &port);
    int  getPlugin_log_path(string &log_path);
    int  getPlugin_log_level(int &level);
    int  kill_thread(void);
private:
    int  cfg_reader(void);
    void cfg_file_check(void);
private:
    int              m_killed     = -1;
    string           m_file       = "~/cfg/config.xml";
    thread          *m_check_loop = nullptr;
    time_t           m_st_mtime   = 0;
    xmlMaster_t      m_xml_master;
    xmlPlugin_t      m_xml_plugin;
};

#endif //_CFG_LOADER_HPP_

