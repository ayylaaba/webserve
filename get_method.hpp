#ifndef GET_METHOD_HPP
#define GET_METHOD_HPP

#include <iostream>
#include <fstream>
#include <string>


#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>
#include "server.hpp"
#include "fd_info.hpp"
#include "request.hpp"
#include <sys/epoll.h>

class fd_info;
// class   request;

class get_method
{
    public:
        // std::map<std::string, std::string>  extentions;
        bool                                checki;

        /*************** GET FUNCTIONS ***************/
        // std::string     get_header(int wich, std::string exten, std::string lentg, fd_info&  fd_inf);
        std::string     generat_html_list(std::string directory);
        std::string     get_index_file(std::map<std::string, std::string> &loca_map);
        bool            check_autoindex(std::map<std::string, std::string> loca_map);
        std::string     get_header(int wich, std::string exten, std::string lentg, fd_info&  fd_inf);
        int             get_mthod(server &server, int fd);
        /*************** REQUEST FUNCTIONS ***************/
        std::streampos  get_fileLenth(std::string path);
        std::string     get_exten_type(std::string path, std::map<std::string, std::string> &exta);
        // void            fill_extentions();
        template <typename T>
        std::string to_string(T value) 
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }

};

#endif