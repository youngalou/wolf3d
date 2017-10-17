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
	env->gun.anim = 0;
	env->gun.wait = 0;
	env->gun.shoot = 0;
	env->gun.ammo = AMMO;
	env->gun.reload = 0;
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
	env->gun.anim = 0;
	env->gun.wait = 0;
	env->gun.shoot = 0;
	env->gun.ammo = AMMO;
	env->gun.reload = 0;
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

char	*g_texfile[NUM_TEX];
char	*g_gunfile[33];

void	set_texfile(void)
{
	g_texfile[0] = NULL;
	g_texfile[1] = "assets/textures/wall1h.xpm";
	g_texfile[2] = "assets/textures/wall2h.xpm";
	g_texfile[3] = "assets/textures/wall3h.xpm";
	g_texfile[4] = "assets/textures/wall4v.xpm";
	g_texfile[5] = "assets/textures/you-won.xpm";
	g_texfile[6] = "assets/textures/skybox.xpm";
	g_gunfile[0] = "assets/weapons/auto-rifle_idle.xpm";
	g_gunfile[1] = "assets/weapons/auto-rifle_shoot1.xpm";
	g_gunfile[2] = "assets/weapons/auto-rifle_shoot2.xpm";
	g_gunfile[3] = "assets/weapons/auto-rifle_reload01.xpm";
	g_gunfile[4] = "assets/weapons/auto-rifle_reload02.xpm";
	g_gunfile[5] = "assets/weapons/auto-rifle_reload03.xpm";
	g_gunfile[6] = "assets/weapons/auto-rifle_reload04.xpm";
	g_gunfile[7] = "assets/weapons/auto-rifle_reload05.xpm";
	g_gunfile[8] = "assets/weapons/auto-rifle_reload06.xpm";
	g_gunfile[9] = "assets/weapons/auto-rifle_reload07.xpm";
	g_gunfile[10] = "assets/weapons/auto-rifle_reload08.xpm";
	g_gunfile[11] = "assets/weapons/auto-rifle_reload09.xpm";
	g_gunfile[12] = "assets/weapons/auto-rifle_reload10.xpm";
	g_gunfile[13] = "assets/weapons/auto-rifle_reload11.xpm";
	g_gunfile[14] = "assets/weapons/auto-rifle_reload12.xpm";
	g_gunfile[15] = "assets/weapons/auto-rifle_reload13.xpm";
	g_gunfile[16] = "assets/weapons/auto-rifle_reload14.xpm";
	g_gunfile[17] = "assets/weapons/auto-rifle_reload15.xpm";
	g_gunfile[18] = "assets/weapons/auto-rifle_reload16.xpm";
	g_gunfile[19] = "assets/weapons/auto-rifle_reload17.xpm";
	g_gunfile[20] = "assets/weapons/auto-rifle_reload18.xpm";
	g_gunfile[21] = "assets/weapons/auto-rifle_reload19.xpm";
	g_gunfile[22] = "assets/weapons/auto-rifle_reload20.xpm";
	g_gunfile[23] = "assets/weapons/auto-rifle_reload21.xpm";
	g_gunfile[24] = "assets/weapons/auto-rifle_reload22.xpm";
	g_gunfile[25] = "assets/weapons/auto-rifle_reload23.xpm";
	g_gunfile[26] = "assets/weapons/auto-rifle_reload24.xpm";
	g_gunfile[27] = "assets/weapons/auto-rifle_reload25.xpm";
	g_gunfile[28] = "assets/weapons/auto-rifle_reload26.xpm";
	g_gunfile[29] = "assets/weapons/auto-rifle_reload27.xpm";
	g_gunfile[30] = "assets/weapons/auto-rifle_reload28.xpm";
	g_gunfile[31] = "assets/weapons/auto-rifle_reload29.xpm";
	g_gunfile[32] = "assets/weapons/auto-rifle_reload30.xpm";
}

void	load_assets(t_env *env)
{
	int		i;

	i = 0;
	while (i++ < NUM_TEX - 1)
	{
		env->tex.img[i] = mlx_xpm_file_to_image(env->mlx, g_texfile[i], &env->tex.width, &env->tex.height);
		env->tex.arr[i] = (int *)mlx_get_data_addr(env->tex.img[i], &env->tex.bpp, &env->tex.sl, &env->tex.endian);
	}
	i = -1;
	while (i++ < 31)
	{
		env->weapon.img[i] = mlx_xpm_file_to_image(env->mlx, g_gunfile[i], &env->weapon.width, &env->weapon.height);
		env->weapon.arr[i] = (int *)mlx_get_data_addr(env->weapon.img[i], &env->weapon.bpp, &env->weapon.sl, &env->weapon.endian);
	}
}

void	init_tex(t_env *env)
{
	set_texfile();
	env->tex.bpp = 4;
	env->tex.sl = 0;
	env->tex.endian = 0;
	env->tex.width = TEX_RES;
	env->tex.height = TEX_RES;
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
