#ifndef REQUEST_HPP
#define REQUEST_HPP

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
#include <sys/epoll.h>


class server;
class request
{
    public:
        std::string uri;
        std::string path;
        std::string method;
        std::string http_version;
        std::map<std::string, std::string> extentions;
        // std::map<int, fd_info>          fd_maps;
    

        bool        checki;
        unsigned long g;
        size_t check;

        /*************** REQUEST FUNCTIONS ***************/

        // std::string     get_ori_uri(std::string path, server &server);
        std::string     get_delet_resp(std::string path, int stat);
        void            fill_extentions();
        std::streampos  get_fileLenth(std::string path);
        std::string     get_exten_type(std::string path);
        // std::string     generat_html_list(std::string directory);
        // std::string     get_index_file(std::map<std::string, std::string> &loca_map);
        bool            check_autoindex(std::map<std::string, std::string> loca_map);
        // int             read_request(server &server, int fd);
        std::string     get_full_uri(std::string path, server &server);
        std::string     delet_method(std::string path, server &server);
        // std::string     get_header(int wich, std::string exten, std::string lentg, fd_info&  fd_inf);    
        void            parse_req(std::string   rq, server &server);
  
        template <typename T>
        std::string to_string(T value) 
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
        request();
        ~request();
};


#endif