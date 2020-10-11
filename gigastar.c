#include <libc.h>
#include <dirent.h>

char *recurdir(char *patern, char *path, char *minipath);

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return((unsigned char)s1[i] - s2[i]);
		i++;
	}
	if ((s1[i] && !s2[i]) || (!s1[i] && s2[i]))
		return((unsigned char)s1[i] - s2[i]);
	return (0);
}

int	ft_strcmpdsm(char *s1, char *s2)
{
	unsigned int i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return((unsigned char)s1[i] - s2[i]);
		i++;
	}
	if (!s1[i] && s2[i] == '.')
		return (1);
	if (!s2[i] && s1[i] == '.')
		return (-1);
	if ((s1[i] && !s2[i]) || (!s1[i] && s2[i]))
		return((unsigned char)s1[i] - s2[i]);
	return (0);
}
typedef struct	s_list
{
	char *name;
	struct s_list *next;
}				t_list;

t_list *new_maillon(t_list *actual, char *name)
{
	t_list *list;

		list = malloc(sizeof(t_list));
		list->name = name;
		list->next = actual;
		return(list);
}

void free_list(t_list *a)
{
	if (a)
	{
		free_list(a->next);
		free(a);
	}
}

void sort_list(t_list *actual)
{
	t_list *a;
	t_list *b;
	char *tmp;

	if (!actual || !actual->next)
		return;
	a = actual;
	while (a)
	{
		b = a->next;
		while (b)
		{
			if (ft_strcmp(a->name, b->name) > 0)
			{
				tmp = a->name;
				a->name = b->name;
				b->name = tmp;
			}
			b = b->next;
		}
		a = a->next;
	}
}

void sort_list_dsm(t_list *actual)
{
	t_list *a;
	t_list *b;
	char *tmp;

	if (!actual || !actual->next)
		return;
	a = actual;
	while (a)
	{
		b = a->next;
		while (b)
		{
			if (ft_strcmpdsm(a->name, b->name) > 0)
			{
				tmp = a->name;
				a->name = b->name;
				b->name = tmp;
			}
			b = b->next;
		}
		a = a->next;
	}
}
int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str && str[i] != '\0')
		i++;
	return (i);
}

char			*ft_strjoin_sep(char *s1, char *s2, char c)
{
	int		i;
	int		x;
	char	*dest;

	i = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(sizeof(char) * (i + 2));
	if (!dest)
		return (NULL);
	x = 0;
	while (s1 && s1[x])
	{
		dest[x] = s1[x];
		x++;
	}
	if (c)
	{
		dest[x] = c;
		x++;
	}
	i = 0;
	while (s2 && s2[i])
	{
		dest[x] = s2[i];
		x++;
		i++;
	}
	dest[x] = 0;
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*ptr;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
		i++;
	if (!(ptr = malloc((len + 1) * sizeof(char))))
		return (NULL);
	if (i <= start)
	{
		ptr[0] = 0;
		return (ptr);
	}
	i = 0;
	while (i < len && s[start + i])
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

int srcchar(char c, char *str)
{
	int x;

	x = 0;
	while (str[x])
	{
		if (str[x] == c)
			return (x);
		x++;
	}
	return (-1);
}

void removechar(char **str, int x)
{
	while ((*str)[x + 1])
	{
		(*str)[x] = (*str)[x + 1];
		x++;
	}
	(*str)[x] = 0;
}

void removedoublestars(char **str_p)
{
	char *str = *str_p;

	int x = 0;
	while (str && str[x] && str[x + 1])
	{
		if (str[x] == '*' && str[x + 1] == '*')
			removechar(str_p, x + 1);
		else
			x++;
	}
}

int numberstars(char *str)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (str && str[x])
	{
		if (str[x] == '*')
			y++;
		x++;
	}
	return (y);
}

int recursive(char * str, char * patern, int rc, int xp, int xs)
{
	int i;
	
	if (xs == 0 && xp == 0 && str[xs] == '.' && str[xs] != patern[xp])
		return (0);
	if (!str[xs] && (!patern[xp] || rc == 0))
		return (1);
	else if (!str[xs])
		return (0);
	if (patern[xp] != '*')
	{
		if (str[xs] == patern[xp])
			return (recursive(str, patern, rc, xp + 1, xs + 1));
		else
			return (0);
	}
	else
	{
		i = rc;
		while (i > -1)
		{
			if (recursive(str, patern, rc - i, xp +1, xs + i))
				return (1);
			i--;
		}
		return (0);
	}
}

int superstar(char *str, char *patern)
{
	int rc;
	int ret;

	removedoublestars(&patern);
	rc = strlen(str) - (strlen(patern) - numberstars(patern));
	ret = recursive(str, patern, rc, 0, 0);
	return (ret);
}

void print_list(t_list *actual, char *minipath)
{
	char *pathedname;

	while (actual)
	{
			if (minipath)
			{
				pathedname = ft_strjoin_sep(minipath, actual->name, '/');
				printf("%s ", pathedname);
				free(pathedname);
			}
			else
				printf("%s ", actual->name);

		actual = actual->next;
	}
}

int megastar(char *patern, char *path, char *minipath)
{
	DIR *dir;
	struct dirent *dirent;
	(void)minipath;
	t_list *list;
	t_list *actual;

	actual = NULL;
	dir = opendir(path);
	if (dir == NULL)
		return (1);
	while ((dirent = readdir(dir)) != NULL)
	{
		if (superstar(dirent->d_name, patern))
		{
			list = malloc(sizeof(t_list));
			list->name = dirent->d_name;
			list->next = actual;
			actual = list;
		}
	}
	sort_list(actual);
	print_list(actual, minipath);
	free_list(actual);
	closedir(dir);
	return (0);
}

void throughdir(char *minipath, char *patern, char *path, t_list *a)
{
	while (a)
	{
		char *newpatern;
		char *newpath;
		(void)patern;
		(void)path;

		newpatern = ft_substr(patern, srcchar('/', patern) + 1,
		ft_strlen(patern) - srcchar('/', patern) + 1);
		newpath = ft_strjoin_sep(path, a->name, '/');
		if ( minipath)
		{
			minipath = ft_strjoin_sep(minipath, a->name, '/');
			recurdir(newpatern, newpath, minipath);
			free(minipath);
		}
		else 
			recurdir(newpatern, newpath, a->name);
		free(newpatern);
		free(newpath);
		a = a->next;
	}
}



char	*recurdir(char *patern, char *path, char *minipath)
{
	DIR *dir;
	struct dirent *dirent;
	char *dirname;
	t_list *actual;
	char *ret;

	actual = NULL;
	if (srcchar('/', patern) == -1)
		return(megastar(patern, path, minipath));
	dir = opendir(path);
	if (dir == NULL)
		return (1);
	dirname = ft_substr(patern, 0, srcchar('/', patern));
	while ((dirent = readdir(dir)) != NULL)
	{
		if (dirent->d_type == 4 && superstar(dirent->d_name, dirname))
		{
			actual = new_maillon(actual, dirent->d_name);
		}
	}
	sort_list_dsm(actual);
	ret = throughdir(minipath, patern, path, actual);
	free_list(actual);
	free(dirname);
	return (ret);
}

int main(int ac, char **ag)
{
	char *patern;
	char *path;
	char *minipath;
	char *ret;

	(void)ac;
	minipath = NULL;
	patern = ag[1];	
	path = malloc(sizeof(char) * 500);
	getcwd(path, 500);
	if (path == NULL)
		return (1);
	ret = recurdir(patern, path, minipath);
	free(path);
}
