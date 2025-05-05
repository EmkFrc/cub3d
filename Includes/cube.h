/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efranco <efranco@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:49:40 by efranco           #+#    #+#             */
/*   Updated: 2025/05/05 16:03:31 by efranco          ###   ########.fr       */
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

int TILESIZE = 64;
int ROWS = 7;
int COLS = 10;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	t_img	*img;
	int		window_width;
	int		window_height;
	double	fov;
	int		res;
	int		num_rays;
	char	map[COLS * ROWS];
}			t_data;

typedef	struct s_player
{
	int	x;
	int y;
	t_img	*circle;
} t_player;

#endif
