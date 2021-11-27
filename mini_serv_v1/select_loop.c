#include "mini_serv.h"

void	select_loop(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd, max_socket = sockfd, id = 0;
    fd_set              read_sockets, write_sockets;//, current_sockets;
	struct sockaddr_in  cli;
    t_client            *list = NULL;
    int                 proceed = 1;
    // char    buf[101];
    // bzero(buf, 101);

    while (1)
    {
        FD_ZERO(&read_sockets);
        FD_ZERO(&write_sockets);
        FD_SET(sockfd, &read_sockets);
        FD_SET(sockfd, &write_sockets);
        for (t_client *cli = list; cli; cli = cli->next)
        {
            FD_SET(cli->socket, &read_sockets);
            FD_SET(cli->socket, &write_sockets);
            if (cli->socket > max_socket)
                max_socket = cli->socket;
        }
        print_fd_set("READ", &read_sockets, max_socket);
        print_fd_set("WRITE", &write_sockets, max_socket);
        errno = 0;
        nb_fd = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
        if (nb_fd < 0)
            exit_fatal('>');
        
        //check if something happened on the server socket (incoming connexion)
        if (FD_ISSET(sockfd, &read_sockets)) 
        {
            len = sizeof(cli);
            connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
            if (connfd < 0)
                exit_fatal('A');
            else
            {
                t_client *new_client = add_client(&list, connfd, id++);
                client_action(new_client, "arrived\n", list, sockfd);
            }
            send(connfd, "Hi mate\n", 8, 0);
        }

        //someone sent something to the server
        if (FD_ISSET(sockfd, &write_sockets)) 
        {
            read_message(cli, list, sockfd);
            // else
            // {
            //     t_client *new_client = add_client(&list, connfd, id++);
            //     client_action(new_client, "arrived\n", list, sockfd);
            // }
            // send(connfd, "Hi mate\n", 8, 0);
        }

        //check the other sockets for activity
        while (proceed)
        {
            for (t_client *cli = list; cli; cli = cli->next)
            {
                if (FD_ISSET(cli->socket, &read_sockets))
                {
                    write(1, "yo\n", 3);
                    read_message(cli, list, sockfd);
                    // if (read_message(cli, list, sockfd) == 0)
                    // {
                    //     client_action(cli, "leftR\n", list, sockfd);
                    //     FD_CLR(cli->socket, &read_sockets);
                    //     FD_CLR(cli->socket, &write_sockets);
                    //     close(cli->socket);
                    //     remove_client(&list, cli->socket);
                    //     proceed = 0;
                    //     break;
                    // }
                }
                // if (FD_ISSET(cli->socket, &write_sockets))
                // {
                //     if (read_message(cli, list, sockfd) <= 0)
                //     {
                //         client_action(cli, "leftW\n", list, sockfd);
                //         remove_client(&list, cli->socket);
                //         FD_CLR(cli->socket, &read_sockets);
                //         FD_CLR(cli->socket, &write_sockets);
                //         close(cli->socket);
                //     }
                // }
            }
            if (proceed == 0)
                proceed = 1;
            else
                proceed = 0;
        }
    }

    // while (1)
    // {
    //     // FD_ZERO(&current_sockets );   /* Empty the FD Set */
    //     FD_ZERO(&read_sockets);   /* Empty the FD Set */
    //     FD_ZERO(&write_sockets);   /* Empty the FD Set */
    //     if (!list)
    //     {
    //         // write(1, "$", 1);
    //         FD_SET(sockfd, &read_sockets);
    //         FD_SET(sockfd, &write_sockets);
    //         // FD_SET(sockfd, &current_sockets);
    //         if (sockfd > max_socket)
    //             max_socket = sockfd;
    //     }
    //     else
    //     {
    //         // write(1, "@", 1);
    //         // FD_SET(connfd, &current_sockets);
    //         for (t_client *cli = list; cli; cli = cli->next)
    //         {
    //             FD_SET(cli->socket, &read_sockets);
    //             FD_SET(cli->socket, &write_sockets);
    //             if (cli->socket > max_socket)
    //                 max_socket = cli->socket;
    //         }
    //     }
    //     // ft_memcpy(&current_sockets, &read_sockets, sizeof(current_sockets));
    //     // ft_memcpy(&current_sockets, &write_sockets, sizeof(current_sockets));
        
    //     print_fd_set("READ", &read_sockets, max_socket);
    //     print_fd_set("WRITE", &write_sockets, max_socket);

    //     /* Selection handling */
    //     nb_fd = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
    //     if (nb_fd < 0)
    //         exit_fatal('>');
    //     else if (nb_fd > 0)
    //     {
    //         write(1, "#", 1);
    //         ft_putnbr(nb_fd);
    //         for (int i = 0; i < max_socket + 1; i++)
    //         {
    //             write(1, "***** ", 6);
    //             ft_putnbr(i);
    //             write(1, " ****\n", 6);
    //             if (FD_ISSET(i, &read_sockets))
    //             {
    //                 if (i == sockfd)// && connfd == -1)// && !get_client(list, connfd))//
    //                 {
    //                     len = sizeof(cli);
    //                     connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    //                     if (connfd < 0)
    //                         exit_fatal('A');
    //                     else
    //                         add_client(&list, connfd, nb_clients++);
    //                     // FD_SET(connfd, &current_sockets);
    //                     FD_SET(connfd, &read_sockets);
    //                     FD_SET(connfd, &write_sockets);
    //                     if (connfd > max_socket)
    //                         max_socket = connfd;
    //                     // connfd = -1;
    //                 }
    //                 else if (get_client(list, i) && FD_ISSET(i, &write_sockets))
    //                 {
    //                     write(1, "--> lez go ", 11);
    //                     // ft_putnbr(connfd);
    //                     // write(1, " -->\n", 5);
    //                     do_something(sockfd, get_client(list, i), &nb_clients);
    //                     // FD_CLR(i, &current_sockets);
    //                     FD_CLR(i, &read_sockets);
    //                     FD_CLR(i, &write_sockets);
    //                     close(i);
    //                     // connfd = -1;
    //                 }
    //             }
    //         }
    //     }
	// }
}