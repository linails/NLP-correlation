/*
 * Progarm Name: main.cpp
 * Created Time: 2017-03-07 21:53:35
 * Last modified: 2017-03-16 09:30:45
 * @author: minphone.linails linails@foxmail.com 
 */

#include <iostream>
#include <cassert>
#include "dic-parser.hpp"
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
    int ret = 0;

    string path;

    cout << "NLP-correlation ..." << endl;

    switch(argc){
        case 1:
            break;
        case 2:
            path = argv[1];
            break;
        case 3:
            cout << "[Warning] argc = 3" << endl;
            break;
    }

    DicParser *pdic = DicParser::get_instance(new DicParser(path));
    if(nullptr != pdic){
        cout << "new successed " << endl;
    }else{
        cout << "[Error] new failed !" << endl;
    }

    ret = pdic->dicparser_main(argc, argv); assert(-1 != ret);

    delete pdic;

    return ret;
}

