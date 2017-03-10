/*
 * Progarm Name: denoise.cpp
 * Created Time: 2017-03-03 23:51:11
 * Last modified: 2017-03-10 23:21:51
 * @author: minphone.linails linails@foxmail.com 
 */

#include "denoise.hpp"
#include <iostream>
#include "dic-parser.hpp"
#include <string>
#include <vector>
#include "stringTools.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

DeNoise::DeNoise(float threshold, map<string, float> &ret_statis, string except)
    :m_threshold(threshold)
    ,m_ret_statis(ret_statis)
    ,m_except(except)
{
}

DeNoise::~DeNoise()
{
}

int  DeNoise::denoise(string &line)
{
    return 0;
}

