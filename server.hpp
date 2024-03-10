#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include "location.hpp"

#include <netinet/in.h>
#include <arpa/inet.h>

class server : public location
{
    public:
        std::string                         listen;
        std::string                         s_root;
        std::string                         _root;
        std::string                         v_index;
        std::vector<server*>                s;
        std::vector<location*>              l;
        std::map<std::string, std::string>  cont;
        location                            obj;

        std::string                         str;
        std::string                         str_l;
        std::vector<std::string>            str_l_vec;
        int                                 s_token;
        std::vector<std::string>            s_vec;
        std::string                         check;
        std::map<int, int>                  req_time;

        void                                mange_file(const char* file);
        void                                handle_err(const std::string  file);
        std::string                         strtrim(std::string &str);
        void                                print_err(std::string str);
        std::vector<std::string>            isolate_str(std::string s, char deli);
        void                                stor_values(std::string key, std::string value, char ch);
        void                                free_l(std::vector<location*> &lv);
        int                                 parse_loca(std::ifstream& rd_cont, std::string &str_);
        int                                 parse_both(std::ifstream& rd_cont, std::string &str_);
        void                                check_size(std::vector<std::string> &s, char c);
        void                                handl_serv(std::string s1, std::string s2);
        int                                 is_num(std::string s);
        int                                 valid_range(std::string s);
        int                                 check_permi(std::string path);
        int                                 check_exist(std::string path, char ch);
        server(std::map<std::string, std::string> &cont_s, std::vector<location*> &l_);   
        server();
};

#endif
