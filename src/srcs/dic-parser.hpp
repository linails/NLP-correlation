/*
 * Progarm Name: dic-parser.hpp
 * Created Time: 2016-12-15 22:09:32
 * Last modified: 2017-03-19 23:41:00
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _DIC_PARSER_HPP_
#define _DIC_PARSER_HPP_

#include <string>
#include "disk-dic.hpp"
#include "statistics.hpp"
#include "cfg-loader.hpp"

using std::string;

class DicParser{
public:
    DicParser(string path = "");
    ~DicParser();
    int  dicparser_main(int argc, char **argv);
public:
    static DicParser*get_instance(DicParser *ptr = nullptr);
    DiskDic         *get_disk(void);
    Statistics      *get_statistics(void);
    CfgLoader       *get_xml_cfg(void);
private:
    int  init(int argc, char **argv);
    int  uninit(void);
    int  parser_xhzd(int argc, char **argv);
    int  parser_xdhycd(int argc, char **argv);
    int  parser_cycd(int argc, char **argv);
    int  parser_hycddq(int argc, char **argv);
    int  parser_hytycfyccd(int argc, char **argv);
    int  later_stage_xhzd(int argc, char **argv);
    int  later_stage_xdhycd(int argc, char **argv);
    int  later_stage_cycd(int argc, char **argv);
    int  later_stage_hytycfyccd(int argc, char **argv);
    int  spell_statistic(int argc, char **argv);
    int  later_stage_spell(int argc, char **argv);
    string addr_translate(string path_ori);
private:
    static DicParser *instance;
    string      m_cfg_path;
    string      m_exorigin;
    CfgLoader  *m_xml_cfg   = nullptr;
    DiskDic    *m_disk      = nullptr;
    Statistics *m_stati     = nullptr;
    string      m_db        = "/home/minphone/corpus.db";
};

#endif //_DIC_PARSER_HPP_

