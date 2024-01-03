NAME	= libft.a
CC		= cc
CCFLAGS	= -Wall -Wextra -Werror
INCLDS	= includes/
SRCS	= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c\
			ft_strlen.c ft_toupper.c ft_tolower.c ft_atoi.c ft_strchr.c ft_strrchr.c\
			ft_strncmp.c ft_strlcpy.c ft_strlcat.c ft_strnstr.c ft_strdup.c\
			ft_memset.c ft_memcpy.c ft_memchr.c ft_memcmp.c ft_memmove.c ft_bzero.c ft_calloc.c\
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c\
			ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c\
			ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_min.c ft_max.c ft_abs.c ft_atol.c ft_strcut.c

GNL_SRCS	= $(addprefix ft_gnl/, get_next_line.c get_next_line_utils.c)
PRT_SRCS	= $(addprefix ft_printf/, ft_count_digits.c ft_printf.c print_base.c print_char.c print_hexa.c print_int.c print_str.c\
			print_unsigned_int.c ft_putnbr_unsigned_fd.c)
OBJS	= $(addprefix obj/, ${SRCS:.c=.o} ${PRT_SRCS:.c=.o} ${GNL_SRCS:.c=.o})

${NAME}: ${OBJS}
	@echo "Creating archive ${NAME}"
	@ar rcs ${NAME} ${OBJS}
	@echo "\n=== ${NAME} created ===\n"

obj/%.o: %.c
	@mkdir -p obj
	@mkdir -p obj/ft_printf
	@mkdir -p obj/ft_gnl
	@echo "- Compiling $<"
	@${CC} ${CCFLAGS} -c $< -o $@ -I $(INCLDS)

all: ${NAME}

clean:
	@echo "* Removing objects files for ${NAME}"
	@rm -f ${OBJS}

fclean: clean
	@echo "* Removing file ${NAME}"
	@rm -f ${NAME}

re: fclean ${NAME}

norm:
	norminette ${SRCS} ${INCLDS}

.PHONY: all clean flcean re bonus norm
