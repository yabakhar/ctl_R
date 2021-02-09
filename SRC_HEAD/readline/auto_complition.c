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





// void	xxxx(char *str,int c_len)
// { /*
// 	// int j = c_len;

// 	// while (str[j] && j > 0)
// 	// {
// 	// 	if (!ft_isalpha(str[j]))
// 	// 		break;
// 	// 	j--;
// 	// }
// 	// int i = j;
// 	// str[j] == ' ' ? j++ : j;
// 	// char *s = ft_strsub(str,j,c_len - j);
// 	// hadchi m7tajo
// 	*/
// 	// ft_putnbr_fd(xxxxc(c_len,str),open("/dev/ttys001",O_RDWR));
// }

int ft_ispace(int index,char *str)
{
	int flag = 0;
	while (str[index])
	{
		if (str[index] == ' ' && flag)
			flag = 2;
		else if (ft_isalnum(str[index]) && !flag)
			flag = 1;
		else if (!ft_isalnum(str[index]) && str[index] != ' ')
			return (0);
		if (ft_isalnum(str[index]) && flag == 2)
			return (1);
		index--;
	}
	return (0);
}

int	ft_tab_checker(int index,char *str)
{
	int flag = 0;
	while(str[index])
	{
		if (ft_isalnum(str[index]) && flag)
			return (1);
		else if (str[index] == ' ' && !flag)
			flag = 1;
		else if (ft_strchr("&|;",str[index]))
			return(0);
		index--;
	}
	return (0);	
}

int ft_ciparateur(int index,char *str)
{
	int flag = 0;
	while (str[index])
	{
		if (ft_strchr("&|;",str[index]) && flag)
			return (0);
		else if (str[index] == ' ' && !flag)
			flag = 1;
		else if (ft_isalnum(str[index]))
			return (1);
		index--;
	}
	return (0);
}

void  ft_parce_complition(t_line *line, char **str)
{
	int r = 0;
	if ((*str)[line->c_len] == 0)
	{
		ft_putnbr_fd(820,open("/dev/ttys002",O_RDWR));
	}
	else if (ft_isalnum((*str)[line->c_len]))
	{
		ft_putnbr_fd(ft_tab_checker(line->c_len,*str),open("/dev/ttys002",O_RDWR));
	}
	else if ((*str)[line->c_len] == ' ')
	{
		ft_putnbr_fd(ft_ispace(line->c_len,*str),open("/dev/ttys002",O_RDWR));
	}
	else if (ft_strchr("&|;",(*str)[line->c_len]))
	{
		ft_putnbr_fd(ft_ciparateur(line->c_len,*str),open("/dev/ttys002",O_RDWR));
	}
}

void ft_auto_complition(t_line *line, char **str)
{
	ft_parce_complition(line, str);
}


// if (ft_isalnum())