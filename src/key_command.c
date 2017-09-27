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

int		key_command(int key, t_env *env)
{
	int		move_x;
	int		move_y;

	if (key == 53)
		exit(0);
	if (key == 123 || key == 124 || key == 13 || (key >= 0 && key <= 2))
	{
		if (key == 123 || key == 124)
		{
			if (key == 123)
				env->player.dir.x += TURN_SPEED;
			else if (key == 124)
				env->player.dir.x -= TURN_SPEED;
			if (env->player.dir.x > 2 * M_PI)
				env->player.dir.x -= 2 * M_PI;
			else if (env->player.dir.x < 0)
				env->player.dir.x += 2 * M_PI;
		}
		else if (key == KEY_W || (key >= KEY_A && key <= KEY_D))
		{
			move_x = cos(env->player.dir.x) * MOVE_SPEED;
			move_y = sin(env->player.dir.x) * MOVE_SPEED;
			if (key == KEY_W)
			{
				if (!check_grid(env, env->player.pos.x, env->player.pos.y - move_y))
					env->player.pos.y -= move_y;
				if (!check_grid(env, env->player.pos.x + move_x, env->player.pos.y))
					env->player.pos.x += move_x;
			}
			if (key == KEY_S)
			{
				if (!check_grid(env, env->player.pos.x, env->player.pos.y + move_y))
					env->player.pos.y += move_y;
				if (!check_grid(env, env->player.pos.x - move_x, env->player.pos.y))
					env->player.pos.x -= move_x;
			}
			move_x = cos(env->player.dir.x - (M_PI / 2)) * MOVE_SPEED;
			move_y = sin(env->player.dir.x - (M_PI / 2)) * MOVE_SPEED;
			if (key == KEY_A)
			{
				if (!check_grid(env, env->player.pos.x, env->player.pos.y + move_y))
					env->player.pos.y += move_y;
				if (!check_grid(env, env->player.pos.x - move_x, env->player.pos.y))
					env->player.pos.x -= move_x;
			}
			if (key == KEY_D)
			{
				if (!check_grid(env, env->player.pos.x, env->player.pos.y - move_y))
					env->player.pos.y -= move_y;
				if (!check_grid(env, env->player.pos.x + move_x, env->player.pos.y))
					env->player.pos.x += move_x;
			}
		}
		//mlx_clear_window(env->mlx, env->win);
		ray_cast(env);
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
