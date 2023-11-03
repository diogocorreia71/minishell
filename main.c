#include "minishell.h"

void	built_in_cmd(char *cmd, char *parsed)
{
	char	*dir;

	if (ft_strncmp(cmd, "pwd", 4) == true)
	{
		dir = getcwd(NULL, 0);
		printf("%s\n", dir);
		free(dir);
	}
	else if (ft_strncmp(cmd, "echo", 5) == true)
		printf("%s\n", parsed);
	/* else if (ft_strcmp(cmd, "cd") == true)
	{

	} */
	else if (ft_strncmp(cmd, "exit", 5) == true)
		exit(EXIT_SUCCESS);
}

char	*divide_cmd(char *str, char *delim)
{
	char	*tmp;
	int		i;
	int		k;

	k = 0;
	i = 0;
	while (str[i] != *delim)
		i++;
	tmp = (char *)malloc(sizeof(char) * (i + 1));
	while (k < i)
	{
		tmp[k] = str[k];
		k++;
	}
	tmp[k] = '\0';
	return (tmp);
}

char	*cmd_path(char *str)
{
	char	*tmp;
	int		k;
	int		i;

	i = 0;
	while (str[i] != ' ' && str[i])
		i++;
	i++;
	k = ft_strlen(str);
	tmp = (char *)malloc(sizeof(char) * (k - i + 1));
	k = 0;
	while (str[i + k])
	{
		tmp[k] = str[i + k];
		k++;
	}
	tmp[k] = '\0';
	return (tmp);
}

void	parse_cmd(char *str, char ***cmd)
{

	*cmd = (char **)malloc(sizeof(char *) * 3);
	*cmd[0] = divide_cmd(str, " ");
	*cmd[1] = cmd_path(str);
	printf("%s\n", *cmd[1]);
}

int main()
{
	char	*cmd_string;
	char	**cmd;
	char	*parsed;
	int i;

	i = 0;

	while (1)
	{
		cmd_string = readline("minishell$ ");
		/* printf("%s\n", cmd); */
		add_history(cmd_string);
		parse_cmd(cmd_string, &cmd);
		/* printf("%s\n", cmd[0]);
		printf("%s\n", cmd[1]); */
		/* built_in_cmd(cmd, parsed); */
	}
}
