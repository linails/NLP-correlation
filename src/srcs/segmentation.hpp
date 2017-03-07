/*
 * Progarm Name: segmentation.hpp
 * Created Time: 2017-02-27 15:40:22
 * Last modified: 2017-03-03 16:13:01
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _SEGMENTATION_HPP_
#define _SEGMENTATION_HPP_

#include <vector>
#include <string>
#include <list>

using std::vector;
using std::string;

/* 
 * Note :
 * string = "0123456789..."
 * 01. 检查是 "01"/"01.." 否存在成词
 * 02. 成词后，再检查 "23"/"23.." 是否成词
 * 03. 最后分析句子是否切割后是否存在孤立成分
 * 04. 有存在孤立成分则说明切分错误
 * 05. 无孤立成分则表明切分正确
 * */

class Segmentation{
public:
    Segmentation(string s);
    ~Segmentation();
    int  result(vector<string> &result);
private:
    int  segmen();
    int  isolated_check(vector<string> &seg);
private:
    string          m_str;
    vector<string>  m_words;
};

#endif //_SEGMENTATION_HPP_

