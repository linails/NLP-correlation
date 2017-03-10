/*
 * Progarm Name: format-later-stage.hpp
 * Created Time: 2017-03-04 08:34:19
 * Last modified: 2017-03-10 22:55:03
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FORMAT_LATER_STAGE_HPP_
#define _FORMAT_LATER_STAGE_HPP_

#include <string>
#include <vector>
#include "statistics.hpp"

using std::string;
using std::vector;

class FormatLaterStage{
public:
    FormatLaterStage();
    ~FormatLaterStage();
    int  calc_statis_for_spell(void);
    int  pre_stage_spell_filter(string &line);
private:
    Statistics  m_statis;
    int         m_statis_calc_flag = -1;
};

#endif //_FORMAT_LATER_STAGE_HPP_

