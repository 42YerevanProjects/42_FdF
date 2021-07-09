/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 18:33:05 by shovsepy          #+#    #+#             */
/*   Updated: 2021/07/09 18:33:08 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	count_values(char *line)
{
	int		len;

	len = 0;
	while (*line)
	{
		if (ft_isdigit(*line))
		{
			len += 1;
			while (ft_isdigit(*line))
				line += 1;
		}
		else if (*line != ' ' && *line != '-')
			ft_error("Invalid characters or read error...!!!", 5);
		line += 1;
	}
	return (len);
}

static void	count_lines(t_fdf *fdf, char *map_file)
{
	int		fd;
	int		len;
	int		rows;
	int		cols;
	char	*line;

	rows = 0;
	cols = 0;
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		ft_error("Error opening file!!!", 1);
	while (get_next_line(fd, &line) == 1)
	{
		if (*line == '\0')
			break ;
		len = count_values(line);
		if (len > cols)
			cols = len;
		(cols == len) ? rows += 1 : ft_error("Not a valid file!!!", 4);
		free(line);
	}
	if (close(fd) < 0)
		ft_error("Error closing file!!!", 2);
	fdf->map.width = cols;
	fdf->map.height = rows;
}

static void	get_values(t_fdf *fdf, int y, int z, char *line)
{
	int		i;
	char	**split;

	if ((split = ft_split(line, ' ')))
	{
		i = 0;
		while (split[i] && (y != fdf->map.width))
		{
			fdf->map.values[z][y] = ft_atoi(split[i++]);
			y += 1;
		}
		free(split);
	}
}

void	ft_read(char *map_file, t_fdf *fdf)
{
	int		y;
	int		z;
	int		fd;
	char	*line;

	y = 0;
	z = 0;
	count_lines(fdf, map_file);
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		ft_error("Error opening file!!!", 1);
	fdf->map.values = (int **)malloc(sizeof(int *) * fdf->map.height);
	if (!fdf->map.values)
		ft_error("Memory Allocation failed!!!", 3);
	while (get_next_line(fd, &line) == 1 && z != fdf->map.height)
	{
		fdf->map.values[z] = (int *)malloc(sizeof(int) * fdf->map.width);
		if (!fdf->map.values[z])
			ft_error("Memory Allocation failed! :O", 3);
		get_values(fdf, y, z, line);
		y = 0;
		z += 1;
		free(line);
	}
	if (close(fd) < 0)
		ft_error("Error closing file! WTF?! :)", 2);
}
