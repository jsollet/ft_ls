/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsollett <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 15:51:53 by jsollett          #+#    #+#             */
/*   Updated: 2021/12/08 13:06:40 by jsollett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_strlen_c(const char *str, char del)
{
	size_t	len;

	len = 0;
	while (*str != '\0' && *str != del)
	{
		str++;
		len++;
	}
	return (len);
}

static	size_t	ft_count(const char *ss, char del, size_t count, size_t i)
{
	size_t	len;

	len = ft_strlen_c(ss, '\0');
	while (*(ss + i) != '\0')
	{
		while (*(ss + i) == del && i < len)
		{
			i++;
			if (i == len)
				return (count++);
		}
		count++;
		while (*(ss + i) != del && i < len)
		{
			i++;
			if (i == len)
				return (count++);
		}
	}
	return (count);
}

static	char	*ft_word(char *word, const char *src, size_t i, size_t offset)
{
	size_t	j;

	j = 0;
	while (j < offset)
	{	
		*(word + j) = *(src + i + j);
		j++;
	}
	*(word + offset) = '\0';
	return (word);
}

static	char	**ft_word1(char **array, const char *src, char c, size_t nw)
{
	char	*word;
	size_t	offset;
	size_t	index;

	index = 0;
	while (*(src + index) != '\0')
	{
		if (*(src + index) == c)
			index++;
		offset = ft_strlen_c(src + index, c);
		if (offset != 0)
		{
			word = malloc((offset + 1) * sizeof(char));
			if (word == NULL)
				return (NULL);
			array[nw] = ft_word(word, src, index, offset);
			index = index + offset;
			nw++;
		}
	}
	return (array);
}

char	**ft_split(char const *src, char c)
{
	char	**array;

	if (src == NULL)
		return (NULL);
	array = malloc((ft_count(src, c, 0, 0) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	if (ft_word1(array, src, c, 0) == NULL)
		return (NULL);
	array[ft_count(src, c, 0, 0)] = NULL;
	return (array);
}
