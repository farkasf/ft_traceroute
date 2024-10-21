/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 03:03:34 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 21:49:25 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

static const char *opt_type[] = {
	['q'] = "nqueries",
	['m'] = "max_ttl",
	['p'] = "port",
	['f'] = "first_ttl"
};

static void	set_def_values(t_args *args)
{
	args->n_queries = DEF_PROBES;
	args->max_ttl = DEF_HOPS;
	args->probe_port = DEF_PORT;
	args->first_ttl = 1;
	args->resolve = 1;
}

static unsigned int	set_opt_value(char flag, char *num, t_args *args)
{
	unsigned int	value;

	value = (unsigned int)ft_atoi(num);
	if (flag == 'q' && (!value || value > MAX_PROBES))
		print_args_error("no more than %d probes per hop\n", args, MAX_PROBES);
	else if (flag == 'm' && value > MAX_HOPS)
		print_args_error("max hops cannot be more than %d\n", args, MAX_HOPS);
	else if (flag == 'p' && (!value || value > 65535))
		print_args_error("invalid port number `%s'\n", args, num);
	else if (flag == 'f' && (!value || value > MAX_HOPS))
		print_args_error("first hop out of range\n", args, NULL);
	return (value);
}

static int	set_opt(unsigned int *option_val, char *flag, int i, int ac, char **av, t_args *args)
{
	if (flag[2] != '\0' && ft_isnumber(av[i] + 2))
	{
		*option_val = set_opt_value(flag[1], &flag[2], args);
		return (0);
	}
	else if (i + 1 < ac && ft_isnumber(av[i + 1]))
		*option_val = set_opt_value(flag[1], av[i + 1], args);
	else
	{
		if (ft_strlen(flag) > 2 || av[i + 1])
			print_args_error("Cannot handle `-%c' option with arg `%s' (argc %d)\n", args, flag[1], \
				ft_strlen(flag) > 2 ? flag + 2 : av[i + 1], ft_strlen(flag) > 2 ? i : i + 1);
		else
			print_args_error("Option `-%c' (argc %d) requires an argument: `-%c %s'\n", args, flag[1], \
				i + 1, flag[1], opt_type[(unsigned char)flag[1]]);
	}
	return (1);
}

void	parse_args(t_args *args, int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (ft_strncmp(av[i], "--help", 6) == 0 && ft_strlen(av[i]) == 6)
		{
			if (args->target)
				free(args->target);
			print_usage();
		}
		i++;
	}

	i = 1;
	set_def_values(args);
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strchr("qmpf", av[i][1]))
			{
				switch (av[i][1])
				{
					case 'q':
						i += set_opt(&args->n_queries, av[i], i, ac, av, args);
						break ;
					case 'm':
						i += set_opt(&args->max_ttl, av[i], i, ac, av, args);
						break ;
					case 'p':
						i += set_opt(&args->probe_port, av[i], i, ac, av, args);
						break ;
					case 'f':
						i += set_opt(&args->first_ttl, av[i], i, ac, av, args);
						break ;
				}	
			}
			else if (av[i][1] == 'n' && av[i][2] == '\0')
				args->resolve = false;
			else
				print_args_error("Bad option `%s' (argc %d)\n", args, av[i], ac);
		}
		else
		{
			if (!args->target)
				args->target = ft_strdup(av[i]);
			else
				print_args_error("Extra arg `%s' (argc %d)\n", args, av[i], ac);
		}
		i++;
	}

	if (!args->target || ft_strlen(args->target) < 1)
		print_args_error("Specify \"host\" missing argument.\n", args, NULL);
}
