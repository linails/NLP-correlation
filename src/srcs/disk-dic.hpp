/*
 * Progarm Name: disk-dic.hpp
 * Created Time: 2017-02-27 15:35:40
 * Last modified: 2017-03-03 15:31:42
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _DISK_DIC_HPP_
#define _DISK_DIC_HPP_

#include "sqlite3.h"
#include <string>
#include <vector>
#include <map>
#include "stringTools.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

typedef struct{
    string word;
    string char_;
    string mean;
    string source;
    string remark;
}WordChars_t;

class DiskDic{
public:
    DiskDic(string db);
    ~DiskDic();
    int  init_tables(void);
    int  insert_alphabet(string alpha, string remark);
    int  insert_alphabet(string alpha);
    int  insert_alphabet(vector<string> &alphas);
    int  get_all_alpha(vector<string> &alphas);
    int  insert_ws_single_full(string word, string spell, string remark);
    int  insert_ws_plural_full(string word, string spell, string remark);
    int  insert_ws_word(string word);
    int  insert_ws_word(vector<string> &v_word);
    int  insert_ws_word_spell(string word, string spell);
    int  insert_ws_word_spell(vector<pair<string, string> > &v_word_spell); // <word,spell>
    int  get_spell(vector<string> &spells, string word);
    int  get_all_spells(vector<string> &spells);
    int  get_word_spell(vector<pair<string, string> > &v_word_spell);
    int  get_words(vector<string> &words, int plural);
    int  get_all_words_ws(vector<string> &words);
    int  insert_wc_full(WordChars_t &wc);
    int  insert_wc_full(vector<WordChars_t> &wcs);
    int  insert_wc_word_mean_source(WordChars_t &wc);
    int  insert_wc_word_mean_source(vector<WordChars_t> &wcs);
    int  get_all_words_wc(vector<string> &words);
private:
    typedef struct{ const char *tag; const char *sql; }SqlOprts_t;
    typedef map<const char *, const char *> SqlMap_t;
    static const char *CorpusTables[];
    static SqlOprts_t CorpusTableOprts[];
private:
    string      m_db;
    sqlite3    *m_conn = nullptr;
    SqlMap_t    m_sqlmap;
    stringTools m_st;
};

#endif //_DISK_DIC_HPP_

