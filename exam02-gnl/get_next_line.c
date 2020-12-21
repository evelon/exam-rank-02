#include <unistd.h>
#include <stdlib.h>
#define BUFSIZE 32

int		ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strdup(char *str)
{
	char	*new;
	int		len;

	len = ft_strlen(str);
	if (!(new = (char *)malloc((len))))
		return(NULL);
	new[len + 1] = '\0';
	while (len >= 0)
	{
		new[len] = str[len];
		len--;
	}
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*new;
	int		i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (!(new = (char *)malloc(sizeof(len1 + len2 + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (i < len1)
	{
		new[i] = s1[i];
		i++;
	}
	while (i <= len1 + len2)
	{
		new[i] = s2[i - len1];
		i++;
	}
	return (new);
}

int		ft_ind_nl(char *str)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = 0;
	while (i < len)
	{
		if (str[i] == '\n')
			return (i);
	}
	return (-1);
}

char	*ft_push_str(char **line, char *str, int *check)
{
	int		ind;
	char	*temp;

	ind = ft_ind_nl(str);
	if (ind == -1)
	{
		temp = ft_strjoin(*line, str);
		if (*line != NULL)
			free(*line);
		*line = NULL;
		*line = temp;
		*check = 0;
		return (str);
	}
	str[ind] = '\0';
	temp = ft_strjoin(*line, str);
	if (*line != NULL)
		free(*line);
	*line = NULL;
	*line = temp;
	str += ind + 1;
	*check = 1;
	return (str);
}

int		get_next_line(char **line)
{
	static char	*backup = NULL;
	char		buffer[BUFSIZE + 1];
	int			check;
	int			ret;

	check = NULL;
	if (line == NULL)
		return (-1);
	buffer[BUFSIZE] = '\0';
	if (backup == NULL)
		backup = ft_strdup("");
	*line = ft_strdup("");
	backup = ft_push_str(line, backup, &check);
	if (check)
		return (1);
	if (backup != NULL)
		free(backup);
	backup = NULL;
	while ((ret = read(0, buffer, BUFSIZE)))
	{
		if (ret == BUFSIZE)
		{
			backup = ft_strdup(ft_push_str(line, buffer, check));
			if (check)
				return (1);
			if (backup != NULL)
				free(backup);
			backup = NULL;
			continue;
		}
		if (ret < BUFSIZE)
		{
			backup = ft_strdup(ft_push_str(line, buffer, check));
			if (check)
				return (1);
			if (backup != NULL)
				free(backup);
			backup = NULL;
			return (0);
		}
	}
	return (0);
}

int
