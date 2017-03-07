/*
 * Progarm Name: stringTools.h
 * Created Time: 2016-05-26 19:47:33
 * Last modified: 2017-03-03 23:40:01
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _STRINGTOOLS_H_
#define _STRINGTOOLS_H_

#include <string>
#include <vector>
#include <list>
#include <map>

using std::string;
using std::vector;
using std::list;
using std::pair;

class stringTools{
public:
    stringTools(string str = "");
    ~stringTools();

    /*
     * filter/match success return 0
     *  pattern 2: {"1hu, [123au], [123]-[123], [<]-[>]"} : filter
     *  pattern 3: {"<[>-<]>"} : filter
     *  pattern 3: {'[①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮]-[^①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮]'} : match
     *  pattern 4: {"[a),b),c)]-[a),b),c)]"} : match, 多字符间隔
     *  pattern 5: {"[string]"} : 过滤 "[string]" 这种结构内容, 完全匹配 : filter
     *  pattern 6: {"[<]-[>], [[]-[]$]"} : match
     * */
    int  filter(const char *pattern2);
    int  filter(const char *pattern2, string &unit);
    int  filter(const char *pattern2, char *unit);
    int  filter(const char *pattern, string &unit, int mode); // default mode = 0 ; = 1 complete
    int  match(const char *pattern, string &unit, int index=0);
    int  match(const char *pattern, char *unit, int index=0);
    int  match(const char *pattern, vector<string> &units);
    int  find(const char *pattern, string &unit, size_t &begin, size_t &end); // default mode = 0 ; = 1 complete
    int  print_utf_code(string &s, int mode = 0);   // mode : [0/bin | 1/hex]
    int  print_utf_code(const char *str, int mode = 0);
    int  split_utf_code(vector<string> &result, string s = "");
    int  split_utf_code(list<string> &result, string s = "");
    int  utf_code2string(string &s, vector<string> utf_code);
    int  utf_code2string(string &s, list<string> utf_code);
    int  utf_count(string &s);
    int  utf_count(const char *str);
    /* 
     * remove = A           | remove = B  |  remove = NULL
     * "AABBB" -> "ABBB"    |  -> "AAB"   |  -> "AB"
     * */
    int  remove_duplicates(string &s, const char *remove = nullptr);
    /* 
     * eg .
     *  "()"-> complete pair
     *  "(" -> incomplete pair
     * pairs = "()[]{}"
     * result : pair<pair, pos> -> pair<"(", 12>
     * */
    int  incomplete_pair_check(vector<pair<string, int> > &result, string &s, const char *pairs = nullptr);
    int  incomplete_pair_del(string &s, const char *pairs = nullptr);
private:
    int  get_pattern_mode(const char *pattern);
    int  get_subpatterns(const char *pattern);
    int  count_char(const char *str, char ch);
    int  utf_string2code(vector<vector<unsigned char> > &vvch, string &s);
    int  utf_string2code(vector<vector<unsigned char> > &vvch, const char *str);
    int  utf_string2code(list<vector<unsigned char> > &lvch, string &s);
    int  utf_string2code(list<vector<unsigned char> > &lvch, const char *str);
    int  utf_code2string(string &s, const list<vector<unsigned char> > &lvch);
    int  utf_code2string(string &s, const vector<vector<unsigned char> > &vvch);
    void mfilter(vector<vector<unsigned char> > &vvch, unsigned char tag);
    void mfilter(vector<vector<unsigned char> > &vvch, const char *tag);
    void mfilter(vector<vector<unsigned char> > &vvch, vector<unsigned char> &tag);
    void mfilter(vector<vector<unsigned char> > &vvch, vector<vector<unsigned char> > &tag);
private:
    string              m_str;
    string              m_pattern;
    vector<string>      m_subpatterns;
    string              m_pairs = "()[]{}<>（）";
};

#endif //_STRINGTOOLS_H_

