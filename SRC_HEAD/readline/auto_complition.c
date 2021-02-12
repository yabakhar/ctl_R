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

void  ft_parce_complition(t_line *line, char **str)
{
	int		cursor_pos;
	char	*lastchar;
	char	*prefix ;
	
	cursor_pos = line->c_len - (1 * (line->c_len > 0));
	prefix = ft_strrsearch(*str, " ;&$|", cursor_pos);
	line->compl.prefix_pos = prefix - (*str);
	get_str_for_search(*str,line, line->c_len);
	if (*prefix == '$')
		line->compl.type = 0;
	else
	{
		lastchar = ft_strrsearch2(*str, " ", line->compl.prefix_pos);
		if(is_in_str(*lastchar, " &|;") || lastchar == str[0])
			line->compl.type = 2;
		else
			line->compl.type = 1;
	}
}

void ft_auto_complition(t_line *line, char **str)
{
	ft_parce_complition(line, str);
	ft_putendl_fd(line->compl.str,open("/dev/ttys002",O_RDWR));
	ft_putnbr_fd(line->compl.type,open("/dev/ttys002",O_RDWR));
	ft_putendl_fd("   ->type",open("/dev/ttys002",O_RDWR));
}
