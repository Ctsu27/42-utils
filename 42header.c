/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42header.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kehuang <kehuang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/07 11:06:52 by kehuang           #+#    #+#             */
/*   Updated: 2018/07/07 11:06:54 by kehuang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define N_AUTHOR_MAX	5

typedef struct	s_auth
{
	int		n;
	char	*name[N_AUTHOR_MAX + 1];
}				t_auth;

typedef struct	s_env
{
	t_auth	aut;
}				t_env;

static void	del_env(t_env *e)
{
	for (int q = 0; q < e->aut.n; q++)
		free(e->aut.name[q]);
}

static char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	end;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	end = start + len;
	if ((dest = (char *)malloc(sizeof(*dest) * (len + 1))) == NULL)
		return (NULL);
	while (start < end)
	{
		dest[i] = s[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}

static void	ft_putstr(char const *str)
{
	int		i = 0;

	while (str[i] != '\0')
		i++;
	while (i > 0)
	{
		write(1, str, (i > 4096) ? 4096 : i);
		i -= (i > 4096) ? 4096 : i;
	}
}

static void	ft_putendl(char const *str)
{
	ft_putstr(str);
	write(1, "\n", 1);
}

static void	usage(char const *name_prog)
{
	ft_putstr("usage: ");
	ft_putstr(name_prog);
	ft_putstr(" <author_file> [file.c file.h] [...]\n");
	ft_putstr("pro tips -> *.[ch] => path/**/*.[ch]\n");
	exit(EXIT_FAILURE);
}

static void	ft_exit(char const *str)
{
	ft_putstr(str);
	exit(EXIT_FAILURE);
}

static void	panic(void)
{
	ft_putendl(strerror(errno));
	exit(EXIT_FAILURE);
}

static void	get_author(t_env *e, char *path)
{
	int		fd;

	if ((fd = open(path, O_RDONLY)) == -1)
		panic();
	char	buf[501] = {0};
	int		byte;

	if ((byte = read(fd, buf, 500)) == -1)
		panic();
	int		i = 0;
	int		j = 0;

	int		total_name = 0;
	char	act = 1;

	while (i < byte)
	{
		if (buf[i] == '\n')
		{
			if ((e->aut.name[e->aut.n] = ft_strsub(buf, j, i - j)) == NULL)
				ft_exit("error: malloc\n");
			e->aut.n++;
			j = i + 1;
			act = 1;
		}
		else
		{
			if (act == 1)
				total_name++;
			act = 0;
		}
		i++;
	}
	close(fd);
	if (total_name != e->aut.n)
		ft_exit("error: number of author not matching with new line\n");
}

char	check_header_author(t_env *e, char *file);

int		main(int argc, char *argv[])
{
	if (argc < 3)
		usage(argv[0]);
	int		tmp;
	char	*tmp2;

	for (int p = 2; p < argc; p++) {
		tmp2 = strrchr(argv[p], '/');
		tmp2 = (tmp2 == NULL) ? argv[p] : tmp2 + 1;
		if ((tmp = strlen(argv[p])) < 2
				|| ((argv[p][tmp - 2] != '.' && argv[p][tmp - 1] != 'c' && argv[p][tmp - 1] != 'h')
				&& strncmp(tmp2, "Makefile", strlen("Makefile")) != 0))
			ft_exit("error: not Makefile || file.[ch]\n");
	}
	char	*name_file;

	name_file = strrchr(argv[1], '/');
	name_file = (name_file == NULL) ? argv[1] : name_file + 1;
	if (strcmp(name_file, "auteur") != 0 && strcmp(name_file, "author") != 0)
		ft_exit("argv[1] is not well named\n");
	t_env	e;

	for (int i = 0; i < 4; i++)
		e.aut.name[i] = NULL;
	e.aut.name[N_AUTHOR_MAX] = 0;
	e.aut.n = 0;

	get_author(&e, argv[1]);

	int		ret;

	for (int i = 2; i < argc; i++)
		if ((ret = check_header_author(&e, argv[i])) == 0)
	{
		ft_putstr(argv[i]);
		ft_putstr("\033[0;31m");
		ft_putstr(" is not matching name with the author file\n");
		ft_putstr("\033[0m");
	}
	else if (ret == 1)
	{
		ft_putstr(argv[i]);
		ft_putstr("\033[0;32m");
		ft_putstr(" has a valid name header\n");
		ft_putstr("\033[0m");
	}
	else if (ret == -1)
	{
		ft_putstr(argv[i]);
		ft_putstr("\033[0;35m");
		ft_putstr(" could not open/read\n");
		ft_putstr("\033[0m");
	}
	else if (ret == -2)
	{
		ft_putstr(argv[i]);
		ft_putstr("\033[0;31m");
		ft_putstr(" not enougth char to have a header\n");
		ft_putstr("\033[0m");
	}
	del_env(&e);
	return (EXIT_SUCCESS);
}

/*
**	[414] == 1st name;
**	[604] == 2nd name;
**	[685] == last name;
*/

char	check_header_author(t_env *e, char *file)
{
	int		fd;

	if ((fd = open(file, O_RDONLY)) == -1)
		return (-1);
	int		bytes;
	char	buf[892] = {0};

	if ((bytes = read(fd, buf, 891)) == -1)
		return (-1);
	if (bytes != 891)
		return (-2);

	unsigned char	ret[3] = {0, 0, 0};

	for (int cur = 0; cur < e->aut.n; cur++)
		ret[0] = ret[0] | (strncmp(e->aut.name[cur], buf + 414,
					strlen(e->aut.name[cur])) == 0);
	if (ret[0] == 0)
		return (0);

	for (int cur = 0; cur < e->aut.n; cur++)
		ret[1] = ret[1] | (strncmp(e->aut.name[cur], buf + 604,
					strlen(e->aut.name[cur])) == 0);
	if (ret[1] == 0)
		return (0);

	for (int cur = 0; cur < e->aut.n; cur++)
		ret[2] = ret[2] | (strncmp(e->aut.name[cur], buf + 685,
					strlen(e->aut.name[cur])) == 0);
	if (ret[2] == 0)
		return (0);

	return (1);
}
