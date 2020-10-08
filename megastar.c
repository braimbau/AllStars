#include <libc.h>
#include <dirent.h>

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

int main(int ac, char **ag)
{
	DIR *dir;
	struct dirent *dirent;
	char *patern;

	(void)ac;
	patern = ag[1];
	char *pwd = malloc(sizeof(char) * 500);
	pwd = getcwd(pwd, 500);
	if (pwd == NULL)
	{
		printf("error : path too long\n");
		return (1);
	}
	dir = opendir(pwd);
	if (dir == NULL)
		return (1);
	while ((dirent = readdir(dir)) != NULL)
	{
		if (superstar(dirent->d_name, patern))
			printf("%s\n", dirent->d_name);
	}
	closedir(dir);
	free(pwd);
}
