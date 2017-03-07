/*
 * Progarm Name: regex_common.cpp
 * Created Time: 2016-05-26 10:14:06
 * Last modified: 2016-06-20 22:12:34
 * @author: minphone.linails linails@foxmail.com 
 * @version 0.0.2
 */

#include "regex_common.h"
#include <iostream>
#include <string>
#include <vector>
#include <regex.h>

using namespace std;

/*regex - common - success retuen 0*/
int regex_common(const char *pattern, const char *str, int &spos, int &epos)
{
    int         rc;
    regex_t     preg;
    regmatch_t  pmatch[2] = {0,};

    if(0 != (rc = regcomp(&preg, pattern, 0))){
        printf("regcomp() failed, returning nonzero (%d)\n", rc);
        return -1;
    }else{
        if(0 != (rc = regexec(&preg, str, 2, pmatch, 0))){
            printf("Failed to match '%s' with '%s',returnning %d.\n",
                    str, pattern, rc);
            return -1;
        }else{
            spos    = pmatch[0].rm_so;
            epos    = pmatch[0].rm_eo - 1;
        }
    }

    regfree(&preg);
    return 0;
}

/*regex - common - success retuen 0*/
int regex_common(const char *pattern, const char *str, string &unit)
{
    int     ret  = 0;
    int     spos = 0;
    int     epos = 0;
    if(0 == (ret = regex_common(pattern,str,spos,epos))){
        while(spos < epos)  unit += str[spos++];
    }
    return ret;
}

/*regex - common - success return 0*/
int regex_common(string &pattern, string &contents, vector<string> &units)
{
    return regex_common(pattern.c_str(),contents,units);
}

/*regex - common - success return 0*/
int regex_common(const char *pattern, string &contents, vector<string> &units)
{
    const char *str = contents.c_str();

    int         rc;
    regex_t     preg;
    regmatch_t  pmatch[2] = {0,};

    if(0 != (rc = regcomp(&preg, pattern, 0))){
        printf("regcomp() failed, returning nonzero (%d)\n", rc);
        return -1;
    }else{

        int         spos = 0;
        int         epos = 0;

        /* string start pos & string end pos*/
        int         sspos= 0;
        int         sepos= 0;

        if(0 != (rc = regexec(&preg, &str[epos], 2, pmatch, 0))){
            return -1;
        }else{

            /* <br>(1) */
            spos   += pmatch[0].rm_so;
            epos   += pmatch[0].rm_eo - 1;
            sspos   = spos;

            string  unit;

            do{
                if(0 != (rc = regexec(&preg, &str[epos], 2, pmatch, 0))) break;
                else{
                    spos    = pmatch[0].rm_so + epos;
                    epos   += pmatch[0].rm_eo - 1;

                    sepos   = spos ;

                    for(; sspos<sepos; sspos++) unit += str[sspos];
                    units.push_back(unit);
                    unit.clear();

                    sspos   = spos;
                }
            }while(1);

            for(; sspos<(int)contents.size(); sspos++) unit += str[sspos];
            units.push_back(unit);
            unit.clear();
        }
    }

    regfree(&preg);
    return 0;
}

/*
 * c++11 regex , gcc version=4.9.2/5.3
 * */
#include <regex>

/*regex c++11 - common - success retuen 0*/
extern int regex_common_c0x(const char *pattern, const char *str ,string &unit)
{
    string contents(str);
    return regex_common_c0x(pattern, contents, unit);
}

/*regex c++11 - common - success retuen 0*/
extern int regex_common_c0x(const char *pattern, string str ,string &unit)
{
    regex   patterns(pattern);
    smatch  result;

    if(true == regex_search(str, result, patterns)){
        unit = result.str();
        return 0;
    }else{
        return -1;
    }
}

/*regex c++11 - common - success return 0*/
int regex_common_c0x(const char *pattern, string &contents, vector<string> &units)
{
    regex   patterns(pattern);
    sregex_token_iterator   end;

    for(sregex_token_iterator i(contents.begin(), contents.end(), patterns);
            i != end; ++i){
        units.push_back(*i);
    }

    if(true == units.empty()) return -1;

    return 0;
}

/*regex c++11 - common - success return 0*/
int regex_common_c0x(string &pattern, string &contents, vector<string> &units)
{
    return regex_common_c0x(pattern.c_str(),contents,units);
}
