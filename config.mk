# SETTINGS
BLD_DIR=$(PWD)/build
TESTS_DIR=$(PWD)/build/tests
INC_DIR=$(PWD)

# COMPILER SETTINGS
CC=tcc -Wall -O3 -g -std=c11
# CC=g++ -Wall -O3 -g -std=c++11
CFLAGS=-I$(INC_DIR)
LIBS=-L$(BLD_DIR) -lm

# COMPILE AND LINKER ALIASES
COMPILE_OBJ = \
	@echo "CC [$<]"; \
	$(CC) $(CFLAGS) -c $< -o $@

MAKE_STATIC_LIB = \
	@echo "AR [libzero.a]"; \
	$(AR) $(ARFLAGS) $@ $^

COMPILE_TEST_OBJ = \
	$(CC) $(CFLAGS) -c $< -o $@

MAKE_TEST = \
	echo "TEST [$(shell basename $@)]"; \
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
