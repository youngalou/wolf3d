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

int		key_press(int key, t_env *env)
{
	if (key == KEY_ESC)
		exit(0);
	if (key == KEY_TAB)
	{
		env->player.dir.x = 3 * M_PI / 2;
		ray_cast(env);
	}
	if (key == KEY_LARR || key == KEY_RARR || key == KEY_W || (key >= KEY_A && key <= KEY_D))
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
	}
	if (key == KEY_M)
	{
		print_grid(env);
	}
	return (0);
}

int		key_release(int key, t_env *env)
{
	if (key == KEY_LARR || key == KEY_RARR || key == KEY_W || (key >= KEY_A && key <= KEY_D))
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
	}
	return (0);
}

int		exit_hook(int key, t_env *env)
{
	(void)key;
	(void)env;
	exit(0);
	return (0);
}

int		forever_loop(t_env *env)
{
	int		move_x;
	int		move_y;

	if (env->key.larr || env->key.rarr || env->key.w || env->key.a || env->key.s || env->key.d)
	{
		if (env->key.larr || env->key.rarr)
		{
			if (env->key.larr)
				env->player.dir.x += TURN_SPEED;
			if (env->key.rarr)
				env->player.dir.x -= TURN_SPEED;
			if (env->player.dir.x > 2 * M_PI)
				env->player.dir.x -= 2 * M_PI;
			else if (env->player.dir.x < 0)
				env->player.dir.x += 2 * M_PI;
		}
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
		move_x = cos(env->player.dir.x - (M_PI / 2)) * MOVE_SPEED;
		move_y = sin(env->player.dir.x - (M_PI / 2)) * MOVE_SPEED;
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
		if (env->mouse == 0)
			ray_cast(env);
	}
	return (0);
}

int		mouse_pos(int x, int y, t_env *env)
{
	y = 0;
	env->player.dir.x = -x * (2 * M_PI / WIN_W);
	if (!env->key.larr && !env->key.rarr && !env->key.w && !env->key.a && !env->key.s && !env->key.d)
	{
		env->mouse = 1;
		ray_cast(env);
	}
	else
		env->mouse = 0;
	return (0);
}