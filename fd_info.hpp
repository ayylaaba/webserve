#ifndef FD_INFO_HPP
#define FD_INFO_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>
#include <sys/epoll.h>
#include "get_method.hpp"

class fd_info
{
    public:
        int                 res_header;
        std::string         stor_uri;
        std::ifstream       read_f;
        int                 rd_done;
        int                 res_sent;
        int                 u_can_send;
        std::streampos      filePosition;
        get_method          get;
        request             requst;
        // post                 *post_; abed class
    
        fd_info(std::string uri_);
        fd_info(const fd_info& copy);
        fd_info&         operator=(const fd_info& copy);
        fd_info&         operator=(std::string _uri);
        fd_info();
        ~fd_info();
};

#endif
