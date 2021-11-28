#include "mini_serv.h"

void	select_loop(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd, max_socket = sockfd, id = 0;
    fd_set              read_sockets, write_sockets;
	struct sockaddr_in  cli;
    t_client            *list = NULL;
    char                *msg = NULL;

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
        // print_fd_set("READ", &read_sockets, max_socket);
        // print_fd_set("WRITE", &write_sockets, max_socket);
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

        //check if someone sent something to the server
        if (FD_ISSET(sockfd, &write_sockets)) 
        {
            write(1, "***************************************\n", 40);
            if (read_from_master_socket(sockfd, &msg) == 0)
                send_to_all(list, msg, strlen(msg));
            else
                write(1, "read_message did not return 0\n", 30);
        }

        //check the other sockets for activity
        for (t_client *cli = list; cli; cli = cli->next)
        {
            if (FD_ISSET(cli->socket, &read_sockets))
            {
                // write(1, "msg modifie !\n", 14);
                // read_from_client(cli->socket, &msg, cli->id);
                if (read_from_client(cli->socket, &msg, cli->id) == 0)
                    // send(cli->socket, msg, strlen(msg), 0);
                {
                    client_action(cli, "leftR\n", list, sockfd);
                    FD_CLR(cli->socket, &read_sockets);
                    FD_CLR(cli->socket, &write_sockets);
                    close(cli->socket);
                    remove_client(&list, cli->socket);
                    break;
                }
            }
            if (FD_ISSET(cli->socket, &write_sockets) && msg)
            {
                send(cli->socket, msg, strlen(msg), 0);
                free(msg);
                msg = NULL;
            }
        }
    }
}