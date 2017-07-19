/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 11:10:14 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/19 14:26:58 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"
#include <stdio.h>

int		check_grid(t_env *env, int x, int y)
{
	x = x / SCALE;
	y = y / SCALE;
	if (x >= env->map.width || y >= env->map.length || x < 0 || y < 0)
		return (1);
	return (env->map.grid[y][x]);
}

int		horizontal_ray(t_env *env, double angle)
{
	t_ixy	H;
	int		Xi;
	int		distance;

	Xi = SCALE / tan(angle);
	H.y = env->player.pos.y / SCALE;
	H.y = (H.y * SCALE) + ((angle < M_PI) ? -1 : 64);
	H.x = env->player.pos.x + ((env->player.pos.y - H.y) / tan(angle));
	while (!check_grid(env, H.x, H.y))
	{
		H.x = H.x + Xi;
		H.y = H.y + ((angle < M_PI) ? -SCALE : SCALE);
	}
	distance = fabs((env->player.pos.y - H.y) / sin(angle));
	return (distance);
}

int		vertical_ray(t_env *env, double angle)
{
	t_ixy	V;
	int		Yi;
	int		distance;

	Yi = SCALE * tan(angle);
	V.x = env->player.pos.x / SCALE;
	V.x = (V.x * SCALE) + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? 64 : -1);
	V.y = env->player.pos.y + ((env->player.pos.x - V.x) / tan(angle));
	while (!check_grid(env, V.x, V.y))
	{
		V.x = V.x + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE);
		V.y = V.y + Yi;
	}
	distance = fabs((env->player.pos.x - V.x) / cos(angle));
	return (distance);
}

void	draw_col(t_env *env, int col, int slice)
{
	int		y;
	int		x;
	int		floor;

	y = (HALF_H * 4) - (slice / 2);
	floor = y + slice;
	while (y < floor)
	{
		x = col;
		while (x < col + 4)
		{
			mlx_pixel_put(env->mlx, env->win, x, y, 0xffff);
			x++;
		}
		y++;
	}
}

void	ray_cast(t_env *env)
{
	double	angle;
	int		d_H;
	int		d_V;
	int		slice;
	int		col;

	angle = env->player.dir.x + (FOV / 2);
	col = 0;
	while (col < WIN_W * 4)
	{
		d_H = horizontal_ray(env, angle);
		d_V = vertical_ray(env, angle);
		slice = env->constant / ((d_H <= d_V) ? d_H : d_V);
		draw_col(env, col, slice * 4);
		angle -= (M_PI / 960);
		col += 4;
	}
}
