NAME			:=	dbltoa.a

MAKEFLAGS		+=	-j
COMPILER		:=	cc

BASE_FLAGS		:=	-std=c99 -Wall -Wextra -Werror

PEDANTIC		:=	-Wpedantic -pedantic-errors -Wundef -Wstrict-prototypes

WARNINGS		:=	-Wshadow -Wconversion -Wsign-conversion			\
					-Wformat=2 -Wuninitialized -Wunreachable-code

CAST_WARNINGS	:=	-Wbad-function-cast
ifeq ($(shell $(COMPILER) --version | grep -c "gcc"),1)
CAST_WARNINGS	+=	-Wcast-function-type
endif

DEPFLAGS		:=	-MMD -MP

OPTIMIZATION	:=	-O2
SECURITY		:=	-fstack-protector-strong
ifeq ($(shell uname -s),Linux)
SECURITY		+=	-D_FORTIFY_SOURCE=2
FSANITIZE		:=	leak
endif

SANITIZERS		:=	-fsanitize=$(FSANITIZE),address,undefined,null,integer-divide-by-zero,signed-integer-overflow,bounds,alignment
DEBUG_FLAGS		:=	-fno-omit-frame-pointer

CFLAGS			:=	$(BASE_FLAGS) $(PEDANTIC) $(WARNINGS) $(CAST_WARNINGS) \
					$(DEPFLAGS) $(OPTIMIZATION) $(SECURITY)

ifneq ($(filter valgrind,$(MAKECMDGOALS)),)
CFLAGS			+=	-g $(DEBUG_FLAGS)
else ifneq ($(filter debug,$(MAKECMDGOALS)),)
CFLAGS			+=	-g3 $(SANITIZERS) $(DEBUG_FLAGS) -fno-sanitize-recover=all
endif

ifneq ($(filter malloc,$(MAKECMDGOALS)),)
CFLAGS			+=	-D MALLOC_WRAP=true
endif

PRINT_NO_DIR	:=	--no-print-directory
RM				:=	rm -rf

SRC_DIR			:=	src
INC_DIR			:=	include
BUILD_DIR		:=	.build
SUB_EXT_INC		:=	../../include

# Extern Library
EXT_DIR			:=	extern_libary
EXT_LIB			:=	$(EXT_DIR)/libftx
EXT_INC			:=	$(EXT_LIB)/$(INC_DIR)
LIBFTX_OBJ_DIR	:=	$(BUILD_DIR)/libftx
LIB_A			:=	libftx.a

DBTOA			:=	dbltoa.c					fraction_conversion.c			fraction_operations.c	\
					ft_binary_to_decimal.c		scientific_notation.c			double_to_string.c		\
					precision_process.c			precision_set.c					utils_dbl.c				\
					ft_addition.c				ft_subtraction.c				ft_multiply.c			\
					ft_division.c

# Generate source file names
SRC				:=	$(addprefix $(SRC_DIR)/, $(DBTOA))

# Generate object file names
OBJ				:=	$(SRC:%.c=$(BUILD_DIR)/%.o)

# Generate Dependency files
DEPS			:=	$(OBJ:.o=.d)

# Creates libftx library, unless dbltoa is being built as a submodule of libftx
CREATE_LIBFTX	:=	$(MAKE) $(PRINT_NO_DIR) -C $(EXT_LIB) SUBMODULES_CMD= $(LIB_A) $(filter debug,$(MAKECMDGOALS))

# Ensures libftx is cloned if missing, then sets up dbltoa as needed for libftx
CLONE_LIBFTX	:=	\
	@if [ ! -d "$(EXT_LIB)" ]; then \
		git clone git@github.com:RJW-db/lib_private.git $(EXT_LIB); \
	fi; \
	$(CREATE_LIBFTX)

DELETE			:=	*.out			**/*.out		.DS_Store	\
					**/.DS_Store	.dSYM/			**/.dSYM/

all: $(NAME)

$(NAME): libftx $(OBJ)
	mkdir -p $(LIBFTX_OBJ_DIR)
	cd $(LIBFTX_OBJ_DIR) && ar x ../../$(EXT_LIB)/$(LIB_A)
	ar rcs $(NAME) $(OBJ) $(LIBFTX_OBJ_DIR)/*.o
	@printf "$(CREATED)" $@ $(CUR_DIR)

$(BUILD_DIR)/%.o: %.c | libftx
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -I $(EXT_INC) -c $< -o $@

# $(CLONE_LIBFTX) is set to nothing when dbltoa is used as a submodule in libftx,
# preventing nested cloning of libftx inside dbltoa.
libftx:
	$(CLONE_LIBFTX)

# It allows passing the correct include path for the external library if used as a submodule
submodule_build: $(OBJ)

submodule:
	@$(MAKE) EXT_INC="$(SUB_EXT_INC)" submodule_build $(filter debug,$(MAKECMDGOALS))

clean:
	@$(RM) $(BUILD_DIR) $(DELETE)
	@printf "$(REMOVED)" $(BUILD_DIR) $(CUR_DIR)$(BUILD_DIR)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(EXT_DIR)
	@rm -f $(INC_DIR)/libftx.h
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:
	$(MAKE) $(PRINT_NO_DIR) fclean
	$(MAKE) $(PRINT_NO_DIR) all

# Submodule: skip `all` to avoid parallel conflicts. Standalone: triggers `all`.
valgrind: $(if $(and $(CLONE_LIBFTX),$(filter valgrind,$(MAKECMDGOALS))),all)
debug: $(if $(and $(CLONE_LIBFTX),$(filter debug,$(MAKECMDGOALS))),all)

print-%:
	$(info $($*))

-include $(DEPS)

.PHONY: all libftx submodule_build submodule	\
		clean fclean re							\
		valgrind debug print-%

# Terminal markup
BOLD			:=	\033[1m
GREEN			:=	\033[32m
MAGENTA			:=	\033[35m
CYAN			:=	\033[36m
RESET			:=	\033[0m

R_MARK_UP		:=	$(MAGENTA)$(BOLD)
CA_MARK_UP		:=	$(GREEN)$(BOLD)

# Current directory and formatted status messages
CUR_DIR			:=	$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED			:=	$(R_MARK_UP)REMOVED $(CYAN)%s$(MAGENTA) (%s) $(RESET)\n
CREATED			:=	$(CA_MARK_UP)CREATED $(CYAN)%s$(GREEN) (%s) $(RESET)\n
