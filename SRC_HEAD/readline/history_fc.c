// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   history.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yabakhar <marvin@42.fr>                    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2019/12/21 23:59:46 by yabakhar          #+#    #+#             */
// /*   Updated: 2020/05/28 06:03:08 by yabakhar         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../includes/sh.h"

// static t_history *history_head;

// void load_hsitory(const char *file)
// {
// 	char *line;
// 	int num;

// 	num = 0;
// 	line = NULL;
// 	int fd = open(file, O_RDONLY);
// 	while (get_next_line(fd, &line) > 0)
// 	{
// 		add_to_history(line,num);
// 		ft_strdel(&line);
// 		num++;
// 	}
// 	ft_strdel(&line);
// 	close(fd);
// }

// t_history *add_to_history(const char *str,int num)
// {
// 	t_history *new;

// 	if (!str)
// 		return(history_head);
// 	if ((new = (t_history *)ft_memalloc(sizeof(t_history))))
// 	{
// 		new->content_len = ft_strlen(str);
// 		new->content = ft_strdup(str);
// 		new->num = num;
// 		new->next = history_head;
// 		if (history_head)
// 			history_head->prev = new;
// 		history_head = new;
// 		return (new);
// 	}
// 	return (history_head);
// }

// void free_history_node(t_history *node)
// {
// 	if (node)
// 	{
// 		if (node->prev == NULL)
// 			history_head = node->next;
// 		else
// 			node->prev->next = node->next;
// 		if (node->next)
// 			node->next->prev = node->prev;
// 		free(node->content);
// 		free(node);
// 	}
// }

// t_history *get_head_history(void)
// {
// 	return (history_head);
// }