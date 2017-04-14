/*
 * Progarm Name: cfg-loader.hpp
 * Created Time: 2017-03-14 18:36:24
 * Last modified: 2017-04-13 11:06:03
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
#include <map>
#include <sstream>

using std::string;
using std::vector;
using std::thread;
using std::map;
using std::make_pair;
using std::stringstream;

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
    int  register_RootChild(string root, string child);
    int  getRoot_child(string &xml, string root, string child);
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
    typedef map<string, map<string, string> > xmlRootChild_t; // map< root, map<child, val> >
    int              m_killed     = -1;
    string           m_file       = "~/cfg/config.xml";
    thread          *m_check_loop = nullptr;
    time_t           m_st_mtime   = 0;
    xmlMaster_t      m_xml_master;
    xmlPlugin_t      m_xml_plugin;
    xmlRootChild_t   m_xml_root_child;
    stringstream     m_ssbuf;
};

#endif //_CFG_LOADER_HPP_

