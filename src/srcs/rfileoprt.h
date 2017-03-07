/*
 * Progarm Name: rfileoprt.h
 * Created Time: 2015-12-28 20:39:01
 * Last modified: 2017-03-02 09:09:02
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _FILEOPRT_H_
#define _FILEOPRT_H_
#include <string>
#include <fstream>
#include <vector>
#include <functional>

using namespace std;

class rFileOprt{
public:
    rFileOprt(const char *p_fn);
    rFileOprt(const std::string &r_fn);
    ~rFileOprt(void);

    /*readfile linebyline,pcb: callback of parse line data*/
    int read_linebyline(void (*pcb)(const char *p_line));
    //int read_linebyline(void (*pcb)(string line));
    int read_linebyline(void (*pcb)(string &r_line));
    int read_linebyline(std::function<void (string)> pcb = nullptr);
    /*read index line*/
    //int read_index_line(unsigned int index,string &dline, void (*pcb)(string line) = NULL);
    int read_index_line(unsigned int index,string &dline, std::function<void (string)> pcb = nullptr);
    int read_index_line(unsigned int index,void (*pcb)(string line));
    int read_index_line(unsigned int index,void (*pcb)(string &r_line));
    int read_index_line(unsigned int index,void (*pcb)(string ,string));
    string read_index_line(unsigned int index);
    /*
     * pcb  = NULL , index reset to 0
     * pcb != NULL , index++ -> pcb();
     * */
    int read_index_line(void (*pcb)(string &,string &));
    /*get version*/
    string get_version(void) const;
    void get_version(string &r_ver) const;
private:
    void load_contents(void);
private:
    static const string    version_fo; /*fileoprt class version*/
    string                 m_oprtfn;
    vector<string>         m_contents;
    ifstream               m_fin;
};

#endif //_RFILEOPRT_H_

