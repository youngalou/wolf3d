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

# define WIN_W 1280
# define WIN_H 800
# define HALF_W WIN_W / 2
# define HALF_H WIN_H / 2
# define SCALE 262144
# define FOV M_PI / 2
# define CONSTANT SCALE * 1108
# define ANGLE_SHIFT FOV / WIN_W
# define MOVE_SPEED 5 * (SCALE / 64)
# define TURN_SPEED M_PI / 128
# define SMOG 75

# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LARR 123
# define KEY_RARR 124
# define KEY_M 46
# define KEY_P 35
# define KEY_ESC 53

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
}				t_key;

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

typedef struct	s_player
{
	t_ixy		pos;
	t_dxy		dir;
}				t_player;

typedef struct	s_map
{
	int			**grid;
	int			width;
	int			length;
}				t_map;

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
	t_key		key;
	int			drawn;
	int			color;
}				t_env;

/*
** --------------- main.c --------------
*/

void	init_map(t_env *env, char *line);
void	load_map(t_env *env, int fd);
t_env	*init_env(void);
void	print_grid(t_env *env);

/*
** --------------- ray_cast.c --------------
*/

int		check_grid(t_env *env, int x, int y);
int		horizontal_ray(t_env *env, double angle);
int		vertical_ray(t_env *env, double angle);
void	draw_col(t_env *env, int col, int slice);
void	clear_img(int *pixels);
void	ray_cast(t_env *env);

/*
** --------------- key_command.c --------------
*/

int		key_press(int key, t_env *env);
int		key_release(int key, t_env *env);
int		exit_hook(int key, t_env *env);
int		forever_loop(t_env *env);

#endif
