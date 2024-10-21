/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 02:16:43 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 02:17:02 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static const char *opt_type[] = {
	['q'] = "nqueries",
	['m'] = "max_ttl",
	['z'] = "sendwait",
	['f'] = "first_ttl"
};

void	print_usage(void)
{
	dprintf(STDOUT_FILENO, "usage\n");
	exit(0);
}

void	set_def_values(t_args *args)
{
	args->n_queries = DEF_PROBES;
	args->max_ttl = DEF_HOPS;
	args->send_wait = 0;
	args->first_ttl = 1;
	args->resolve = 1;
}

unsigned int	set_opt_value(char flag, char *num)
{
	unsigned int	value;

	value = (unsigned int)ft_atoi(num);
	if (flag == 'q' && (!value || value > MAX_PROBES))
		dprintf(STDERR_FILENO, "no more than % probes per hop\n", MAX_PROBES); //+EXIT
	else if (flag == 'm' && value > MAX_HOPS)
		dprintf(STDERR_FILENO, "max hops cannot be more than %d\n", MAX_HOPS); //+EXIT
	else if (flag == 'z' && (!value || ft_atoi(num) < 0))
		dprintf(STDERR_FILENO, "bad sendtime `%s' specified\n", num); //+EXIT
	else if (flag == 'f' && (!value || value > MAX_HOPS))
		dprintf(STDERR_FILENO, "first hop out of range\n"); //+EXIT
	return (value);
}

int	set_opt(unsigned int *option_val, char *flag, int i, int ac, char **av)
{
	if (flag[2] != '\0' && ft_isnumber(av[i] + 2))
	{
		*option_val = set_opt_value(flag[1], &flag[2]);
		return (0);
	}
	else if (i + 1 < ac && ft_isnumber(av[i + 1]))
	{
		*option_val = set_opt_value(flag[1], av[i + 1]);
		return (1);
	}
	else
	{
		if (ft_strlen(flag) > 2 || av[i + 1])
			dprintf(STDERR_FILENO, "Cannot handle `-%c' option with arg `%s' (argc %d)\n", flag[1], ft_strlen(flag) > 2 ? flag + 2 : av[i + 1], ft_strlen(flag) > 2 ? i : i + 1);
		else
			dprintf(STDERR_FILENO, "Option `-%c' (argc %d) requires an argument: `-%c %s'\n", flag[1], i, flag[1], opt_type[(unsigned char)flag[1]]);
		exit(EXIT_FAILURE);
	}
}

void	parse_args(t_args *args, int ac, char **av)
{
	int	i;

	i = 1;
	set_def_values(args);
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strchr("qmzf", av[i][1]))
			{
				switch (av[i][1])
				{
					case 'q':
						i += set_opt(&args->n_queries, av[i], i, ac, av);
						break ;
					case 'm':
						i += set_opt(&args->max_ttl, av[i], i, ac, av);
						break ;
					case 'z':
						i += set_opt(&args->send_wait, av[i], i, ac, av);
						break ;
					case 'f':
						i += set_opt(&args->first_ttl, av[i], i, ac, av);
						break ;
				}	
			}
			else if (av[i][1] == 'n' && av[i][2] == '\0')
				args->resolve = false;
			else
			{
				dprintf(STDERR_FILENO, "Bad option `%s' (argc %d)\n", av[i], ac);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (!args->target)
				args->target = ft_strdup(av[i]);
			else
			{
				dprintf(STDERR_FILENO, "Extra arg `%s' (argc %d)\n", av[i], ac);
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}

	if (!args->target || ft_strlen(args->target) < 1)
	{
		dprintf(STDERR_FILENO, "Specify \"host\" missing argument.\n");
		exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av)
{
	t_troute	troute;

	if (ac < 2)
		print_usage();

	memset(&troute, 0, sizeof(troute));
	parse_args(&troute.args, ac, av);

	printf("Target: %s\n", troute.args.target);
	printf("Number of queries (-q): %u\n", troute.args.n_queries);
	printf("Max TTL (-m): %u\n", troute.args.max_ttl);
	printf("Send wait (-z): %u\n", troute.args.send_wait);
	printf("First TTL (-f): %u\n", troute.args.first_ttl);
	printf("Resolve DNS (-n): %s\n", troute.args.resolve ? "enabled" : "disabled");

	return (0);
}

/*
Specify "host" missing argument.

traceroute fjklsajd
fjklsajd: Name or service not known
Cannot handle "host" cmdline arg `fjklsajd' on position 1 (argc 1)

-q (default 10)
if (!probes_per_hop || probes_per_hop > MAX_PROBES)
	ex_error ("no more than " _TEXT(MAX_PROBES) " probes per hop");

traceroute google.com -q
Option `-q' (argc 2) requires an argument: `-q nqueries'

-m (default 255)
if (max_hops > MAX_HOPS)
		ex_error ("max hops cannot be more than " _TEXT(MAX_HOPS));

traceroute google.com -m
Option `-m' (argc 2) requires an argument: `-m max_ttl'

-n (resolve)

-z
	if (send_secs < 0)
		ex_error ("bad sendtime `%g' specified", send_secs);
	if (send_secs >= 10)	//it is milliseconds
		send_secs /= 1000;

traceroute google.com -z
Option `-z' (argc 2) requires an argument: `-z sendwait'

-f
if (!first_hop || first_hop > max_hops)
		ex_error ("first hop out of range");

traceroute google.com -f
Option `-f' (argc 2) requires an argument: `-f first_ttl'
*/
