/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arbernar <arbernar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 17:44:32 by arbernar          #+#    #+#             */
/*   Updated: 2021/10/27 17:31:04 by arbernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_ptr(char *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

static char	*extract_excess(char **stack, char **excess, char **line)
{
	int		i;

	i = 0;
	while ((*stack)[i] != '\n' && (*stack)[i] != '\0')
		i++;
	if (ft_strchr(*stack, '\n'))
	{
		*line = ft_substr(*stack, 0, i + 1);
		*excess = ft_strdup(&(*stack)[i + 1]);
	}
	else
	{
		*line = ft_strdup(*stack);
		*excess = NULL;
	}
	return (*line);
}

static int	read_stack(int fd, char **excess, char **stack, char **buffer)
{
	char	*temp;
	int		size_of_last_read;

	if (*excess)
	{
		*stack = ft_strdup(*excess);
		free(*excess);
		*excess = NULL;
	}
	else
		*stack = ft_strdup("");
	size_of_last_read = 1;
	while (!ft_strchr(*stack, '\n') && size_of_last_read)
	{
		size_of_last_read = read(fd, *buffer, BUFFER_SIZE);
		(*buffer)[size_of_last_read] = '\0';
		temp = *stack;
		*stack = ft_strjoin(*stack, *buffer);
		free_ptr(temp);
	}
	free_ptr(*buffer);
	return (size_of_last_read);
}

char	*get_next_line(int fd)
{
	static char	*excess[MAX_FD];
	char		*stack;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, excess, 0) || fd > MAX_FD)
		return (NULL);
	buffer = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!excess[fd])
		excess[fd] = ft_strdup("");
	if (read_stack(fd, &excess[fd], &stack, &buffer) == 0 && *stack == '\0')
	{
		free(stack);
		return (NULL);
	}
	extract_excess(&stack, &excess[fd], &line);
	free_ptr(stack);
	return (line);
}
