/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_r.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabakhar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:50:24 by yabakhar          #+#    #+#             */
/*   Updated: 2021/01/16 14:50:38 by yabakhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void mode_r(t_line *line)
{
    ft_clear_mode_r(line);
    line->mode_r.flag = 1;
}

void	init_mode_r(t_line *line)
{
	line->mode_r.y = (line->c_o.y + line->index + 1);
	line->mode_r.x = 0;
	tputs(tgoto(tgetstr("cm", 0), line->mode_r.x,
        line->mode_r.y), 0, ft_output);
}

void	ft_clear_mode_r(t_line *line)
{
    init_mode_r(line);
    prompte_mode_r(0,&line->mode_r.s);
    line->mode_r.x = (int)ft_strlen("bck-i-search: ");
}

void prompte_mode_r(char c, char **str)
{
    char ttab[2];

    ttab[0] = c;
    ttab[1] = '\0';
    if (str && *str)
        *str = ft_freejoin(*str, ttab, 0);
    else if (str && !*str)
        *str = ft_strdup(ttab);
    ft_putstr("bck-i-searKh: ");
    ft_putstr(*str);
}
