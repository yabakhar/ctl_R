/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_readline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 14:27:02 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	ft_print_print(char **str, t_line *line, char *buff)
{
	int	i;

	i = -1;
	while (buff[++i])
		if (ft_isprint(buff[i]) || buff[i] == '\n')
			ft_printnbl(str, line, buff[i]);
	ft_clear(line, *str);
}


int keyshandle3(t_line *line, char **str)
{
	int r;

	r = 0;
	if (line->r == DEL && line->slct == 0 && !line->mode_r.flag && (r = 1))
		ft_delet(str, line);
	else if (line->r == DEL && line->slct == 0 && line->mode_r.flag && (r = 1))
		delet_mode_r(&line->mode_r.s,line);
	return(r);
}

int		keyshendle2(t_line *line, char **str, int flag)
{
	int	r;

	r = 0;
	if (line->r == ALT_RTH && line->slct == 0 && (r = 1))
		ft_alt_rth(*str, line);
	else if (line->r == ALT_LFT && line->slct == 0 && (r = 1))
		ft_alt_lft(*str, line);
	else if (line->r == CTRL_L && line->slct == 0 && (r = 1))
		ft_ctl_l(line, *str);
	else if (line->r == ALT_D && (!line->b_line) && line->slct == 0)
		ft_ctl_d(flag);
	else if (line->r == DEL && line->slct == 0 && (r = 1))
		ft_delet(str, line);
	return (r);
}

int		keyshendle1(t_line *line, char **str, t_node **current)
{
	int r;

	r = 0;
	if ((line->r == HOME || line->r == DEEP) && line->slct == 0 && (r = 1))
		home_deep(line, *str);
	else if (line->r == UP && line->slct == 0 && (r = 1))
		ft_history_goto(current, (*current)->next, line);
	else if (line->r == DOWN && line->slct == 0 && (r = 1))
		ft_history_goto(current, (*current)->prev, line);
	else if (line->r == ALT_V && line->sltstr && (r = 1))
	{
		line->slct = 0;
		line->slctd = 0;
		line->slctf = 0;
		ft_print_print(str, line, line->sltstr);
	}
	return (r);
}

int		keyshendle(t_line *line, char **str)
{
	int r;

	r = 0;
	if (line->r == LEFT && (r = 1))
		move_left(line, *str);
	else if (line->r == RIGHT && (r = 1))
		move_right(line, *str);
	else if (line->r == ALT_S && (r = 1))
		ft_chack_selction(line, *str);
	else if (line->r == 18 && line->slct == 0 && !line->mode_r.flag && (r = 1))
		mode_r(line);
	else if (line->r == ALT_C && (r = 1))
		ft_copie(line, *str);
	else if (line->r == PAGE_DOWN && line->slct == 0 && (r = 1))
		move_down(line);
	else if (line->r == PAGE_UP && line->slct == 0 && (r = 1))
		move_up(line);
	return (r);
}

int		ft_readline_builtines(int flag, char *buff, t_line *line, t_node **current)
{
	int k;

	k = 0;
	if (keyshendle(line, &(*current)->tmp))
		return (1);
	else if (keyshendle1(line, &(*current)->tmp, current))
		return (1);
	else if (keyshandle3(line, &(*current)->tmp))
		return (1);
	else if (keyshendle2(line, &(*current)->tmp, flag))
		return (1);
	else if ((line->r == END && line->slct == 0) || g_clt_c == 1)
		return (0);
	else if (line->slct == 0 && !line->mode_r.flag)
	{
		ft_print_print(&(*current)->tmp, line, buff);
		return (1);
	}
	else if (line->mode_r.flag)
	{
		t_node *node;
		line->mode_r.tmp = line->r;
		tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		prompte_mode_r(line->mode_r.tmp, &line->mode_r.s);
		if (get_index_mode_r(line,&k))
		{
			node = add_to_history(NULL);
			while (--k > 0)
				node = (node)->next;
			*current = node;
		}
		ft_history_goto(current, (*current), line);
		tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		ft_putstr("bck-i-search: ");
   		ft_putstr(line->mode_r.s);
		return(1);
	}
	return (0);
}
