/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 11:41:41 by lyoung            #+#    #+#             */
/*   Updated: 2017/08/22 11:36:41 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# include "libft/superlibft.h"
# include "minilibx/mlx.h"
# include <math.h>
# include <stdio.h> //make sure to remove this include!!!

// # define DISTANCETOPLANE 640

# define WIN_W 800
# define WIN_H 500
# define HALF_W (WIN_W / 2)
# define HALF_H (WIN_H / 2)
# define SCALE 262144
# define FOV (M_PI / 3)
# define WALL_HEIGHT (SCALE * 700)
# define ANGLE_SHIFT (FOV / WIN_W)
# define ANGLE_START (3 * M_PI / 2)
# define FL_CONSTANT 366478	//	366478 : 700	|	362740 : 1108	|	288030 : 880	|	268446 : 640

# define NUM_TEX 8
# define TEX_RES 64
# define RENDER_SCALE (SCALE / TEX_RES)
# define TEX_W 64
# define TEX_H 64
# define SKY_W 2500

# define GUN_W 450
# define GUN_H 200
# define FRAMES 33
# define SHOOT 3
# define WAIT 3
# define AMMO 20

# define MOVE_SPEED (5 * (RENDER_SCALE))
# define TURN_SPEED (M_PI / 128)
# define MOUSE_SENS (4 * M_PI / WIN_W)
# define COLLISION 5

# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LARR 123
# define KEY_RARR 124
# define KEY_R 15
# define KEY_M 46
# define KEY_SPACE 49
# define KEY_ESC 53
# define KEY_TAB 48

typedef struct	s_ixy
{
	int			x;
	int			y;
}				t_ixy;

typedef struct	s_dxy
{
	double		x;
	double		y;
}				t_dxy;

typedef struct	s_map
{
	int			**grid;
	int			width;
	int			length;
	t_ixy		exit;
}				t_map;

typedef struct	s_player
{
	t_ixy		pos;
	t_dxy		dir;
}				t_player;

typedef struct	s_rgb
{
	int			r;
	int			g;
	int			b;
	int			rgb;
}				t_rgb;

typedef struct	s_key
{
	int			w;
	int			a;
	int			s;
	int			d;
	int			uarr;
	int			darr;
	int			larr;
	int			rarr;
	int			space;
}				t_key;

typedef struct	s_mouse
{
	int			init;
	t_ixy		pos;
}				t_mouse;

typedef struct	s_ray
{
	int			dist;
	int			off;
	int			tex;
	int			slice;
}				t_ray;

typedef struct	s_gun
{
	int			anim;
	int			wait;
	int			shoot;
	int			ammo;
	int			reload;
	int			height;
	int			move;
}				t_gun;

typedef struct		s_img
{
	void			*img[FRAMES];
	int				*arr[FRAMES];
	int				bpp;
	int				sl;
	int				endian;
	int				width;
	int				height;
}					t_img;

typedef struct	s_sprite
{
	void		*img;
	int			*arr;
	t_ixy		pos;
	int			dist;
}				t_sprite;

typedef struct	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	int			*pixels;
	int			bpp;
	int			sl;
	int			endian;
	t_map		map;
	t_player	player;
	t_sprite	sprite;
	t_rgb		color;
	t_img		tex;
	t_img		weapon;
	t_key		key;
	t_mouse		mouse;
	t_ray		H;
	t_ray		V;
	int			d_H;
	int			d_V;
	int			tex_H;
	int			tex_V;
	int			x0;
	int			y0;
	double		smog;
	int			drawn;
	int			won;
	int			flash;
	t_gun		gun;
	int			dist[WIN_W];
}				t_env;

/*
** --------------- main.c --------------
*/

void	init_map(t_env *env, char *line);
void	load_map(t_env *env, int fd);
void	load_assets(t_env *env);
void	reset(t_env *env);
t_env	*init_env(void);
void	print_grid(t_env *env);

/*
** --------------- ray_cast.c --------------
*/

int		check_grid(t_env *env, int x, int y);
void	horizontal_ray(t_env *env, double angle);
void	vertical_ray(t_env *env, double angle);
void	draw_col(t_env *env, t_ray ray, int col, double angle);
void	add_smog(t_env *env);
void	clear_img(int *pixels);
void	ray_cast(t_env *env);

/*
** --------------- key_command.c --------------
*/

int		exit_hook(int key, t_env *env);
int		mouse_pos(int x, int y, t_env *env);
int		key_press(int key, t_env *env);
int		key_release(int key, t_env *env);
void	animations(t_env *env);
int		forever_loop(t_env *env);

#endif


/* adding smog to walls
double	smog;

smog = (slice < WIN_W) ? ((double)slice / WIN_H) : 1.5;
env->color.r = env->color.rgb >> 16;
env->color.g = (env->color.rgb >> 8) & 0xFF;
env->color.b = env->color.rgb & 0xFF;
env->color.rgb = ((int)(env->color.r * smog) << 16) + ((int)(env->color.g * smog) << 8) + (env->color.b * smog);

*/

// drawing ceiling

// j = 0;
// while (j < y)
// {
// 	straight = ((double)HALF_H / ((double)j - (double)HALF_H)) * 288030;
// 	real = straight / cos(angle - env->player.dir.x);
// 	dist.x = env->player.pos.x - (real * cos(angle));
// 	dist.y = env->player.pos.y + (real * sin(angle));
// 	dist.x = (dist.x / (RENDER_SCALE)) % TEX_W;
// 	dist.y = (dist.y / (RENDER_SCALE)) % TEX_H;
// 	env->color.rgb = env->tex.str[3][dist.x + (dist.y * TEX_RES)];
// 	env->pixels[col + (j * (WIN_W))] = env->color.rgb;
// 	j++;
// }


/*

Both of these operations are very costly in terms of time complexity. After dropping these functions,
performance has improved significantly, but there are definitely more optimizations to be made.
1st option: Use put_img to place the skybox first, then just lay the render over it.
			(Need to figure out how to make empty pixels not render as black)
2nd option: Optimize floor casting. Sheer number of mathematical operations makes runtime very slow.
			Find anything you can precalculate outside the loop.
3rd option: Reduce window resolution. Raycasting in general is not optimized for large windows.

*/