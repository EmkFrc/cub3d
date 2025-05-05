#include <math.h>
#include <mlx.h>
#include <mlx_int.h>
#include <stdlib.h>

#define COLS 10
#define ROWS 7
#define TILESIZE 64
#define WIN_NAME "So_Long"

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
	char				map[COLS * ROWS];
}						t_data;

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
	// Exemple de map
	for (int i = 0; i < COLS * ROWS; i++)
		data->map[i] = tmp[i];
	// data->window_width = 800;
	// data->window_height = 600;
	data->player.rayon = 7;
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


void loop_og(t_data *data)
{
	(void)data;
	draw_map(data);
	draw_circle(data, data->player.x, data->player.y, data->player.rayon,
		0xFF0000);
	draw_line(data, data->player.x, data->player.y, data->player.x + cos(data->player.angle) * 50, data->player.y + sin(data->player.angle) * 50, 0x0000FF);
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
	data.player.angle = 1.754;
	draw_line(&data, data.player.x, data.player.y, data.player.x + 50, data.player.y, 0xFFFFFF);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_loop_hook(data.mlx, (int (*)(void *))loop_og, (void *)&data);
	mlx_loop(data.mlx);
	terminal_prog(&data);
	return (0);
}
