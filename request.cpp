#include "request.hpp"

// not_found fun 
// std::string     generateNotFoundResponse(const std::string& requestedPath) {
//     std::string response;
//     response += "HTTP/1.1 404 Not Found\r\n";
//     response += "Content-Type: text/html\r\n\r\n";
//     response += "<html><head><title>404 Not Found</title></head><body>";
//     response += "<h1>404 Not Found</h1>";
//     response += "<p>The requested resource " + requestedPath + " was not found on this server.</p>";
//     response += "</body></html>";
//     return response;
// }




std::string     request::get_header(int wich, std::string exten, std::string lentg, fd_info& fd_inf)
{
    std::string response;
    if (wich == 0)
    {
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: " + exten + "\r\n" + "Content-Length: " + lentg + "\r\n\r\n";
        fd_inf.res_header = 1;
        return (response);
    }
    else if (wich == 1)
    {
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: " + exten + "\r\n" + "Content-Length: " + lentg + "\r\n\r\n";
        fd_inf.res_header = 1;
        return (response);
    }
    return "";
}

void            request::parse_req(std::string   rq, server &server)
{
    std::string line;
    size_t last = rq.find("\r\n");
    // std::cout << "last character = " << last << " == " << rq.substr(0, last) << "\n";
    std::vector<std::string> vec = server.isolate_str(rq.substr(0, last) , ' ');
    std::string     extention_type;

    method        = vec[0];
    path          = vec[1];
    http_version  = vec[2];
    fill_extentions();
    uri = get_full_uri(path, server); // 

    std::cout << "Full Path = " << uri << std::endl;

}

int    request::read_request(server &server, int fd)
{
    std::map<int, fd_info>::iterator it = fd_maps.find(fd);
    std::string         line;
    std::string         response;
    std::string         extention_type;
    std::stringstream   StringSize;
    std::streampos      fileSize;
    std::string         buff_s;

    if (it == fd_maps.end()) // print error
        exit(1);
    fileSize = get_fileLenth(it->second.stor_uri); // get full lenth of the file
    extention_type = get_exten_type(it->second.stor_uri);
    StringSize << fileSize;
    if (!server.check_exist(it->second.stor_uri, 'f'))
    {
        if (!it->second.res_header)
        {
            response = get_header(0, extention_type, StringSize.str(), it->second);
            it->second.read_f.open(it->second.stor_uri.c_str());
            send(fd, response.c_str(), response.size(), 0);
        }
        else
        {
            char            buff[1024];
            int     x = it->second.read_f.read(buff, 1024).gcount();
            if (it->second.read_f.gcount() > 0)
                send(fd, buff, x, 0);
            if (it->second.read_f.eof() || it->second.read_f.gcount() < 1024)
            {
                it->second.rd_done = 1;
                return 1;
            }
        }
        // if (!it->second.res_header)
        // {
        //     response = get_header(0, extention_type, StringSize.str(), it->second);
        //     it->second.read_f.open(it->second.stor_uri.c_str());
        // }
        // // std::ifstream    read_fd(it->second.stor_uri.c_str());
        // it->second.read_f()
        
        // if (read_f.is_open())
        // {
        //     char            *buff = new char[fileSize];

        //     read_fd.seekg(it->second.filePosition);
        //     read_fd.read(buff, 1024);
        //     buff_s.assign(buff, read_fd.gcount());
        //     it->second.filePosition = read_fd.tellg();
        //     if (read_fd.gcount() > 0)
        //     {
        //         if (it->second.res_header)
        //         { 
        //             response += buff_s;
        //             buff_s = response;
        //         }
        //         send(fd, buff_s.c_str(), buff_s.size(), 0);
        //     }
        //     if (read_fd.eof())
        //     {
        //         it->second.rd_done = 1;
        //         return 1;
        //     }
        }
    // }
//     else if (it->second.stor_uri[it->second.stor_uri.length() - 1] == '/' /*&& !checki*/)
//     {
//         std::stringstream size;
//         buff_s = generat_html_list(it->second.stor_uri.substr(0, it->second.stor_uri.find_last_of("/")));
//         size << buff_s.size();
//         if (!it->second.res_header)
//             response = get_header(1, "text/html", size.str(), it->second);
//         std::cout << "********************\n" << response << "\n*********************\n";
//         std::cout << "HI \n";
//         std::cout << " uri_ = " << it->second.stor_uri.substr(0, it->second.stor_uri.find_last_of("/")) << "\n";
//         std::cout << "********************\n" << buff_s << "\n*********************\n";
//             std::cout << "not was here \n" << it->second.res_header << "\n";
//         if (it->second.res_header)
//         { 
//             std::cout << "was here \n" << it->second.res_header << "\n";

//             response += buff_s;
//             buff_s = response;
//         }
//         send(fd, buff_s.c_str(), buff_s.size(), 0);
//         it->second.rd_done = 1;
//         return 1;
//     }
//    else /// creeate html files for 404 error not found // the 
//     {
//         extention_type = get_exten_type(uri); 
//         response = "HTTP/1.1 404 Not Found\r\n";
//         response += "Content-Type:" + extention_type + "\r\n" + "Content-Length: " + StringSize.str() + "\n\r\n\r";
//         std::ifstream    read_fd(uri.c_str()); //you need change path ornuf
//         if (read_fd.is_open())
//         {
//             std::string res;
//             res.resize(1024);
//               while (1)
//             {
//                 char    *buff = new char[1024];
//                 read_fd.read(buff, 1024);
//                 res.resize(read_fd.gcount());
//                 response += buff;
//                 if (read_fd.eof())
//                     break;
//             }
//         }
    // }
    return 0;
}

std::string     request::get_delet_resp(std::string path, int stat)
{
    std::stringstream   StringSize;
    std::string         extention_type;
    std::string         response = "HTTP/1.1 200 OK\r\n";
    std::streampos      fileSize = get_fileLenth(path);

    StringSize << fileSize;
    std::cout << "-------------- sizeOFtheFile that We Want To Delete == " << StringSize.str() << "\n";
    if (stat == 1)
    {
        extention_type = get_exten_type(uri); 
        response += "Content-Type:" + extention_type + "\r\n" +  "Content-Length:" + StringSize.str() + "\n\r\n\r";
        response += "<h1> IS DELETE SUCCESSFUL </h1>";
    }
    else if (stat == 4)
    {
        extention_type = get_exten_type(uri); 
        response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type:" + extention_type + "\r\n" +  "Content-Length:" +  StringSize.str() + "\n\r\n\r";
        response += "<h1>THE FILE NOT FOUND</h1>";
    }
    return (response);
}

std::string     request::get_ori_uri(std::string path, server &server)
{
    std::string line;
    std::string method, path_, http_version, uri;
    std::string response;

    size_t last = path.find("\r\n"); // understand more delete (recursive) and make a respone.
    
    std::cout << "last character = " << last << " request_line == " << path.substr(0, last) << "\n";
    
    std::vector<std::string> vec = server.isolate_str(path.substr(0, last) , ' ');

    method        =  vec[0];
    path_         =  vec[1];
    http_version  =  vec[2];
 
    std::cout << "method = " << method << " path == " << path_ << " http_version == " << http_version << "\n";

    path = get_full_uri(path_, server); // get full path in the server

    return (path);
}

std::string     request::delet_method(std::string path, server &server)
{
    std::string line;
    std::string path_;

    std::cout << "Path == " << path << "\n";

    struct stat path_stat;
    int stat_ = stat(path.c_str(), &path_stat);
    std::string res;

    if(stat_)
    {
        res = get_delet_resp(path, 4);
    }
    if (!S_ISDIR(path_stat.st_mode))
    {
        if (!remove(path.c_str()))
        {
            res = get_delet_resp(path, 1);
            std::cout << "haa9 Mcha ----> " << "Path =" << path << "\n";
            return (res);
        }
        else
        {
            res = get_delet_resp(path, 4);
            std::cout << "haa9 Mcha ----> " << "Path =" << path << "\n";
            std::cout << "\nMabghach Ya Mchi \n";
            return (res);
        }
    }
    else
    {
        DIR *direct = opendir(path.c_str());
        if (!direct)
        {
            perror("Folder does'nt exist");
            exit (1);
        }
        else
        {
            std::cout << "Here \n";
            struct dirent* entry = NULL;
            while ((entry = readdir(direct)))
            {
                std::cout << "Here 2 " << entry->d_name << "\n";
                if (std::string(entry->d_name).compare("..") && std::string(entry->d_name).compare("."))
                {
                    std::cout << "check  < -- > : " << path << "\n"; 
                    if (path[path.length() - 1] != '/')
                    {
                        std::cout << "entre 1"<< "\n";
                        line = path + "/" + entry->d_name;
                        delet_method(line, server);
                    }
                    else
                    {
                        std::cout << "entre 2"<< "\n";
                        line = path + entry->d_name;
                        delet_method(line, server);
                    }
                }
            }
        }
        closedir(direct);
        if (!remove(path.c_str()))
        {
            std::cout << " path "  << path <<" Is Deleted Successfull !\n";
            return ( get_delet_resp(path,1));
        }
        else
        {
            std::cout <<  " path " << path <<"Is Not Deleted Successfull !\n";
            return ( get_delet_resp(path,4));
        }
    }
    return (" -- ");
}

void        request::fill_extentions()
{   
    // i think you need more than that 
    extentions["html"] = "text/html"; 
    extentions["txt"]  = "text/plain"; 
    extentions["jpg"] = "image/jpg"; 
    extentions["jpeg"] = "image/jpeg";
    extentions["png"] = "image/png";
    extentions["mp3"] = "audio/mpeg";
    extentions["mp4"] = "video/mp4";
    extentions["webm"] = "video/webm";
    extentions["pdf"] = "application/pdf";
    extentions["zip"] = "application/zip";
    extentions["woff"] = "application/font-woff";
}

std::string            request::get_index_file(std::map<std::string, std::string> &loca_map)
{
    std::map<std::string, std::string>::iterator it_e = loca_map.end();
    for (std::map<std::string, std::string>::iterator it_b = loca_map.begin(); it_b != it_e; it_b++)
    {
        if (!(*it_b).first.compare("index"))
        {
            std::cout << "mkhaskch tdkhl yahad wld ..\n";

            return ((*it_b).second);
        }
    }
    return (0);
}

bool            request::check_autoindex(std::map<std::string, std::string> loca_map)
{
    std::map<std::string, std::string>::iterator it_e = loca_map.end();
    for (std::map<std::string, std::string>::iterator it_b = loca_map.begin(); it_b != it_e; it_b++)
    {
        if (!(*it_b).first.compare("autoindex"))
        {
            if (!(*it_b).second.compare("on"))
                checki = true;
            break;
        }
    }
    return (checki);
}

std::string     request::get_full_uri(std::string path, server &server)
{
    std::string loca_fldr; 
    std::string rest_fldr;
    std::string full_path;
    size_t pos;
    
    check = 0;
    path = path.substr(1);
    pos = path.find("/");
    loca_fldr = path.substr(0, pos); // folder that isolat from request line
    rest_fldr = path.substr(pos + 1); // chyata lib9at mn wrat folder dyal request line.

    // std::cout << "first_fldr = " << loca_fldr << "\n";
    // std::cout << "rest of folder = " << rest_fldr << "\n";

    for (size_t i = 0; i < server.s.size(); i++)
    {
        for (size_t j = 0; j < server.s[i]->l.size(); j++)
        {
            std::map<std::string, std::string>::iterator      ite = server.s[i]->l[j]->cont_l.end();
            for (std::map<std::string, std::string>::iterator itb = server.s[i]->l[j]->cont_l.begin(); itb != ite; itb++)
            {
                if (!(*itb).first.compare("location") &&  !(*itb).second.substr(1).compare(loca_fldr))
                {
                    std::cout << "check location == " << (*itb).first << " check Second == " << (*itb).second << "\n";
                    checki = check_autoindex(server.s[i]->l[j]->cont_l);
                    std::map<std::string, std::string>::iterator it_e = server.s[i]->l[j]->cont_l.end();
                    for (std::map<std::string, std::string>::iterator it_b = server.s[i]->l[j]->cont_l.begin(); it_b != it_e; it_b++)
                    {
                        if (!(*it_b).first.compare("root"))
                        {
                            if (!rest_fldr.empty()) // rest 3amr
                            {
                                full_path = (*it_b).second + "/" + rest_fldr; 
                                check = 1;
                                break;
                            }
                            else
                            {
                                std::map<std::string, std::string>::iterator indx = server.s[i]->l[j]->cont_l.find("index");
                                if (indx != server.s[i]->l[j]->cont_l.end())
                                {
                                    full_path = (*it_b).second + "/" + (*indx).second; 
                                    check = 1;
                                    break;
                                }
                                else
                                    full_path = (*it_b).second + "/"; 
                            }
                        }
                    }
                    if (check)
                        break;
                }
            }
            if (check)
                break;
        }
    }
    return (full_path);
}

std::string    request::generat_html_list(std::string directory)
{
    std::string resp;

    DIR *dir = opendir(directory.c_str());

    if (dir)
    {
        resp += "<html><head><title>Index of " + directory + " </title></head><body>";
        resp += "<h1>Index of " + directory + "</h1><hr>";

        struct dirent* entry;
        while ((entry = readdir(dir)))
        {
            resp += "<a href=\""+ std::string(entry->d_name) + " \">" + std::string(entry->d_name) + "</a><br>";
        }
        resp += "<hr></body></html>";
        closedir(dir);
    }
    else
    {
        perror("Folder Not Found");
        exit(1);
    }
    return resp;
}

std::string     request::get_exten_type(std::string path)
{
    std::string exten;
    size_t      pos = path.find_last_of(".");

    exten = path.substr(pos + 1);
    std::map<std::string, std::string>::iterator b = extentions.find(exten);
    if (b != extentions.end())
    {
        return ((*b).second);
    }
    return ("");
}

std::streampos  request::get_fileLenth(std::string path)
{
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return -1; // Return -1 to indicate error
    }
    file.seekg(0, std::ios::end);
    std::streampos file_Size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::cout << "===== Be Attention Here === \n" ;
    std::cout << "Size === " << to_string(file_Size) << "\n";
    file.close();
    return file_Size;
}

request::request(/* args */){
}

request::~request(){
}


/*  1- segv in the second client's request
    2- list doesn't work.
    3- aleardy bind.
    4- post man


*/ 


/*
1 get url
2 find server (skip)
3 find route exmple /vid/hh/t.html ghay ya5do /vid just first folder last /
4 if file = (skip) folder
else folder
{
    if has / at the end = continue
    if no slash
    {
        responce HTTP/1.1 301 Moved Permanently\r\n
        location : /folder/\r\n\rn
    }
}

6- fi halt l9it folder f lakher / radi n9lb ela folder location
    6.1 changer m7i l folder lowal f url o badlo b root  (path li 3adk fl pc) o 5ali shi la5or
    exmple : /BADALNI/5alini/ root = /nfs/homes/abouassi/Desktop/root_dir/dir2/ (hadi kamla fbalst BADLNI !!!)
        url =  /cc/ => changer just folder (site) /nfs/homes/abouassi/Desktop/root_dir/dir2/
    6.2 if uri fih file f lkher openih o returnih;
    else
    6.3 if index found in folder's location will add it to path
    6.4 if index not found will seach on autoindex if on will list.
    6.5 error if not found index and even atou_index
    }

7- if file 


1- image problen == content type. <all extentions>

2- check permissions

3- redirection return.

4 - response header

5- content length




if folder*/
