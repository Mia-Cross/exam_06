#include "mini_serv.h"

void exit_fatal(char c)
{
	write(2, &c, 1);
	write(2, ".Fatal error\n", 13);
	exit(1);
}

int *add_fd_to_tab(int fd, int *tab)
{
	int i = 0;
	for ( ; tab[i]; i++)
		;
	int *newtab = malloc(sizeof(int*) * (i + 1));
	for (int j = 0; j < i; j++)
		newtab[j] = tab[j];
	newtab[i++] = fd;
	newtab[i] = -42;
	free(tab);
	return (newtab);
}

fd_set tab_to_fd_set(int *tab)
{
	fd_set newset;
	FD_ZERO(&newset);
	print_current_set(tab);
	for (int i = 0; tab[i] != -42; i++)
		FD_SET(i, &newset);
	return (newset);
}

int nb_len(int x) {
	int len = 0;
	while (x /= 10 && x > 0)
		len++;
	return len;
}

void client_action(const char *action, int client)
{
	write(1, "server: client ", 15);
	ft_putnbr(client);
	write(1, " just ", 6);
	write(1, action, strlen(action));
	// write(1, "\n", 1);
}

void ft_memcpy(void *src, void *dest, int size)
{
	unsigned char *s = src;
	unsigned char *d = dest;
	for (int i = 0; i < size; i++)
		d[i] = s[i];
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

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