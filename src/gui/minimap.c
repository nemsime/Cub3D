#include "../../include/cub3d.h"
#define INRANGE(x, mid, eps)	(x >= mid - eps && x <= mid + eps)

#define MM_PADING	10
#define MMCOL		0x00303065
#define MMCOLW		0x008080A5
#define MMCOLP		0x00A58080
#define MMSCALE		13

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
			
			if ( INRANGE(x,  c->pos.x*MMSCALE, 3)  &&  INRANGE(y,  c->pos.y*MMSCALE, 3) )
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOLP);
			else if (y % MMSCALE == 0 || x % MMSCALE == 0) 		//grid
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOL);
		}
	}
}
