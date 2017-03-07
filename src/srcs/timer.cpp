/*
 * Progarm Name: timer.cpp
 * Created Time: 2016-02-04 09:30:42
 * Last modified: 2016-03-03 13:26:16
 * @author: minphone.linails linails@foxmail.com 
 * @version 0.0.2
 */

#include "timer.h"
#include <sys/time.h>   //for gettimeofday()
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

/*default constructor*/
Timer::Timer()
    :timingflag(false)
{
    start.tv_sec = 0;
    start.tv_usec = 0;
    stop.tv_sec = 0;
    stop.tv_usec = 0;
}

/*default destructor*/
Timer::~Timer()
{
}

/*timing : stop / start : toggle*/
void Timer::timing(void)
{
    if(timingflag == false){
        this->start_timing();
    }else{
        std::string diff(this->stop_timing());
        std::cout << "Timing stop ! : " << diff << std::endl;
    }
}

/*start_timing*/
void Timer::start_timing(void)
{
    if(timingflag == false){
        gettimeofday(&this->start,NULL);
        timingflag = true;
    }
}

/*stop_timing : return diffval time*/
std::string Timer::stop_timing(void)
{
    if(timingflag == true){
        gettimeofday(&this->stop,NULL);
        timingflag = false;
    }

    unsigned sec,msec,usec;
    sec = this->stop.tv_sec - this->start.tv_sec;
    usec = this->stop.tv_usec - this->start.tv_usec;
    usec = this->stop.tv_usec > this->start.tv_usec ?
           this->stop.tv_usec - this->start.tv_usec :
           this->start.tv_usec - this->stop.tv_usec;
    msec = usec/1000;
    usec -= msec*1000;
    unsigned hour,min;
    min = sec/60;
    sec -= min*60;
    hour = min/60;
    min -= hour*60;

    char buf[128];
    sprintf(buf,"[H:M:S:ms:us] %.2d:%.2d:%.2d:%.3d:%.3d"
            ,hour,min,sec,msec,usec);
    return buf;
}


