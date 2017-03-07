/*
 * Progarm Name: statistics.hpp
 * Created Time: 2017-03-03 23:50:41
 * Last modified: 2017-03-05 12:41:59
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _STATISTICS_HPP_
#define _STATISTICS_HPP_

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;
using std::map;
using std::pair;

class Statistics{
public:
    Statistics();
    ~Statistics();
    int  increase(string char_);
    int  decrease(string char_);
    int  get_statis(map<string, float> &statis);
    int  clear(void);
private:
    int  calc(void);
private:
    map<string, int>    m_word_freq; // <'p', 12>,<'a', 32> ...
    map<string, float>  m_statis;    // <'p', 0.243>,<'a', 0.534> ...
    bool                m_statis_updated_flag = false;
};

#endif //_STATISTICS_HPP_

