/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_comlition.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabakhar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 12:17:19 by yabakhar          #+#    #+#             */
/*   Updated: 2021/02/06 12:17:20 by yabakhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

int is_in_str(char c, const char *cmp)
{
	while(*cmp)
	{
		if(*cmp  == c)
			return(1);
		cmp++;
	}
	return(0);
}

char	*ft_strrsearch(char *s, const char *cmp, size_t size)
{
	while (size > 0)
	{
		if (is_in_str(s[size], cmp))
			return (s + size);
		size--;
	}
	return (s + size);
}

char	*ft_strrsearch2(char *s, const char *cmp, size_t size)
{
	while (size > 0)
	{
		if (!is_in_str(s[size], cmp))
			return (s + size);
		size--;
	}
	return (s + size);
}


void	get_str_for_search(char *str,t_line *line, int cursor_pos)
{
	int tmp;
	int len;
	int debut;

	tmp = line->compl.prefix_pos;
	len = cursor_pos - tmp;
	debut = (tmp > 0) ? tmp : tmp++;
	line->compl.str = ft_strsub(str,debut,len);
}

void  ft_parce_completion(t_line *line, char **str)
{
	int		cursor_pos;
	char	*lastchar;
	char	*prefix ;
	
	cursor_pos = line->c_len - (1 * (line->c_len > 0));
	prefix = ft_strrsearch(*str, " ;&$|", cursor_pos);
	line->compl.prefix_pos = prefix - (*str);
	get_str_for_search(*str,line, line->c_len);
	if (*prefix == '$')
		line->compl.type = 1;
	else
	{
		lastchar = ft_strrsearch2(*str, " ", line->compl.prefix_pos);
		if(is_in_str(*lastchar, " &|;") || lastchar == str[0])
			line->compl.type = 0;
		else
			line->compl.type = 2;
	}
}

#include <dirent.h>

void ddd(char *str,int flag,t_line *line)
{
	DIR *dir;
	struct dirent *dent;
	dir = opendir(str);
	t_affcmpl *affcmpl;
	t_affcmpl *affcmpltmp = affcmpl;
	if (dir != NULL)
	{
		while((dent=readdir(dir))!=NULL)
		{
			if ((flag && !ft_strncmp(line->compl.str,dent->d_name,line->compl.len))|| !flag)
			{
				affcmpl = ft_memalloc(sizeof(t_affcmpl));
				affcmpl->content = ft_strdup(dent->d_name);
				ft_putendl_fd(affcmpl->content,open("/dev/ttys002",O_RDWR));
				affcmpl = affcmpl->next;
			}
		}
		closedir(dir);
	}
}

void make_path_file(t_line *line)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strchr(line->compl.str,'/') && line->compl.str[0] != '/' && line->compl.str[0] != '.')
	{
		line->compl.path = ft_strjoin("./", line->compl.str);
		line->compl.search = ft_strdup(ft_strrchr(line->compl.str, '/') + 1);
		tmp = line->compl.path;
		line->compl.path = ft_strsub(line->compl.path, 0, ft_strlen(line->compl.path) - ft_strlen(line->compl.search));
		ft_strdel(&tmp);
	}
	else if (!ft_strchr(line->compl.str,'/'))
	{
		line->compl.path = ft_strdup("./");
		line->compl.search = ft_strdup(line->compl.str);
	}
	ft_putendl_fd(line->compl.path,open("/dev/ttys002",O_RDWR));
	ft_putendl_fd(line->compl.search,open("/dev/ttys002",O_RDWR));
}

void make_path_completion(t_line *line)
{
	line->compl.len = ft_strlen(line->compl.str);
	if (!line->compl.type)
	{

		// if (!line->compl.len || line->compl.len == 1)
		// 	ddd("/dev",0,line);
		// else
		// 	ddd("/dev",1,line);
			/* PATH : /some Character */
	}
	// else if (line->compl.type == 1)
	// {
	// 	if (line->compl.len == 1)
	// 		ft_putendl_fd(line->compl.str,open("/dev/ttys002",O_RDWR));//NULL parameter
	// 	else
	// 		ft_putendl_fd(line->compl.str,open("/dev/ttys002",O_RDWR));
	// }
	if (line->compl.type == 2)
	{ 
		make_path_file(line);
		// if (!line->compl.len)
		// 	ddd(".",0,line);
		// // 	//  NULL file /* PATH : ./ */
		// else
		// 	ddd(".",1,line);
		// 	 /* PATH : ./some Character */
	}
}


void ft_auto_completion(t_line *line, char **str)
{
	if (line->compl.str && *line->compl.str)
		ft_strdel(&(line->compl.str));
	ft_parce_completion(line, str);
	char * tmp = ft_strdup(line->compl.str);
	ft_strdel(&line->compl.str);
	line->compl.str = ft_strtrim(tmp);
	ft_strdel(&(tmp));
	make_path_completion(line);
}

// t_pp		*ft_stock(char **av,t_line *line)
// {
	// t_pp	*list;
	// t_pp	*node;
	// int		i;
// 
	// i = 0;
	// if (!(list = ft_memalloc(sizeof(t_pp))))
		// return (0);
	// node = list;
	// while (av[i])
	// {
		// if (!ft_strncmp(av[i],line->compl.str,ft_strlen(line->compl.str)))
			// list->content = ft_strdup(av[i]);
		// list->index = i;
		// if (av[i + 1])
			// list->next = ft_memalloc(sizeof(t_pp));
		// else
			// list->next = NULL;
		// list = list->next;
		// i++;
	// }
	// return (node);
// }
