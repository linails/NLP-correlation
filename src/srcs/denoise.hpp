/*
 * Progarm Name: denoise.hpp
 * Created Time: 2017-03-03 23:51:20
 * Last modified: 2017-03-10 23:19:32
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _DENOISE_HPP_
#define _DENOISE_HPP_

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class DeNoise{
public:
    DeNoise(float threshold, map<string, float> &ret_statis, string except = "");
    ~DeNoise();
    int  denoise(string &line);
private:
    int     m_threshold;
    map<string, float>  m_ret_statis;    // <'p', 0.243>,<'a', 0.534> ...
    string  m_except;
};

#endif //_DENOISE_HPP_

