/*
 * Progarm Name: denoise.cpp
 * Created Time: 2017-03-03 23:51:11
 * Last modified: 2017-03-12 23:07:04
 * @author: minphone.linails linails@foxmail.com 
 */

#include "denoise.hpp"
#include <iostream>
#include "dic-parser.hpp"
#include <string>
#include <vector>
#include "stringTools.h"
#include <list>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;

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
    stringTools  st;
    list<string> split;
    string       block;

    st.split_utf_code(split, line);

    this->clear_head_noise(split);

    for(auto iter = split.begin();
             iter!= split.end();){
        if(0 == this->filter_char_check(*iter)){
            break;
        }else{
            block += *iter;
            iter++;
        }
    }

    if(false == block.empty()){
        if(block == "、")   block.clear();
        else                st.utf_code2string(line, split);
    }

    pair<int, int> pos;
    if((false == block.empty()) && (-1 != st.block_check(pos, line, block))){
        cout << "line = block" << endl;
        cout << "<int, int> : " << " < " << pos.first << " , " << pos.second << " > " << endl;
        line = block;
    }else{
        for(auto iter = split.begin();
                 iter!= split.end();){
            if(-1 != this->filter_char_check(*iter)){
                iter = split.erase(iter);
            }else{
                iter++;
            }
        }

        /* 
         * clear "、"
         * */
        if(0 != this->is_middle(split, "、")){
            auto iter = split.begin();
            while((iter = find(split.begin(), split.end(), "、")) != split.end()){
                if(iter != split.end()) split.erase(iter);
            }
        }

        /* 
         * especially for "sline : l、chōnɡ"
         * */
        {
            auto iter = find(split.begin(), split.end(), "、");
            if(iter != split.end()){
                if(*split.begin() == "l"){
                    split.erase(iter);
                    iter = split.begin();
                    split.erase(iter);
                }
            }
        }

        st.utf_code2string(line, split);
    }

    return 0;
}

int  DeNoise::clear_head_noise(list<string> &split)
{
    for(auto iter = split.begin(); 
             iter!= split.end();){
        if(-1 != this->filter_char_check(*iter)){
            iter = split.erase(iter);
        }else{
            break;
        }
    }

    return 0;
}

/* 
 * ret = 0 , have to filter
 * */
int  DeNoise::filter_char_check(string char_)
{
    int ret = -1;

    if(this->m_threshold > this->m_ret_statis[char_]){

        ret = 0;

        if(string::npos != this->m_except.find(char_)){
            ret = -1;
        }
    }

    return ret;
}

int  DeNoise::is_middle(list<string> &split, const char *char_)
{
    if(nullptr != char_){
        if((1 != split.size()) && (false == split.empty())){
            auto begin = split.begin();
            auto rbegin= split.rbegin();
            if((*begin != char_) && (*rbegin != char_)){
                return 0;
            }
        }
    }

    return -1;
}

