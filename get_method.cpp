#include "get_method.hpp"

extern std::map<int, fd_info> fd_maps;



int    get_method::get_mthod(server &server, int fd)
{
    std::map<int, fd_info>::iterator it = fd_maps.find(fd);
    std::string         line;
    std::string         response;
    std::string         extention_type;
    std::stringstream   StringSize;
    std::streampos      fileSize;
    std::string         buff_s;

    std::cout << "00000000000000000000000000000000000000000000000000000\n";
    std::cout  << it->second.requst->uri << "\n";
    if (it == fd_maps.end()) // print error
        exit(1);
    fileSize = get_fileLenth(it->second.requst->uri); // get full lenth of the file
    extention_type = get_exten_type(it->second.requst->uri,  it->second.requst->extentions);
    StringSize << fileSize;
    std::cout << "file lentgh : " << StringSize.str() << "\n";
    std::cout << "file Extention : " << extention_type << "\n";

    std::cout << "22222222222222222222222222222222222222222222222222222222222\n";
    if (!server.check_exist(it->second.requst->uri, 'f'))
    {
        if (!it->second.res_header)
        {
            response = get_header(0, extention_type, StringSize.str(), it->second);
            it->second.read_f.open(it->second.requst->uri.c_str());
            std::cout << "Wsal l hena b3da send 1 \n";
            send(fd, response.c_str(), response.size(), 0);
        }
        else
        {
            char            buff[1024];
            int     x = it->second.read_f.read(buff, 1024).gcount();
            if (it->second.read_f.gcount() > 0)
            {
                std::cout << "Wsal l hena b3da send 2 \n";
                send(fd, buff, x, 0);
            }
            if (it->second.read_f.eof() || it->second.read_f.gcount() < 1024)
            {
                it->second.rd_done = 1;
                return 1;
            }
        }
    }
    else if (it->second.requst->uri[it->second.requst->uri.length() - 1] == '/' /*&& !checki*/)
    {
        std::stringstream size;
        buff_s = generat_html_list(it->second.requst->uri.substr(0, it->second.requst->uri.find_last_of("/")));
        size << buff_s.size();
        std::cout << "********************\n" << response << "\n*********************\n";
        std::cout << " uri_ = " << it->second.requst->uri.substr(0, it->second.requst->uri.find_last_of("/")) << "\n";
        std::cout << "********************\n" << buff_s << "\n*********************\n";
        if (!it->second.res_header)
        {
            response = get_header(1, "text/html", size.str(), it->second);
            send(fd, response.c_str(), response.size(), 0);
            it->second.res_header = 1;
        }
        else if (it->second.res_header)
        { 
            std::cout << "was here \n" << it->second.res_header << "\n";
            send(fd, buff_s.c_str(), buff_s.size(), 0);
            it->second.rd_done = 1;
            return 1;
        }
    }
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

std::string     get_method::get_exten_type(std::string path, std::map<std::string, std::string> &exta)
{
    std::string exten;
    size_t      pos = path.find_last_of(".");
    exten = path.substr(pos + 1);
    std::cout << "B Enter *---------------------------------------------------------/\n";
    std::map<std::string, std::string>::iterator b = exta.find(exten);
    std::cout << "A Enter *---------------------------------------------------------/\n";
    if (b != exta.end())
    {
        return ((*b).second);
    }
    std::cout << "C Enter *---------------------------------------------------------/\n";
    return ("");
}

std::streampos  get_method::get_fileLenth(std::string path)
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

std::string      get_method::get_header(int wich, std::string exten, std::string lentg, fd_info& fd_inf)
{
    std::string response;

    std::cout << "--=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=\n";
    std::cout << "file lentgh : " << lentg << "\n";
    std::cout << "file Extention : " << exten << "\n";    
    std::cout << "--=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=\n";
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

std::string    get_method::generat_html_list(std::string directory)
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

std::string            get_method::get_index_file(std::map<std::string, std::string> &loca_map)
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

bool            get_method::check_autoindex(std::map<std::string, std::string> loca_map)
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
