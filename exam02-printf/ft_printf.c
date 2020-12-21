/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jolim <ezemango@gmail.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 16:11:34 by jolim             #+#    #+#             */
/*   Updated: 2020/12/19 16:11:34 by jolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
specifier s의 경우 width는 string을 자르지 않고 가능한 경우 왼쪽에 공백을 넣는다. precision은 string의 왼쪽부터 시작해서 자른다. </br>
specifier d의 경우 숫자가 잘리는 경우는 없다. width는 왼쪽에 공백을 넣는다. precision이 width값보다 큰 경우 width는 무시되며 왼쪽에 0을 넣는다. </br>
specifier x의 경우 음수를 제외하고 d와 비슷함. </br>
ft_printf 과제에 있는 utils: strlen, strdup, memset, strjoin.
*/

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

int		ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len++])
		;
	return (len - 1);
}

char	*ft_strdup(char *str)
{
	char	*new;
	int		len;
	int		i;

	len = ft_strlen(str);
	if (!(new = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (i <= len)
	{
		new[i] = str[i];
		i++;
	}
	return (new);
}

char	*ft_strset(char c, int len)
{
	char	*str;
	int		i;

	if (!(str = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*new;
	int		i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (!(new = (char *)malloc(sizeof(len1 + len2 + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (i < len1)
	{
		new[i] = s1[i];
		i++;
	}
	while (i <= len1 + len2)
	{
		new[i] = s2[i - len1];
		i++;
	}
	return (new);
}

int		ft_atoi_pp(char **fmt)
{
	int	ret;

	ret = 0;
	while ('0' <= **fmt && **fmt <= '9')
	{
		ret *= 10;
		ret += **fmt - '0';
		(*fmt)++;
	}
	return (ret);
}

char	*ft_utoa_base(long int number, int base)
{
	long int	n;
	int			scale;
	char		*digit;
	char		*ret;

	if (number == 0)
		return (ft_strdup("0"));
	digit = "0123456789abcdef";
	n = number;
	scale = 0;
	while (n != 0)
	{
		scale++;
		n /= base;
	}
	if (!(ret = (char *)malloc((scale + 1) * sizeof(char))))
		return (NULL);
	ret[scale] = '\0';
	n = number;
	while (scale-- >= 0)
	{
		ret[scale] = digit[n % base];
		n /= base;
	}
	return (ret);
}

char	*ft_dtoa(int number, int base, int precision)
{
	signed long int	n;
	int				sign;
	int				len;
	char			*str;
	char			*temp;

	sign = 0;
	n = (signed long int)number;
	if (n < 0)
	{
		sign = 1;
		n *= -1;
	}
	str = ft_utoa_base(n, base);
	len = ft_strlen(str);
	if (precision > len)
	{
		temp = ft_strjoin(ft_strset('0', precision - len), str);
		free(str);
		str = temp;
	}
	if (sign == 1)
	{
		temp = ft_strjoin("-", str);
		free(str);
		str = temp;
	}
	return (str);
}

int		ft_print_s(char **fmt, va_list *ap, int width, int precision)
{
	char	*str;
	char	*temp;
	int		len;
	int		ret;

	str = ft_strdup(va_arg(*ap, char *));
	len = ft_strlen(str);
	if (precision != 0 && precision < len)
		str[precision] = '\0';
	len = ft_strlen(str);
	if (width > len)
	{
		temp = ft_strjoin(ft_strset(' ', width - len), str);
		free(str);
		str = temp;
	}
	ret = write(1, str, ft_strlen(str));
	free(str);
	return (ret);
}

int		ft_print_dx(char **fmt, va_list *ap, int width, int precision)
{
	int		value;
	char	*str;
	char	*temp;
	int		len;
	int		ret;

	value = va_arg(*ap, int);
	if (**fmt == 'd')
		str = ft_dtoa(value, 10, precision);
	if (**fmt == 'x')
		str = ft_dtoa(value, 16, precision);
	len = ft_strlen(str);
	if (width > len)
	{
		temp = ft_strjoin(ft_strset(' ', width - len), str);
		free(str);
		str = temp;
	}
	ret = write(1, str, ft_strlen(str));
	free(str);
	return (ret);
}

int		ft_read_print_spec(char **fmt, va_list *ap)
{
	int	width;
	int precision;
	int	ret;

	width = 0;
	precision = 0;
	width = ft_atoi_pp(fmt);

	if (**fmt == '.')
	{
		(*fmt)++;
		precision = ft_atoi_pp(fmt);
		(*fmt)++;
	}
	if (**fmt == 's')
		return(ft_print_s(fmt, ap, width, precision));
	if (**fmt == 'd' || **fmt == 'x')
		return(ft_print_dx(fmt, ap, width, precision));
	return (0);
}

int		ft_printf(const char *format,...)
{
	va_list	ap;
	char	*fmt;
	int		count;
	int		p;

	va_start(ap, format);
	fmt = (char *)format;
	count = 0;
	p = 0;
	while (*(fmt + p))
	{
		if (*(fmt + p) == '%')
		{
			count += write(1, fmt, p);
			fmt += p + 1;
			count += ft_read_print_spec(&fmt, &ap);
			fmt++;
			p = -1;
		}
		p++;
	}
	count += write(1, fmt, p);
	return (count);
}

int		main()
{
	ft_printf("Hexadecimal for %d is %x\n", 42, 42);
}
