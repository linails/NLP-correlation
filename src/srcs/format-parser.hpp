/*
 * Progarm Name: format-parser.hpp
 * Created Time: 2016-12-17 23:40:13
 * Last modified: 2017-01-03 23:10:35
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FORMAT_PARSER_HPP_
#define _FORMAT_PARSER_HPP_

#include <string>
#include <vector>

using std::string;
using std::vector;

class FormatParser{
public:
    FormatParser();
    FormatParser(const string &fn, const string &content);
    ~FormatParser();
    int  get_wordcell(string &wc);
    int  jieba_test(void);
private:
    string      m_fn;
    string      m_content;
};

#endif //_FORMAT_PARSER_HPP_

