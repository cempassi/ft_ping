/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmerge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 03:12:23 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/03 02:28:33 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmerge(t_list **alst, t_list *blst)
{
	if (*alst == NULL)
	{
		*alst = blst;
		return (*alst);
	}
	ft_lstaddback(alst, blst);
	return (*alst);
}
