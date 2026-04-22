#include "../../include/cub3d.h"

#define INRANGE(x, mid, eps)	(x >= mid - eps && x <= mid + eps)

// static int point_in_triangle(float px, float py,
//                              t_dpoint dir, t_dpoint pos)
// {
// 	int s = 5;
// 	t_dpoint a = {pos.x + dir.x * s, pos.y + dir.y*s};
// 	t_dpoint b = {pos.x + (dir.x ? -1 * dir.x * s : s) , pos.y - (dir.y? dir.y * s : s) };
// 	t_dpoint c = {pos.x + (dir.x ? -1 * dir.x * s : -s) , pos.y - (dir.y? dir.y * s : -s) };
//     float denom = (b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y);

//     float w1 = ((b.y - c.y)*(px - c.x) + (c.x - b.x)*(py - c.y)) / denom;
//     float w2 = ((c.y - a.y)*(px - c.x) + (a.x - c.x)*(py - c.y)) / denom;
//     float w3 = 1.0f - w1 - w2;

//     return (w1 >= 0 && w2 >= 0 && w3 >= 0);
// }


void	add_minimap(t_map *m, t_img* i, t_coord* c)
{
	int x, y;

	x = -1;
	while (++x < (m->width + 2) * MMSCALE)
	{
		y = -1;
		while(++y < (m->height + 2) * MMSCALE) 
		{
			char res = rc_hit_wall(m, x/MMSCALE, y/MMSCALE);
			if (res == 1) 				//wall
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOLW);
			else if (res == 0)
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOL);
			else continue;

			float px = c->pos.x * MMSCALE;
			float py = c->pos.y * MMSCALE;
			int s= 5;

			if (INRANGE(x, px, s) && INRANGE(y, py, s) && 
				(x - px) * (x - px) + (y - py)* (y - py) < s*s)
			{
				// if (point_in_triangle(x, y, c->dir, (t_dpoint){px, py}))
					put_color(i, x + MM_PADING, y + MM_PADING, MMCOLP);
			}
			else if (y % MMSCALE == 0 || x % MMSCALE == 0) 		//grid
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOL);
		}
	}
}
