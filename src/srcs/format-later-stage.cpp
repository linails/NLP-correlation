/*
 * Progarm Name: format-later-stage.cpp
 * Created Time: 2017-03-04 08:34:13
 * Last modified: 2017-03-12 14:31:44
 * @author: minphone.linails linails@foxmail.com 
 */

#include "format-later-stage.hpp"
#include <iostream>
#include "dic-parser.hpp"
#include "stringTools.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using std::cout;
using std::endl;

FormatLaterStage::FormatLaterStage()
{
}

FormatLaterStage::~FormatLaterStage()
{
    if(nullptr != this->m_denoise){
        delete this->m_denoise;
        this->m_denoise = nullptr;
    }
}

int  FormatLaterStage::calc_statis_for_spell(void)
{
    int ret = 0;

    this->m_statis.get_statis(this->m_ret_statis);

    for(auto &u : this->m_ret_statis){
        printf("(%s - %f)\n", u.first.c_str(), u.second);
    }

    this->m_statis_calc_flag = 0;

    return ret;
}

int  FormatLaterStage::pre_stage_spell_filter(string &line)
{
    stringTools     st;
    vector<string>  result;
    int             pos = -1;

    if((int)string::npos != (pos = line.find("spel : "))){

        string sline(line, pos + strlen("spel : "), string::npos);
        if(false == sline.empty()){

            if(string::npos != sline.find("[")) st.filter("[", sline, 1);
            if(string::npos != sline.find("]")) st.filter("]", sline, 1);

            st.filter("',°—“”•②③④⑤⑥⑦⑧⑩　１］", sline);

            st.split_utf_code(result, sline);

            for(auto &s : result){ this->m_statis.increase(s); }

            if(-1 != m_statis_calc_flag){

                if(nullptr == this->m_denoise){
                    float threshold = 0.000092;
                    string except("IOếềÁňŌĔĒaÀÉĀÈRαPǘKNF");

                    this->m_denoise = new DeNoise(threshold, this->m_ret_statis, except);
                    if(nullptr == this->m_denoise){
                        cout << "[Error] new DeNoise failed !" << endl;
                        exit(1);
                    }
                }

                this->m_denoise->denoise(sline);

                cout << "DeNoise ..." << endl;
            }

            cout << "sline : " << sline << endl;
        }
    }

    return 0;
}



