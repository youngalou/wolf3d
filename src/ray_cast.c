/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 11:10:14 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/18 16:40:06 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"
#include <stdio.h>

int		check_grid(t_env *env, int x, int y)
{
	x = x / SCALE;
	y = y / SCALE;
	//ft_printf("%d\t%d\n", x, y);
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
		H.y = H.y + SCALE;
	}
	distance = abs(env->player.pos.x - H.x) / cos(angle);
	return (distance);
}

int		vertical_ray(t_env *env, double angle)
{
	t_ixy	V;
	int		Yi;
	int		distance;

	Yi = SCALE * tan(angle);
	V.x = env->player.pos.x / SCALE;
	V.x = (V.x * SCALE) + ((angle > M_PI / 2 && angle < 4 * M_PI / 3) ? 64 : -1);
	V.y = env->player.pos.y + ((env->player.pos.x - V.x) / tan(angle));
	while (!check_grid(env, V.x, V.y))
	{
		V.x = V.x + SCALE;
		V.y = V.y + Yi;
	}
	distance = abs(env->player.pos.y - V.y) / sin(angle);
	return (distance);
}

void	draw_col(t_env *env, int col, double slice)
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
			ft_printf("%d\t%d\n", x, y);
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
	double	var;

	angle = env->player.dir.x - (FOV / 2);
	var = SCALE / 277;
	col = 0;
	while (col < WIN_W * 4)
	{
		d_H = horizontal_ray(env, angle);
		d_V = vertical_ray(env, angle);
		slice = ((d_H <= d_V) ? d_H : d_V) / 4; //using divide by 4 instead of mult by (SCALE / 277)
		printf("%lf\t%d\n", var, slice);
		draw_col(env, col, slice * 4);
		angle += (M_PI / 960);
		col += 4;
	}
}
