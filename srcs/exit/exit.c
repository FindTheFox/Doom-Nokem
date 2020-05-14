/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saneveu <saneveu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 02:28:52 by saneveu           #+#    #+#             */
/*   Updated: 2020/03/19 21:59:49 by saneveu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/3d_engine.h"

/*
static void free_mesh(t_env *e)
{
    int i;

    i = -1;
    while (++i < e->nbmesh)
        ft_memdel((void **)e->mesh[i].tris);
    //free((t_mesh*)e->mesh);
}
*/

void        ft_exit(t_env *env, char *s, int flag)
{
    if (env->rend)
        SDL_DestroyRenderer(env->rend);
    if (env->window)
        SDL_DestroyWindow(env->window);
    //free_mesh(env);
    TTF_CloseFont(env->menu.font);
    SDL_Quit();
    TTF_Quit();
    if (s)
        ft_putstr_fd(s, 2);
    exit(flag == 1 ? EXIT_SUCCESS : EXIT_FAILURE);
}