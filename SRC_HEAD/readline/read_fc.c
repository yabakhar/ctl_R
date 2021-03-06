/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabakhar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 00:09:39 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/28 18:20:56 by yabakhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void opt_error(char c)
{
	ft_putstr("42sh: fc: -");
	ft_putchar(c);
	ft_putendl(" : invalid option");
	ft_putendl("fc: usage: fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]");
}

int ft_isnumber(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int check_index_fc(char *str, int *index)
{
	if ((str[0] == '-') && (ft_isnumber(str + 1)))
	{
		(*index) = ft_atoi(str);
		return (1);
	}
	else if (ft_isnumber(str))
	{
		(*index) = ft_atoi(str);
		return (1);
	}
	return (0);
}

int check_isnbrvalide(char *str)
{
	if ((str[0] == '-') && (ft_isnumber(str + 1)))
		return (1);
	else if (ft_isnumber(str))
		return (1);
	return (0);
}

int index2_fc(char *str, int *index)
{
	t_node *list;
	int i;

	list = add_to_history(NULL);
	i = 0;
	while (list)
	{
		if (!ft_strncmp(str, list->content, ft_strlen(str)))
			(*index) = i;
		list = list->next;
		i++;
	}
	if ((*index) > 0)
		return (1);
	else
		return (0);
}

int get_index_fc(char *hold, int *index)
{
	if (!(check_index_fc(hold, index)) && !(index2_fc(hold, index)))
	{
		ft_putendl("42sh: fc: history specification out of range");
		return (0);
	}
	return (1);
}

int preparer_path(char *editeur, t_opt *opt)
{
	char *path = ft_strjoin("/usr/bin/", editeur);
	if (access(path, F_OK) == 0)
	{
		if (!ft_strcmp("vim", editeur) || !ft_strcmp("vi", editeur) ||
			!ft_strcmp("nano", editeur) || !ft_strcmp("emacs", editeur))
		{
			opt->editeur = editeur;
			ft_strdel(&path);
			return (1);
		}
	}
	else
	{
		ft_putstr("42sh: command not found: ");
		ft_putendl(editeur);
		ft_strdel(&path);
		return (0);
	}
	return (0);
}

int check_debut_fin_e(t_opt *opt, char **hold)
{
	if (!opt->count)
	{
		ft_putendl("fc: argument expected: -e");
		return (0);
	}
	else if (opt->count > 3)
	{
		ft_putendl("fc: too many arguments");
		return (0);
	}
	else if (opt->count <= 3)
	{
		if (!preparer_path(hold[opt->check], opt))
			return (0);
		if (opt->count == 1)
			opt->debut = -1;
		else if (opt->count >= 2)
		{
			if (!(get_index_fc(hold[opt->check + 1], &opt->debut)))
				return (0);
			if (opt->count > 2)
				if (!(get_index_fc(hold[opt->check + 2], &opt->fin)))
					return (0);
		}
		return (1);
	}
	return (0);
}

void count_arg_fc(t_opt *opt)
{
	if (opt->check == 0)
		opt->check = opt->i;
	opt->count += 1;
}

int check_opt(t_opt *opt, char *hold)
{
	int i;

	if (check_isnbrvalide(hold))
	{
		count_arg_fc(opt);
		return (1);
	}
	else if (hold[0] == '-')
	{
		i = 1;
		while (hold[i])
		{
			if (hold[i] == 'l' && !opt->check)
				opt->l = 1;
			else if (hold[i] == 'n' && !opt->check)
				opt->n = 1;
			else if (hold[i] == 'e' && !opt->check)
				opt->e = 1;
			else if (hold[i] == 's' && !opt->check)
				opt->s = 1;
			else if (hold[i] == 'r' && !opt->check)
				opt->r = 1;
			else
			{
				if (!ft_strchr("lners", hold[i]) && !opt->check)
				{
					opt_error(hold[i]);
					return (0);
				}
				else
					count_arg_fc(opt);
			}
			i++;
		}
	}
	else
		count_arg_fc(opt);
	return (1);
}

void execute_commande_fc(const char *file)
{
	char *line = NULL;
	int fd;

	if ((fd = open(file, O_RDONLY | O_CREAT, 00600)) == -1)
		return ;
	while (get_next_line(fd, &line) > 0)
	{
		ft_putendl(line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	close(fd);
}

t_node *ft_get_tail(t_node *history)
{
	while (history)
	{
		if (history->next == NULL)
			return (history);
		history = history->next;
	}
	return (history);
}

int				ft_calc(char **hold)
{
	int i;

	i = 0;
	while (hold[i])
		i++;
	if (i > 0)
		i--;
	return (i);
}

void ft_fc_l3adiya(t_opt *opt, char **hold)
{
	t_node *tail;
	t_node *history;
	int fd;
	char **result;
	int size;
	if ((fd = open(PATH_FC_FILE, O_RDWR | O_TRUNC | O_CREAT, 00600)) == -1)
		return;
	history = add_to_history(NULL);
	if (!opt->count)
	{
		tail = ft_get_tail(history);
		ft_putendl(tail->content);
	}
	if (ft_get_debut_fin(opt, hold))
	{
		if (opt->count == 1)
		{
			opt->debut = ((ft_abs(opt->debut) > opt->sizeoflist) || (opt->debut == 0)) ? (-1) : opt->debut;
			opt->debut = (opt->debut < 0) ? (opt->sizeoflist - ft_abs(opt->debut)) : opt->debut;
			get_index_in_list(&history, opt->debut);
			ft_putendl_fd(history->content, fd);
		}
		else if (opt->count >= 2)
		{
			ft_calc_debut_fin(opt);
			ft_calc_range_of_debut_fin(opt, &size, &result);
			ft_affiche_tab_e(result, ft_abs(size), opt, ft_sin(size));
		}
	}
	close(fd);
	execute_commande_fc(PATH_FC_FILE);
}

void ft_handel_fc(t_opt *opt, char **hold)
{
	t_node *history;

	history = add_to_history(NULL);
	if (opt->s == 1)
	{
		if (!opt->count)
		{
			t_node *tail;
			tail = ft_get_tail(history);
			ft_putendl(tail->content);
		}
		else if (opt->count >= 1)
		{
			if (!(get_index_fc(hold[opt->check], &opt->debut)))
				return;
			ft_putendl(get_content_in_list(opt->debut));
		}
	}
	else if (opt->l == 1)
		fc_l(opt, hold);
	else if (opt->e == 1)
		fc_e(opt, hold, history);
	else if ((opt->i - opt->count) == 1 || opt->n || opt->r)
		ft_fc_l3adiya(opt, hold);
}

void parce_param_fc(char **hold)
{
	t_opt opt;
	t_node *history;

	ft_bzero(&opt, sizeof(t_opt));
	history = add_to_history(NULL);
	opt.sizeoflist = calc_list(history);
	while (++opt.i < (ft_calc(hold) + 1))
	{
		if (!(check_opt(&opt, hold[opt.i])))
			return;
	}
	ft_handel_fc(&opt, hold);
}
