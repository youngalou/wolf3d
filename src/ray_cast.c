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

void	horizontal_ray(t_env *env, double angle)
{
	t_ixy	H;
	int		Xi;

	Xi = SCALE / tan(angle);
	H.y = env->player.pos.y / SCALE;
	H.y = (H.y * SCALE) + ((angle > M_PI) ? SCALE : -1);
	H.x = env->player.pos.x + ((env->player.pos.y - H.y) / tan(angle));
	while (!(env->tex_H = check_grid(env, H.x, H.y)))
	{
		H.x += ((angle > M_PI) ? -Xi : Xi);
		H.y += ((angle > M_PI) ? SCALE : -SCALE);
	}
	env->d_H = fabs((env->player.pos.y - H.y) / sin(angle)) * cos(env->player.dir.x - angle);
	if (env->d_H < 0)
		env->d_H = INT_MAX;
	env->x0 = (H.x % SCALE) / 1024;
}

void	vertical_ray(t_env *env, double angle)
{
	t_ixy	V;
	int		Yi;

	Yi = SCALE * tan(angle);
	V.x = env->player.pos.x / SCALE;
	V.x = (V.x * SCALE) + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	V.y = env->player.pos.y + ((env->player.pos.x - V.x) * tan(angle));
	while (!(env->tex_V = check_grid(env, V.x, V.y)))
	{
		V.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE); //unit circle to pixel board conv
		V.y += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? Yi : -Yi);
	}
	env->d_V = fabs((env->player.pos.x - V.x) / cos(angle)) * cos(env->player.dir.x - angle);
	if (env->d_V < 0) //prevents tan() resulting in undefined with 90 degree angles
		env->d_V = INT_MAX;
	env->y0 = (V.y % SCALE) / 1024;
}

# define COL 0
# define SLICE 1
# define OFF 2
# define TEX 3

void	draw_col(t_env *env, int data[4])
{
	int		y;
	int		floor;
	double	i;
	double	smog;

	if (data[SLICE] > 5000)
	 	data[SLICE] = 5000;
	smog = (data[SLICE] < WIN_W) ? ((double)data[SLICE] / WIN_H) : 1.5;
	y = HALF_H - (data[SLICE] / 2);
	floor = y + data[SLICE];
	i = 0;
	while (y < floor)
	{
		if (y >= 0 && y < WIN_H)
		{
			env->color.rgb = env->tex.str[data[TEX]][data[OFF] + ((int)i * 256)];
			// env->color.r = env->color.rgb >> 16;
			// env->color.g = (env->color.rgb >> 8) & 0xFF;
			// env->color.b = env->color.rgb & 0xFF;
			// env->color.rgb = ((env->color.r << 16) * smog) + ((env->color.g << 8) * smog) + (env->color.b * smog);
			env->pixels[data[COL] + (y * (WIN_W))] = env->color.rgb;
		}
		i += (64 / (double)data[SLICE]);
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
	int		data[4];

	if (env->drawn == 1)
	{
		clear_img(env->pixels);
		mlx_clear_window(env->mlx, env->win);
		env->drawn = 0;
	}
	angle = env->player.dir.x + (FOV / 2);
	while (angle > 2 * M_PI)
		angle -= (2 * M_PI);
	data[COL] = 0;
	while (data[COL] < WIN_W)
	{
		while (angle < 0)
			angle += (2 * M_PI);
		horizontal_ray(env, angle);
		vertical_ray(env, angle);
		data[SLICE] = CONSTANT / ((env->d_H <= env->d_V) ? env->d_H : env->d_V);
		data[OFF] = (env->d_H <= env->d_V) ? env->x0 : env->y0;
		data[OFF] -= (data[OFF] % 4);
		data[TEX] = (env->d_H <= env->d_V) ? env->tex_H : env->tex_V;
		draw_col(env, data);
		angle -= ANGLE_SHIFT; //put angle shift and FOV into struct to make FOV changeable
		data[COL]++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	mlx_put_image_to_window(env->mlx, env->win, env->weapon.img[env->anim], HALF_W - (GUN_W / 2), WIN_H - GUN_H);
	if (env->won)
		mlx_put_image_to_window(env->mlx, env->win, env->tex.img[5], HALF_W - 210, HALF_H - 150);
	env->drawn = 1;
}
