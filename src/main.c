/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 11:41:21 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/21 16:37:53 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"

void	init_map(t_env *env, char *line)
{
	env->map.width = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->map.length = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->player.pos.x = (ft_atoi(line) * SCALE) + (SCALE / 2);
	while (*line >= '0' && *line <= '9')
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->player.pos.y = (ft_atoi(line) * SCALE) + (SCALE / 2);
}

void	load_map(t_env *env, int fd)
{
	char	*line;
	int		y;
	int		x;

	get_next_line(fd, &line);
	init_map(env, line);
	env->map.grid = (int**)malloc(sizeof(int**) * env->map.length);
	y = 0;
	while (y < env->map.length)
	{
		env->map.grid[y] = (int*)malloc(sizeof(int*) * env->map.width);
		get_next_line(fd, &line);
		x = 0;
		while (x < env->map.width)
		{
			env->map.grid[y][x] = ft_atoi(line);
			while (*line >= '0' && *line <= '9')
				line++;
			while (*line == ' ' || *line == '\t')
				line++;
			x++;
		}
		y++;
	}
}

t_env	*init_env(void)
{
	t_env	*env;

	env = (t_env*)malloc(sizeof(t_env));
	env->map.grid = 0;
	env->map.width = 0;
	env->map.length = 0;
	env->player.dir.x = M_PI / 2;
	env->player.dir.y = 0;
	env->constant = SCALE * 277;
	env->bpp = 4;
	env->sl = 0;
	env->endian = 0;
	env->drawn = 0;
	return (env);
}

void	print_grid(t_env *env)
{
	int		y;
	int		x;

	y = 0;
	while (y < env->map.length)
	{
		x = 0;
		while (x < env->map.width)
		{
			ft_printf("%d ", env->map.grid[y][x]);
			x++;
		}
		ft_putchar('\n');
		y++;
	}
}

void	open_mlx(t_env *env)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W * 4, WIN_H * 4, "WOLF3D");
	env->img = mlx_new_image(env->mlx, WIN_W * 4, WIN_H * 4);
	env->pixels = (int*)mlx_get_data_addr(env->img, &env->bpp, &env->sl, &env->endian);
	ray_cast(env);
	mlx_hook(env->win, 2, 0, key_command, env);
	mlx_loop(env->mlx);
}

int		main(int argc, char **argv)
{
	t_env	*env;
	int		fd;

	env = NULL;
	if (argc != 2)
	{
		ft_printf("%{red}Invalid arguments.%{eoc}\nUsage:\t./wolf3d [map]\n");
		return (0);
	}
	env = init_env();
	fd = open(argv[1], O_RDONLY);
	load_map(env, fd);
	print_grid(env);
	open_mlx(env);
	return (0);
}
