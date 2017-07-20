/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/19 11:14:26 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/19 17:00:14 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"

int		key_command(int key, t_env *env)
{
	if (key == 53)
		exit(0);
	if (key == 123 || key == 124 || key == 13 || (key >= 0 && key <= 2))
	{
		if (key == 123 || key == 124)
		{
			if (key == 123)
				env->player.dir.x += M_PI / SCALE;
			else if (key == 124)
				env->player.dir.x -= M_PI / SCALE;
			if (env->player.dir.x > 2 * M_PI)
				env->player.dir.x -= 2 * M_PI;
			else if (env->player.dir.x < 0)
				env->player.dir.x += 2 * M_PI;
		}
		else if (key == 13 || (key >= 0 && key <= 2))
		{
			if (key == 13)
			{
				env->player.pos.y -= sin(env->player.dir.x) * SPEED;
				env->player.pos.x += cos(env->player.dir.x) * SPEED;
			}
			else if (key == 1)
			{
				env->player.pos.y += sin(env->player.dir.x) * SPEED;
				env->player.pos.x -= cos(env->player.dir.x) * SPEED;
			}
			else if (key == 2)
			{
				env->player.pos.y -= sin(env->player.dir.x - (M_PI / 2)) * SPEED;
				env->player.pos.x += cos(env->player.dir.x - (M_PI / 2)) * SPEED;
			}
			else if (key == 0)
			{
				env->player.pos.y += sin(env->player.dir.x - (M_PI / 2)) * SPEED;
				env->player.pos.x -= cos(env->player.dir.x - (M_PI / 2)) * SPEED;
			}
		}
		mlx_clear_window(env->mlx, env->win);
		ray_cast(env);
	}
	return (0);
}
