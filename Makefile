NAME		:= pipex
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g3

SRC_DIR		:= src
INCLD_DIR	:= includes
LIBFT_DIR	:= libft
OBJS_DIR	:= objs

LIBFT		:= $(LIBFT_DIR)/libft.a
LIB			:= $(LIBFT)

define INCLUDES :=
	$(INCLD_DIR)/
	$(LIBFT_DIR)/$(INCLD_DIR)/
endef
INCLUDES := $(strip $(INCLUDES))

define SRC :=
	main.c
endef
SRC := $(strip $(SRC))

OBJS		:= $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRC))
INCLD_FLAG	:= $(addprefix -I,$(INCLUDES))
MAKE_FLAG	:= --no-print-directory --silent

### PROJECT ###

$(NAME): $(LIBFT) $(OBJS)
	@echo '* Assembling $(NAME)'
	@$(CC) $(CFLAGS) $(OBJS) $(INCLD_FLAG) $(LIB) -o $@

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@echo '- Compiling $<'
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLD_FLAG) -c $< -o $@

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	@echo '! Removing objs'
	@rm -rf $(OBJS)

.PHONY: fclean
fclean: clean
	@echo '! Removing $(NAME)'
	@rm -rf $(NAME)

.PHONY: re
re: fclean $(NAME)

### LIBFT ###

$(LIBFT):
	@make -C $(LIBFT_DIR) $(MAKE_FLAG)

.PHONY: cleanlib
cleanlib:
	@make -C $(LIBFT_DIR) clean $(MAKE_FLAG)

.PHONY: fcleanlib
fcleanlib:
	@make -C $(LIBFT_DIR) fclean $(MAKE_FLAG)

.PHONY: relib
relib:
	@make -C $(LIBFT_DIR) re $(MAKE_FLAG)

.PHONY: norm
norm:
	@make -C $(LIBFT_DIR) norm $(MAKE_FLAG)
	@norminette $(SRC_DIR) $(INCLD_DIR)
