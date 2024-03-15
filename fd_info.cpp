#include "fd_info.hpp"
#include "request.hpp"
#include "get_method.hpp"

fd_info::fd_info(std::string uri_)
{
    stor_uri    = uri_;
    res_header  = 0;
    rd_done     = 0;
    res_sent    = 0;
    u_can_send  = 0; 
}


fd_info::fd_info(const fd_info& copy)
{
    this->u_can_send  = copy.u_can_send; 
    this->filePosition = copy.filePosition;
    this->rd_done = copy.rd_done;
    this->res_header = copy.res_header;
    this->stor_uri = copy.stor_uri;
}

// fd_info&         fd_info::operator=(std::string _uri)
// {
//     stor_uri = _uri;
//     return (*this);
// }

fd_info&         fd_info::operator=(const fd_info& copy)
{
    this->u_can_send  = copy.u_can_send; 
     this->filePosition = copy.filePosition;
     this->rd_done      = copy.rd_done;
    //  this->read_f    = copy.read_f;
     this->res_header   = copy.res_header;
     this->stor_uri     = copy.stor_uri;
     this->res_sent     = copy.res_sent;
    return (*this);
}

fd_info::~fd_info()
{
}

fd_info::fd_info()
{
    res_header  = 0;
    rd_done     = 0;
    u_can_send  = 0;
    res_sent    = 0;
}

// {
//     first_time;
//     {
//         check request requerments
//     }
//     file_requested;
// }

// get_method
// {
//     first_time;
//     file_read;
//     end_of_send;
// }

