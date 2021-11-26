#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct      s_client
{
    int             id;
    int             socket;
    struct s_client *next;
}                   t_client;

void	ft_putnbr(int nb);

void	exit_fatal(char c);
int		extract_message(char **buf, char **msg);
char	*str_join(char *buf, char *add);
void	select_loop(int sockfd);
fd_set  tab_to_fd_set(int *tab);
int     *add_fd_to_tab(int fd, int *tab);
void print_fd_set(char *name, fd_set *set, int max_socket);
void print_current_set(int *tab);
void ft_memcpy(void *src, void *dest, int size);
void do_something(int sockfd, t_client *cli, int* nb_clients);
void client_action(const char *action, int client);

void add_client(t_client **list, int socket, int id);
t_client *get_client(t_client *list, int socket);
