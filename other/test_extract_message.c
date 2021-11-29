#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        // write(1, (*buf)[i], 1);
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

int main()
{
    char *msg = NULL;
    char *buf = malloc(100);
    strcpy(buf, "this is my header\nthis is a second sentence\nhere's the third\0");

    printf("BUF at the beginning <%p> = [%s]\nMSG at the beginning <%p> = [%s]\n", buf, buf, msg, msg);
    extract_message(&buf, &msg);
    printf("BUF after extraction <%p> = [%s]\nMSG after extraction <%p> = [%s]\n", buf, buf, msg, msg);
}

// BUF after extraction <0x6040000003d0> = [this is a second sentence
// here's the third]
// MSG after extraction <0x60b000000040> = [this is my header
// ]