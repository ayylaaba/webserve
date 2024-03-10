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


size_t MAX_CLIENTS = 10;
int MAX_EVENTS = 1024;
int BUFFER_SIZE = 1024;

int main(int ac, char **av) 
{
    int                 serverSocket;
    // int                 newSocket;
    // int                 rd_sock;
    request             rq;
    server              parse;
    int         respo;

    // fd_set              readfds;
    // int                 activity;

    if (ac < 2)
        parse.print_err("Argement Not Valid");
    
    parse.mange_file(av[1]);
    // exit (1);
    size_t k,a;
    a = 0;
    k = 0;

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


    // printf("I am listening for a client...\n");
    // if ((newSocket = accept(serverSocket, 0, 0)) < 0)  // Check for error in accept
    //     parse.print_err("client socket not created yet \n");
    // char buffer[3000];
    // rd_sock = read(newSocket, buffer, sizeof(buffer));  // Read from the accepted socket
    // std::string reqst = buffer;

    // rq.parse_req(buffer, parse);

    // rq.fd_maps[newSocket] = new fd_info(rq.uri);
    // std::map<int, fd_info*>::iterator it = rq.fd_maps.find(newSocket);
    // fd_info *obj = it->second;
    // std::cout << "---------------->>>>> " << obj->rd_done << "<<<------\n";
    // std::cout << "Uri Full Path == " << rq.uri << "\n";
    // long    g = 0;
    // while (1)
    // {
    //     /*
    //         1- sawl wach drori mn files dyal errors  wla n9dr nswb string fihya error lik i do to atoindex
    //         2- response in case of file if not found.
    //         3- atouindex what i should to do if someone click on the file or directory 
    //     */
    //     std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
    //     int res = rq.read_request(parse, newSocket);
    //     if (res && obj->rd_done) // t9dr checki b re_done by it.scond assign it to fd_info ....
    //     {
    //         std::cout << "Read Is End \n";
    //         break;
    //     }
    //     g++;
    // }
    // std::cout << "g == " << g << " , newSocket === " << newSocket << "\n";
    // close(newSocket);  // Close the accepted socket, not the listening socket

    // Initialize clientSockets array
        std::cout << "chof kidir brojola \n";

    int epoll_fd = epoll_create(5);
    struct epoll_event envts;
    envts.data.fd = serverSocket;
    envts.events = EPOLLIN;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &envts);


    std::string tmp;
    int bytesRead;

    while (1) 
    {
        printf("I am listening for an event...\n");
        char buffer[BUFFER_SIZE];
        int client_sock;
        struct epoll_event events[MAX_EVENTS];
        int NumFd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < NumFd; i++) 
        {
            if (events[i].data.fd == serverSocket) 
            {
                std::cout << "connect a new client ...\n";
                client_sock = accept(serverSocket, NULL, NULL);

                struct epoll_event envts_client;
                envts_client.data.fd = client_sock;
                envts_client.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &envts_client) == -1) 
                {
                    close(client_sock);
                    perror("Issue In Adding Client To Epoll");
                    exit(1);
                }
                parse.req_time[client_sock] = 0;
            }
            else
            {
                // std::cout << "L EVENT JAT FHAD L FD = " << events[i].data.fd << " \n";
                std::map<int, fd_info>::iterator it_fd = rq.fd_maps.find(events[i].data.fd );
                std::map<int, int>::iterator it = parse.req_time.find(events[i].data.fd );
                std::cout<<"waaaaaaaaaaaaaaaaee:::"<<rq.fd_maps.size()<<std::endl;
                if (events[i].events & EPOLLIN && !it->second)
                {
                    std::cout << "FD READY TO READ -_- = " << events[i].data.fd << " \n";
                    bytesRead = recv(events[i].data.fd , buffer, BUFFER_SIZE, 0);
                    if (bytesRead <= 0)
                    {
                        close(events[i].data.fd  );
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
                        exit(1);
                    }
                    rq.parse_req(buffer, parse);                /// just for parsing request
                    std::cout<<"----------------"<<std::endl;   
                    rq.fd_maps[client_sock] = fd_info(rq.uri);  // must change.
                    std::cout<<"----------------"<<std::endl;
                    parse.req_time[events[i].data.fd] = 1;
                }
                // map<fd, client>
                // client
                // {
                //     request
                //         {
                //             method, path, version, headers;
                //         }
                //     respons
                //         {
                //             respons line, headers; 
                //         }
                //     get
                //         {
                //             file;
                //             setheader;
                //             what will you need;
                //         };
                //     delete{...}
                //     abid : {post, ...};
                // }
                else if (events[i].events & EPOLLOUT && (*it).second && !(it_fd)->second.rd_done)
                {
                    std::cout << "ready  writing " << " \n";
                    respo = rq.read_request(parse, events[i].data.fd);  // Replace this with your response logic
                    // std::map<int, fd_info>::iterator it = rq.fd_maps.find(events[i].data.fd);
                    if (respo /*&& it->second.rd_done*/)
                    {
                        // exit (1);
                        close(events[i].data.fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
                        rq.fd_maps.erase(events[i].data.fd);
                        break ;
                    }
                }

            }
        }
    }



    // std::vector<int> clientSockets;
    // for (size_t i = 0; i < clientSockets.size(); ++i) {
    //     clientSockets[i] = 0;
    // }

    // int maxSockets = serverSocket;
    // while (true) 
    // {
    //     FD_ZERO(&readfds);
    //     FD_SET(serverSocket, &readfds);
    //     for (size_t i = 0; i < MAX_CLIENTS; ++i) 
    //     {
    //         std::cout << "gogo\n";
    //         if (clientSockets[i] > 0)
    //         {
    //             std::cout << "laaaa\n";
    //             FD_SET(clientSockets[i], &readfds);
    //         }
    //     }
    //     // Use select to monitor sockets
    //     std::cout << "7adi kinamss \n";
    //     activity = select(maxSockets + 1, &readfds, 0, 0, 0);
    //     if ((activity < 0) && (errno != EINTR)) 
    //         perror("Select error");
    //     // Check if there is a new connection
    //     if (FD_ISSET(serverSocket, &readfds)) 
    //     {
    //         std::cout << "maybe here\n";
    //         if ((newSocket = accept(serverSocket, 0, 0)) < 0) {
    //             perror("Accept failed");
    //             exit(EXIT_FAILURE);
    //         }     
    //         std::cout << "socket Client == " << newSocket << "\n";
    //         for (size_t i = 0; i < MAX_CLIENTS; i++)            // Add new socket to array of sockets
    //         {
    //             std::cout << "It Enter \n";
    //             if (clientSockets[i] == 0) 
    //             {
    //                 std::cout << "Was ADD To The ARRAY == " << newSocket << "\n";
    //                 clientSockets[i] = newSocket;
    //                 break;
    //             }
    //         }
    //         if (newSocket > maxSockets)             // Update maxSockets if needed
    //             maxSockets = newSocket;
    //         char buffer[3000];
    //         rd_sock = read(newSocket, buffer, sizeof(buffer));  // Read from the accepted socket
    //         rq.parse_req(buffer, parse);
    //         std::cout << "************** Request ************** \n";
    //         std::cout << buffer << "\n";
    //         std::cout << "************************************* \n";
    //         rq.fd_maps[newSocket] = new fd_info(rq.uri);
    //         std::cout << "New connection, socket fd is " << newSocket << ", IP is: "
    //                     << inet_ntoa(serverAddr.sin_addr) << ", port is: " << ntohs(serverAddr.sin_port) << std::endl;
    //     }
    //     // Check data from clients
    //     for (size_t i = 0; i < MAX_CLIENTS; ++i)     
    //     {
    //         int socketDesc = clientSockets[i];

    //         if (FD_ISSET(socketDesc, &readfds)) 
    //         {
    //             std::cout << "deja t create had socketDes an it ready to read data from it => " << socketDesc << std::endl;   
    //             std::cout << "Segv \n";
    //             int stat = rq.read_request(parse, socketDesc);
    //             std::map<int, fd_info*>::iterator it = rq.fd_maps.find(socketDesc);
    //             fd_info* obj = (*it).second; 
    //             if (stat && obj->rd_done)
    //             {
    //                 close(socketDesc);
    //                 clientSockets[i] = 0;
    //             }
    //         }
    //     }
    // }
    return 0;
}
