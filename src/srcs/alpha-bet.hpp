/*
 * Progarm Name: alpha-bet.hpp
 * Created Time: 2017-03-02 10:58:36
 * Last modified: 2017-03-02 14:10:23
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _ALPHA_BET_HPP_
#define _ALPHA_BET_HPP_

#include <string>
#include <vector>

using std::string;
using std::vector;

class AlphaBet{
public:
    AlphaBet();
    ~AlphaBet();
    int  add_word(string word);
    int  add_words(vector<string> &words);
    int  add_spell(string spell);
    int  add_spell(string spell, int); // filter mode
    int  add_spell(vector<string> &spells);
    int  get_alplas(vector<string> &alphas);
private:
private:
    vector<string> m_words;
    vector<string> m_spells;
    vector<string> m_alphas;
};

#endif //_ALPHA_BET_HPP_

