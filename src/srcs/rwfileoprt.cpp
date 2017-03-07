/*
 * Progarm Name: rwfileoprt.cpp
 * Created Time: 2016-05-15 19:46:24
 * Last modified: 2016-12-15 22:07:56
 * @author: minphone.linails linails@foxmail.com 
 */

#include "rwfileoprt.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

const string rwFileOprt::version_fo("v 0.0.1"); /*fileoprt class version*/

rwFileOprt::rwFileOprt(const std::string &r_fn)
    :rFileOprt(r_fn)
    ,m_oprtfn(r_fn)
{
}

rwFileOprt::~rwFileOprt()
{
}

void rwFileOprt::generate_subfn(void)
{
}


