/*
 * Progarm Name: main.cpp
 * Created Time: 2017-03-07 21:53:35
 * Last modified: 2017-03-14 14:17:00
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

    DicParser *pdic = DicParser::get_instance(new DicParser());
    if(nullptr != pdic){
        cout << "new succeed " << endl;
    }else{
        cout << "[Error] new failed !" << endl;
    }

    ret = pdic->dicparser_main(argc, argv); assert(-1 != ret);

    delete pdic;

    return ret;
}

