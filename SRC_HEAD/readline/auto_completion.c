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

void	ft_d(t_line *line)
{
	tputs(tgoto(tgetstr("cm", 0), 0, line->c_o.y), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
	ft_prompte();
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	print_line(g_str);
	move_cursor_v(line);
	cur_goto(line, line->cursor);
}


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

int			get_blen(t_affcmpl *head)
{
	int		blen;
	int		k;

	blen = 0;
	k = 0;
	while (head)
	{
		k = ft_strlen(head->content);
		blen = (blen < k) ? k : blen;
		head = head->next;
	}
	return (blen);
}

void ft_putnchar(char c, int n)
{
	while(n)
	{
		ft_putchar(c);
		n--;
	}
}

void afficher_file(t_affcmpl *head, t_line *line)
{
	struct winsize	w;

	ioctl(0, TIOCGWINSZ, &w);
	int row = line->c_o.y + count_row(line);
	int blen;
	int col = 0;
	int d = 0;
	blen = get_blen(head);
	int k = w.ws_col / (blen + 1);
	if (k == 0)
		k++;
	ft_putchar('\n');
	while (head->next)
	{
		tputs(tgoto(tgetstr("cm", 0), col, row), 0, ft_output);
		ft_putstr(head->content);
		head = head->next;
		d++;
		if (d % k == 0)
		{
			col = 0;
			if (head->next)
			{
				ft_putchar('\n');
				row += 1;
			}
		}
		else
			col += blen + 1;
	}
	ft_putchar('\n');
	line->c_o.y = row + 1;
	ft_d(line);
}
void completion_str(t_affcmpl *head, t_line *line,char **str)
{
	int plus_len = ft_strlen(head->content) - line->compl.len;
	char *third_str = ft_strsub(*str,line->cursor,line->b_line -line->cursor);
	char *second_str = ft_strsub(*str,0,line->cursor - line->compl.len);
	char *tmp = ft_freejoin(second_str,head->content,0);
	ft_strdel(str);
	*str = ft_freejoin(tmp,third_str,2);
	line->b_line += plus_len;
	line->len += plus_len;
	line->cursor += plus_len;
	ft_multilne(*str, line);
	line->c_len += plus_len;
	ft_clear(line, *str);
}

t_affcmpl *stock_path_file(char *str,int flag,t_line *line)
{
	DIR *dir;
	struct dirent *dent;
	t_affcmpl *affcmpl;
	t_affcmpl *affcmpltmp;

	affcmpl = ft_memalloc(sizeof(t_affcmpl));
	affcmpltmp = affcmpl;
	line->compl.count = 0;
	if ((dir = opendir(str)))
	{
		while((dent=readdir(dir))!=NULL)
		{
			if ((flag && !ft_strncmp(line->compl.search,dent->d_name,line->compl.len))|| !flag)
			{
				affcmpl->content = ft_strdup(dent->d_name);
				affcmpl->next = ft_memalloc(sizeof(t_affcmpl));
				affcmpl = affcmpl->next;
				line->compl.count++;
			}
		}
		closedir(dir);
	}
	return(affcmpltmp);
}

void make_path_file(t_line *line)
{
	if (ft_strchr(line->compl.str,'/'))
	{
		line->compl.search = ft_strdup(ft_strrchr(line->compl.str, '/') + 1);
		line->compl.path = ft_strsub(line->compl.str, 0, ft_strlen(line->compl.str) - ft_strlen(line->compl.search));
	}
	else if (!ft_strchr(line->compl.str,'/'))
	{
		line->compl.path = ft_strdup("./");
		line->compl.search = ft_strdup(line->compl.str);
	}
}

void make_path_completion(t_line *line,char **str)
{
	
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
		t_affcmpl *affcmpltmp;
		int flag;
		make_path_file(line);
		line->compl.len = ft_strlen(line->compl.search);
		flag = (!line->compl.len) ? 0: 1;
		affcmpltmp = stock_path_file(line->compl.path,flag,line);
		if (line->compl.count > 1)
			afficher_file(affcmpltmp,line);
		else if (line->compl.count == 1)
			completion_str(affcmpltmp,line,str);
	}
}


void ft_auto_completion(t_line *line, char **str)
{
	if (line->compl.str && *line->compl.str)
		ft_strdel(&(line->compl.str));
	if (line->compl.search && *line->compl.search)
		ft_strdel(&(line->compl.search));
	if (line->compl.path && *line->compl.path)
		ft_strdel(&(line->compl.path));
	ft_parce_completion(line, str);
	char * tmp = ft_strdup(line->compl.str);
	ft_strdel(&line->compl.str);
	line->compl.str = ft_strtrim(tmp);
	ft_strdel(&(tmp));
	make_path_completion(line,str);
}
