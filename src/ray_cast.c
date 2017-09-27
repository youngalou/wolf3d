/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 11:10:14 by lyoung            #+#    #+#             */
/*   Updated: 2017/08/22 11:37:54 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../wolf3d.h"
#include <stdio.h>

int		check_grid(t_env *env, int x, int y)
{
	x /= SCALE;
	y /= SCALE;
	if (x >= env->map.width || y >= env->map.length || x < 0 || y < 0) //make sure position is within map
		return (-1);
	return (env->map.grid[y][x]);
}

int		horizontal_ray(t_env *env, double angle)
{
	t_ixy	H;
	int		Xi;
	int		distance;

	Xi = SCALE / tan(angle);
	H.y = env->player.pos.y / SCALE;
	H.y = (H.y * SCALE) + ((angle < M_PI) ? -1 : SCALE);
	H.x = env->player.pos.x + ((env->player.pos.y - H.y) / tan(angle));
	while (!check_grid(env, H.x, H.y))
	{
		H.x += ((angle > M_PI) ? -Xi : Xi);
		H.y += ((angle < M_PI) ? -SCALE : SCALE);
	}
	distance = fabs((env->player.pos.y - H.y) / sin(angle)) * cos(env->player.dir.x - angle);
	if (distance < 0)
		distance = INT_MAX;
	return (distance);
}

int		vertical_ray(t_env *env, double angle)
{
	t_ixy	V;
	int		Yi;
	int		distance;

	Yi = SCALE * tan(angle);
	V.x = env->player.pos.x / SCALE;
	V.x = (V.x * SCALE) + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	V.y = env->player.pos.y + ((env->player.pos.x - V.x) * tan(angle));
	while (!check_grid(env, V.x, V.y))
	{
		V.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE); //unit circle to pixel board conv
		V.y += ((angle < M_PI / 2 || angle > 3 * M_PI / 2) ? -Yi : Yi);
	}
	distance = fabs((env->player.pos.x - V.x) / cos(angle)) * cos(env->player.dir.x - angle);
	if (distance < 0) //prevents tan() resulting in undefined with 90 degree angles
		distance = INT_MAX;
	return (distance);
}

void	draw_col(t_env *env, int col, int slice)
{
	int		y;
	int		floor;

	if (slice > WIN_H)
		slice = WIN_H;
	y = HALF_H - (slice / 2);
	floor = y + slice;
	env->color = 0xff - (0xff * SMOG / slice); //add smog (probably will be replaced with a function)
	while (y < floor)
	{
		env->pixels[col + (y * (WIN_W))] = env->color;
		y++;
	}
}

void	clear_img(int *pixels)
{
	int		i;

	i = 0;
	while (i < (WIN_W * WIN_H))
	{
		pixels[i] = 0;
		i++;
	}
}

void	ray_cast(t_env *env)
{
	double	angle;
	int		d_H;
	int		d_V;
	int		slice;
	int		col;

	if (env->drawn == 1)
	{
		clear_img(env->pixels);
		env->drawn = 0;
	}
	angle = env->player.dir.x + (FOV / 2);
	if (angle > 2 * M_PI)
		angle -= (2 * M_PI);
	col = 0;
	while (col < WIN_W)
	{
		d_H = horizontal_ray(env, angle);
		d_V = vertical_ray(env, angle);
		slice = CONSTANT / ((d_H <= d_V) ? d_H : d_V);
		//printf("%d\t%d\t%d\n", d_H, d_V, col);
		draw_col(env, col, slice);
		angle -= ANGLE_SHIFT; //put angle shift and FOV into struct to make FOV changeable
		if (angle < 0)
			angle += (2 * M_PI);
		col++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	env->drawn = 1;
}
