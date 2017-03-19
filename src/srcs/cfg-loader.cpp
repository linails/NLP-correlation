/*
 * Progarm Name: cfg-loader.cpp
 * Created Time: 2017-03-14 18:36:20
 * Last modified: 2017-03-19 23:09:25
 * @author: minphone.linails linails@foxmail.com 
 */

#include "cfg-loader.hpp"
#include <iostream>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <typeinfo>
#include <chrono>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using std::cout;
using std::endl;

using namespace rapidjson;

CfgLoader::CfgLoader(string cfg_path)
{
    if(false == cfg_path.empty()){
        this->m_file = cfg_path;
    }

    /* 
     * update this->m_file 
     * eg. ~/cfg/config.xml => /home/minphone/cfg/config.xml
     * */
    {
        string home = getenv("HOME");

        int pos = -1;
        if((int)string::npos != (pos = this->m_file.find("~"))){
            string tail(this->m_file, pos + strlen("~"), string::npos);
            this->m_file = home + tail;
        }
    }

    this->m_check_loop = new thread(&CfgLoader::cfg_file_check, this);
    if(nullptr == this->m_check_loop){
        cout << "[Error] new failed in xmlParser !" << endl;
    }
}

CfgLoader::~CfgLoader()
{
    cout << "~CfgLoader() ... " << endl;

    this->m_check_loop->join();
    delete this->m_check_loop;
    this->m_check_loop = nullptr;
}

int  CfgLoader::getMaster_ip(string &ip, int &port)
{
    ip   = this->m_xml_master.master_ip;
    if(false == this->m_xml_master.master_port.empty())
        port = atoi(this->m_xml_master.master_port.c_str());
    else
        port = -1;
    return 0;
}

int  CfgLoader::getPlugin_ip(string &ip, int &port)
{
    ip   = this->m_xml_plugin.script_ip;
    if(false == this->m_xml_plugin.script_port.empty())
        port = atoi(this->m_xml_plugin.script_port.c_str());
    else
        port = -1;
    return 0;
}

int  CfgLoader::getPlugin_log_level(int &level)
{
    if(false == this->m_xml_plugin.log_level.empty())
        level = atoi(this->m_xml_plugin.log_level.c_str());
    else
        level = -1;
    return 0;
}

int  CfgLoader::cfg_reader(void)
{
    int ret = 0;

    /* 
     * load xml file
     * */
    TiXmlDocument doc(this->m_file.c_str());
    if(!doc.LoadFile()){
        cout << "xml file load failed !" << endl;
        ret = -1;
    }else{
        cout << "xml file load successed !" << endl;
    }

    /* hdoc 是 doc 指向的对象 */
    TiXmlHandle hDoc(&doc);

    /* 指向元素的指针 */
    TiXmlElement *pElem = hDoc.FirstChildElement().Element(); // 指向根节点

    TiXmlHandle hRoot(pElem);

    /* 
     * read elements
     * */
    {

        auto element = [](
                        string &dst,
                        TiXmlElement *node,
                        string element_name,
                        string root_or_child){
            if( NULL != node ){
                TiXmlHandle h_root(node);

                TiXmlElement *cnode = h_root.FirstChild(element_name.c_str()).Element();
                if( NULL != cnode ){
                    if("root" == root_or_child){
                        const char *p_txt = cnode->GetText();
                        if(NULL != p_txt) dst = p_txt;
                    } else {
                        TiXmlHandle h_cnode(cnode);
                        TiXmlElement *ccnode = h_cnode.FirstChild(root_or_child.c_str()).Element();
                        if( NULL != ccnode ){
                            const char *p_txt = ccnode->GetText();
                            if(NULL != p_txt) dst = p_txt;
                        }
                    }
                }
                return 0;
            }else
                return -1;
        };

        /* 
         * read master Element 
         * */
        {
            TiXmlElement *master = hRoot.FirstChild("master").Element();
            if(NULL != master){
                cout << "find Element <master>" << endl;
            }else{
                cout << "[Error] no element <master>" << endl;
            }

            element(this->m_xml_master.master_ip,         master, "master_ip",           "root");
            element(this->m_xml_master.master_port,       master, "master_port",         "root");
            element(this->m_xml_master.log_path,          master, "log_path",            "root");
            element(this->m_xml_master.plugin_python,     master, "plugin_python",       "root");
            element(this->m_xml_master.db_sqlite,         master, "db-sqlite",           "root");
            element(this->m_xml_master.exorigin_path,     master, "exOrigin-path",       "root");

        }


        /* 
         * read plugin Element 
         * */
        {
            TiXmlElement *plugin = hRoot.FirstChild("script_plugin").Element();
            if(NULL != plugin){
                cout << "find Element <script_plugin>" << endl;
            }else{
                cout << "[Error] no element <script_plugin>" << endl;
            }

            element(this->m_xml_plugin.script_ip,         plugin, "script_ip",       "root");
            element(this->m_xml_plugin.script_port,       plugin, "script_port",     "root");
            element(this->m_xml_plugin.log_path,          plugin, "log_path",        "root");
            element(this->m_xml_plugin.log_level,         plugin, "log_level",       "root");

        }
    }

    /*
     * print info
     * */
    {
        auto print = [](const char *str, string &s){
            printf("%s = %s \n", str, s.c_str());
        };

        print("master_ip", this->m_xml_master.master_ip);
        print("master_port", this->m_xml_master.master_port);
        print("log_path", this->m_xml_master.log_path);
        print("plugin_python", this->m_xml_master.plugin_python);
        print("db_sqlite", this->m_xml_master.db_sqlite);
        print("exorigin_path", this->m_xml_master.exorigin_path);


        print("script_ip", this->m_xml_plugin.script_ip);
        print("script_port", this->m_xml_plugin.script_port);
        print("log_path", this->m_xml_plugin.log_path);
        print("log_level", this->m_xml_plugin.log_level);
    }

    return ret;
}

void CfgLoader::cfg_file_check(void)
{
    struct stat file_stat;

    while(1){
        if(-1 != this->m_killed) break;

        stat(this->m_file.c_str(), &file_stat);

        if(this->m_st_mtime != file_stat.st_mtime){
            cout << "Time : " << file_stat.st_mtime << endl;

            this->cfg_reader();

            this->m_st_mtime = file_stat.st_mtime;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        //cout << "[xmlParser check config-file stat thread] sleep ... " << endl;

    }
}

int  CfgLoader::getMaster_exorigin_path(string &exorigin)
{
    exorigin = this->m_xml_master.exorigin_path; return 0;
}

int  CfgLoader::getMaster_plugin_python(string &plugin_python)
{
    plugin_python = this->m_xml_master.plugin_python; return 0;
}

int  CfgLoader::kill_thread(void){ this->m_killed = 0; return 0; }
int  CfgLoader::getPlugin_log_path(string &log_path){ log_path = this->m_xml_plugin.log_path; return 0; }
int  CfgLoader::getMaster_log_path(string &log_path){ log_path = this->m_xml_master.log_path; return 0; }
int  CfgLoader::getMaster_db_sqlite(string &sqlite){ sqlite = this->m_xml_master.db_sqlite; return 0; }

