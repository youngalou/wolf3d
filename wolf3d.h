/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoung <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 11:41:41 by lyoung            #+#    #+#             */
/*   Updated: 2017/07/13 13:25:02 by lyoung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# include "libft/superlibft.h"
# include "minilibx/mlx.h"

typedef struct	s_player
{
	int			pos_x;
	int			pos_y;
	int			pos_z;
	int			dir_x;
	int			dir_y;
	int			dir_z;
}				t_player;

typedef struct	s_map
{
	int			**grid;
	int			width;
	int			length;
}				t_map;

typedef struct		s_env
{
	struct s_map	map;
	struct s_player	player;
}					t_env;

#endif
