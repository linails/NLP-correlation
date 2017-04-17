/*
 * Progarm Name: socket-ipc.hpp
 * Created Time: 2017-03-09 08:54:09
 * Last modified: 2017-04-17 18:00:46
 * @author: minphone.linails linails@foxmail.com 
 */

#ifndef _SOCKET_IPC_HPP_
#define _SOCKET_IPC_HPP_

#include <string>
#include <thread>
#include <time.h>
#include <list>
#include <mutex>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>

using std::string;
using std::thread;
using std::list;
using std::recursive_mutex;

class SocketIPC{
public:
    SocketIPC();
    ~SocketIPC();
    int  swrite(string &s);
    int  sread(string &s);
    int  kill_thread(void);
private:
    int  init_socket(void);
    int  reader(void);
    int  writer(void);
    int  listener(void);
private:
    thread            *m_reader      = nullptr;
    thread            *m_writer      = nullptr;
    thread            *m_listener    = nullptr;
    int                m_killed      = -1;
    list<string>       m_reader_buf;
    list<string>       m_writer_buf;
    struct sockaddr_in m_serv_addr;
    struct sockaddr_in m_client_addr;
    int                m_serv_sock   = -1;
    int                m_client_sock = -1;
    int                m_port        = -1;
    recursive_mutex    m_lock_read;
    recursive_mutex    m_lock_write;
};

#endif //_SOCKET_IPC_HPP_

