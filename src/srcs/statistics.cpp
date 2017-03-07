/*
 * Progarm Name: statistics.cpp
 * Created Time: 2017-03-03 23:50:34
 * Last modified: 2017-03-06 20:45:36
 * @author: minphone.linails linails@foxmail.com 
 */

#include "statistics.hpp"
#include <iostream>
#include <cstdio>

using std::cout;
using std::endl;

Statistics::Statistics()
{
}

Statistics::~Statistics()
{
}

int  Statistics::increase(string char_)
{
    auto iter = this->m_word_freq.find(char_);
    if(iter != this->m_word_freq.end()){
        this->m_word_freq[char_]++;
    }else{
        this->m_word_freq[char_] = 1;
    }

    this->m_statis_updated_flag = false;

    return 0;
}

int  Statistics::decrease(string char_)
{
    auto iter = this->m_word_freq.find(char_);
    if(iter != this->m_word_freq.end()){
        this->m_word_freq[char_]--;
    }else{
        return -1;
    }

    this->m_statis_updated_flag = false;

    return 0;
}

int  Statistics::get_statis(map<string, float> &statis)
{
    if(false == this->m_statis_updated_flag){
        this->calc();
    }

    statis = this->m_statis;

    return 0;
}

int  Statistics::clear(void)
{
    this->m_word_freq.clear();

    this->m_statis.clear();

    this->m_statis_updated_flag = false;

    return 0;
}

int  Statistics::calc(void)
{
    size_t all = 0;

    for(auto &u : this->m_word_freq){
        printf("(%s - %d)\n", u.first.c_str(), u.second);
        all += u.second;
    }

    this->m_statis.clear();

    for(auto &u : this->m_word_freq){
        this->m_statis[u.first] = (u.second * 1.0) / (all * 1.0);
    }

    this->m_statis_updated_flag = true;

    return 0;
}


