/*
 * Progarm Name: denoise.hpp
 * Created Time: 2017-03-03 23:51:20
 * Last modified: 2017-03-12 19:40:02
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _DENOISE_HPP_
#define _DENOISE_HPP_

#include <string>
#include <vector>
#include <map>
#include <list>

using std::string;
using std::vector;
using std::map;
using std::list;

class DeNoise{
public:
    DeNoise(float threshold, map<string, float> &ret_statis, string except = "");
    ~DeNoise();
    int  denoise(string &line);
private:
    int  clear_head_noise(list<string> &split);
    int  filter_char_check(string char_);
    int  is_middle(list<string> &split, const char *char_ = nullptr);
private:
    float               m_threshold;
    map<string, float>  m_ret_statis;    // <'p', 0.243>,<'a', 0.534> ...
    string              m_except;
};

#endif //_DENOISE_HPP_

