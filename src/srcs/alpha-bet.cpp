/*
 * Progarm Name: alpha-bet.cpp
 * Created Time: 2017-03-02 10:58:33
 * Last modified: 2017-03-02 14:13:21
 * @author: minphone.linails linails@foxmail.com 
 */

#include "alpha-bet.hpp"
#include <iostream>
#include <cassert>
#include "disk-dic.hpp"
#include "dic-parser.hpp"
#include "stringTools.h"
#include <algorithm>
#include <list>

using std::cout;
using std::endl;
using std::list;

AlphaBet::AlphaBet()
{
}

AlphaBet::~AlphaBet()
{
}

int  AlphaBet::add_word(string word)
{
    this->m_words.push_back(word); return 0;
}

int  AlphaBet::add_words(vector<string> &words)
{
    for(auto &s : words){ this->m_words.push_back(s); } return 0;
}

/* 
 * filter mode
 * */
int  AlphaBet::add_spell(string spell, int)
{
    list<string>    alphas;

    this->m_spells.push_back(spell);

    /* 
     * filter for alpha
     * */
    {
        stringTools     st;

        st.filter(" …，　ˋ［］<>、“”。:《》〈〉A", spell);
        st.filter("ㄍㄞ爲烏親軆冩單壓參噦嘍囂塹夾尷尲掃擷斕", spell);
        st.filter("时备替一的色人随作职责樅梔柵櫟轂无有与对", spell);
        st.filter("几渘澠濺燾璵癩矯磟蘪絡縮羥衇檾萵薟螞説謎釕鈧鋃", spell);
        st.filter("鏹颶餛餫餺饅魷鱖鸕鴯鵓鷓鷦齟龜", spell);
        st.filter("；偾姹同没相耔名箫脉韪鲠", spell);
        auto fl = spell.find("["); if(fl != string::npos) spell.erase(fl);
        auto fy = spell.find("]"); if(fy != string::npos) spell.erase(fy);

        st.split_utf_code(alphas, spell);

        #if 0
        for(auto iter = alphas.begin();
                 iter!= alphas.end(); iter++){
            auto isexist = find(this->m_words.begin(), this->m_words.end(), *iter);
            if(isexist != this->m_words.end()){
                iter = alphas.erase(iter);
            }
        }
        #endif
    }

    for(auto &u : alphas){
        auto isexist = find(this->m_alphas.begin(), this->m_alphas.end(), u);
        if(isexist == this->m_alphas.end()){
            this->m_alphas.push_back(u);
        }
    }

    return 0;
}

int  AlphaBet::add_spell(string spell)
{
    return 0;
}

int  AlphaBet::add_spell(vector<string> &spells)
{
    for(auto &spell : spells){ this->add_spell(spell); } return 0;
}

int  AlphaBet::get_alplas(vector<string> &alphas)
{
    alphas = this->m_alphas; return 0;
}


