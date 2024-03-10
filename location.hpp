/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayylaaba <ayylaaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 17:45:30 by ayylaaba          #+#    #+#             */
/*   Updated: 2024/02/15 16:26:49 by ayylaaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

class server;

class location
{
    public:
        std::string                         l_root;
        std::string                         l_index;
        std::vector<std::string>            s;
        std::map<std::string, std::string>  cont_l;
        std::map<std::string, std::string>  key_value;
        std::vector<std::string>            l_vec;
        std::vector<std::string>            allowed_methods;
        std::vector<location*>              l;
        int                                 l_token;
        void                                handl_loca(std::map<std::string, std::string>& m, std::vector<std::string> &methds, std::string root);
        int                                 check_exist(std::string path, char ch);
        int                                 check_permi(std::string path);
        void                                print_err(std::string str);
        location(std::map<std::string, std::string> &c, std::vector <std::string> &v_s);
        location();
};

#endif
