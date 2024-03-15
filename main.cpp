/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayylaaba <ayylaaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:03:40 by ayylaaba          #+#    #+#             */
/*   Updated: 2024/02/21 13:10:48 by ayylaaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"
#include <sys/select.h>
#include "fd_info.hpp"
size_t MAX_CLIENTS = 10;
int MAX_EVENTS = 1024;
int BUFFER_SIZE = 1024;
std::map<int, fd_info>  fd_maps; 

int main(int ac, char **av) 
{
    int                 serverSocket;
    server              parse;
    int                 respo;

    if (ac < 2)
        parse.print_err("Argement Not Valid");
    
    // system("leaks ./webserve");
    // exit(1);
    
    parse.mange_file(av[1]);

     // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    int sp = 1;
    // Bind the socket
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &sp, sizeof(sp));
    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port 8080...\n";

    int epoll_fd = epoll_create(5);
    struct epoll_event envts;
    envts.data.fd = serverSocket;
    envts.events = EPOLLIN;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &envts);

    std::string tmp;
    int bytesRead;

    while (1) 
    {
        std::cout << "\t\tI am listening for an event...\n";

        std::string buffer;
        int         client_sock;
        struct      epoll_event events[MAX_EVENTS];
        int NumFd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < NumFd; i++) 
        {
            if (events[i].data.fd == serverSocket) 
            {
                std::cout << "connect a new client ...\n";
                client_sock = accept(serverSocket, NULL, NULL);

                struct epoll_event envts_client;
                envts_client.data.fd = client_sock;
                envts_client.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &envts_client) == -1) 
                {
                    close(client_sock);
                    perror("Issue In Adding Client To Epoll");
                    exit(1);
                }
                parse.req_time[client_sock] = 0;
                fd_maps[events[i].data.fd] = fd_info(); // each client has a obj of fd_info() that include methods an everything
            }
            else
            {
                std::map<int, fd_info>::iterator it_fd = fd_maps.find(events[i].data.fd);
                std::cout<< "\t\t Event was in this  Client ::: " <<  events[i].data.fd  << std::endl;
                if (events[i].events & EPOLLRDHUP /*&& !it->second*/)
                {
                    std::cout << " Enter epoll hrdhuo \n";
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
                    close(events[i].data.fd  );
                    continue ;
                }
                if (events[i].events & EPOLLIN)
                {
                    std::cout << "FD READY TO READ -_- = " << events[i].data.fd << " \n";
                    buffer.resize(BUFFER_SIZE);
                    bytesRead = recv(events[i].data.fd , &buffer[0], BUFFER_SIZE, 0);
                    std::cout << "\n\n\t -> bytesRead ==== " << bytesRead << std::endl;
                    if (bytesRead > 0)
                        buffer.resize(bytesRead);
                    if (bytesRead <= 0)
                    {
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
                        close(events[i].data.fd);
                        continue ;
                    }
                    fd_maps[events[i].data.fd].requst.parse_req(buffer, parse);  // must change.
                    fd_maps[events[i].data.fd].u_can_send = 1; // must change.
                }
                else if (events[i].events & EPOLLOUT && !it_fd->second.rd_done && it_fd->second.u_can_send) // must not always enter to here i think ask about it 
                {
                    std::cout << "ready  writing " << " \n";
                    respo = 0;
                    if (!fd_maps[events[i].data.fd].requst.method.compare("GET"))
                        respo = (*it_fd).second.get.get_mthod(events[i].data.fd);
                    std::cout << "\t\t stat kaml wla ba9i == " << it_fd->second.rd_done << std::endl;
                    if (respo)
                    {
                        std::cout << "\t\t SF KAML GHADI UTM7A HAD "  << events[i].data.fd << std::endl;
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
                        close(events[i].data.fd);
                        fd_maps.erase(events[i].data.fd);
                        continue ;
                    }
                }
            }
        }
    }
    return 0;
}
