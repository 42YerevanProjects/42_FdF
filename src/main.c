/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 17:36:19 by shovsepy          #+#    #+#             */
/*   Updated: 2021/07/12 16:46:10 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Display a usage message on how to run and use fdf.
*/

static void	fdf_usage(void)
{
	ft_putstr_fd("Usage:\n\t", 1);
	ft_putstr_fd("FDF", 1);
	ft_putstr_fd("fdf_file\n", 1);
	ft_putstr_fd("Controls:\n", 1);
	ft_putstr_fd("\t[Esc]        -> Exit the program.\n", 1);
	ft_putstr_fd("\t[R]          -> Reset the map.\n", 1);
	ft_putstr_fd("\t[M]          -> Colorize this shit!.\n", 1);
	ft_putstr_fd("\t[B][N]       -> Zoom in and zoom out.\n", 1);
	ft_putstr_fd("\t[J][K]       -> Spiky time!\n", 1);
	ft_putstr_fd("\t[Space]      -> Change the camera(isometric  \
		<-> plain).\n", 1);
	ft_putstr_fd("\t[W][A][S][D] -> Move the map.\n", 1);
}

/*
** Reset the map to the initial values.
*/

static void	reset_map(t_fdf *fdf)
{
	fdf->map.coordinate_z = 0;
	fdf->map.coordinate_y = 0;
	fdf->map.x_value = 1.00;
	fdf->map.angle_y = cos(M_PI / 3);
	fdf->map.angle_z = fdf->map.angle_y * sin(M_PI / 6);
	if (fdf->map.width > fdf->map.height)
		fdf->map.zoom = ceil((WIN_WIDTH / fdf->map.width) + MAGIC_ZOOM);
	else
		fdf->map.zoom = ceil((WIN_HEIGHT / fdf->map.height) + MAGIC_ZOOM);
	fdf->map.isometric = 1;
	fdf->color.red = 0x4F;
	fdf->color.green = 0x4F;
	fdf->color.blue = 0x4F;
}

/*
** Colorize the map with a random color.
** Colors go from 0x00 to 0x7F, because of technical issues (integer overflow).
*/

static void	random_color(t_fdf *fdf)
{
	fdf->color.red = (rand() % 0x7F);
	fdf->color.green = (rand() % 0x7F);
	fdf->color.blue = (rand() % 0x7F);
}

/*
** Assign a key code (macros defined in "includes/keys.h") to a specific task,
** like changing colors, moving the map, changing the view, zoom level, etc.
** There is an ugly code for changing the view (angle) of the map, because of
** norm... I guess? Everytime "map.isometric" isn't even, rotate the map.
*/

static int	fdf_keys(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_ESCAPE)
		ft_exit(fdf);
	else if (keycode == KEY_ANSI_R)
		reset_map(fdf);
	else if (keycode == KEY_ANSI_M)
		random_color(fdf);
	else if (keycode == KEY_ANSI_W || keycode == KEY_UPARROW)
		fdf->map.coordinate_z -= 1;
	else if (keycode == KEY_ANSI_S || keycode == KEY_DOWNARROW)
		fdf->map.coordinate_z += 1;
	else if (keycode == KEY_ANSI_D || keycode == KEY_RIGHTARROW)
		fdf->map.coordinate_y += 1;
	else if (keycode == KEY_ANSI_A || keycode == KEY_LEFTARROW)
		fdf->map.coordinate_y -= 1;
	else if (keycode == KEY_ANSI_B)
		fdf->map.zoom += 1;
	else if ((keycode == KEY_ANSI_N) && (fdf->map.zoom > MAX_ZOOM))
		fdf->map.zoom -= 1;
	else if ((keycode == KEY_ANSI_J) && (fdf->map.x_value < MAX_X))
		fdf->map.x_value += 0.25;
	else if ((keycode == KEY_ANSI_K) && (fdf->map.x_value > -MAX_X))
		fdf->map.x_value -= 0.25;
	else if (keycode == KEY_SPACE)
		key_space(fdf);
	return (0);
}

/*
** Main function, creates the fdf structure and a network connection between
** the program and the x-server, making it possible to draw on the screen,
** and getting the keyboard inputs.
** Didn't used mlx_key_hook, because I wanted to press and hold the keys :D
*/

int	main(int argc, char **argv)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)malloc(sizeof(t_fdf));
	if (argc == 2)
	{	
		if (!fdf)
			ft_error("Memory allocation failed", 3);
		ft_read(argv[1], fdf);
		reset_map(fdf);
		fdf->mlx.init = mlx_init();
		fdf->mlx.win = mlx_new_window(fdf->mlx.init, WIN_WIDTH, WIN_HEIGHT, \
			"FDF");
		mlx_hook(fdf->mlx.win, 2, 3, fdf_keys, fdf);
		mlx_loop_hook(fdf->mlx.init, ft_draw, fdf);
		mlx_loop(fdf->mlx.init);
	}
	else
		fdf_usage();
	 ft_free((void **) fdf->map.values);
	 free(fdf);
	return (0);
}
