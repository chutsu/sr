include ../config.mk

SRCS = $(wildcard *.c)
OBJS = $(addprefix $(BLD_DIR)/, $(SRCS:.c=.o))

.PHONY: default
default: $(BLD_DIR)/libsr.a

$(BLD_DIR)/%.o : %.c %.h
	$(COMPILE_OBJ)

$(BLD_DIR)/libsr.a: $(OBJS)
	$(MAKE_STATIC_LIB)
