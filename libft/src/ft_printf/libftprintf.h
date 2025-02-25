/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:27:12 by jsollett          #+#    #+#             */
/*   Updated: 2022/03/31 12:26:08 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>


typedef int(*t_funcptr)(int);

typedef struct s_last
{
	char	conv;
	size_t	offset_s;
	size_t	offset_c;
	size_t	offset_p;
	size_t	offset_w;
	int		width;
	int		prec;
}	t_last;

typedef struct s_flag
{
	int	plus;
	int	minus;
	int	zero;
	int	sharp;
	int	space;
}	t_flag;

typedef struct s_n
{
	int		count;
	int		iter;
	int		signe;
	char	conversion[17];
	int		nspace;
}	t_n;

typedef struct s_pgrm
{
	int		fd; // ajout fd
	va_list	args;
	t_last	last;
	t_flag	flag;
	t_n		n;
	size_t	sign;
	size_t	count;
	int		done;
	int		delta;
}	t_pgrm;
// serie libft modifie
size_t	ft_strlen_c1(const char *str, char del);
size_t	ft_intlen(long long int n);
void	ft_putchar_i(char c, t_pgrm *result);
void	ft_putstr_i(char *str, t_pgrm *result);
void	ft_nputchar_i(int n, char c, t_pgrm *result);
int		ft_issp(int c);
int		ft_issw(int c);
size_t	ft_atoi1(t_funcptr before, const char *str);
// serie conversion numerique
int		power_10(int expo);
int		cal(long long int i);
int		calu(long long int i);
void	ft_putnbr_1(int nb, t_n *a);
void	ft_putnbr_u1(unsigned int nb, t_n *a);
void	cb_bx(unsigned long long int nb, int base, int depth, t_n *a);
void	cb_lx(unsigned long long int nb, int base, int depth, t_n *a);
// serie parsing
void	ft_sign(t_pgrm *result);
void	ft_printsign(t_pgrm *result);
void	ft_lastdelim(const char *inst, const char *code, t_pgrm *result);
void	ft_ptdelim(const char *inst, size_t offset_c, t_pgrm *result);
void	initstruct(size_t len, t_pgrm *pgrm);
void	ft_countflag(const char *inst, t_pgrm *fs);
void	ft_getwidth(const char *inst, t_pgrm *result);
void	ft_clean(t_pgrm *result);
void	select_arg(t_pgrm *result);
// serie print helper
void	print_s0(t_pgrm *result);
void	space_u(t_pgrm *result);
void	space_s(t_pgrm *result);
void	zero(t_pgrm *result);
void	left_u(t_pgrm *result);
void	right_ui(t_pgrm *result);
void	left_i(t_pgrm *result);
//void	right_i(t_pgrm *result);
void	left_px(t_pgrm *result);
void	right_px(t_pgrm *result);
void	print_nf(char *c, t_pgrm *conversion);
int		ft_printf(const char *inst, ...);
// serie print
void	print_s(t_pgrm *result);
void	print_c(t_pgrm *result);
void	print_pp(t_pgrm *result);
void	print_u(t_pgrm *result);
void	print_id(t_pgrm *result);
void	print_p(t_pgrm *result);
void	print_x(t_pgrm *result);
void	print_ux(t_pgrm *result);
#endif
