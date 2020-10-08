#include <libc.h>
#include <dirent.h>

char			*ft_strcat(char *dest, char *src, int i)
{
	unsigned int j;

	j = 0;
	if (src == NULL)
		return (NULL);
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char			*ft_strcpy(char *dest, char *src)
{
	unsigned int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

static int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char			*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*dest;
	char	*str1;
	char	*str2;

	str1 = (char *)s1;
	str2 = (char *)s2;
	i = ft_strlen(str1);
	j = ft_strlen(str2);
	dest = malloc(sizeof(char) * (i + j + 1));
	if (!dest)
		return (NULL);
	dest = ft_strcpy(dest, str1);
	dest = ft_strcat(dest, str2, i);
	dest[i + j + 1] = '\0';
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
	char *actual;
	int rc;
	int ret;

	removedoublestars(&patern);
	actual = malloc(sizeof(char) * (strlen(str) + 1 ));
	actual[0] = 0;

	rc = strlen(str) - (strlen(patern) - numberstars(patern));
	ret = recursive(str, patern, rc, 0, 0);
	free(actual);
	return (ret);
}

int megastar(char *patern, char *path, char *minipath)
{
	DIR *dir;
	struct dirent *dirent;
	char *pathedname;
	char *tmp;

	dir = opendir(path);
	if (dir == NULL)
		return (1);
	while ((dirent = readdir(dir)) != NULL)
	{
		if (superstar(dirent->d_name, patern))
		{
			if (minipath)
			{
				pathedname = ft_strjoin(minipath, "/");
				tmp = pathedname;
				pathedname = ft_strjoin(pathedname, dirent->d_name);
				printf("%s ", pathedname);
				free(tmp);
				free(pathedname);
			}
			else
				printf("%s ", dirent->d_name);
		}
	}
	closedir(dir);
	return (0);
}

int recurdir(char *patern, char *path, char *minipath)
{
	DIR *dir;
	struct dirent *dirent;
	char *dirname;
	char *newpatern;
	char *newpath;
	char *tmp;

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
			newpatern = ft_substr(patern, srcchar('/', patern) + 1,
					ft_strlen(patern) - srcchar('/', patern) + 1);
			newpath = ft_strjoin(path, "/");
			tmp = newpath;
			newpath = ft_strjoin(newpath, dirent->d_name);
			free(tmp);
			if (minipath)
			{
				minipath = ft_strjoin(minipath, "/");
				tmp = minipath;
				minipath = ft_strjoin(minipath, dirent->d_name);
				free(tmp);
				recurdir(newpatern, newpath, minipath);
				free(minipath);
			}
			else
				recurdir(newpatern, newpath, dirent->d_name);
			free(newpatern);
			free(newpath);
		}
	}
	free(dirname);
	return (0);
}

int main(int ac, char **ag)
{
	char *patern;
	char *path;
	char *minipath;

	(void)ac;
	minipath = NULL;
	patern = ag[1];	
	path = malloc(sizeof(char) * 500);
	getcwd(path, 500);
	if (path == NULL)
	{
		printf("error : path too long\n");
		return (1);
	}
	recurdir(patern, path, minipath);
	free(path);
}
