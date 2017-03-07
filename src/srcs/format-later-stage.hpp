/*
 * Progarm Name: format-later-stage.hpp
 * Created Time: 2017-03-04 08:34:19
 * Last modified: 2017-03-07 15:09:04
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FORMAT_LATER_STAGE_HPP_
#define _FORMAT_LATER_STAGE_HPP_

#include <string>
#include <vector>

using std::string;
using std::vector;

class FormatLaterStage{
public:
    FormatLaterStage();
    ~FormatLaterStage();
    int  pre_stage_spell(string &line);
private:
};

#endif //_FORMAT_LATER_STAGE_HPP_

