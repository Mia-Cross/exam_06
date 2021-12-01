#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	ft_putnbr(int nb)
{
	char c;
	long p;

	if (nb < 0)
	{
		if (nb == -2147483648)
		{
			write(1, "-2147483648", 11);
			return ;
		}
		write(1, "-", 1);
		p = -1 * nb;
	}
	if (nb >= 0)
		p = nb;
	if (p > 9)
		ft_putnbr(p / 10);
	c = p % 10 + 48;
	write(1, &c, 1);
	// write(1, "\n", 1);
}

void print_fd_set(char *name, fd_set *set, int max_socket)
{
    write(1, name, strlen(name));
    write(1, "_SET = [ ", 9);
    for (int i = 0; i < max_socket + 1; i++)
    {
        if (FD_ISSET(i, set))
        {
            ft_putnbr(i);
            write(1, ", ", 2);
        }
    }
    write(1, "]\n", 2);
}

void print_bytes_recv(int author_id, ssize_t bytes_recv, ssize_t bytes_recv_total)
{
	write(1, "Client ", 7);
	ft_putnbr(author_id);
	write(1, " received ", 10);
	ft_putnbr(bytes_recv_total);
	write(1, ", returned ", 11);
	ft_putnbr(bytes_recv);
	write(1, "|", 1);
}

void display_cli_id(int connfd, int id)
{
	char buf[30];
	bzero(buf, 30);
	int ret = sprintf(buf, "Hi mate, you are number %d !\n", id);
	send(connfd, buf, ret, 0);
}

void print_writeable_socket(int id)
{
	write(1, "Socket c-", 9);
	ft_putnbr(id);
	write(1, " became writable\n", 17);
}

// void print_current_set(int *tab)
// {
//     write(1, "CURRENT_SET = [ ", 16);
//     for (int i = 0; tab[i] != -42; i++)
//     {
//         ft_putnbr(i);
//         write(1, ", ", 2);
//     }
//     write(1, "]\n", 2);
// }
