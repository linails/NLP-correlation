/*
 * Progarm Name: format-later-stage.cpp
 * Created Time: 2017-03-04 08:34:13
 * Last modified: 2017-03-07 15:16:59
 * @author: minphone.linails linails@foxmail.com 
 */

#include "format-later-stage.hpp"
#include "denoise.hpp"
#include <iostream>
#include "dic-parser.hpp"

using std::cout;
using std::endl;

FormatLaterStage::FormatLaterStage()
{
}

FormatLaterStage::~FormatLaterStage()
{
}

int  FormatLaterStage::pre_stage_spell(string &line)
{
    int ret = 0;

    string format("wc.spel : ");
    string filter("',[]°—“”•②③④⑤⑥⑦⑧⑩　１］");
    string except("IOếềÁňŌĔĒaÀÉĀÈRαPǘKNF");

    float threshold = 0.000092;

    return ret;
}

