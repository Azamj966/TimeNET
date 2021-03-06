//-< W32SOCK.H >-----------------------------------------------------*--------*
// SAL                       Version 1.0         (c) 1997  GARRET    *     ?  *
// (System Abstraction Layer)                                        *   /\|  *
//                                                                   *  /  \  *
//                          Created:      8-May-97    K.A. Knizhnik  * / [] \ *
//                          Last update: 21-Nov-98    K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Windows sockets  
//-------------------------------------------------------------------*--------*

#ifndef __W32SOCK_H__
#define __W32SOCK_H__

#include "sockio.h"

class win_socket : public socket_t { 
  protected: 
    SOCKET        s; 
    int           errcode;  // error code of last failed operation 
    char*         address;  // host address

    enum error_codes { 
	ok = 0,
	not_opened = -1,
	bad_address = -2,
	connection_failed = -3,
	broken_pipe = -4, 
	invalid_access_mode = -5,
	message_truncated = -6
    };

  public: 
    boolean   open(int listen_queue_size, int socket_type, int flags = 0);
    boolean   connect(int max_attempts, time_t timeout);

    int       read(void* buf, size_t min_size, size_t max_size, time_t timeout);
    boolean   read(void* buf, size_t size);
    boolean   write(void const* buf, size_t size);

    boolean   sendto(int ip_addr, int ip_port, void const* buf, size_t size);
    int       recvfrom(int& ip_addr, int& ip_port, void* buf, size_t size);

    boolean   is_ok(); 
    boolean   close();
    boolean   shutdown();
    void      get_error_text(char* buf, size_t buf_size);

    socket_t* accept();
    boolean   cancel_accept();

    win_socket(const char* address); 
    win_socket(SOCKET new_sock);

    ~win_socket();
};

#define SOCKET_BUF_SIZE (8*1024) 
#define ACCEPT_TIMEOUT  (30*1000)

class local_win_socket : public socket_t { 
  protected: 
    enum error_codes { 
	ok = 0,
	not_opened = -1,
	broken_pipe = -2,
	timeout_expired = -3,
	invalid_access_mode = -5,
	message_truncated = -6
    };
    enum socket_signals {
        RD,  // receive data
	RTR, // ready to receive
        TD,  // transfer data
	RTT  // ready to transfer
    };
    //------------------------------------------------------
    // Mapping between signals at opposite ends of socket:
    // TD  ---> RD
    // RTR ---> RTT
    //------------------------------------------------------

    struct socket_buf { 
        volatile int RcvWaitFlag;
        volatile int SndWaitFlag;
	volatile int DataEnd;
	volatile int DataBeg;
	char Data[SOCKET_BUF_SIZE - 4*sizeof(int)];  
    };
    struct accept_data { 
        HANDLE Signal[4];
	HANDLE BufHnd;
    };
    struct connect_data { 
	HANDLE Mutex;
	int    Pid;
    };
    socket_buf* RcvBuf;
    socket_buf* SndBuf;
    HANDLE      Signal[4];	   
    HANDLE      Mutex;
    HANDLE      BufHnd;
    int         Error;
    char*       Name;

  public: 
    boolean   open(int listen_queue_size);
    boolean   connect(int max_attempts, time_t timeout);

    int       read(void* buf, size_t min_size, size_t max_size,time_t timeout);
    boolean   read(void* buf, size_t size);
    boolean   write(void const* buf, size_t size);

    boolean   sendto(int ip_addr, int ip_port, void const* buf, size_t size);
    int       recvfrom(int& ip_addr, int& ip_port, void* buf, size_t size);

    boolean   is_ok(); 
    boolean   close();
    boolean   shutdown();
    void      get_error_text(char* buf, size_t buf_size);

    socket_t* accept();
    boolean   cancel_accept();
    
    local_win_socket(const char* address); 
    local_win_socket(); 

    ~local_win_socket();
};
	   
#endif





