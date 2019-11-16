include ../../config.mk

SRCS = $(wildcard *.c)
TESTS = $(addprefix $(TESTS_DIR)/, $(SRCS:.c=))

.PHONY: default
default: $(TESTS)

$(BLD_DIR)/%.o: %.c $(BLD_DIR)/libsr.a
	$(COMPILE_TEST_OBJ)

$(TESTS_DIR)/test_%: $(BLD_DIR)/test_%.o
	$(MAKE_TEST)
