/*
 * Progarm Name: formatParsing.h
 * Created Time: 2016-05-15 12:14:15
 * Last modified: 2017-03-03 14:59:39
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FORMATPARSING_H_
#define _FORMATPARSING_H_

#include <string>
#include <vector>
#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include <map>
#include <tuple>

using std::string;
using std::vector;
using namespace cppjieba;
using std::pair;
using std::tuple;

typedef vector<vector<string> > ContentsType_t; 

/* 
 * tuple<char_, attr> -> eg. pair<动, xxxxx>
 *                       eg. pair<形, xxxxx>
 * */
typedef vector<tuple<string, string>> AttrXdhycd_t;

typedef struct WordCell{
    string             word;       //亶
    string             spell;      //dan
    vector<string>     attr;
    ContentsType_t     contents;   //(形声。本义:谷多) , 同本义 [full of grains] ... 
    AttrXdhycd_t       attr_xdhycd;
}WordCell_t;

class formatTool{
public:
    formatTool(string fname, string line);
    ~formatTool();
    int  get_wordcell(WordCell_t &wc);
private:
    void formatParsing_xhzd(string &s);      /*for 新华字典*/
    void formatParsing_xdhycd(string &s);    /*for 现代汉语词典*/
    void formatParsing_cycd(string &s);      /*for 成语词典*/
    void formatParsing_hycddq(string &s);    /*for 汉语词典大全*/
    void formatParsing_hytycfyccd(string &s);/*for 汉语同义词反义词词典*/
    int  regex_split(vector<string> &patterns, string &s, vector<string> &units);

    /*
     * pair eg. "[]" / "()"
     *
     * return eg."[unit]"
     *            ^    ^
     *            |    |
     * Index: < first,last >
     *
     * Failed : pos = <-1, -1>
     * */
    int  unit_in_block_check(pair<int, int> &pos, string &unit, string &oristr, const char *spair);

    /* 
     * unit : str
     *
     * return eg."strxxxxstr"
     *             ^      ^
     *             |      |
     * Index: < first , last >
     * Failed : pos = <-1, -1>
     * */
    int  block_check(pair<int, int> &pos, string &oristr, string &unit);
private:
    static const char *const DICT_PATH      ;
    static const char *const HMM_PATH       ;
    static const char *const USER_DICT_PATH ;
    static const char *const IDF_PATH       ;
    static const char *const STOP_WORD_PATH ;
    static const char *fnameList[];
    static const char *property_words[];
    static void (formatTool::*formatParsingList[])(string &);
    string      m_fname;
    string      m_line;
    WordCell_t  m_wc;
    //static Jieba jieba;
};

#endif //_FORMATPARSING_H_

