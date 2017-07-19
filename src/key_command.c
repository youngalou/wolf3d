/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/19 11:14:26 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/19 14:38:31 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"

int		key_command(int key, t_env *env)
{
	if (key == 53)
		exit(0);
	if (key == 123 || key == 124)
	{
		if (key == 123)
			env->player.dir.x += M_PI / SCALE / 2;
		if (key == 124)
			env->player.dir.x -= M_PI / SCALE / 2;
	}
	if (key == 13 || (key >= 0 && key <= 2))
	{
		if (key == 1 || key == 13)
			env->player.pos.y += ((key == 13) ? -4 : 4);
		if (key == 0 || key == 2)
			env->player.pos.x += ((key == 0) ? -4 : 4);
	}
	mlx_clear_window(env->mlx, env->win);
	ray_cast(env);
	return (0);
}
