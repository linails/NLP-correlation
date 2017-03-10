/*
 * Progarm Name: format-later-stage.hpp
 * Created Time: 2017-03-04 08:34:19
 * Last modified: 2017-03-10 23:17:56
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FORMAT_LATER_STAGE_HPP_
#define _FORMAT_LATER_STAGE_HPP_

#include <string>
#include <vector>
#include <map>
#include "statistics.hpp"
#include "denoise.hpp"

using std::string;
using std::vector;
using std::map;

class FormatLaterStage{
public:
    FormatLaterStage();
    ~FormatLaterStage();
    int  calc_statis_for_spell(void);
    int  pre_stage_spell_filter(string &line);
private:
    DeNoise            *m_denoise = nullptr;
    Statistics          m_statis;
    int                 m_statis_calc_flag = -1;
    map<string, float>  m_ret_statis;    // <'p', 0.243>,<'a', 0.534> ...
};

#endif //_FORMAT_LATER_STAGE_HPP_

