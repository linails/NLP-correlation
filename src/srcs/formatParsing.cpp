/*
 * Progarm Name: formatParsing.cpp
 * Created Time: 2016-05-15 12:14:11
 * Last modified: 2017-03-04 00:32:21
 * @author: minphone.linails linails@foxmail.com 
 */

#include "formatParsing.h"
#include <iostream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include "regex_common.h"
#include "stringTools.h"
#include <cstring>
#include <cassert>
#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include "segmentation.hpp"
#include "dic-parser.hpp"

using std::map;
using std::cout;
using std::endl;

/*format parsing for xinhuazidian*/
void formatTool::formatParsing_xhzd(string &s)
{
    #if 0
    /* 
     * Only For debug
     * */
    auto print_units = [](vector<string> &us){
        cout << "us.size() : " << us.size() << endl;
        for(auto &u : us){
            cout << "us : " << u << endl;
        }
    };
    #endif

    /* get wc.word */
    {
        regex_common_c0x("^[\\w\\W][^`<\\s]+", s, this->m_wc.word);

        stringTools st;
        st.filter("[ *1234567890·]", this->m_wc.word);
    }


    /* get wc.attr */
    {
        vector<string> units;

        regex_common_c0x("`\\d`[\\W\\w][^\\(]*", s, units);
        assert(units.size() == 1);

        regex_common_c0x("`\\d`[\\W\\w][^`<]*", units[0], this->m_wc.attr);

        stringTools st;
        for(size_t i=0; i<this->m_wc.attr.size(); i++){
            st.filter("[ `*1234567890·－]", this->m_wc.attr[i]);
        }
    }


    /* get units */
    {
        vector<string> units;

        vector<string> patterns;
        patterns.push_back("(\\(\\d\\)).+?(?=\\(\\d\\))");
        patterns.push_back("(\\(\\d\\)).+$");
        this->regex_split(patterns, s, units);

        //print_units(units);

        /*
         * 针对 (10) 及以上的条目进行处理
         * */
        {
            auto filter_ = [](vector<string> &units){
                stringTools st;
                for(auto &u : units){
                    vector<string> result;
                    st.split_utf_code(result, u);
                    if("(" == result[result.size()-2]){
                        result.pop_back();
                        result.pop_back();
                    }
                    st.utf_code2string(u, result);
                }
            };

            if(9 <= units.size()){
                vector<string> subunits;

                vector<string> subpatterns;
                subpatterns.push_back("(\\d\\)).+?(?=\\d\\))");
                subpatterns.push_back("(\\d\\)).+$");
                this->regex_split(subpatterns, units[8], subunits);
                {
                    if(false == subunits.empty()){
                        cout << "subunits.size() = " << subunits.size() << endl;
                        filter_(subunits);
                        
                        for(size_t i=0; i<subunits.size(); i++){
                            if(0 == i) units[8] = subunits[i];
                            else       units.push_back(subunits[i]);
                        }
                    }
                }
            }
        }

        vector<string> subunits;
        for(auto iter = units.begin();
                 iter!= units.end(); iter++){

            regex_common_c0x("<br>[^<]+", *iter, subunits);
            if(false == subunits.empty()){
                for(auto iiter = subunits.begin();
                         iiter!= subunits.end(); iiter++){
                    iiter->erase(0, iiter->find_first_not_of("<br>"));
                }
                this->m_wc.contents.push_back(subunits);
            }

            subunits.clear();
        }


        /*
         * filter this->m_wc.contents 
         * */
        {
            stringTools st;
            for(auto &vu : this->m_wc.contents){
                for(auto &u : vu){
                    st.filter("∶ -", u);

                    vector<string> result; 
                    vector<string> remain; 
                    st.split_utf_code(result, u);
                    do{
                        remain.clear();
                        auto end   = find(result.begin(), result.end(), "]");
                        auto begin = find(result.begin(), result.end(), "[");

                        if(begin > end){ end = find(begin, result.end(), "]"); }
                        if((begin != result.end()) && (end != result.end())){
                            for(auto iter = result.begin();
                                     iter!= result.end(); iter++){
                                if((iter < begin) || (iter >end)){
                                    remain.push_back(*iter);
                                }
                            }
                            st.utf_code2string(u, remain);
                        }else{
                            st.utf_code2string(u, result);
                        }

                        result = remain;
                        //print_units(result);
                    }while(remain.end() != find(remain.begin(), remain.end(), "["));
                }
            }
        }

    }
}

/*format parsing for xiandaihanyucidian*/
void formatTool::formatParsing_xdhycd(string &s)
{
    vector<string> alphas;
    vector<string> words;
    DicParser::get_instance()->get_disk()->get_all_alpha(alphas);
    DicParser::get_instance()->get_disk()->get_all_words_ws(words);
    //cout << "alphas : " << alphas << endl;

    {
        stringTools st;
        st.filter("[<]-[>]", s);
        st.filter("﻿", s, 1);

        /* get wc.word 
         * 1> "^.[^*<\\s]+" */
        regex_common_c0x("^.[^*<\\s]+", s, this->m_wc.word);
    }


    /* 
     * get spell 
     * */
    {
        stringTools st;
        string bref(s, 0, s.find("①"));

        st.filter("[ *1234567890·]", bref);
        this->m_wc.spell = bref;

        {
            pair<int, int> pos;
            if((0 != this->unit_in_block_check(pos, this->m_wc.word, bref, "[]")) ||
               (1 == st.utf_count(this->m_wc.word))){
                if(0 == this->block_check(pos, bref, this->m_wc.word)){
                    printf("pos<int, int> = <%d, %d>\n", pos.first, pos.second);
                    this->m_wc.spell = string(bref, pos.second, string::npos);
                }else{
                    cout << "[Warning] Line = " << __LINE__ << endl;
                }
            }
        }


        if(1 < st.utf_count(this->m_wc.word)){
            this->m_wc.spell = string(bref, bref.find("]") + 1, string::npos);
        }

        st.filter("()～〈 《-－∥□（）［[", this->m_wc.spell);
        st.filter("\\\\s\\\\up", this->m_wc.spell, 1);

        string fstring;
        list<string> spell_l; st.split_utf_code(spell_l, this->m_wc.spell);
        for(auto iter = spell_l.begin();
                 iter!= spell_l.end(); iter++){
            auto isexist = find(words.begin(), words.end(), *iter);
            if(isexist != words.end()){
                fstring = *iter;
                //cout << "fstring : " << fstring << endl;
                break;
            }
        }

        if(false == fstring.empty()){
            this->m_wc.spell = string(this->m_wc.spell, 0, this->m_wc.spell.find(fstring));
            st.filter("［[、—", this->m_wc.spell);
            st.filter("[［]-[］]", this->m_wc.spell);
        }
    }

    //cout << "spell : " << this->m_wc.spell << endl;

    /*
     * get wc.attr_xdhycd.second | attr
     * */
    {
        vector<string> units;

        stringTools st(s);
        st.match("[①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮○]-[^①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮○]", units);
        for(auto &u : units){
            st.filter("[①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮○1234567890· ]", u);

            int index = -1;
            if((int)string::npos != (index = u.find(this->m_wc.spell))){
                u = string(u, index + this->m_wc.spell.length(), string::npos);
            }

            st.filter("\\n", u, 1);
            //st.filter("[［]-[］]", u); // !

            st.remove_duplicates(u, "。");

            this->m_wc.attr_xdhycd.push_back(make_tuple("", u));
        }
    }


    /* 
     * get wc.attr_xdhycd.first | get char_ by segmentation
     * */
    for(auto &u : this->m_wc.attr_xdhycd){
    }

}

/*format parsing for chengyucidian*/
void formatTool::formatParsing_cycd(string &s)
{
    /* get wc.word */
    {
        regex_common_c0x("^[\\w\\W][^<\\s]+", s, this->m_wc.word);
        stringTools st;
        st.filter("\"", this->m_wc.word);
    }

    /* get wc.attr */
    {
    }
}

/*format parsing for chengyucidiandaquan*/
void formatTool::formatParsing_hycddq(string &s)
{
}

/*format parsing for hanyutongyicifanyicicidian*/
void formatTool::formatParsing_hytycfyccd(string &s)
{
}

//---------------------------------------------------------

const char *formatTool::property_words[]  = {
    "名",
    "副",
    "数",
    "形",
    "动",
    "介",
    "〈方〉",
    "〈口〉",
    "〈书〉"
};

const char *formatTool::fnameList[] = {
    "cycd.txt",
    "hycddq.txt",
    "hytycfyccd.txt",
    "xdhycd.txt",
    "xhzd.txt"
};

void (formatTool::* formatTool::formatParsingList[])(string &) = {
    &formatTool::formatParsing_cycd,
    &formatTool::formatParsing_hycddq,
    &formatTool::formatParsing_hytycfyccd,
    &formatTool::formatParsing_xdhycd,
    &formatTool::formatParsing_xhzd
};

/* 
 * For jieba dics
 * */
const char *const formatTool::DICT_PATH      = "../src/libs/cppjieba/dict/jieba.dict.utf8";
const char *const formatTool::HMM_PATH       = "../src/libs/cppjieba/dict/hmm_model.utf8";
const char *const formatTool::USER_DICT_PATH = "../src/libs/cppjieba/dict/user.dict.utf8";
const char *const formatTool::IDF_PATH       = "../src/libs/cppjieba/dict/idf.utf8";
const char *const formatTool::STOP_WORD_PATH = "../src/libs/cppjieba/dict/stop_words.utf8";

//Jieba formatTool::jieba(formatTool::DICT_PATH, formatTool::HMM_PATH, formatTool::USER_DICT_PATH);

formatTool::formatTool(std::string fname,std::string line)
    :m_fname(&fname[fname.rfind('/') + 1])
    ,m_line(line)
{
    for(int i = 0; i < (int)(sizeof(fnameList)/sizeof(*fnameList)); i++){
        if(0 == this->m_fname.compare(fnameList[i])){
            (this->*formatParsingList[i])(this->m_line);
            break;
        }
    }
}

formatTool::~formatTool()
{
}

int  formatTool::regex_split(vector<string> &patterns, string &s, vector<string> &units)
{
    int ret = 0;

    char *ptr = new char[s.size() + 1];

    if(nullptr != ptr){

        size_t len = 0;
        vector<string> last;

        regex_common_c0x(patterns[0].c_str(), s, units);
        if(0 != units.size()){

            memcpy(ptr, s.c_str(), s.size());
            ptr[s.size()] = '\0';

            for(auto &u : units) len += u.size();

            if(len < s.size()){
                string sptr = &ptr[len];
                regex_common_c0x(patterns[1].c_str(), sptr, last);

                if(last.size() > 0){
                    units.push_back(last[0]);
                }
            }
        }else{
            units.push_back(s);
        }

        delete [] ptr;
    }else{
        ret = -1;
        cout << "[Error] new failed !" << endl;
    }

    return ret;
}

int  formatTool::get_wordcell(WordCell_t &wc)
{
    wc = this->m_wc; return 0;
}

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
int  formatTool::unit_in_block_check(pair<int, int> &pos, string &unit, string &oristr, const char *spair)
{
    int ret = -1;

    /* init pos */
    pos.first = -1;
    pos.second= -1;

    stringTools st;
    if(2 == st.utf_count(spair)){
        string finder;
        vector<string> spair_utf;
        st.split_utf_code(spair_utf, spair);

        finder += spair_utf[0];
        finder += unit;
        finder += spair_utf[1];

        if((int)string::npos != (pos.first = oristr.find(finder))){
            pos.second= pos.first + finder.size();
            ret = 0;
        }
    }

    return ret;
}

/* 
 * unit : str
 *
 * return eg."strxxxxstr"
 *            ^        ^
 *            |        |
 * Index: < first , last >
 * Failed : pos = <-1, -1>
 * */
int  formatTool::block_check(pair<int, int> &pos, string &oristr, string &unit)
{
    int ret = -1;

    /* init pos */
    pos.first = -1;
    pos.second= -1;

    if((int)string::npos != (pos.first = oristr.find(unit))){
        if(0 == pos.first){
            if((int)string::npos != (pos.second = oristr.find(unit, unit.length()))){
                pos.second += unit.length();
                ret = 0;
            }
        }
    }

    return ret;
}

