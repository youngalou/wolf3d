/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 11:41:41 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/19 14:10:01 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# include "libft/superlibft.h"
# include "minilibx/mlx.h"
# include <math.h>

# define WIN_W 320
# define WIN_H 200
# define HALF_W 160
# define HALF_H 100
# define SCALE 64
# define FOV M_PI / 3

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

typedef struct		s_player
{
	struct s_ixy	pos;
	struct s_dxy	dir;
}				t_player;

typedef struct	s_map
{
	int			**grid;
	int			width;
	int			length;
}				t_map;

typedef struct		s_env
{
	void			*mlx;
	void			*win;
	struct s_map	map;
	struct s_player	player;
	int				constant;
}					t_env;

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

void	ray_cast(t_env *env);

/*
** --------------- key_command.c --------------
*/

int		key_command(int key, t_env *env);

#endif
