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

void ft_affichat(t_line *line,int wahed_variable)
{
	int fd = open("/dev/ttys002",O_RDWR);

	ft_putendl_fd("-----------",fd);
	ft_putstr_fd("wahed_variable ====>   ",fd);
	ft_putnbr_fd(wahed_variable,fd);
	ft_putendl_fd("",fd);

	ft_putstr_fd("line->c_o.y ====>   ",fd);
	ft_putnbr_fd(line->c_o.y,fd);
	ft_putendl_fd("",fd);


	ft_putstr_fd("line->row ====>   ",fd);
	ft_putnbr_fd(line->row,fd);
	ft_putendl_fd("",fd);

	ft_putstr_fd("line->mode_r.y ====>   ",fd);
	ft_putnbr_fd(line->mode_r.y,fd);
	ft_putendl_fd("",fd);

	int l7sab_jdid  = line->c_o.y - (wahed_variable - line->row);
	ft_putstr_fd("l7sab_jdid ====>   ",fd);
	ft_putnbr_fd(l7sab_jdid,fd);
	ft_putendl_fd("",fd);
} 	

void	xxxxxxxx(t_line *line,t_node **current)
{
	int len;

	if (line->mode_r.s && *line->mode_r.s)
		len = ft_strlen(line->mode_r.s)/*+ string dyal prompte*/;
	else
		len = 10;
	int wahed_variable =  (len / line->col);
	if ((len % line->col) > 0)
		wahed_variable += 1; 
	wahed_variable += line->mode_r.y;
	if (((wahed_variable - line->row) > 0))
	{
		line->c_o.y = line->c_o.y - (wahed_variable - line->row);
		tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
		ft_putstr("\n");
		move_cursor_v(line);
	}
}

void	ft_disable_mode_r(t_line *line)
{
	if (line->mode_r.flag)
	{
		tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		if (line->mode_r.s && *line->mode_r.s)
			ft_strdel(&line->mode_r.s);
		cur_goto(line, line->cursor);
		line->mode_r.flag = 0;
	}
}

int    get_index_mode_r(t_line *line,int *index)
{
    t_node *node = add_to_history(NULL);
    int i = 0;
    *index = 0;
    while (node)
    {
        i++;
        if (ft_strstr(node->content, line->mode_r.s))
        {
            *index = i;
            break;
        }
        node = node->next;
    }
    if ((*index) > 0)
		return (1);
	else
    	return(0);
}

void mode_r(t_line *line,t_node **current)
{
    if (!line->mode_r.flag)
    {
        if (line->mode_r.s && *line->mode_r.s)
            ft_strdel(&line->mode_r.s);
        line->mode_r.y = line->c_o.y + count_row(line);
		xxxxxxxx(line,current);
		tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
		move_cursor_v(line);
        prompte_mode_r(0,&line->mode_r.s);
        ft_putstr("bck-i-search: ");
        line->mode_r.flag = 1;
    }
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
}


void        print_prompte_(t_line *line, int error)
{
	line->mode_r.y = line->c_o.y + count_row(line);
	tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
    if (error)
	    ft_putstr("failing ");
	ft_putstr("bck-i-search: ");
	ft_putstr(line->mode_r.s);
}

void        search_mode_r(t_line *line, t_node **current)
{
    t_node *node;
    int k;

	k = 0;
    node = NULL;
    line->mode_r.tmp = line->r;
	prompte_mode_r(line->mode_r.tmp, &line->mode_r.s);
	if (get_index_mode_r(line, &k))
	{
		node = add_to_history(NULL);
		while (--k > 0)
			node = (node)->next;
		*current = node;
		tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		ft_history_goto(current, (*current), line);
		xxxxxxxx(line,current);
		print_prompte_(line, 0);
	}
	else
	{
		xxxxxxxx(line,current);
		print_prompte_(line, 1);
	}
}

void		delet_mode_r(char **str,t_line *line)
{
	char	*tmp;
    int     len;

    len = ft_strlen(*str);
	tmp = ft_strsub(*str, 0, len - 1);
	ft_strdel(str);
	*str = ft_strdup(tmp);
    ft_strdel(&tmp);
	tputs(tgoto(tgetstr("cm", 0), 0, line->mode_r.y), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
    if (!(get_index_mode_r(line, &len)))
        ft_putstr("failing ");
	ft_putstr("bck-i-search: ");
   	ft_putstr(line->mode_r.s);
}
