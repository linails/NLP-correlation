/*
 * Progarm Name: disk-dic.cpp
 * Created Time: 2017-02-27 15:35:36
 * Last modified: 2017-03-05 21:56:46
 * @author: minphone.linails linails@foxmail.com 
 */

#include "disk-dic.hpp"
#include <iostream>
#include "sqlite3.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

/*
 * Tables
 *
 * Note :
 *  01 word     : 词
 *  02 spell    : 拼音
 *  03 remark   : 备注
 *  04 source   : 出处
 *  05 mean     : 意义
 *  06 char     : 词性
 *  07 plural   : 单复数，即字数
 *  08 synonym  : 同义词
 *  09 antonym  : 反义词
 *  10 idiom    : 成语、习语
 *  11 eg       : 示例
 *  12 use      : 用法
 *  13 story    : 故事
 *  14 sp       : 语句片段/说法片段
 *  F0 noise_sign         : 噪声标记
 *  F1 allegorical_saying : 歇后语
 *  F2 lantern_riddle     : 灯谜面
 * */
const char *DiskDic::CorpusTables[] = {

/* 
 * 拼音字母表
 * */
"create table CorpusAlphabet(\
    id integer primary key unique,\
    alpha text unique,\
    remark text)",

/* 
 * word - spell 
 * */
"create table CorpusWordSpell(\
    id integer primary key unique,\
    word text,\
    spell text,\
    remark text,\
    plural integer default 0,\
    unique(word,spell))",

/*
 * word - mean
 * */
"create table CorpusWordMeans(\
    id integer primary key unique,\
    word text,\
    mean text,\
    source text,\
    remark text,\
    unique(word,mean))",

/* 
 * word - characteristic
 * */
"create table CorpusWordChars(\
    id integer primary key unique,\
    word text,\
    char text,\
    mean text,\
    source text,\
    remark text,\
    unique(word,char))",

/* 
 * 拼音组成字母统计表
 * */
"create table CorpusSpellStatis(\
    id integer primary key unique,\
    word text unique,\
    count integer,\
    noise_sign integer default 0)",

/* 
 * 同义词
 * */
"create table CorpusWordSynonym(\
    id integer primary key unique,\
    word text,\
    synonym text,\
    unique(word, synonym))",

/*
 * 反义词
 * */
"create table CorpusWordAntonym(\
    id integer primary key unique,\
    word text,\
    antonym text,\
    unique(word, antonym))",

/* 
 * 成语 - 意义
 * */
"create table CorpusIdiomsMean(\
    id integer primary key unique,\
    idiom text unique,\
    mean text,\
    source text,\
    eg text,\
    allegorical_saying text,\
    lantern_riddle text,\
    use text,\
    story text)",

"create table CorpusIdiomsSynonym(\
    id integer primary key unique,\
    idiom text,\
    synonym text,\
    unique(idiom, synonym))",

"create table CorpusIdiomsAntonym(\
    id integer primary key unique,\
    idiom text,\
    antonym text,\
    unique(idiom, antonym))",

/*
 * statement-part
 * */
"create table CorpusStatementPart(\
    id integer primary key unique,\
    sp text unique,\
    remark text)"
};

DiskDic::SqlOprts_t DiskDic::CorpusTableOprts[] = {
{"full|insert-AB",                      "insert into CorpusAlphabet values('%s','%s')"},
{"part|insert-AB-alpha",                "insert into CorpusAlphabet(alpha) values('%s')"},
{"part|insert-AB-alpha|bulk",           "insert into CorpusAlphabet(alpha) values(?)"},
{"update-AB-remark",                    "update CorpusAlphabet set remark='%s' where alpha='%s'"},
{"select-AB|full-all",                  "select * from CorpusAlphabet"},
{"full|insert-WS|single",               "insert into CorpusWordSpell values('%s','%s','%s')"},
{"full|insert-WS|plural",               "insert into CorpusWordSpell values('%s','%s','%s','%d')"},
{"full|insert-WS|bulk",                 "insert into CorpusWordSpell values(?,?,?,?)"},
{"part|insert-WS-word",                 "insert into CorpusWordSpell(word) values('%s')"},
{"part|insert-WS-words",                "insert into CorpusWordSpell(word, plural) values('%s',1)"},
{"part|insert-WS-word/spell",           "insert into CorpusWordSpell(word, spell) values('%s','%s')"},
{"part|insert-WS-words/spell",          "insert into CorpusWordSpell(word, spell, plural) values('%s','%s','%d')"},
{"part|insert-WS-words/spell|bulk",     "insert into CorpusWordSpell(word, spell, plural) values(?,?,?)"},
{"part|insert-WS-words/plural|bulk",    "insert into CorpusWordSpell(word, plural) values(?,?)"},
{"update-WS-spell",                     "update CorpusWordSpell set spell='%s' where word='%s'"},
{"update-WS-remark",                    "update CorpusWordSpell set remark='%s' where word='%s'"},
{"update-WS-plural",                    "update CorpusWordSpell set plural='%d' where word='%s'"},
{"select-WS|full-all",                  "select * from CorpusWordSpell"},
{"select-WS|full-by(id)",               "select * from CorpusWordSpell where id='%d'"},
{"select-WS|full-by(word)",             "select * from CorpusWordSpell where word='%s'"}, // 可能返回多个 - 同音
{"select-WS-spell",                     "select spell from CorpusWordSpell where word='%s'"},
{"select-WS-spells|full",               "select spell from CorpusWordSpell"},
{"select-WS-words|full",                "select word from CorpusWordSpell"},
{"select-WS-remark",                    "select remark from CorpusWordSpell where word='%s'"},
{"select-WS-plural",                    "select plural from CorpusWordSpell where word='%s'"},
{"select-WS-word|by(plural)",           "select word from CorpusWordSpell where plural='%d'"},
{"full|insert-WM",                      "insert into CorpusWordMeans values('%s','%s','%s','%s')"},
{"full|insert-WM|bulk",                 "insert into CorpusWordMeans values(?,?,?,?)"},
{"part|insert-WM-word",                 "insert into CorpusWordMeans(word) values('%s')"},
{"part|insert-WM-word/mean",            "insert into CorpusWordMeans(word, mean) values('%s','%s')"},
{"part|insert-WM-word/mean/source",     "insert into CorpusWordMeans(word, mean, source) values('%s','%s','%s')"},
{"part|insert-WM-word/mean/source|bulk","insert into CorpusWordMeans(word, mean, source) values(?,?,?)"},
{"part|insert-WM-word/remark",          "insert into CorpusWordMeans(word, remark) values('%s','%s')"},
{"update-WM-mean",                      "update CorpusWordMeans set mean='%s' where word='%s'"},
{"update-WM-source",                    "update CorpusWordMeans set source='%s' where word='%s'"},
{"update-WM-remark",                    "update CorpusWordMeans set remark='%s' where word='%s'"},
{"select-WM|full-by(id)",               "select * from CorpusWordMeans where id='%d'"},
{"select-WM|full-by(word)",             "select * from CorpusWordMeans where word='%s'"}, // 可能返回多个 - 多义
{"select-WM-mean",                      "select mean from CorpusWordMeans where word='%s'"},
{"select-WM-source",                    "select source from CorpusWordMeans where word='%s'"},
{"select-WM-remark",                    "select remark from CorpusWordMeans where word='%s'"},
{"full|insert-WC",                      "insert CorpusWordChars into values('%s','%s','%s','%s','%s')"},
{"full|insert-WC|bulk",                 "insert CorpusWordChars into values(?,?,?,?,?)"},
{"part|insert-WC-word",                 "insert into CorpusWordChars(word) values('%s')"},
{"part|insert-WC-word/char",            "insert into CorpusWordChars(word, char) values('%s','%s')"},
{"part|insert-WC-word/mean",            "insert into CorpusWordChars(word, mean) values('%s','%s')"},
{"part|insert-WC-word/mean/source",     "insert into CorpusWordChars(word, mean, source) values('%s','%s','%s')"},
{"part|insert-WC-word/remark",          "insert into CorpusWordChars(word, remark) values('%s','%s')"},
{"part|insert-WC-word/mean/source|bulk","insert into CorpusWordChars(word, mean, source) values(?,?,?)"},
{"part|insert-WC-word/char|bulk",       "insert into CorpusWordChars(word, char) values(?,?)"},
{"part|insert-WC-word/mean|bulk",       "insert into CorpusWordChars(word, mean) values(?,?)"},
{"update-WC-char",                      "update CorpusWordChars set char='%s' where word='%s'"},
{"update-WC-mean",                      "update CorpusWordChars set mean='%s' where word='%s'"},
{"update-WC-source",                    "update CorpusWordChars set source='%s' where word='%s'"},
{"update-WC-remark",                    "update CorpusWordChars set remark='%s' where word='%s'"},
{"select-WC|full-by(id)",               "select * from CorpusWordChars where id='%d'"},
{"select-WC|full-by(word)",             "select * from CorpusWordChars where word='%s'"}, // 可能返回多个 - 多义
{"select-WC|full-by(char)",             "select * from CorpusWordChars where char='%s'"}, // 可能返回多个
{"select-WC-word|full",                 "select word from CorpusWordChars"},
{"select-WC-char",                      "select char from CorpusWordChars where word='%s'"},
{"select-WC-mean",                      "select mean from CorpusWordChars where word='%s'"},
{"select-WC-source",                    "select source from CorpusWordChars where word='%s'"},
{"select-WC-remark",                    "select remark from CorpusWordChars where word='%s'"},
};

DiskDic::DiskDic(string db)
    :m_db(db)
{
    /*1.open database*/
    int result = sqlite3_open(m_db.c_str(), &this->m_conn);
    if(result != SQLITE_OK){
        sqlite3_close(this->m_conn);
        cout << "[Error] open db failed" << endl;
        return;
    }else{
        cout << "open succeed !" << endl;
    }

    for(size_t i=0; i<sizeof(CorpusTableOprts)/sizeof(*CorpusTableOprts); i++){
        this->m_sqlmap[CorpusTableOprts[i].tag] = CorpusTableOprts[i].sql;
    }
}

DiskDic::~DiskDic()
{
    if(nullptr != this->m_conn){
        sqlite3_close(this->m_conn);
    }
}

int  DiskDic::init_tables(void)
{
    int ret = 0;
    stringTools st;

    for(size_t i=0; i<sizeof(CorpusTables)/sizeof(*CorpusTables); i++){
        sqlite3_stmt *stmt = NULL;
        string sql(CorpusTables[i]);

        st.remove_duplicates(sql, " ");

        if(SQLITE_OK != sqlite3_prepare_v2(this->m_conn,
                                           #if 0
                                           CorpusTables[i],
                                           strlen(CorpusTables[i]),
                                           #else
                                           sql.c_str(),
                                           sql.length(),
                                           #endif
                                           &stmt,
                                           NULL)){
            if(stmt){
                sqlite3_finalize(stmt);
            }
            cout << "[Error] Create Table failed : " << sql << endl;
            ret = -1;
            continue;
        }

        if(SQLITE_DONE != sqlite3_step(stmt)){
            sqlite3_finalize(stmt);
            cout << "[Error] step failed " << endl;
            ret = -1;
            continue;
        }

        if(stmt){
            sqlite3_finalize(stmt);
            cout << "Create Table Successed : " << sql << endl;
        }
    }

    return ret;
}

int  DiskDic::insert_alphabet(string alpha, string remark)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql, this->m_sqlmap["full|insert-AB"], alpha.c_str(), remark.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_alphabet(string alpha)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql, this->m_sqlmap["part|insert-AB-alpha"], alpha.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_alphabet(vector<string> &alphas)
{
    sqlite3_stmt *stmt  = NULL;
    string beginSQL     = "begin transaction";
    string commitSQL    = "commit";

    /*
     * begin transaction
     * */
    if(sqlite3_prepare_v2(this->m_conn,
                          beginSQL.c_str(),
                          beginSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);


    /* 
     * insert data
     * */
    auto bulk_init = [&stmt, this](){
        stmt = NULL;
        if(sqlite3_prepare_v2(this->m_conn, 
                              this->m_sqlmap["part|insert-AB-alpha|bulk"], 
                              strlen(this->m_sqlmap["part|insert-AB-alpha|bulk"]),
                              &stmt,
                              NULL) != SQLITE_OK){
            if(stmt) sqlite3_finalize(stmt);
            return -1;
        }else
            return 0;
    };

    if(0 != bulk_init()) return -1;

    for(auto &alpha : alphas){

        sqlite3_bind_text(stmt, 1, alpha.c_str(),  alpha.size(), SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            sqlite3_finalize(stmt);
            if(0 != bulk_init()) return -1;
            cout << "[Error] sqlite-step failed !" << endl;
            continue;
        }

        sqlite3_reset(stmt);
    }
    if(stmt) sqlite3_finalize(stmt);


    /*
     * commit
     * */
    stmt = NULL;
    if(sqlite3_prepare_v2(this->m_conn, 
                          commitSQL.c_str(),
                          commitSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_all_alpha(vector<string> &alphas)
{
    sqlite3_stmt *stmt  = NULL;

    if(sqlite3_prepare_v2(this->m_conn,
                          this->m_sqlmap["select-AB|full-all"],
                          strlen(this->m_sqlmap["select-AB|full-all"]),
                          &stmt, NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    alphas.clear();

    do{
        int ret = sqlite3_step(stmt);

        if(SQLITE_ROW == ret){
            /* 
             * buf[0], [1], [2], [3] ...
             * */
            //int id = sqlite3_column_int(stmt, 0); cout << "id = " << id << endl;
            string alpha((const char *)sqlite3_column_text(stmt, 1));
            alphas.push_back(alpha);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }

    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_ws_single_full(string word, string spell, string remark)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql,
            this->m_sqlmap["full|insert-WS|single"],
            word.c_str(),
            spell.c_str(),
            remark.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_ws_plural_full(string word, string spell, string remark)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql,
            this->m_sqlmap["full|insert-WS|plural"],
            word.c_str(),
            spell.c_str(),
            remark.c_str(),
            this->m_st.utf_count(word));

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_ws_word(string word)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql,
            this->m_sqlmap["part|insert-WS-word"],
            word.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_ws_word(vector<string> &v_word)
{
    sqlite3_stmt *stmt  = NULL;
    string beginSQL     = "begin transaction";
    string commitSQL    = "commit";

    /*
     * begin transaction
     * */
    if(sqlite3_prepare_v2(this->m_conn,
                          beginSQL.c_str(),
                          beginSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);


    /* 
     * insert data
     * */
    auto bulk_init = [&stmt, this](){
        stmt = NULL;
        if(sqlite3_prepare_v2(this->m_conn, 
                              this->m_sqlmap["part|insert-WS-words/plural|bulk"], 
                              strlen(this->m_sqlmap["part|insert-WS-words/plural|bulk"]),
                              &stmt,
                              NULL) != SQLITE_OK){
            if(stmt) sqlite3_finalize(stmt);
            return -1;
        }else
            return 0;
    };

    if(0 != bulk_init()) return -1;

    for(auto &word : v_word){
        int plural = this->m_st.utf_count(word);
        sqlite3_bind_text(stmt, 1, word.c_str(),  word.size(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt,  2, plural);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            sqlite3_finalize(stmt);
            if(0 != bulk_init()) return -1;
            cout << "[Error] sqlite-step failed !" << endl;
            continue;
        }

        sqlite3_reset(stmt);
    }
    if(stmt) sqlite3_finalize(stmt);


    /*
     * commit
     * */
    stmt = NULL;
    if(sqlite3_prepare_v2(this->m_conn, 
                          commitSQL.c_str(),
                          commitSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_ws_word_spell(string word, string spell)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };
    int  count         = 0;

    if(1 < (count = this->m_st.utf_count(word))){
        sprintf(sql, this->m_sqlmap["part|insert-WS-words/spell"], word.c_str(), spell.c_str(), count);
    }else{
        sprintf(sql, this->m_sqlmap["part|insert-WS-word/spell"], word.c_str(), spell.c_str());
    }

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

/*
 * <word,spell>
 * */
int  DiskDic::insert_ws_word_spell(vector<pair<string, string> > &v_word_spell)
{
    sqlite3_stmt *stmt  = NULL;
    string beginSQL     = "begin transaction";
    string commitSQL    = "commit";

    /*
     * begin transaction
     * */
    if(sqlite3_prepare_v2(this->m_conn,
                          beginSQL.c_str(),
                          beginSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    if(stmt) sqlite3_finalize(stmt);


    /* 
     * insert data
     * */
    auto bulk_init = [&stmt, this](){
        stmt = NULL;
        if(sqlite3_prepare_v2(this->m_conn, 
                              this->m_sqlmap["part|insert-WS-words/spell|bulk"], 
                              strlen(this->m_sqlmap["part|insert-WS-words/spell|bulk"]),
                              &stmt,
                              NULL) != SQLITE_OK){
            if(stmt) sqlite3_finalize(stmt);
            return -1;
        }else
            return 0;
    };

    if(0 != bulk_init()) return -1;

    for(auto &ws : v_word_spell){
        int plural = this->m_st.utf_count(ws.first);

        sqlite3_bind_text(stmt, 1, ws.first.c_str(),  ws.first.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, ws.second.c_str(), ws.second.size(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt,  3, plural);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            sqlite3_finalize(stmt);
            if(0 != bulk_init()) return -1;
            cout << "[Error] sqlite-step failed !" << endl;
            continue;
        }

        sqlite3_reset(stmt);
    }

    if(stmt) sqlite3_finalize(stmt);


    /*
     * commit
     * */
    stmt = NULL;
    if(sqlite3_prepare_v2(this->m_conn, 
                          commitSQL.c_str(),
                          commitSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_spell(vector<string> &spells, string word)
{
    sqlite3_stmt *stmt  = NULL;
    char sql[1024]      = {0, };
    sprintf(sql, this->m_sqlmap["select-WS-spell"], word.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    spells.clear();

    do{
        int ret = sqlite3_step(stmt);
        if(SQLITE_ROW == ret){
            string spell;
            const char *ps = (const char *)sqlite3_column_text(stmt, 0);
            if(NULL != ps) spell = ps;

            spells.push_back(spell);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }
    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_word_spell(vector<pair<string, string> > &v_word_spell)
{
    sqlite3_stmt *stmt  = NULL;

    if(sqlite3_prepare_v2(this->m_conn,
                          this->m_sqlmap["select-WS|full-all"],
                          strlen(this->m_sqlmap["select-WS|full-all"]),
                          &stmt, NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    v_word_spell.clear();

    do{
        int ret = sqlite3_step(stmt);

        if(SQLITE_ROW == ret){
            /* 
             * buf[0], [1], [2], [3] ...
             * */
            //int id = sqlite3_column_int(stmt, 0); cout << "id = " << id << endl;
            string word((const char *)sqlite3_column_text(stmt, 1));

            string spell;
            const char *ps = (const char *)sqlite3_column_text(stmt, 2);
            if(NULL != ps) spell = ps;

            v_word_spell.push_back(make_pair(word, spell));
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }

    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_words(vector<string> &words, int plural)
{
    sqlite3_stmt *stmt  = NULL;
    char sql[1024]      = {0, };
    sprintf(sql, this->m_sqlmap["select-WS-word|by(plural)"], plural);

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    do{
        int ret = sqlite3_step(stmt);
        if(SQLITE_ROW == ret){
            string word= (const char *)sqlite3_column_text(stmt, 0);
            words.push_back(word);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }
    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_all_spells(vector<string> &spells)
{
    sqlite3_stmt *stmt  = NULL;

    if(sqlite3_prepare_v2(this->m_conn,
                          this->m_sqlmap["select-WS-spells|full"],
                          strlen(this->m_sqlmap["select-WS-spells|full"]),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    spells.clear();

    do{
        int ret = sqlite3_step(stmt);
        if(SQLITE_ROW == ret){
            string spell;
            const char *ps = (const char *)sqlite3_column_text(stmt, 0);
            if(NULL != ps) spell = ps;

            spells.push_back(spell);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }
    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_all_words_ws(vector<string> &words)
{
    sqlite3_stmt *stmt  = NULL;

    if(sqlite3_prepare_v2(this->m_conn,
                          this->m_sqlmap["select-WS-words|full"],
                          strlen(this->m_sqlmap["select-WS-words|full"]),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    words.clear();

    do{
        int ret = sqlite3_step(stmt);
        if(SQLITE_ROW == ret){
            string word= (const char *)sqlite3_column_text(stmt, 0);
            words.push_back(word);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }
    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_wc_full(WordChars_t &wc)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql,
            this->m_sqlmap["full|insert-WC"],
            wc.word.c_str(),
            wc.char_.c_str(),
            wc.mean.c_str(),
            wc.source.c_str(),
            wc.remark.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_wc_full(vector<WordChars_t> &wcs)
{
    sqlite3_stmt *stmt  = NULL;
    string beginSQL     = "begin transaction";
    string commitSQL    = "commit";

    /*
     * begin transaction
     * */
    if(sqlite3_prepare_v2(this->m_conn,
                          beginSQL.c_str(),
                          beginSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);


    /* 
     * insert data
     * */
    auto bulk_init = [&stmt, this](){
        stmt = NULL;
        if(sqlite3_prepare_v2(this->m_conn, 
                              this->m_sqlmap["full|insert-WC|bulk"], 
                              strlen(this->m_sqlmap["full|insert-WC|bulk"]),
                              &stmt,
                              NULL) != SQLITE_OK){
            if(stmt) sqlite3_finalize(stmt);
            return -1;
        }else
            return 0;
    };

    if(0 != bulk_init()) return -1;

    for(auto &wc : wcs){

        sqlite3_bind_text(stmt, 1, wc.word.c_str(),  wc.word.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, wc.char_.c_str(),  wc.char_.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, wc.mean.c_str(),  wc.mean.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, wc.source.c_str(),  wc.source.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, wc.remark.c_str(),  wc.remark.size(), SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            sqlite3_finalize(stmt);
            if(0 != bulk_init()) return -1;
            cout << "[Error] sqlite-step failed !" << endl;
            continue;
        }

        sqlite3_reset(stmt);
    }
    if(stmt) sqlite3_finalize(stmt);


    /*
     * commit
     * */
    stmt = NULL;
    if(sqlite3_prepare_v2(this->m_conn, 
                          commitSQL.c_str(),
                          commitSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_wc_word_mean_source(WordChars_t &wc)
{
    sqlite3_stmt *stmt = NULL;
    char sql[1024]     = {0, };

    sprintf(sql,
            this->m_sqlmap["part|insert-WC-word/mean/source"],
            wc.word.c_str(),
            wc.mean.c_str(),
            wc.source.c_str());

    if(sqlite3_prepare_v2(this->m_conn,
                          sql,
                          strlen(sql),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }

    /* 
     * release stmt
     * */
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::insert_wc_word_mean_source(vector<WordChars_t> &wcs)
{
    sqlite3_stmt *stmt  = NULL;
    string beginSQL     = "begin transaction";
    string commitSQL    = "commit";

    /*
     * begin transaction
     * */
    if(sqlite3_prepare_v2(this->m_conn,
                          beginSQL.c_str(),
                          beginSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);


    /* 
     * insert data
     * */
    auto bulk_init = [&stmt, this](){
        stmt = NULL;
        if(sqlite3_prepare_v2(this->m_conn, 
                              this->m_sqlmap["part|insert-WC-word/mean/source|bulk"], 
                              strlen(this->m_sqlmap["part|insert-WC-word/mean/source|bulk"]),
                              &stmt,
                              NULL) != SQLITE_OK){
            if(stmt) sqlite3_finalize(stmt);
            return -1;
        }else
            return 0;
    };

    if(0 != bulk_init()) return -1;

    for(auto &wc : wcs){

        sqlite3_bind_text(stmt, 1, wc.word.c_str(),  wc.word.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, wc.mean.c_str(),  wc.mean.size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, wc.source.c_str(),  wc.source.size(), SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) != SQLITE_DONE){
            sqlite3_finalize(stmt);
            if(0 != bulk_init()) return -1;
            cout << "[Error] sqlite-step failed !" << endl;
            continue;
        }

        sqlite3_reset(stmt);
    }
    if(stmt) sqlite3_finalize(stmt);


    /*
     * commit
     * */
    stmt = NULL;
    if(sqlite3_prepare_v2(this->m_conn, 
                          commitSQL.c_str(),
                          commitSQL.size(),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

int  DiskDic::get_all_words_wc(vector<string> &words)
{
    sqlite3_stmt *stmt  = NULL;

    if(sqlite3_prepare_v2(this->m_conn,
                          this->m_sqlmap["select-WC-word|full"],
                          strlen(this->m_sqlmap["select-WC-word|full"]),
                          &stmt,
                          NULL) != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return -1;
    }

    words.clear();

    do{
        int ret = sqlite3_step(stmt);
        if(SQLITE_ROW == ret){
            string word= (const char *)sqlite3_column_text(stmt, 0);
            words.push_back(word);
        }else if(SQLITE_DONE == ret){
            break;
        }else{
            cout << "[Error] select failed" << endl;
            sqlite3_finalize(stmt);
            return -1;
        }
    }while(1);

    if(stmt) sqlite3_finalize(stmt);

    return 0;
}

