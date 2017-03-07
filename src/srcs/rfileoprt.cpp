/*
 * Progarm Name: rfileoprt.cpp
 * Created Time: 2015-12-28 20:38:55
 * Last modified: 2017-03-02 09:09:11
 * @author: minphone.linails linails@foxmail.com 
 */

#include "rfileoprt.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ios;

const std::string rFileOprt::version_fo("v 0.0.6");

/*constructor*/
rFileOprt::rFileOprt(const char *p_fn)
    :m_fin(p_fn)
{
    this->m_oprtfn = p_fn;
    this->load_contents();
}

/*constructor*/
rFileOprt::rFileOprt(const std::string &r_fn)
    :m_fin(r_fn)
{
    this->m_oprtfn = r_fn;
    this->load_contents();
}

/*default destructor*/
rFileOprt::~rFileOprt(void)
{
    this->m_fin.close();
}

/*readfile linebyline ,pcb: callback of parse line data*/
int rFileOprt::read_linebyline(void (*pcb)(const char *p_line))
{
    if(pcb != NULL){
        for(auto iter = this->m_contents.begin();
                 iter!= this->m_contents.end(); iter++){
            pcb(iter->c_str());
        }
        return 0;
    }
    return -1;
}

#if 0
/*readfile linebyline ,pcb: callback of parse line data*/
int rFileOprt::read_linebyline(void (*pcb)(std::string line))
{
    if(pcb != NULL){
        for(auto iter = this->m_contents.begin();
                 iter!= this->m_contents.end(); iter++){
            pcb(*iter);
        }
        return 0;
    }
    return -1;
}
#endif

/*readfile linebyline ,pcb: callback of parse line data*/
int rFileOprt::read_linebyline(void (*pcb)(std::string &r_line))
{
    if(pcb != NULL){
        for(auto iter = this->m_contents.begin();
                 iter!= this->m_contents.end(); iter++){
            pcb(*iter);
        }
        return 0;
    }
    return -1;
}

int rFileOprt::read_linebyline(std::function<void (string)> pcb)
{
    if(pcb != NULL){
        for(auto iter = this->m_contents.begin();
                 iter!= this->m_contents.end(); iter++){
            pcb(*iter);
        }
        return 0;
    }
    return -1;
}

#if 0
/*readfile index line*/
int rFileOprt::read_index_line(unsigned int index,std::string &dline, void (*pcb)(string line))
{
    if(index <= this->m_contents.size() && index >= 1){
        dline = this->m_contents[--index];

        if(NULL != pcb){
            pcb(dline);
        }
        return 0;
    }
    return -1;
}
#endif

int rFileOprt::read_index_line(unsigned int index,string &dline, std::function<void (string)> pcb)
{
    if(index <= this->m_contents.size() && index >= 1){
        dline = this->m_contents[--index];

        if(NULL != pcb){
            pcb(dline);
        }
        return 0;
    }
    return -1;
}

/*readfile index line & parse*/
int rFileOprt::read_index_line(unsigned int index,void (*pcb)(std::string line))
{
    if(pcb != NULL){
        pcb(this->m_contents[--index]);
        return 0;
    }
    return -1;
}

/*readfile index line & parse*/
int rFileOprt::read_index_line(unsigned int index,void (*pcb)(std::string &r_line))
{
    if(pcb != NULL){
        pcb(this->m_contents[--index]);
        return 0;
    }
    return -1;
}

int rFileOprt::read_index_line(unsigned int index,void (*pcb)(string,string))
{
    if(pcb != NULL){
        pcb(this->m_oprtfn,this->m_contents[--index]);
        return 0;
    }
    return -1;
}

/*readfile index line*/
std::string rFileOprt::read_index_line(unsigned int index)
{
    return this->m_contents[--index];
}

/*
 * pcb  = NULL , index reset to 0
 * pcb != NULL , index++ -> pcb();
 * */
int rFileOprt::read_index_line(void (*pcb)(string &,string &))
{
    static auto iter = this->m_contents.begin();
    if(pcb != NULL){
        pcb(this->m_oprtfn,*iter++);
        if(iter == this->m_contents.end())
            iter = this->m_contents.begin();
        return 0;
    }else{
        iter = this->m_contents.begin();
        return -1;
    }
}

/*get version*/
std::string rFileOprt::get_version(void) const
{
    return version_fo;
}

/*get version*/
void rFileOprt::get_version(std::string &r_ver) const
{
    r_ver = version_fo;
}

void rFileOprt::load_contents(void)
{
    string sline;
    while(getline(this->m_fin,sline)){
        this->m_contents.push_back(sline);
    }
    this->m_fin.close();
}

