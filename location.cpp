/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayylaaba <ayylaaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 17:45:30 by ayylaaba          #+#    #+#             */
/*   Updated: 2024/02/17 10:24:50 by ayylaaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

location::location(std::map<std::string, std::string> &c, std::vector <std::string> &v_s)
{
    std::map<std::string, std::string>::iterator    it = c.find("index");
    if (it != c.end())
    {
        std::cout << "ghadi t7lo lyom \n";
        std::cout << " -> " << (*it).second << "<-- \n";
    }
    cont_l = c;
    allowed_methods = v_s;
}

int     location::check_exist(std::string path, char ch)
{
    if (ch == 'd')
    {
        DIR* dir = opendir(path.c_str());
        if (!dir)
        {
            return (1);
        }
        return (0);
    }
    else
    {
        if (access(path.c_str(), R_OK))
            return 1;
        return 0;
    }
} 

void            location::print_err(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}


int     location::check_permi(std::string path)
{
    struct stat inf;
    
    if (stat(path.c_str(), &inf) != 0)
        return (1);
    return (0);
} 

void        location::handl_loca(std::map<std::string, std::string>& m, std::vector<std::string> &methods, std::string root)
{

    std::map<std::string, std::string>::iterator it = m.begin();
    std::map<std::string, std::string>::iterator ite = m.end();
    while (it != ite)
    {
        if (!(*it).first.compare("root"))
        {
            if (check_exist((*it).second, 'd') || check_permi((*it).second))
                print_err("syntaxt_error");
        }
        if (!(*it).first.compare("allow_methods"))
        {
            for (size_t i = 1; i < methods.size(); i++)
            {
                if (methods[i].compare("GET") && methods[i].compare("DELET")
                && methods[i].compare("POST"))
                    print_err("syntaxt_error");
            }
        }
        if (!(*it).first.compare("autoindex"))
        {
            if ((*it).second.compare("on") && (*it).second.compare("off"))
                print_err("syntaxt_error");
        }
        if (!(*it).first.compare("index"))
        {
            std::string path = root + "/" + (*it).second;
            if (check_exist(path, 'f') || check_permi(path))
                print_err("syntaxt_error");
        }
        it++;
    }
}

location::location(){
    
}