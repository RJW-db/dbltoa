NAME			:=	dbltoa.a

#	Compiler and Flags
COMPILER		:=	gcc
CFLAGS			+=	-Wall -Wextra
CFLAGS			+=	-Werror
CFLAGS			+=	-Wunreachable-code -Wpedantic -Wconversion -Wshadow
CFLAGS			+=	-MMD -MP
# CFLAGS			+=	-g
#	Werror cannot go together with fsanitize, because fsanitize won't work correctly.
# CFLAGS			+=	-fsanitize=address

#	Utilities
PRINT_NO_DIR	:=	--no-print-directory
RM				:=	rm -rf

#		Base Directories
SRC_DIR			:=	src/
INC_DIR			:=	include/
BUILD_DIR		:=	.build/

#		Extern Libraries
EXT_DIR			:=	extern_libaries/
EXT_LIB_DIR		:=	$(EXT_DIR)libft/
EXT_INC_DIR		:=	$(EXT_LIB_DIR)$(INC_DIR)

#		Source files by category
DBTOA			:=	dbltoa.c					fraction_conversion.c			fraction_operations.c	\
					ft_binary_to_decimal.c		scientific_notation.c			double_to_string.c		\
					precision_process.c			precision_set.c					utils_dbl.c				\
					ft_addition.c				ft_subtraction.c				ft_multiply.c			\
					ft_division.c

#		Extra Sources
DBL_SRCS		:=	$(addprefix $(SRC_DIR), $(DBTOA))

#		Generate object file names
DBL_OBJS		:=	$(DBL_SRCS:%.c=$(BUILD_DIR)%.o)

#		Generate Dependency files
DEPS			:=	$(DBL_OBJS:.o=.d)

#		Header files
HEADERS			:=	$(INC_DIR)dbltoa.h $(EXT_INC_DIR)libft.h
# HEADERS			:=	$(INC_DIR)dbltoa.h $(EXT_INC_DIR)libft.h ../../include/libft.h

#		Remove these created files
DELETE			:=	*.out			**/*.out			.DS_Store										\
					**/.DS_Store	.dSYM/				**/.dSYM/

#		Default target
all: $(NAME)

#		Main target
$(NAME): libft $(DBL_OBJS)
	ar rcs $(NAME) $(DBL_OBJS)
	@printf "$(CREATED)" $@ $(CUR_DIR)

#		Compile .c files to .o files
$(BUILD_DIR)%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -I $(EXT_INC_DIR) -I ../../include/ -c $< -o $@

libft:
	@if [ ! -d "$(EXT_LIB_DIR)" ]; then \
		git clone git@github.com:RJW-db/lib_private.git $(EXT_LIB_DIR); \
	fi
	@$(MAKE) $(PRINT_NO_DIR) -C $(EXT_LIB_DIR) base

#		standalone is when this is a submodule of libft and need different header locations
standalone_build: $(DBL_OBJS)

standalone:
	@$(MAKE) EXT_INC_DIR="../../include/" standalone_build

clean:
	@$(RM) $(BUILD_DIR) $(DELETE)
	@printf "$(REMOVED)" $(BUILD_DIR) $(CUR_DIR)$(BUILD_DIR)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(EXT_DIR)
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re: fclean all

print-%:
	$(info $($*))

#		Include dependencies
-include $(DEPS)

.PHONY:	all libft standalone_build standalone clean fclean re print-%

# ----------------------------------- colors --------------------------------- #
BOLD			=	\033[1m
GREEN			=	\033[32m
MAGENTA			=	\033[35m
CYAN			=	\033[36m
RESET			=	\033[0m

R_MARK_UP		=	$(MAGENTA)$(BOLD)
CA_MARK_UP		=	$(GREEN)$(BOLD)

# ----------------------------------- messages ------------------------------- #
CUR_DIR			:=	$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED			:=	$(R_MARK_UP)REMOVED $(CYAN)%s$(MAGENTA) (%s) $(RESET)\n
CREATED			:=	$(CA_MARK_UP)CREATED $(CYAN)%s$(GREEN) (%s) $(RESET)\n
