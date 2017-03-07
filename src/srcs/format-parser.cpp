/*
 * Progarm Name: format-parser.cpp
 * Created Time: 2016-12-17 23:39:24
 * Last modified: 2017-01-03 23:16:16
 * @author: minphone.linails linails@foxmail.com 
 */

#include "format-parser.hpp"
#include <iostream>
#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using namespace cppjieba;

FormatParser::FormatParser()
{
    this->jieba_test();
}

FormatParser::FormatParser(const string &fn, const string &content)
{
}

FormatParser::~FormatParser()
{
}

int  FormatParser::get_wordcell(string &wc)
{
    int ret = 0;

    return ret;
}

int  FormatParser::jieba_test(void)
{
    cout << " ... jieba-test ..." << endl;

    int ret = 0;

    const char* const DICT_PATH      = "../src/libs/cppjieba/dict/jieba.dict.utf8";
    const char* const HMM_PATH       = "../src/libs/cppjieba/dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../src/libs/cppjieba/dict/user.dict.utf8";
    const char* const IDF_PATH       = "../src/libs/cppjieba/dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../src/libs/cppjieba/dict/stop_words.utf8";

    Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH);

    vector<string> words;
    vector<Word> jiebawords;
    string s;
    string result;

    s = "⑧形表示赞许、同意或结束等语气：～，就这么办｜～了，不要再说了。";
    cout << s << endl;
    cout << "[demo] Cut With HMM" << endl;
    jieba.Cut(s, words, true);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    cout << "[demo] Cut Without HMM " << endl;
    jieba.Cut(s, words, false);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    return ret;
}

