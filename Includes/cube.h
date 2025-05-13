/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efranco <efranco@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:49:40 by efranco           #+#    #+#             */
/*   Updated: 2025/05/13 17:28:49 by efranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include "mlx_linux/mlx.h"
# include "mlx_linux/mlx_int.h"

#define COLS 10
#define ROWS 7
#define TILESIZE 64
#define	FOV (60 * (M_PI / 180))
#define WIN_NAME "So_Long"

typedef struct s_data t_data;

typedef enum s_direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
}	e_direction;

typedef struct s_vec2
{
	int x;
	int y;
}	t_vec2;

typedef struct s_dvec2
{
	double x;
	double y;
}	t_dvec2;

typedef struct s_uvec2
{
	unsigned int x;
	unsigned int y;
}	t_uvec2;


typedef struct s_player
{
	double					x;
	double					y;
	double 					angle;
	int					rayon;
}						t_player;

typedef struct s_ray
{
	double rayangle;
	e_direction		facing_horizontal;
	e_direction		facing_vertical;
} t_ray;
typedef struct s_data
{
	void				*mlx;
	void				*win;
	t_img				*img;
	t_player			player;
	int					window_width;
	int					window_height;
	double				fov;
	int					res;
	int					num_rays;
	t_ray				*rays;
	char				map[COLS * ROWS];
}						t_data;


#endif
