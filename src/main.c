#include <math.h>
#include <mlx.h>
#include <mlx_int.h>
#include <stdlib.h>
#include <Includes/cube.h>

void	put_pixel(t_data *data, int x, int y, unsigned int color)
{
	if (x < 0)
		x = 0;
	if (x > data->window_width)
		x = data->window_width;
	if (y < 0)
		y = 0;
	if (y > data->window_height)
		y = data->window_height;
	((unsigned int *)data->img->data)[(y * data->img->size_line / 4
			+ x)] = color;
}
void	draw_square(t_data *data, int ox, int oy, int color)
{
	int	i;
	int	j;

	i = 1;
	while (i < TILESIZE - 1)
	{
		j = 1;
		while (j < TILESIZE - 1)
		{
			put_pixel(data, ox + j, oy + i, color); // vert
			j++;
		}
		i++;
	}
}
void	draw_map(t_data *data)
{
	char	cell;
	int		color;

	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			cell = data->map[y * COLS + x];
			color = (cell == '1') ? 0x00FF00 : 0xFFFFFF;
			draw_square(data, x * TILESIZE, y * TILESIZE, color);
		}
	}
}

// char	tmp[COLS * ROWS] = {
// 	'1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
// 	'1', '0', '0', '0', '0', '0', '0', '0', '0', '1',
// 	'1', '0', '0', '0', '0', '1', '0', '0', '0', '1',
// 	'1', '0', '0', '1', '0', '0', '1', '0', '0', '1',
// 	'1', '0', '0', '0', '1', '0', '0', '0', '0', '1',
// 	'1', '0', '0', '0', '0', '0', '0', '0', '0', '1',
// 	'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'
// };


void	init_data(t_data *data)
{
	char	tmp[COLS * ROWS] = {
		'1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
		'1', '0', '0', '0', '0', '0', '0', '0', '0', '1',
		'1', '0', '0', '0', '0', '1', '0', '0', '0', '1',
		'1', '0', '0', '1', '0', '0', '1', '0', '0', '1',
		'1', '0', '0', '0', '1', '0', '0', '0', '0', '1',
		'1', '0', '0', '0', '0', '0', '0', '0', '0', '1',
		'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'
	};

	data->window_width = COLS * TILESIZE;
	data->window_height = ROWS * TILESIZE;
	data->fov = 60 * (M_PI / 180);
	data->res = 4;
	data->num_rays = data->window_width / data->res;
	data->rays = malloc(sizeof(t_ray) * data->num_rays);
	if (!data->rays)
		exit(EXIT_FAILURE);
	for (int i = 0; i < COLS * ROWS; i++)
		data->map[i] = tmp[i];
	data->player.rayon = 7;
	data->player.angle = 0;
	data->player.x = data->window_width / 2;
	data->player.y = data->window_height / 2;
}

int	init_win(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (0);
	data->win = mlx_new_window(data->mlx, data->window_width,
			data->window_height, WIN_NAME);
	if (!data->win)
		return (0);
	data->img = mlx_new_image(data->mlx, data->window_width,
			data->window_height);
	if (!data->img)
		return (0);
	return (1);
}

void	terminal_prog(t_data *data)
{
	if (data->mlx)
		mlx_loop_end(data->mlx);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	free(data->mlx);
}

int	close_window(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	mlx_loop_end(data->mlx);
	return (0);
}
void	draw_circle(t_data *data, int cx, int cy, int radius, int color)
{
	int	x;
	int	y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(data, cx + x, cy + y, color);
			x++;
		}
		y++;
	}
}
void	draw_line(t_data *data, int x0, int y0, int x1, int y1, int color)
{
	double	dx = x1 - x0;
	double	dy = y1 - y0;
	double	step;
	double	x;
	double	y;

	step = fmax(fabs(dx), fabs(dy));
	if (step == 0)
	{
		put_pixel(data, x0, y0, color);
		return;
	}
	dx /= step;
	dy /= step;
	x = x0;
	y = y0;
	while (step > 0)
	{
		put_pixel(data, (int)x, (int)y, color);
		x += dx;
		y += dy;
		step--;
	}
}
void	normalizeangle(t_data *data)
{
	if (data->player.angle > 2 * M_PI)
		data->player.angle -= 2 * M_PI;
	if (data->player.angle < 0)
		data->player.angle += 2 * M_PI;
}
int	turn_line(int keycode, t_data *data)
{
	if (keycode == 'd')
		data->player.angle += 0.2;
	else if (keycode == 'a')
		data->player.angle -= 0.2;
	else if (keycode == 'w')
	{
		data->player.y += sin(data->player.angle) * 8.5;
		data->player.x += cos(data->player.angle) * 8.5;
	}
	else if(keycode == 's')
	{
		data->player.y -= sin(data->player.angle) * 8.5;
		data->player.x -= cos(data->player.angle) * 8.5;
	}
	normalizeangle(data);
	return (0);
}
int circle_direction(int keycode, t_data *data)
{
	if (keycode == 'w')
		data->player.y -= 20;
	else if(keycode == 'a')
		data->player.x += 20;
	else if(keycode == 's')
		data->player.y += 20;
	else if(keycode == 'd')
		data->player.x -= 20;
	return (0);
}
void	castAllRays(t_data *data)
{
	double	angle;
	int i = 0;

	angle = data->player.angle - FOV / 2;
	while (i < data->num_rays)
	{
		data->rays[i].rayangle = angle;
		angle += FOV / data->num_rays;
		i++;
	}
}
void	where_is_facing(t_data *data, int i)
{
	if (data->rays[i].rayangle > 0 && data->rays[i].rayangle < M_PI)
	{
		data->rays[i].facing_horizontal = DOWN;
	}
	else
	{
		data->rays[i].facing_horizontal = UP;
	}
	if (data->rays[i].rayangle < 0.5 * M_PI || data->rays[i].rayangle > 1.5 * M_PI)
	{
		data->rays[i].facing_vertical = RIGHT;
	}
	else
	{
		data->rays[i].facing_vertical = LEFT;
	}
}
bool isWall(t_data *data, float x, float y)
{
	if (x < 0 || x >= data->window_width || y < 0 || y >= data->window_height)
		return true;
	int map_x = (int)(x / TILESIZE);
	int map_y = (int)(y / TILESIZE);
	return (data->map[map_y * COLS + map_x] == '1');
}
float distance_between(float x1, float y1, float x2, float y2)
{
	return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void get_horizontal_intersection(t_data *data, int i)
{
	float angle = data->rays[i].rayangle;
	float y_intercept, x_intercept;
	float y_step, x_step;

	data->rays[i].found_horizontal_wall = false;

	y_intercept = floor(data->player.y / TILESIZE) * TILESIZE;
	if (angle > 0 && angle < M_PI)
		y_intercept += TILESIZE;

	x_intercept = data->player.x + (y_intercept - data->player.y) / tan(angle);

	if (angle > 0 && angle < M_PI)
		y_step = TILESIZE;
	else
		y_step = -TILESIZE;

	x_step = TILESIZE / tan(angle);
	if ((angle > M_PI_2 && angle < 3 * M_PI_2) && x_step > 0)
		x_step *= -1;
	if (!(angle > M_PI_2 && angle < 3 * M_PI_2) && x_step < 0)
		x_step *= -1;

	float next_x = x_intercept;
	float next_y = y_intercept;

	while (next_x >= 0 && next_x <= data->window_width &&
		   next_y >= 0 && next_y <= data->window_height)
	{
		float check_y;
		if (angle > 0 && angle < M_PI)
			check_y = next_y;
		else
			check_y = next_y - 1;

		if (isWall(data, next_x, check_y))
		{
			data->rays[i].horizontal_hit_x = next_x;
			data->rays[i].horizontal_hit_y = next_y;
			data->rays[i].found_horizontal_wall = true;
			break;
		}
		next_x += x_step;
		next_y += y_step;
	}
}

void get_vertical_intersection(t_data *data, int i)
{
	float angle = data->rays[i].rayangle;
	float x_intercept, y_intercept;
	float x_step, y_step;

	data->rays[i].found_vertical_wall = false;

	x_intercept = floor(data->player.x / TILESIZE) * TILESIZE; //aligner le rayon sur la première ligne verticale devant lui
	if (angle < M_PI_2 || angle > 3 * M_PI_2) // viser le bord a droite
		x_intercept += TILESIZE;

	y_intercept = data->player.y + (x_intercept - data->player.x) * tan(angle); // y pour trouver la hauteur donc opp = adj * tan(a)

	if (angle < M_PI_2 || angle > 3 * M_PI_2) // si x va a droite positif si va vers la gauche negatif
		x_step = TILESIZE;
	else
		x_step = -TILESIZE;

	y_step = TILESIZE * tan(angle); // on monte suivant la tan
	if (angle > M_PI && y_step > 0) // on inverse , si il va vers le bas positif si il va vers le haut negatif
		y_step *= -1;
	if (angle < M_PI && y_step < 0)
		y_step *= -1;

	float next_x = x_intercept;
	float next_y = y_intercept;

	while (next_x >= 0 && next_x <= data->window_width &&
		   next_y >= 0 && next_y <= data->window_height)
	{
		float check_x;
		if (angle < M_PI_2 || angle > 3 * M_PI_2) //angle vers la droite
			check_x = next_x;
		else
			check_x = next_x - 1;

		if (isWall(data, check_x, next_y))
		{
			data->rays[i].vertical_hit_x = next_x;
			data->rays[i].vertical_hit_y = next_y;
			data->rays[i].found_vertical_wall = true;
			break;
		}
		next_x += x_step;
		next_y += y_step;
	}
}

void get_wall_hit(t_data *data, int i)
{
	data->rays[i].found_horizontal_wall = false;
	data->rays[i].found_vertical_wall = false;
	data->rays[i].horizontal_hit_x = 0;
	data->rays[i].horizontal_hit_y = 0;
	data->rays[i].vertical_hit_x = 0;
	data->rays[i].vertical_hit_y = 0;

	get_horizontal_intersection(data, i);
	get_vertical_intersection(data, i);

	float hor_dist;
	if (data->rays[i].found_horizontal_wall)
	{
		hor_dist = distance_between(data->player.x, data->player.y,
									data->rays[i].horizontal_hit_x,
									data->rays[i].horizontal_hit_y);
	}
	else
	{
		hor_dist = 99999;
	}

	float vert_dist;
	if (data->rays[i].found_vertical_wall)
	{
		vert_dist = distance_between(data->player.x, data->player.y,
									 data->rays[i].vertical_hit_x,
									 data->rays[i].vertical_hit_y);
	}
	else
	{
		vert_dist = 99999;
	}

	float angle_diff = data->rays[i].rayangle - data->player.angle;
	hor_dist *= cos(angle_diff);
	vert_dist *= cos(angle_diff);

	if (vert_dist < hor_dist)
	{
		data->rays[i].distance = vert_dist;
		data->rays[i].was_hit_vertical = true;
		data->rays[i].wall_hit_x = data->rays[i].vertical_hit_x;
		data->rays[i].wall_hit_y = data->rays[i].vertical_hit_y;
	}
	else
	{
		data->rays[i].distance = hor_dist;
		data->rays[i].was_hit_vertical = false;
		data->rays[i].wall_hit_x = data->rays[i].horizontal_hit_x;
		data->rays[i].wall_hit_y = data->rays[i].horizontal_hit_y;
	}
}


void loop_og(t_data *data)
{
	(void)data;
	draw_map(data);
	draw_circle(data, data->player.x, data->player.y, data->player.rayon,
		0xFF0000);

	// double len = 256;
	int i = 0;
	castAllRays(data);
	while(i < data->num_rays)
	{
		data->rays[i].found_horizontal_wall = false;
		data->rays[i].found_vertical_wall = false;
		get_wall_hit(data, i);
		// double len = totalen(data);
		// int x1 = data->player.x + cos(data->rays[i].rayangle) * len;
		// int y1 = data->player.y + sin(data->rays[i].rayangle) * len;
		draw_line(data, data->player.x, data->player.y, data->rays[i].wall_hit_x, data->rays[i].wall_hit_y, 0x0000FF);
		i++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	// data->player.angle += 0.1;
	// check si angle > 2 * M_PI ou que angle < 0
	usleep(10000);
}
int	main(void)
{
	t_data	data;

	init_data(&data);
	if (!init_win(&data))
		return (1);
	mlx_hook(data.win, 17, 0, close_window, &data);
	draw_map(&data);
	draw_circle(&data, data.player.x, data.player.y, data.player.rayon,
		0xFF0000); // Rouge
	// data.player.angle = 1.754;
	mlx_key_hook(data.win, turn_line, &data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_loop_hook(data.mlx, (int (*)(void *))loop_og, (void *)&data);
	mlx_loop(data.mlx);
	terminal_prog(&data);
	return (0);
}
