/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 11:41:21 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/28 14:14:33 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"

void	init_map(t_env *env, char *line)
{
	char	*tmp;

	tmp = line;
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
	while (*line >= '0' && *line <= '9')
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->map.exit.x = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->map.exit.y = ft_atoi(line);
	ft_strdel(&tmp);
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

void	reset(t_env *env)
{
	env->player.dir.x = ANGLE_START;
	env->player.dir.y = 0;
	env->key.w = 0;
	env->key.a = 0;
	env->key.s = 0;
	env->key.d = 0;
	env->key.uarr = 0;
	env->key.darr = 0;
	env->key.larr = 0;
	env->key.rarr = 0;
	env->key.space = 0;
	env->mouse.init = 0;
	env->won = 0;
	env->shoot = 0;
	env->anim = 0;
	env->wait = 0;
	ray_cast(env);
}

t_env	*init_env(void)
{
	t_env	*env;

	env = (t_env*)malloc(sizeof(t_env));
	env->map.grid = 0;
	env->map.width = 0;
	env->map.length = 0;
	env->player.dir.x = ANGLE_START;
	env->player.dir.y = 0;
	env->bpp = 4;
	env->sl = 0;
	env->endian = 0;
	env->drawn = 0;
	env->key.w = 0;
	env->key.a = 0;
	env->key.s = 0;
	env->key.d = 0;
	env->key.uarr = 0;
	env->key.darr = 0;
	env->key.larr = 0;
	env->key.rarr = 0;
	env->key.space = 0;
	env->mouse.init = 0;
	env->won = 0;
	env->shoot = 0;
	env->anim = 0;
	env->wait = 0;
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
			if (env->player.pos.x / SCALE == x && env->player.pos.y / SCALE == y)
				ft_printf("%{red}P %{eoc}");
			else if (env->map.grid[y][x] > 0 && env->map.grid[y][x] < 9)
				ft_printf("%{b_blue}s %{eoc}", " ");
			else if (env->map.exit.x == x && env->map.exit.y == y)
				ft_printf("%{b_green}s %{eoc}", " ");
			else
				ft_printf("  ");
			x++;
		}
		ft_putchar('\n');
		y++;
	}
}

void	set_texfile(t_env *env)
{
	env->texfile[0] = NULL;
	env->texfile[1] = "assets/textures/wall1h.xpm";
	env->texfile[2] = "assets/textures/wall2h.xpm";
	env->texfile[3] = "assets/textures/wall3h.xpm";
	env->texfile[4] = "assets/textures/wall4v.xpm";
	env->texfile[5] = "assets/textures/you-won.xpm";
	env->gunfile[0] = "assets/weapons/bolt-rifle1.xpm";
	env->gunfile[1] = "assets/weapons/bolt-rifle2.xpm";
	env->gunfile[2] = "assets/weapons/bolt-rifle3.xpm";
	env->gunfile[3] = "assets/weapons/bolt-rifle4.xpm";
	env->gunfile[4] = "assets/weapons/bolt-rifle5.xpm";
	env->gunfile[5] = "assets/weapons/bolt-rifle6.xpm";
	env->gunfile[6] = "assets/weapons/bolt-rifle7.xpm";
	env->gunfile[7] = "assets/weapons/bolt-rifle8.xpm";
	env->gunfile[8] = "assets/weapons/bolt-rifle9.xpm";
	env->gunfile[9] = "assets/weapons/bolt-rifle10.xpm";
	env->gunfile[10] = "assets/weapons/bolt-rifle11.xpm";
}

void	load_assets(t_env *env)
{
	int		i;

	i = 0;
	while (i++ < NUM_TEX - 1)
	{
		env->tex.img[i] = mlx_xpm_file_to_image(env->mlx, env->texfile[i], &env->tex.width, &env->tex.height);
		env->tex.str[i] = (unsigned char *)mlx_get_data_addr(env->tex.img[i], &env->tex.bpp, &env->tex.sl, &env->tex.endian);
	}
	i = -1;
	while (i++ < FRAMES - 1)
	{
		env->weapon.img[i] = mlx_xpm_file_to_image(env->mlx, env->gunfile[i], &env->weapon.width, &env->weapon.height);
		env->weapon.str[i] = (unsigned char *)mlx_get_data_addr(env->weapon.img[i], &env->weapon.bpp, &env->weapon.sl, &env->weapon.endian);
	}
}

void	init_tex(t_env *env)
{
	set_texfile(env);
	env->tex.bpp = 4;
	env->tex.sl = 0;
	env->tex.endian = 0;
	env->tex.width = TEX_W;
	env->tex.height = TEX_H;
	env->weapon.bpp = 4;
	env->weapon.sl = 0;
	env->weapon.endian = 0;
	env->weapon.width = GUN_W;
	env->weapon.height = GUN_H;
	load_assets(env);
}

void	open_mlx(t_env *env)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W, WIN_H, "WOLF3D");
	env->img = mlx_new_image(env->mlx, WIN_W, WIN_H);
	env->pixels = (int*)mlx_get_data_addr(env->img, &env->bpp, &env->sl, &env->endian);
	init_tex(env);
	ray_cast(env);
	mlx_hook(env->win, 2, 0, key_press, env);
	mlx_hook(env->win, 3, 0, key_release, env);
	mlx_hook(env->win, 6, 0, mouse_pos, env);
	mlx_hook(env->win, 17, 0, exit_hook, env);
	mlx_loop_hook(env->mlx, forever_loop, env);
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
	open_mlx(env);
	return (0);
}
