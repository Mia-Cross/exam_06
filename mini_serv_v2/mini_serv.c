/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   given_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 06:14:04 by charmstr          #+#    #+#             */
/*   Updated: 2021/06/27 06:14:20 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLI 32000

void display_cli_id(int connfd, int id);

void exit_error(char *error)
{
	write(1, error, strlen(error));
	write(1, "\n", 1);
	exit(1);
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	free(*msg);
	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void send_to_all(int *cli_fd, int id, char **msg, int cli_max)
{
	if (!*msg)
		return;
	for (int i = 0; i < cli_max; i++)
	{
		if (cli_fd[i] && i != id)
			send(cli_fd[i], *msg, strlen(*msg), 0);
	}
	free(*msg);
	*msg = NULL;
}

void client_action(int *cli, int id, char *action, int cli_max)
{
	char *str = malloc(strlen(action) + strlen("client 2147483647 just ") + 1);
	if (!str)
		exit_error("Fatal error");
	int last = sprintf(str, "client %d: ", id);
	strcpy(str + last, action);
	send_to_all(cli, id, &str, cli_max);
}

char *put_prefix(char *str, int id)
{
	char *msg = malloc(strlen(str) + strlen("client 2147483647: ") + 1);
	if (!msg)
		exit_error("Fatal error");
	int last = sprintf(msg, "client %d: ", id);
	strcpy(msg + last, str);
	free(str);
	return (msg);
}

int receive_message(int socket, char **msg, int id)
{
	int bytes_recv = 1;
	char *buf = malloc(101);
	char *tmpmsg = NULL;

	if (!buf)
		exit_error("Fatal error");
	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(socket, buf, 100, MSG_DONTWAIT);
		// buf = put_prefix(buf, id);
		tmpmsg = str_join(tmpmsg, buf);
	}
	printf("*msg = |%s|\n", *msg);
	while (extract_message(&tmpmsg, &buf))
	{
		buf = put_prefix(buf, id);
		*msg = str_join(*msg, buf);
	}
	free(buf);
	printf("*msg = |%s|\n", *msg);
	if (tmpmsg && *tmpmsg != 0)
	{
		tmpmsg = put_prefix(tmpmsg, id);
		*msg = str_join(*msg, tmpmsg);
	}
	free(tmpmsg);
	printf("*msg = |%s|\n", *msg);
	return (bytes_recv);
}

int main(int ac, char **av)
{
	int sockfd, connfd, id = 0, max_socket = 0;
	unsigned int len;
	struct sockaddr_in servaddr, cli;
	int cli_fd[MAX_CLI];
	char *msg = NULL;
	fd_set read_sockets, write_sockets;

	if (ac != 2)
		exit_error("Wrong number of arguments");

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		exit_error("Fatal error");
	bzero(&servaddr, sizeof(servaddr));
	bzero(&cli_fd, sizeof(cli_fd));


	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		exit_error("Fatal error");
	if (listen(sockfd, 10) != 0)
		exit_error("Fatal error");


	while (1)
	{
		FD_ZERO(&read_sockets);
		FD_ZERO(&write_sockets);
		FD_SET(sockfd, &read_sockets);
		for (int i = 0; i < id; i++)
		{
			if (cli_fd[i])
			{
				FD_SET(cli_fd[i], &read_sockets);
				FD_SET(cli_fd[i], &write_sockets);
			}
		}

		if (select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL) == -1)
			exit_error("Fatal error");

		write(1, "gne", 3);
		if (FD_ISSET(sockfd, &read_sockets))
		{
			len = sizeof(cli);
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
			if (connfd < 0)
				exit_error("Fatal error");
			cli_fd[id++] = connfd;
			client_action(cli_fd, id - 1, "arrived\n", id);
			display_cli_id(connfd, id - 1);
		}

		for (int i = 0; i < id; i++)
		{
			if (FD_ISSET(cli_fd[i], &read_sockets))
			{
				if (receive_message(cli_fd[i], &msg, i) == 0)
				{
					send_to_all(cli_fd, i, &msg, id);
					client_action(cli_fd, i, "left\n", id);
					FD_CLR(cli_fd[i], &read_sockets);
					FD_CLR(cli_fd[i], &write_sockets);
					close(cli_fd[i]);
					cli_fd[i] = 0;
				}
			}
			if (FD_ISSET(cli_fd[i], &write_sockets))
				send_to_all(cli_fd, i, &msg, id);
		}
	}
	
	close(sockfd);
	return (0);
}
