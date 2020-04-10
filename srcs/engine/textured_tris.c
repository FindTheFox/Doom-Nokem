/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textured_tris.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saneveu <saneveu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 01:08:25 by saneveu           #+#    #+#             */
/*   Updated: 2020/04/10 17:34:00 by saneveu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/3d_engine.h"

static void     sort_orthogonal(t_triangle *t)
{
    if (t->p[1].y < t->p[0].y)
    {
        ft_fswap(&t->p[0].x, &t->p[1].x);
        ft_fswap(&t->p[0].y, &t->p[1].y);
        ft_fswap(&t->tx[0].u, &t->tx[1].u);
        ft_fswap(&t->tx[0].v, &t->tx[1].v);
        ft_fswap(&t->tx[0].w, &t->tx[1].w);
    }
    if (t->p[2].y < t->p[0].y)
    {
        ft_fswap(&t->p[2].x, &t->p[0].x);
        ft_fswap(&t->p[2].y, &t->p[0].y);
        ft_fswap(&t->tx[2].u, &t->tx[0].u); 
        ft_fswap(&t->tx[2].v, &t->tx[0].v); 
        ft_fswap(&t->tx[2].w, &t->tx[0].w); 
    }
    if (t->p[2].y < t->p[1].y)
    {
        ft_fswap(&t->p[2].x, &t->p[1].x);
        ft_fswap(&t->p[2].y, &t->p[1].y);
        ft_fswap(&t->tx[2].u, &t->tx[1].u); 
        ft_fswap(&t->tx[2].v, &t->tx[1].v); 
        ft_fswap(&t->tx[2].w, &t->tx[1].w); 
    }
}

static void     update_step(t_filltex *f)
{
    f->simple[0] -= f->step[0];
	f->simple[1] += f->step[1];
	f->simple[2] -= f->step[2];
	f->simple[3] += f->step[3];
	f->simple[4] -= f->step[4];
	f->simple[5] += f->step[5];
}

static void     compute_step(t_filltex *f, float tstep)
{
    f->step[0] = tstep * f->su;
	f->step[1] = tstep * f->eu;
	f->step[2] = tstep * f->sv;
	f->step[3] = tstep * f->ev;
	f->step[4] = tstep * f->sw;
	f->step[5] = tstep * f->ew;
	f->simple[0] = f->su;
	f->simple[1] = 0;
	f->simple[2] = f->sv;
	f->simple[3] = 0;
	f->simple[4] = f->sw;
	f->simple[5] = 0;
}

static void     texturize(t_env *e, t_filltex *f, float i, int j)
{
    //f->tex_u = ((1.0f - f->t) * f->su) + (f->t * f->eu);
    //f->tex_v = ((1.0f - f->t) * f->sv) + (f->t * f->ev);
    //f->tex_w = ((1.0f - f->t) * f->sw) + (f->t * f->ew);
    //printf("tu = %f ty = %f, tw = %f\n", f->tex_u, f->tex_v, f->tex_w);
    
    //if ()

    put_pixel(e, i, j, get_pixel(e->mesh[e->mesh_id].img, f->tex_u / f->tex_w, f->tex_v / f->tex_w));
}

static void     texture_p_image(t_env *e, t_filltex *f, t_triangle *tri, float i)
{
    float   tstep;
    float   j;

    f->tex_u = f->su;
    f->tex_v = f->sv;

    f->t = 0.0f;
    j = f->ax - 1;
    tstep = 1.0f / (f->bx - f->ax);
    compute_step(f, tstep);
    while (++j < f->bx)
    {
        //printf("t = %f\n", t);
        f->tex_u = f->simple[0] + f->step[1];
        f->tex_v = f->simple[2] + f->step[3];
        f->tex_w = f->simple[4] + f->step[5];
        update_step(f);
        texturize(e, f, i, j);
        f->t += tstep;
    }
}

static void     calc_step(t_filltex *f)
{
    if (f->dy1)
    {
        //printf("1\n");
        f->dax_s = f->dx1 / f->dy1;
        f->du1_s = f->du1 / f->dy1;
        f->dv1_s = f->dv1 / f->dy1;
        f->dw1_s = f->dw1 / f->dy1;
    }
    if (f->dy2)
    {
        //printf("2\n");
        f->dbx_s = f->dx2 / f->dy2;
        f->du2_s = f->du2 / f->dy2;
        f->dv2_s = f->dv2 / f->dy2;
        f->dw2_s = f->dw2 / f->dy2;

    }
}

static void     fill_triangle_texture_2bis(t_env *e, t_filltex *f, t_triangle *t)
{
    float   i;
    float   end;

    calc_step(f);
    i = ceil(t->p[0].y) - 1;
    end = ceil(t->p[1].y);
    while (++i <= end)
    {
        //printf("3\n");
        f->ax = t->p[1].x + (float)(i - t->p[1].y) * f->dax_s; 
        f->bx = t->p[0].x + (float)(i - t->p[0].y) * f->dbx_s;
        //printf("ax = %f     bx = %f\n", f->ax, f->bx);
 
        f->su = t->tx[1].u + (float)(i - t->p[1].y) * f->du1_s; 
        f->sv = t->tx[1].v + (float)(i - t->p[1].y) * f->dv1_s;
        f->sw = t->tx[1].w + (float)(i - t->p[1].y) * f->dw1_s;
        
        f->eu = t->tx[0].u + (float)(i - t->p[0].y) * f->du2_s; 
        f->ev = t->tx[0].v + (float)(i - t->p[0].y) * f->dv2_s;
        f->ew = t->tx[0].w + (float)(i - t->p[0].y) * f->dw2_s; 
        if (f->ax > f->bx)
        {
            ft_fswap(&f->ax, &f->bx);
            ft_fswap(&f->su, &f->eu);
            ft_fswap(&f->sv, &f->ev);
            ft_fswap(&f->sw, &f->ew);
        }
        texture_p_image(e, f, t, i);
    }
}

static void     fill_triangle_texture_2(t_env *e, t_filltex *f, t_triangle *t)
{
    float   i;
    float   end;
    
    calc_step(f);
    i = ceil(t->p[0].y) - 1;
    end = ceil(t->p[1].y);
    //printf("start %f\nend %f\n", t->p[0].y, t->p[1].y);
    while (++i <= end)
    {
        f->ax = t->p[0].x + (i - t->p[0].y) * f->dax_s; 
        f->bx = t->p[0].x + (i - t->p[0].y) * f->dbx_s;
        //printf("ax = %d     bx = %d\n", f->ax, f->bx);        
        f->su = t->tx[0].u + (i - t->p[0].y) * f->du1_s; 
        f->sv = t->tx[0].v + (i - t->p[0].y) * f->dv1_s;
        f->sw = t->tx[0].w + (i - t->p[0].y) * f->dw1_s;
        f->eu = t->tx[0].u + (i - t->p[0].y) * f->du2_s; 
        f->ev = t->tx[0].v + (i - t->p[0].y) * f->dv2_s;
        f->ew = t->tx[0].w + (i - t->p[0].y) * f->dw2_s; 
        if (f->ax > f->bx)
        {
            ft_fswap(&f->ax, &f->bx);
            ft_fswap(&f->su, &f->eu);
            ft_fswap(&f->sv, &f->ev);
            ft_fswap(&f->sw, &f->ew);
        }
        texture_p_image(e, f, t, i);
    }
}

void            fill_triangle_texture(t_env *e, t_triangle t)
{
    t_filltex   fill;
    
    ft_memset(&fill, 0, sizeof(t_fill));
    //printf("===============START==============================\n");
    sort_orthogonal(&t);
    fill.dy1 = t.p[1].y - t.p[0].y;
    fill.dx1 = t.p[1].x - t.p[0].x;
    fill.du1 = t.tx[1].u - t.tx[0].u;
    fill.dv1 = t.tx[1].v - t.tx[0].v;
    fill.dw1 = t.tx[1].w - t.tx[0].w;

    fill.dy2 = t.p[2].y - t.p[0].y;
    fill.dx2 = t.p[2].x - t.p[0].x;
    fill.du2 = t.tx[2].u - t.tx[0].u;
    fill.dv2 = t.tx[2].v - t.tx[0].v;
    fill.dw2 = t.tx[2].w - t.tx[0].w;
    //printf("u %f v %f w %f\n", t.tx[2].u, t.tx[2].v, t.tx[2].w);
    //printf("x %f y %f z %f\n", t.p[2].x, t.p[2].y, t.p[2].z);
    printf("\n----<> Enter Fille triangle Phase 1\n");
    if (fill.dy1)
        fill_triangle_texture_2(e, &fill, &t);
    printf("\n----<> Out\n");
    fill.dy1 = t.p[2].y - t.p[1].y;
    fill.dx1 = t.p[2].x - t.p[1].x;
    fill.du1 = t.tx[2].u - t.tx[1].u;
    fill.dv1 = t.tx[2].v - t.tx[1].v;
    fill.dv1 = t.tx[2].w - t.tx[1].w;
    printf("\n----<> Enter Fille triangle Phase 2\n");
    if (fill.dy1)
        fill_triangle_texture_2bis(e, &fill, &t);
    printf("\n----<> Out\n");
}