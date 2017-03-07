/*
 * Progarm Name: timer.h
 * Created Time: 2016-02-04 09:30:46
 * Last modified: 2016-02-04 10:38:24
 * @author: minphone.linails linails@foxmail.com 
 * @version 0.0.1
 */

#ifndef _TIMER_H_
#define _TIMER_H_
#include <string>
#include <sys/time.h>

class Timer{
public:
    Timer();
    ~Timer();
    void timing(void);
    void start_timing(void);
    std::string stop_timing(void);/*stop_timing : return diffval time*/
private:
    bool timingflag = false;
    struct timeval start,stop;
};

#endif //_TIMER_H_

