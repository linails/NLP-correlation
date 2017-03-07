/*
 * Progarm Name: main.cpp
 * Created Time: 2017-03-07 21:53:35
 * Last modified: 2017-03-07 22:31:13
 * @author: minphone.linails linails@foxmail.com 
 */

#include <iostream>
#include <cassert>
#include "dic-parser.hpp"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    int ret = 0;

    cout << "NLP-correlation ..." << endl;

    DicParser dicparser;

    ret = dicparser.dicparser_main(argc, argv); assert(-1 != ret);

    return ret;
}

