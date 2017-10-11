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
	H.y = ((env->player.pos.y / SCALE) * SCALE) + ((angle > M_PI) ? SCALE : -1);
	H.x = env->player.pos.x + ((env->player.pos.y - H.y) / tan(angle));
	while (!(env->H.tex = check_grid(env, H.x, H.y)))
	{
		H.x += ((angle > M_PI) ? -Xi : Xi);
		H.y += ((angle > M_PI) ? SCALE : -SCALE);
	}
	env->H.dist = fabs((env->player.pos.y - H.y) / sin(angle)) * cos(env->player.dir.x - angle);
	if (env->H.dist < 0)
		env->H.dist = INT_MAX;
	env->H.off = (H.x % SCALE) / (RENDER_SCALE);
}

void	vertical_ray(t_env *env, double angle)
{
	t_ixy	V;
	int		Yi;

	Yi = SCALE * tan(angle);
	V.x = ((env->player.pos.x / SCALE) * SCALE) + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	V.y = env->player.pos.y + ((env->player.pos.x - V.x) * tan(angle));
	while (!(env->V.tex = check_grid(env, V.x, V.y)))
	{
		V.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE); //unit circle to pixel board conv
		V.y += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? Yi : -Yi);
	}
	env->V.dist = fabs((env->player.pos.x - V.x) / cos(angle)) * cos(env->player.dir.x - angle);
	if (env->V.dist < 0) //prevents tan() resulting in undefined with 90 degree angles
		env->V.dist = INT_MAX;
	env->V.off = (V.y % SCALE) / (RENDER_SCALE);
}

void	draw_col(t_env *env, t_ray ray, int col, double angle)
{
	int		slice;
	int		y;
	int		floor;
	double	i;
	double	smog;
	double	straight;
	int		real;
	int		j;
	int		sky;
	t_ixy	dist;

	slice = WALL_HEIGHT / ray.dist;
	if (slice > 5000)
	 	slice = 5000;
	smog = (slice < WIN_W) ? ((double)slice / WIN_H) : 1.5;
	y = HALF_H - (slice / 2);
	floor = y + slice;
	j = 0;
	while (j < y)
	{
		sky = angle * (SKY_W / (2 * M_PI));
		env->color.rgb = env->tex.str[6][sky + (j * SKY_W)];
		env->pixels[col + (j * (WIN_W))] = env->color.rgb;
		j++;
	}
	i = 0;
	while (y < floor)
	{
		if (y >= 0 && y < WIN_H)
		{
			env->color.rgb = env->tex.str[ray.tex][ray.off + ((int)i * TEX_RES)];
			env->pixels[col + (y * (WIN_W))] = env->color.rgb;
		}
		i += (64 / (double)slice);
		y++;
	}
	while (y < WIN_H)
	{
		straight = ((double)HALF_H / ((double)y - (double)HALF_H)) * FL_CONSTANT;
		real = straight / cos(angle - env->player.dir.x);
		dist.x = env->player.pos.x + (real * cos(angle));
		dist.y = env->player.pos.y - (real * sin(angle));
		//ft_printf("%d\t%d\t%d\n", col, dist.x, dist.y);
		dist.x = (dist.x / 4096) % TEX_RES;
		dist.y = (dist.y / 4096) % TEX_RES;
		env->color.rgb = env->tex.str[1][dist.x + (dist.y * TEX_RES)];
		env->pixels[col + (y * (WIN_W))] = env->color.rgb;
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
	int		col;

	// if (env->drawn == 1)
	// {
	// 	clear_img(env->pixels);
	// 	mlx_clear_window(env->mlx, env->win);
	// 	env->drawn = 0;
	// }
	angle = env->player.dir.x + (FOV / 2);
	while (angle > 2 * M_PI)
		angle -= (2 * M_PI);
	col = 0;
	while (col < WIN_W)
	{
		while (angle < 0)
			angle += (2 * M_PI);
		horizontal_ray(env, angle);
		vertical_ray(env, angle);
		if (env->H.dist <= env->V.dist)
			draw_col(env, env->H, col, angle);
		else
			draw_col(env, env->V, col, angle);
		angle -= ANGLE_SHIFT; //put angle shift and FOV into struct to make FOV changeable
		col++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	mlx_put_image_to_window(env->mlx, env->win, env->weapon.img[env->anim], HALF_W - (GUN_W / 2), WIN_H - GUN_H);
	if (env->won)
		mlx_put_image_to_window(env->mlx, env->win, env->tex.img[5], HALF_W - 210, HALF_H - 150);
	env->drawn = 1;
}
