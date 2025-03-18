NAME			:=	dbltoa.a
# COMPILER		:=	cc
COMPILER		:=	gcc
RM				:=	rm -rf
PRINT_NO_DIR	:=	--no-print-directory

#		Compiler flags
CFLAGS			+=	-MMD -MP
CFLAGS			+=	-Wall -Wextra
# # Werror cannot go together with fsanitize, because fsanitize won't work correctly.
# CFLAGS			+=	-Werror
CFLAGS			+=	-g
# CFLAGS			+=	-fsanitize=address
# CFLAGS			+=	-Wunused -Wuninitialized -Wunreachable-code
# OFLAGS are optimization flags that might have been passed from the parent Makefile.
CFLAGS			+=	$(OFLAGS)

# ENABLE_MALLOC_WRAP := 0
# ifeq ($(MAKECMDGOALS),malloc_wrap)
#   ENABLE_MALLOC_WRAP := 1
# endif

# ifeq ($(MAKECMDGOALS),test)
#   ENABLE_MALLOC_WRAP := 1
# endif

# # Apply the flags if malloc wrapping is enabled
# ifeq ($(ENABLE_MALLOC_WRAP),1)

ifeq ($(MAKECMDGOALS),malloc_wrap)
	CFLAGS	+= -D MALLOC_WRAP=true
	CFLAGS := $(filter-out -Ofast, $(CFLAGS))
	CFLAGS := $(filter-out -O3, $(CFLAGS))
	ifeq ($(shell uname -s),Linux)
		CFLAGS	+= -Wl,--wrap=malloc
	endif
endif

#		Build directory for objects and dependencies
BUILD_DIR		:=	.build/
INC_DIR			:=	include/
TESTER_DIR		:=	tester/

#		Source Directory
SRC_DIR			:=	src/

#		Extern Libraries
EXT_DIR			:=	extern_libaries/libft/
EXT_INC_DIR		:=	$(EXT_DIR)$(INC_DIR)

#		Source files by category
DBTOA			:=	dbltoa.c					fraction_conversion.c			fraction_operations.c	\
					ft_binary_to_decimal.c		scientific_notation.c			double_to_string.c		\
					precision_process.c			precision_set.c					utils_dbl.c				\
					ft_addition.c				ft_subtraction.c				ft_multiply.c			\
					ft_division.c

#	Extra Sources
DBL_SRCS		:=	$(addprefix $(SRC_DIR), $(DBTOA))

#		Generate object file names
DBL_OBJS		:=	$(DBL_SRCS:%.c=$(BUILD_DIR)%.o)

#		Generate Dependency files
DEPS			:=	$(DBL_OBJS:.o=.d)
#		Header files
HEADERS			:=	$(INC_DIR)dbltoa.h $(EXT_INC_DIR)libft.h	
# HEADERS			:=	$(addprefix $(INC_DIR), $(HEADERS_FILES))

#		Remove these created files
DELETE			:=	*.out																				\
					.DS_Store																			\
					*.dSYM/

#		Default target
all: $(NAME)

#		Main target
$(NAME): libft $(DBL_OBJS)
	ar rcs $(NAME) $(DBL_OBJS)
	@printf "$(CREATED)" $@ $(CUR_DIR)

#		Compile .c files to .o files

$(BUILD_DIR)%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -I $(EXT_INC_DIR) -c $< -o $@

libft:
	git clone git@github.com:RJW-db/lib_private.git $(EXT_DIR)
	@$(MAKE) $(PRINT_NO_DIR) -C $(EXT_DIR)

called_by_libft: $(DBL_OBJS)


clean:
	@$(RM) $(BUILD_DIR) $(DELETE)
	@printf "$(REMOVED)" $(BUILD_DIR) $(CUR_DIR)$(BUILD_DIR)

fclean: clean
	@$(RM) $(NAME)
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re: fclean all

print-%:
	$(info $($*))

#		Include dependencies
-include $(DEPS)

.PHONY:	all libft clean fclean re print-%

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