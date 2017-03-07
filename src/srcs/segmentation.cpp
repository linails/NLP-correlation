/*
 * Progarm Name: segmentation.cpp
 * Created Time: 2017-02-27 15:40:19
 * Last modified: 2017-03-03 16:10:37
 * @author: minphone.linails linails@foxmail.com 
 */

#include "segmentation.hpp"
#include <iostream>
#include "dic-parser.hpp"

using std::cout;
using std::endl;

Segmentation::Segmentation(string s)
    :m_str(s)
{
    DicParser::get_instance()->get_disk()
                             ->get_all_words_ws(this->m_words);
}

Segmentation::~Segmentation()
{
}

int  Segmentation::result(vector<string> &result)
{
    return 0;
}

int  Segmentation::isolated_check(vector<string> &seg)
{
    return 0;
}

