/*
 * Progarm Name: rwfileoprt.h
 * Created Time: 2016-05-15 19:46:28
 * Last modified: 2016-12-15 22:07:52
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _RWFILEOPRT_H_
#define _RWFILEOPRT_H_
#include "rfileoprt.h"
#include <map>

using namespace std;

class rwFileOprt : public rFileOprt{
public:
    rwFileOprt(const std::string &r_fn);
    ~rwFileOprt();
private:
    void generate_subfn(void);
private:
    typedef vector<map<pair<string,ofstream>,vector<string> > > ContentType_t;
    static const string version_fo; /*fileoprt class version*/
    string              m_oprtfn;
    ContentType_t       m_contents;
};




#endif //_RWFILEOPRT_H_

