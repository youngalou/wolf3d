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

# define WIN_W 1280
# define WIN_H 800
# define HALF_W WIN_W / 2
# define HALF_H WIN_H / 2
# define SCALE (64 * 4096)
# define FOV M_PI / 2
# define CONSTANT SCALE * 880
# define ANGLE_SHIFT FOV / WIN_W
# define MOVE_SPEED 5 * (SCALE / 64)
# define TURN_SPEED M_PI / 128
# define COLLISION 5

# define NUM_TEX 6
# define TEX_W 64
# define TEX_H 64


# define GUN_W 582
# define GUN_H 342
# define FRAMES 11
# define WAIT 3

# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LARR 123
# define KEY_RARR 124
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
	t_ixy		exit;
}				t_map;

typedef struct	s_rgb
{
	int			r;
	int			g;
	int			b;
	int			rgb;
}				t_rgb;

typedef struct		s_img
{
	void			*img[FRAMES];
	unsigned char	*str[FRAMES];
	int				bpp;
	int				sl;
	int				endian;
	int				width;
	int				height;
}					t_img;

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
	t_rgb		color;
	t_img		tex;
	t_img		weapon;
	t_key		key;
	t_mouse		mouse;
	char		*texfile[NUM_TEX];
	char		*gunfile[FRAMES];
	int			t;
	int			d_H;
	int			d_V;
	int			tex_H;
	int			tex_V;
	int			x0;
	int			y0;
	int			drawn;
	int			won;
	int			shoot;
	int			anim;
	int			wait;
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
void	draw_col(t_env *env, int data[4]);
void	clear_img(int *pixels);
void	ray_cast(t_env *env);

/*
** --------------- key_command.c --------------
*/

int		key_press(int key, t_env *env);
int		key_release(int key, t_env *env);
int		exit_hook(int key, t_env *env);
int		forever_loop(t_env *env);
int		mouse_pos(int x, int y, t_env *env);

#endif
