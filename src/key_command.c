/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/19 11:14:26 by lyoung            #+#    #+#             */
/*   Updated: 2017/08/22 11:18:38 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"

void	free_all(t_env *env)
{
	int		i;

	i = -1;
	while (i++ < NUM_TEX - 1)
	{
		free(env->tex.img[i]);
		free(env->tex.arr[i]);
	}
	i = -1;
	while (i++ < FRAMES - 1)
	{
		free(env->weapon.img[i]);
		free(env->weapon.arr[i]);
	}
	i = -1;
	while (i++ < env->map.length - 1)
		free(env->map.grid[i]);
	free(env->map.grid);
	free(env->win);
	free(env->img);
	free(env->pixels);
	free(env);
}

int		exit_hook(int key, t_env *env)
{
	(void)key;
	free_all(env);
	exit(0);
	return (0);
}

int		mouse_pos(int x, int y, t_env *env)
{
	if (!env->mouse.init)
	{
		env->mouse.init = 1;
		env->mouse.pos.x = x;
		env->mouse.pos.y = y;
		return (0);
	}
	y = 0;
	env->player.dir.x -= (x - env->mouse.pos.x) * MOUSE_SENS;
	if (env->player.dir.x < 0)
		env->player.dir.x += 2 * M_PI;
	else if (env->player.dir.x > 2 * M_PI)
		env->player.dir.x -= 2 * M_PI;
	if (!env->key.larr && !env->key.rarr && !env->key.w && !env->key.a && !env->key.s && !env->key.d)
		ray_cast(env);
	env->mouse.pos.x = x;
	env->mouse.pos.y = y;
	return (0);
}

int		key_press(int key, t_env *env)
{
	if (key == KEY_ESC)
		exit_hook(0, env);
	if (key == KEY_TAB)
		reset(env);
	if (key == KEY_LARR || key == KEY_RARR || key == KEY_W || (key >= KEY_A && key <= KEY_D) || key == KEY_SPACE || key == KEY_R)
	{
		if (key == KEY_LARR || key == KEY_RARR)
		{
			if (key == KEY_LARR)
				env->key.larr = 1;
			if (key == KEY_RARR)
				env->key.rarr = 1;
		}
		if (key == KEY_W || (key >= KEY_A && key <= KEY_D))
		{
			if (key == KEY_W)
				env->key.w = 1;
			if (key == KEY_A)
				env->key.a = 1;
			if (key == KEY_S)
				env->key.s = 1;
			if (key == KEY_D)
				env->key.d = 1;
		}
		if (key == KEY_SPACE)
			env->key.space = 1;
		if (key == KEY_R && !env->gun.shoot)
			env->gun.reload = 1;
	}
	if (key == KEY_M)
		print_grid(env);
	return (0);
}

int		key_release(int key, t_env *env)
{
	if (key == KEY_LARR || key == KEY_RARR || key == KEY_W || (key >= KEY_A && key <= KEY_D) || key == KEY_SPACE)
	{
		if (key == KEY_LARR || key == KEY_RARR)
		{
			if (key == KEY_LARR)
				env->key.larr = 0;
			if (key == KEY_RARR)
				env->key.rarr = 0;
		}
		if (key == KEY_W || (key >= KEY_A && key <= KEY_D))
		{
			if (key == KEY_W)
				env->key.w = 0;
			if (key == KEY_A)
				env->key.a = 0;
			if (key == KEY_S)
				env->key.s = 0;
			if (key == KEY_D)
				env->key.d = 0;
		}
		if (key == KEY_SPACE)
			env->key.space = 0;
	}
	return (0);
}

void	animations(t_env *env)
{
	if ((env->key.space || env->gun.shoot) && env->gun.ammo > 0 && !env->gun.reload)
	{
		if (env->key.space)
			env->gun.shoot = 1;
		env->gun.wait++;
		if (env->gun.wait % WAIT == 0)
		{
			env->gun.anim++;
			if (env->gun.anim >= SHOOT)
			{
				env->gun.ammo--;
				env->gun.shoot = 0;
				env->gun.anim = 0;
				env->gun.wait = 0;
				if (!env->gun.ammo && !env->won)
					env->flash = 0;
			}
		}
	}
	if (env->gun.reload && !env->gun.shoot)
	{
		if (env->gun.anim < 3)
			env->gun.anim = 3;
		env->gun.wait++;
		if (env->gun.wait % WAIT == 0)
		{
			env->gun.anim++;
			if (env->gun.anim >= (FRAMES - 1))
			{
				env->gun.ammo = AMMO;
				env->gun.reload = 0;
				env->gun.anim = 0;
				env->gun.wait = 0;
			}
		}
	}
}

int		forever_loop(t_env *env)
{
	int		move_x;
	int		move_y;

	if (env->key.larr || env->key.rarr)
	{
		if (env->key.larr || env->key.rarr)
		{
			if (env->key.larr)
				env->player.dir.x += TURN_SPEED;
			if (env->key.rarr)
				env->player.dir.x -= TURN_SPEED;
			while (env->player.dir.x > 2 * M_PI)
				env->player.dir.x -= 2 * M_PI;
			while (env->player.dir.x < 0)
				env->player.dir.x += 2 * M_PI;
		}
	}
	if (env->key.w || env->key.a || env->key.s || env->key.d)
	{
		move_x = cos(env->player.dir.x) * MOVE_SPEED;
		move_y = sin(env->player.dir.x) * MOVE_SPEED;
		if (env->key.w)
		{
			if (!check_grid(env, env->player.pos.x, env->player.pos.y - (COLLISION * move_y)))
				env->player.pos.y -= move_y;
			if (!check_grid(env, env->player.pos.x + (COLLISION * move_x), env->player.pos.y))
				env->player.pos.x += move_x;
		}
		if (env->key.s)
		{
			if (!check_grid(env, env->player.pos.x, env->player.pos.y + (COLLISION * move_y)))
				env->player.pos.y += move_y;
			if (!check_grid(env, env->player.pos.x - (COLLISION * move_x), env->player.pos.y))
				env->player.pos.x -= move_x;
		}
		move_x = cos(env->player.dir.x - (M_PI / 2)) * (MOVE_SPEED / 2);
		move_y = sin(env->player.dir.x - (M_PI / 2)) * (MOVE_SPEED / 2);
		if (env->key.a)
		{
			if (!check_grid(env, env->player.pos.x, env->player.pos.y + (COLLISION * move_y)))
				env->player.pos.y += move_y;
			if (!check_grid(env, env->player.pos.x - (COLLISION * move_x), env->player.pos.y))
				env->player.pos.x -= move_x;
		}
		if (env->key.d)
		{
			if (!check_grid(env, env->player.pos.x, env->player.pos.y - (COLLISION * move_y)))
				env->player.pos.y -= move_y;
			if (!check_grid(env, env->player.pos.x + (COLLISION * move_x), env->player.pos.y))
				env->player.pos.x += move_x;
		}
		if (env->player.pos.x / SCALE == env->map.exit.x && env->player.pos.y / SCALE == env->map.exit.y)
		{
			env->won = 1;
			env->flash = 0;
		}
		if (!env->gun.reload)
		{
			env->gun.height += (env->gun.move) ? -1 : 1;
			if (env->gun.height >= 325)
				env->gun.move = 1;
			else if (env->gun.height <= 300)
				env->gun.move = 0;
		}
	}
	if (env->key.space || env->gun.shoot || env->gun.reload)
		animations(env);
	ray_cast(env);
	return (0);
}