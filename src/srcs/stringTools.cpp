/*
 * Progarm Name: stringTools.cpp
 * Created Time: 2016-05-26 19:47:27
 * Last modified: 2017-03-03 23:40:27
 * @author: minphone.linails linails@foxmail.com 
 */

#include "stringTools.h"
#include "regex_common.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <list>
#include "mstring.hpp"

using std::cout;
using std::endl;
using std::list;

stringTools::stringTools(std::string str)
    :m_str(str)
{
}

stringTools::~stringTools()
{
}

/*
 * filter success return 0
 *  pattern 2: {"1hu, [123au], [123]-[123], [<]-[>]"}
 * */
int  stringTools::filter(const char *pattern2)
{
    return this->filter(pattern2, this->m_str);
}

/*
 * filter success return 0
 *  pattern 2: {"1hu, [123au], [123]-[123], [<]-[>]"}
 * */
int stringTools::filter(const char *pattern2, std::string &unit)
{
    int ret = 0;

    /* 
     * For debug print
     * */
    #if 0
    auto fp = [](vector<vector<unsigned char> > &vvch){
        cout << "vvch : ";
        for(auto &vch : vvch){
            string s;
            for(auto ch : vch) s += ch;
            cout << s ;
        }
        cout << endl;
    };

    auto fpl = [](list<vector<unsigned char> > &lvch){
        cout << "lvch : ";
        for(auto &vch : lvch){
            string s;
            for(auto ch : vch) s += ch;
            cout << s ;
        }
        cout << endl;
    };
    #endif

    this->get_subpatterns(pattern2);

    auto filter_01 = [&](string pat, string &unit){
        int ret = 0;
        vector<vector<unsigned char> > vvch_u;
        vector<vector<unsigned char> > vvch_p;

        if(0 == this->count_char(pat.c_str(), '[')){
            ret = this->utf_string2code(vvch_u, unit); assert(-1 != ret);
            ret = this->utf_string2code(vvch_p, pat);  assert(-1 != ret);

            this->mfilter(vvch_u, vvch_p);

        }else{
            ret = this->utf_string2code(vvch_u, unit); assert(-1 != ret);
            ret = this->utf_string2code(vvch_p, pat);  assert(-1 != ret);

            //fp(vvch_p);
            this->mfilter(vvch_p, '[');
            this->mfilter(vvch_p, ']');
            //fp(vvch_p);

            //fp(vvch_u);
            this->mfilter(vvch_u, vvch_p);
            //fp(vvch_u);
        }

        this->utf_code2string(unit, vvch_u);

        return ret;
    };

    auto filter_2 = [&](string pat, string &unit){
        int ret = 0;
        unsigned char tag = '-';

        list<vector<unsigned char> > lvch_u;
        vector<vector<unsigned char> > vvch_p;

        ret = this->utf_string2code(lvch_u, unit); assert(-1 != ret);
        ret = this->utf_string2code(vvch_p, pat);  assert(-1 != ret); // [<]-[>]

        auto check = [&tag](vector<unsigned char> &uv){
            if(1 == uv.size() && uv[0] == tag)  return 1;
            else                                return 0;
        };

        auto iter = find_if(vvch_p.begin(), vvch_p.end(), check);
        /* 
         * exist '-'
         * */
        if(iter != vvch_p.end()){
            vector<vector<unsigned char> > vvch_pf(vvch_p.begin(), iter);
            vector<vector<unsigned char> > vvch_pl(iter+1, vvch_p.end());
            //fp(vvch_pf);
            //fp(vvch_pl);

            this->mfilter(vvch_pf, "[]");
            this->mfilter(vvch_pl, "[]");
            //fp(vvch_pf);
            //fp(vvch_pl);

            int flag = -1;

            for(auto iter = lvch_u.begin(); iter!= lvch_u.end(); ){

                /* 
                 * find first/last : '<'-'>'
                 * */
                auto find_fl = [&](vector<vector<unsigned char> > &vvch_){
                    int ret = 0;

                    auto incheck = [&](vector<unsigned char> &uv){
                        if(*iter == uv)  return 1;
                        else             return 0;
                    };

                    auto iiter = find_if(vvch_.begin(), vvch_.end(), incheck);
                    if(iiter == vvch_.end()) ret = -1;

                    return ret;
                };

                /* 
                 * flag
                 *  -1 : To find first
                 *   0 :
                 *   1 : To find last 
                 * */
                if(-1 == flag){
                    if(0 == (flag = find_fl(vvch_pf))){
                        iter = lvch_u.erase(iter);
                    }else
                        iter++;
                }else if(0 == flag){
                    iter = lvch_u.erase(iter);
                    flag = 1;
                }else if(1 == flag){
                    if(0 == find_fl(vvch_pl)){
                        iter = lvch_u.erase(iter);
                        flag = -1;
                    }else
                        iter = lvch_u.erase(iter);
                }else{
                    iter++;
                }
            }

            //fpl(lvch_u);

            this->utf_code2string(unit, lvch_u);

        }else
            ret = -1;

        return ret;
    };

    for(auto iter = this->m_subpatterns.begin();
             iter!= this->m_subpatterns.end(); iter++){
        //cout << "subpattern : " << *iter << endl;

        /*
         * 0 ... 1 : 1hu, [123au], 
         * 2 :       [123]-[123], [<]-[>]
         * 3 :       {"<[>-<]>"} : filter
         * */
        switch(this->count_char((*iter).c_str(), '[')){
            case 0 ... 1:
                //cout << "[ : 0 ... 1" << endl;
                ret = filter_01(*iter, unit);
                break;
            case 2:
                //cout << "[ : " << 2 << endl;
                ret = filter_2(*iter, unit);
                break;
        }

    }

    return ret;
}

/* 
 *  
 * */
int  stringTools::filter(const char *pattern, string &unit, int mode) // default mode = 0 ; = 1 complete
{
    int ret = 0;


    /* 
     * pattern2
     * */
    if(0 == mode){ 
        ret = this->filter(pattern, unit);


    /* 
     * pattern 5: {"[string]"} : 过滤 "[string]" 这种结构内容, 完全匹配
     * */
    }else{
        int pos = -1;
        do{
            if((int)string::npos != (pos = unit.find(pattern))){
                if(0 != pos){
                    string head(unit, 0, pos);
                    string end(unit, pos + strlen(pattern), string::npos);

                    unit = head + end;
                }else{
                    unit = string(unit, pos + strlen(pattern), string::npos);
                }
            }
        }while(-1 != pos);
    }

    return ret;
}

/*
 * filter success return 0
 *  pattern 2: {"1hu, [123au], [123]-[123], [<]-[>]"}
 * */
int stringTools::filter(const char *pattern2, char *unit)
{
    int     ret = 0;
    string  sunit(unit);

    memset(unit, '\0', sunit.size());
    
    if(0 == (ret = this->filter(pattern2, sunit))){
        #if 1
        for(auto iter = sunit.begin();
                 iter!= sunit.end(); iter++){
            *unit++ = *iter;
        }
        #else
        memcpy(unit, sunit.c_str(), sunit.size());
        #endif
    }

    return ret;
}

void stringTools::mfilter(vector<vector<unsigned char> > &vvch, unsigned char tag)
{
    auto check = [&tag](vector<unsigned char> &uv){
        if(1 == uv.size() && uv[0] == tag)  return 1;
        else                                return 0;
    };

    #if 0
    auto iter = find_if(vvch.begin(), vvch.end(), check);
    if(iter != vvch.end()){
        vvch.erase(iter);
    }
    #else
    list<vector<unsigned char> > lvch;
    auto assign_l = [&lvch](vector<unsigned char> &vc){ lvch.push_back(vc);};
    auto assign_v = [&vvch](vector<unsigned char> &vc){ vvch.push_back(vc);};

    for_each(vvch.begin(), vvch.end(), assign_l);

    for(auto iter = lvch.begin(); iter!= lvch.end(); ){
        if(0 != check(*iter)) {
            string s;
            for(auto ch : *iter) s+=ch;
            iter = lvch.erase(iter);
        }else
            iter++;
    }

    vvch.clear();
    for_each(lvch.begin(), lvch.end(), assign_v);
    #endif
}

void stringTools::mfilter(vector<vector<unsigned char> > &vvch, vector<unsigned char> &tag)
{
    auto check = [&tag](vector<unsigned char> &uv){
        if(tag == uv)   return 1;
        else            return 0;
    };

    #if 0
    auto iter = find_if(vvch.begin(), vvch.end(), check);
    if(iter != vvch.end()){
        vvch.erase(iter);
    }
    #else

    list<vector<unsigned char> > lvch;
    auto assign_l = [&lvch](vector<unsigned char> &vc){ lvch.push_back(vc);};
    auto assign_v = [&vvch](vector<unsigned char> &vc){ vvch.push_back(vc);};

    for_each(vvch.begin(), vvch.end(), assign_l);

    for(auto iter = lvch.begin(); iter!= lvch.end(); ){
        if(0 != check(*iter)) {
            string s;
            for(auto ch : *iter) s+=ch;
            iter = lvch.erase(iter);
        }else
            iter++;
    }

    vvch.clear();
    for_each(lvch.begin(), lvch.end(), assign_v);

    #endif
}

void stringTools::mfilter(vector<vector<unsigned char> > &vvch, vector<vector<unsigned char> > &tag)
{
    for(auto &vch : tag) this->mfilter(vvch, vch);
}

void stringTools::mfilter(vector<vector<unsigned char> > &vvch, const char *tag)
{
    vector<vector<unsigned char> > vtag;
    this->utf_string2code(vtag, tag);

    this->mfilter(vvch, vtag);
}

int stringTools::match(const char *pattern, string &unit, int index)
{
    int ret = 0;

    return ret;
}

int stringTools::match(const char *pattern, char *unit, int index)
{
    int ret = 0;

    return ret;
}

int stringTools::match(const char *pattern, vector<string> &units)
{
    int ret = 0;

    int mode = this->get_pattern_mode(pattern);

    auto match_mode3 = [this](const char *pattern, vector<string> &units){
        const char * pair_l = pattern;
        const char * pair_r = pattern; while(*pair_r++ != ']');

        string start(pair_l++ + 1, pair_r++ - 1); while(*pair_l++ != '['); while(*pair_r++ != ']');
        string end;

        if(0 == this->count_char(pattern, '^')){
            string str(pair_l - 1 + 1, pair_r - 1); end = str;
        }else{
            string str(pair_l - 1 + 2, pair_r - 1); end = str;
        }

        //cout << "start : " << start << " - size : " << start.size() << endl;
        //cout << "end : " << end << " - size : " << end.size() << endl;


        /* 
         * split utf code of start & end & this->m_str
         * */
        vector<string> vstart;
        vector<string> vend;
        vector<string> vstr;

        this->split_utf_code(vstart, start);
        this->split_utf_code(vend, end);
        this->split_utf_code(vstr, this->m_str);


        /*
         * get units
         * */

        auto check = [](vector<string> &chk, string &u){
            for(auto &c : chk) if(c == u) return 1; return 0;
        };

        auto check_start = [&vstart, &check](string &u){ return check(vstart, u); };
        auto check_end   = [&vend,   &check](string &u){ return check(vend, u); };

        for(auto iter = vstr.begin();
                 iter!= vstr.end(); iter++){

            /* get start index */
            auto iterfs = find_if(iter, vstr.end(), check_start);
            if(iterfs != vstr.end()){
                /* get end index */
                auto iterfe = find_if(iterfs+1, vstr.end(), check_end);

                vector<string> vtmp(iterfs, iterfe);
                string result;
                for(auto &vs : vtmp){ result += vs; }
                units.push_back(result);

                if(iterfe != vstr.end()){
                    iter = --iterfe;
                }else{
                    break;
                }
            }else{
                vector<string> vtmp(iter, iterfs);
                string result;
                for(auto &vs : vtmp){ result += vs; }
                units.push_back(result);

                break;
            }

        }
    };

    switch(mode){
        case 1 :
            cout << "mode : " << mode << endl;
            break;
        case 2 :
            cout << "mode : " << mode << endl;
            break;
        case 3 : //pattern 3: {'[①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮]-[^①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮]'} : match
            match_mode3(pattern, units);
            break;
        case 4 : //pattern 4: {"[a),b),c)]-[a),b),c)]"} : match, 多字符间隔
            cout << "mode : " << mode << endl;
            break;
        case 6 : //pattern 6: {"[<]-[>], [[]-[]$]"} : match
            cout << "mode : " << mode << endl;
            break;
        default:
            cout << "mode : " << mode << endl;
            break;
    }

    return ret;
}

int  stringTools::find(const char *pattern, string &unit, size_t &begin, size_t &end) // default mode = 0 ; = 1 complete
{
    int ret = 0;

    return ret;
}

/* 
 * mode : [0/bin | 1/hex]
 * */
int  stringTools::print_utf_code(string &s, int mode)
{
    int ret = 0;

    auto print_utf_v = [](vector<unsigned char> &vch, int mode){
        string s;

        if(0 == mode) cout << "0b : ";
        else          cout << "0x : ";
        

        for(auto ch : vch){
            if(0 == mode){
                for(int i=0; i<8; i++){
                    if(ch & (0x01 << (7-i))) cout << "1";
                    else                     cout << "0";
                }
            }else{
                printf("%.2x", ch);
            }
            cout << "-";
            s.push_back(ch);
        }
        cout << s << endl;
    };

    vector<vector<unsigned char> > vvch;

    ret = this->utf_string2code(vvch, s);

    for(auto &u : vvch) print_utf_v(u, mode);

    return ret;
}

int  stringTools::print_utf_code(const char *str, int mode)
{
    int ret = 0;

    string s(str);
    ret = this->print_utf_code(s, mode);

    return ret;
}

int  stringTools::split_utf_code(vector<string> &result, string s)
{
    int ret = 0;

    vector<vector<unsigned char> > vvch;

    if(true == s.empty()){
        ret = this->utf_string2code(vvch, this->m_str);
    }else{
        ret = this->utf_string2code(vvch, s);
    }

    string st;
    for(auto &u : vvch){
        st.clear();
        for(auto ch : u) st.push_back(ch);
        result.push_back(st);
    }

    return ret;
}

int  stringTools::split_utf_code(list<string> &result, string s)
{
    int ret = 0;

    vector<vector<unsigned char> > vvch;

    if(true == s.empty()){
        ret = this->utf_string2code(vvch, this->m_str);
    }else{
        ret = this->utf_string2code(vvch, s);
    }

    string st;
    for(auto &u : vvch){
        st.clear();
        for(auto ch : u) st.push_back(ch);
        result.push_back(st);
    }

    return ret;
}

int  stringTools::utf_code2string(string &s, vector<string> utf_code)
{
    s.clear(); for(auto &u : utf_code) s += u; return 0;
}

int  stringTools::utf_code2string(string &s, list<string> utf_code)
{
    s.clear(); for(auto &u : utf_code) s += u; return 0;
}

int  stringTools::utf_count(string &s)
{
    vector<string> result;

    this->split_utf_code(result, s);

    return result.size();
}

int  stringTools::utf_count(const char *str)
{
    string s(str);
    return this->utf_count(s);
}

/* 
 * remove = A           | remove = B  |  remove = NULL
 * "AABBB" -> "ABBB"    |  -> "AAB"   |  -> "AB"
 * */
int  stringTools::remove_duplicates(string &s, const char *remove)
{
    int     fpos    = -1;
    int     spos    = -1;
    int     ret     = -1;
    int     index   = 0;


    /* 
     * remove != null
     * */
    if(NULL != remove){
        do{
            if((int)string::npos != (fpos = s.find(remove, index))){
                index = fpos + strlen(remove);
                if((int)string::npos != (spos = s.find(remove, index))){
                    if((int)(strlen(remove)) == (spos - fpos)){
                        cout << "find duplicates ! - " << remove << endl;
                        string head(s, 0, spos);
                        string end(s, spos+strlen(remove), string::npos);

                        s = head;
                        ret = 0;
                        if(true == end.empty()) break;
                        else{
                            while((int)string::npos != (spos = end.find(remove))){
                                if(0 == spos){
                                    end = string(end, strlen(remove), string::npos);
                                }else
                                    break;
                            }
                            s += end;
                        }
                    }
                }else{
                    break;
                }
            }else
                break;
        }while(1);


    /*
     * remove = null
     * */
    }else{
        list<string> utf_list;
        this->split_utf_code(utf_list, s);

        auto begin = utf_list.begin();
        auto end   = begin; end++;

        do{
            if(end != utf_list.end()){
                if(*begin == *end){
                    end = utf_list.erase(end);
                }else{
                    begin = end;
                    end++;
                }
            }else
                break;
        }while(1);

        this->utf_code2string(s, utf_list);
    }

    return ret;
}

/* 
 * eg .
 *  "()"-> complete pair
 *  "(" -> incomplete pair
 * pairs = "()[]{}"
 * result : pair<pair, pos> -> pair<"(", 12>
 * */
int  stringTools::incomplete_pair_check(vector<pair<string, int> > &result, string &s, const char *pairs)
{
    return 0;
}

/* 
 * eg .
 *  "()"-> complete pair
 *  "(" -> incomplete pair
 * pairs = "()[]{}"
 * */
int  stringTools::incomplete_pair_del(string &s, const char *pairs)
{
#if 0
    int     fpos    = -1;
    int     spos    = -1;
    int     index   = 0;
    vector<string>                utf_char;
    vector<pair<string, string> > utf_pairs;

    if(nullptr != pairs)    this->split_utf_code(utf_char, pairs);
    else                    this->split_utf_code(utf_char, this->m_pairs);

    for(int i=0; i<utf_char.size()/2; i++){
        utf_pairs.push_back(make_pair(utf_char[0+i], utf_char[1+i]));
    }

    for(auto &u : utf_pairs){
        do{
            if(string::npos != (fpos = s.find(u.first, index))){
                if(string::npos != (spos = s.find(u.second, fpos))){
                }else{
                }
            }else{
            }
        }while(1);
    }
#endif

    return 0;
}

/*success return pattern mode >= 0
 * mode :
 *    0) fixed string
 *    1) []
 *    2) fixed string & []
 *    3) []-[]
 * */
int stringTools::get_pattern_mode(const char *pattern)
{
    int ret = 0;

    string s(pattern);

    if(0 == s.size()){
        ret = -1;
    }else{

        switch(this->count_char(pattern, '[')){
            case 1:
                if(s[0] == '[' && s[s.size()] == ']'){
                    ret = 1;
                }else if(string::npos != s.find('[') && string::npos != s.find(']')){
                        ret = 2;
                }
                break;
            case 2:
                //cout << "count_char '[' : " << 2 << endl;
                ret = 3;
                break;
        }
    }

    return ret;
}

/*
 * get sub patterns success return 0
 *  pattern 2: {"1hu, [123au], [123]-[123], [<]-[>]"}
 * */
int stringTools::get_subpatterns(const char *pattern)
{
    int ret = 0;

    const char *ptr = pattern;
    string subpattern;

    this->m_subpatterns.clear();

    while(*ptr != '\0'){
        if(',' == *ptr){
            this->m_subpatterns.push_back(subpattern);

            if(' ' == *++ptr) ptr++;

            subpattern.clear();
        }else{
            char ch = *ptr++;
            subpattern += ch;
        }
    }
    /* 
     * push_back last subpattern
     * */
    this->m_subpatterns.push_back(subpattern);

    return ret;
}

int  stringTools::count_char(const char *str, char ch)
{
    int ret = 0;
    #if 0
    const char *index = str;

    while(*index != '\0'){
        if(*index++ == ch){
            if(index - str < 2){
                ret++;
            }else if(*(index - 2) != '\\'){
                ret++;
            }
        }
    }
    #else
    while(*str != '\0') if(*str++ == ch) ret++;
    #endif

    return ret;
}

int  stringTools::utf_string2code(vector<vector<unsigned char> > &vvch, string &s)
{
    int ret = 0;

    /* 
     * 编码格式为 utf-8
     *
     * utf-8 是 unicode 的实现方式之一
     * utf-8 最大的一个特点，就是它是一种变长的编码方式，它可以使用 1~6 字节来
     * 表示一个符号，根据不同的符号而变化字节长度
     *
     * 规则：(只有两条)
     *  1> 对于单字节的符号，字节的第一位设为0，后面7位为这个符号的 unicode 码，
     *     因此对于英语字母，utf-8 与 ascii 相同
     *  2> 对于 n 字节的符号(n>1), 第一个字节的前 n 位都设为1，第 n+1 位设为0，
     *     后面字节的前两位一律设置为 10，剩下的没有提及的二进制位，即为这个符号
     *     的 unicode 编码
     *
     *  eg .
     *    1 byte : 0xxxxxxx
     *    2 byte : 110xxxxx 10xxxxxx
     *    3 byte : 1110xxxx 10xxxxxx 10xxxxxx
     *    4 byte : 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
     *    5 byte : 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
     *    6 byte : 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
     * */

    vector<unsigned char> vch;

    for(auto ch : s){

        unsigned char c = ch;
        /* 
         *    1 byte : 0xxxxxxx
         *    2 byte : 110xxxxx 10xxxxxx
         * */

        if(0 == (c & 0x80)){            // ascii

            if(true != vch.empty()) vvch.push_back(vch);

            vch.clear();
            vch.push_back(c);

        }else if(0xc0 == (c & 0xc0)){   // first byte of 2-6 bytes

            if(true != vch.empty()) vvch.push_back(vch);

            vch.clear();
            vch.push_back(c);

        }else{
            vch.push_back(c);
        }

    }

    vvch.push_back(vch);

    return ret;
}

int  stringTools::utf_string2code(vector<vector<unsigned char> > &vvch, const char *str)
{
    string s(str);
    return this->utf_string2code(vvch, s);
}

int  stringTools::utf_string2code(list<vector<unsigned char> > &lvch, string &s)
{
    int ret = 0;

    vector<unsigned char> vch;

    for(auto ch : s){

        unsigned char c = ch;
        /* 
         *    1 byte : 0xxxxxxx
         *    2 byte : 110xxxxx 10xxxxxx
         * */

        if(0 == (c & 0x80)){            // ascii

            if(true != vch.empty()) lvch.push_back(vch);

            vch.clear();
            vch.push_back(c);

        }else if(0xc0 == (c & 0xc0)){   // first byte of 2-6 bytes

            if(true != vch.empty()) lvch.push_back(vch);

            vch.clear();
            vch.push_back(c);

        }else{
            vch.push_back(c);
        }

    }

    lvch.push_back(vch);

    return ret;
}

int  stringTools::utf_string2code(list<vector<unsigned char> > &lvch, const char *str)
{
    string s(str);
    return this->utf_string2code(lvch, s);
}

int  stringTools::utf_code2string(string &s, const list<vector<unsigned char> > &lvch)
{
    s.clear();

    for(auto &vch : lvch){
        string result;
        for(auto ch : vch) result += ch;
        s += result;
    }

    return 0;
}

int  stringTools::utf_code2string(string &s, const vector<vector<unsigned char> > &vvch)
{
    s.clear();

    for(auto &vch : vvch){
        string result;
        for(auto ch : vch) result += ch;
        s += result;
    }

    return 0;
}

