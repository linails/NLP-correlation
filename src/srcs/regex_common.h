/*
 * Progarm Name: regex_common.h
 * Created Time: 2016-05-26 10:14:10
 * Last modified: 2016-06-20 22:06:45
 * @author: minphone.linails linails@foxmail.com 
 * @version 0.0.3
 */

#ifndef _REGEX_COMMON_H_
#define _REGEX_COMMON_H_

#include <string>
#include <vector>

/*regex - common - success retuen 0*/
extern int regex_common(
        const char *pattern, 
        const char *str ,
        int &spos, 
        int &epos
        );

/*regex - common - success retuen 0*/
extern int regex_common(
        const char *pattern, 
        const char *str ,
        std::string &units
        );

/*regex - common - success return 0*/
extern int regex_common(
        std::string &pattern,
        std::string &contents,
        std::vector<std::string> &units
        );

/*regex - common - success return 0*/
extern int regex_common(
        const char *pattern, 
        std::string &contents,
        std::vector<std::string> &units
        );

/*regex c++11 - common - success retuen 0*/
extern int regex_common_c0x(
        const char *pattern, 
        const char *str ,
        std::string &unit
        );

/*regex c++11 - common - success retuen 0*/
extern int regex_common_c0x(
        const char *pattern, 
        std::string str ,
        std::string &unit
        );

/*regex c++11 - common - success return 0*/
extern int regex_common_c0x(
        const char *pattern, 
        std::string &contents,
        std::vector<std::string> &units
        );

/*regex c++11 - common - success return 0*/
extern int regex_common_c0x(
        std::string &pattern, 
        std::string &contents, 
        std::vector<std::string> &units
        );

#endif //_REGEX_COMMON_H_

