/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleon-la <jleon-la@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:38:04 by nkrasimi          #+#    #+#             */
/*   Updated: 2024/11/07 18:55:30 by jleon-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define BUFFER_SIZE 1

typedef struct	s_map
{
	char	directions[4][200];
	int	ndirections;
	char	**colors;
	int	ncolors;
	char	**map;
	int	nmap;
	char	*path;
	char	**spaceless_map;

	void	*mlx;
	void	*mlx_win;
}		t_map;

void	mess(int fd, char *str)
{
	int i;
	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

size_t	ft_strlen(const char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*memory;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((!s1 || s1 == NULL) || (!s2 || s2 == NULL))
		return (NULL);
	memory = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!memory)
		return (NULL);
	while (s1[i] && s1[i] != '\0')
	{
		memory[i] = s1[i];
		i++;
	}
	while (s2[j] && s2[j] != '\0')
	{
		memory[i] = s2[j];
		i++;
		j++;
	}
	memory[i] = '\0';
	return (memory);
}

/* Estas 3 funciones son para parsear el map */
int	is_valid_line(char *map_line)
{
	int	i;

	i = 0;
	if (map_line[i] != '1')
		return (0);
	while (map_line[i])
	{
		if (map_line[i] != '1' && map_line[i] != '0' && map_line[i] != 'N')
			return (0);
		i++;
	}
	i -= 1;
	if (map_line[i] != '1')
		return (0);
	return (1);
}

void	remove_spaces(char *str)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	if (len == 0)
		mess(2, "Error. Empty line\n");
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
}

/* static int        get_next_line(char **line) */
/* { */
/*         char		*buf; */
/*         int		i; */
/*         int		r; */
/*         int		c; */
/*         i = 0; */
/*         r = 0; */
/*         buf = (char *)malloc(50000); */
/*         if (!buf) */
/*                 return (-1); */
/*         r = read(0, &c, 1); */
/*         while (r && c != '\n' && c != '\0') */
/*         { */
/*                 if (c != '\n' && c != '\0') */
/*                         buf[i] = c; */
/*                 i++; */
/*                 r = read(0, &c, 1); */
/*         } */
/*         buf[i] = '\0'; */
/*         *line = buf; */
/*         return (r); */
/* } */

// Con esta funcion sabremos si realmente se esta leyendo y guardando bien el mapa
void	print_matrix(char **matrix)
{
	int i;
	int ii; 

	i = 0;
	while (matrix[i])
	{   
		ii = 0;
		while (matrix[i][ii])
		{
			 write(1, &matrix[i][ii], 1);
			ii++;
		}
		i++;
	}
}   
// Empiezo el split
int	countstr(char *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char	*chargearrs(char *s, char c, size_t *i)
{
	size_t	count;
	char	*string;

	count = 0;
	while (s[*i + count] != c && s[*i + count] != '\0')
		count++;
	string = malloc((count + 1) * sizeof(char));
	if (!string)
		return (NULL);
	count = 0;
	while (s[*i] != c && s[*i] != '\0')
	{
		string[count] = s[*i];
		count++;
		(*i)++;
	}
	while (s[*i] == c && s[*i] != '\0')
		(*i)++;
	string[count] = '\0';
	return (string);
}

void	getfrees(char **memory)
{
	int	i;

	i = 0;
	while (memory[i])
	{
		free(memory[i]);
		i++;
	}
	free(memory);
}

char	**ft_split(char const *s, char c)
{
	size_t	str_count;
	size_t	lap_c;
	size_t	i;
	char	**memory;

	lap_c = 0;
	str_count = countstr((char *)s, c);
	memory = (char **)malloc(sizeof(char *) * (str_count + 1));
	if (!memory || !s)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] == c)
	{
		i++;
	}
	while (str_count > lap_c)
	{
		memory[lap_c] = chargearrs((char *)s, c, &i);
		lap_c++;
	}
	memory[lap_c] = NULL;
	if (!memory)
		getfrees(memory);
	return (memory);
}
//Fin del split
void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = 0;
		i++;
	}
}

char	*ft_strchr(const char *s, int c)
{
	int	i;
	int	l;

	i = 0;
	l = 0;
	while (s[l])
	{
		l++;
	}
	while (i <= l)
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	return (NULL);
}

size_t	ft_strlen2(const char *length)
{
	int	i;

	i = 0;
	while (length[i] && length[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin2(char *s1, char *s2)
{
	char	*memory;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((!s1 || s1 == NULL) || (!s2 || s2 == NULL))
		return (NULL);
	memory = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!memory)
		return (NULL);
	while (s1[i] && s1[i] != '\0')
	{
		memory[i] = s1[i];
		i++;
	}
	while (s2[j] && s2[j] != '\0' && s2[j] != '\n')
		memory[i++] = s2[j++];
	if (s2[j] == '\n')
		memory[i++] = s2[j++];
	memory[i] = '\0';
	free(s1);
	return (memory);
}

/* void	*ft_memcpy(void *dst, const void *src, size_t n) */
/* { */
/* 	size_t	i; */

/* 	i = 0; */
/* 	if (dst == 0 && src == 0) */
/* 	{ */
/* 		return (dst); */
/* 	} */
/* 	while (i < n) */
/* 	{ */
/* 		((unsigned char *)dst)[i] = ((unsigned char *)src)[i]; */
/* 		i++; */
/* 	} */
/* 	return (dst); */
/* } */

void	get_leftover(char *buffer)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	j = 0;
	if (!buffer[i])
	{
		ft_bzero(&buffer[j], BUFFER_SIZE - j);
		return ;
	}
	while (buffer[i + j])
	{
		buffer[j] = buffer[i + j];
		j++;
	}
	ft_bzero(&buffer[j], BUFFER_SIZE - j);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = malloc(sizeof(char) * 1);
	if (!line)
		return (free(line), NULL);
	line[0] = '\0';
	if (ft_strlen2(buffer) >= 1)
		line = ft_strjoin2(line, buffer);
	while (!ft_strchr(buffer, '\n'))
	{
		ft_bzero(&buffer, BUFFER_SIZE);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(line), NULL);
		if (bytes_read == 0)
			break ;
		line = ft_strjoin2(line, buffer);
	}
	if (line[0] == '\0')
		return (free(line), NULL);
	return (get_leftover(buffer), line);
}

char	*noendl_dup(const char *s1)
{
	size_t		i;
	char		*memory;

	memory = malloc((ft_strlen(s1)) * sizeof(char));
	i = 0;
	if (!memory)
		return (0);
	while (i < (ft_strlen(s1) - 1))
	{
		memory[i] = s1[i];
		i++;
	}
	memory[i] = '\0';
	return (memory);
}

void checkdirections(t_map *map, int i)
{
	/* Aqui voy a meter un split para cada una de las partes y negare un strcmp para ver si es el str que deberia en la primera parte y en la segunda ver si la ruta existe*/
	char	**arr;
	char	*noendline;
	int	fd;
       
	arr = ft_split(map->directions[i], ' ');
	if (i == 0 && strcmp(arr[0], "NO"))
		return (mess(2, "North word is wrong\n"), exit(1));
	if (i == 1 && strcmp(arr[0], "SO"))
		return (mess(2, "South word is wrong\n"), exit(1));
	if (i == 2 && strcmp(arr[0], "WE"))
		return (mess(2, "West word is wrong\n"), exit(1));
	if (i == 3 && strcmp(arr[0], "EA"))
		return (mess(2, "East word is wrong\n"), exit(1));
	/* Me falta ver si la ruta existe  */
	noendline = noendl_dup(arr[1]);
	/* mess(1, noendline); */
	fd = open(noendline, O_RDONLY);
	if (fd > 0)
		close(fd);
	else
		return (mess(2, "The image of one of the directions was not found in the system\n"), exit(1));
}

int check3atributtes(t_map *map)
{
	int	fd;
	int	i;
	int	ii;
	char	*line;

	fd = open(ft_strjoin("./maps/", map->path), O_RDONLY);
	// Tenemos que importar el GNL para poder parsear esto correctamente bueno y la libft tambien
	if (fd > 0)
	{
		mess(1, "Todo el parseo fuciona correctamente\n");
		/* Primero checkeo las 4 direcciones y las meto en un doble punter de la estructura */
		i = 0;
		while (i < map->ndirections)
		{
			line = get_next_line(fd);
			if (line && strcmp(line, "\n"))
			{
				/* Aqui quiero incluir la linea valida dentro del array de strings de la estructura */
				ii = 0;
				while (line[ii])
				{
					map->directions[i][ii] = line[ii];
					ii++;
				}
				map->directions[i][ii] = '\0';
				mess(1, map->directions[i]);
				/* mess(1, "\n"); */
				checkdirections(map, i);
				i++;
			}
		}
		/* Despues lo mismo con los colores del suelo y del cielo */
		
		/* Para terminar meto las lineas de mapa y me aseguro de quitar espacios y ver que los caracteres son correctos REMINDER:El mapa que le tengo que pasar a nico es la version con espacios*/

		/* Floodfill para asegurarme que el mapa esta correctamente cerrado */
	}
	else
		return (mess(2, "El mapa no se ha podido abrir\n"), 0);
	return (close(fd), 1);
}

int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}

/* int ismap(char *str) */
/* { */
/* 	int	i; */
/* 	char	*check; */

/* 	i = 0; */
/* 	while (str[i]) */
/* 		i++; */
/* 	if (i < 5) */
/* 		mess(2, "Tiene que tener algo delante de la extension .cub\n"), exit(1); */
/* 	i -= 5; */
/* 	check = &str[i]; */
/* 	if (!ft_isprint(check[0])) */
/* 		mess(2, "El caracter que va delante de la extension .cub tiene que ser imprimible\n"), exit(1); */
/* 	if (check[1] == '.' && check[2] == 'c' && check[3] == 'u' && */
/* 	check[4] == 'b' && check[5] == '\0') */
/* 		return (1); */
/* 	else */
/* 		return (0); */
/* } */
int ismap(char *str)
{
        int     i;  
        int     ii;
        char    *check;

        i = 0;
        while (str[i])
                i++;
        ii = i;
        if (i < 5)
                mess(2, "Tiene que tener algo delante de la extension .cub\n"), exit(1);
        i -= 4;
        check = &str[i];
	i = 0;
        while (i <= ii - 4)
        {   
                if (!ft_isprint(str[i]))
                        return (mess(2, "Todos los caracteres que van delante de la extension .cub tienen que ser imprimibles\n"), exit(1), 0);
                i++;
        }
        if (check[0] == '.' && check[1] == 'c' && check[2] == 'u' &&
        check[3] == 'b' && check[4] == '\0')
                return (1);
        else
                return (0);
}

void	init_values(t_map *mapa)
{
	mapa->ndirections = 4;
	mapa->ncolors = 2;
}

int main (int ac, char **av)
{
    /* size_t	i; */
	t_map	mapa;
	init_values(&mapa);	
	
    /* i = 0; */
    /* Vamos a ver nico aqui tenemos que hacer el parseo de los argumentos por terminal */
	if (ac != 2)
		return (mess(1, "Debes introducir dos argumentos por terminal\n"), 1);
	if (av[1][0] == '\0')
		return (mess(1, "Argumento vacio, especifica un mapa\n"), 1);
    /* Leer el mapa y comprobar que todo esta bien con el floodfill */
	if (ismap(av[1]) == 1)
    	{
	        mapa.path = av[1];
		if (check3atributtes(&mapa) == 0)
			return (mess(2, "Saliendo...\n"), 1);
    	}
	else
		return (mess(1, "El mapa tiene que acabar en .cub\n"), 1);
    /* Raycasting */

    /* Por ahora vamos a dejar el resto para luego */

    return (0);
}

