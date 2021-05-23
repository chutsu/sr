include ../config.mk

.PHONY: default
default: $(BLD_DIR)/libsr.a $(BLD_DIR)/test_sr

$(BLD_DIR)/%.o : %.c %.h
	$(COMPILE_OBJ)

$(BLD_DIR)/libsr.a: $(BLD_DIR)/sr.o
	$(MAKE_STATIC_LIB)

$(BLD_DIR)/test_sr: test_sr.c
	$(MAKE_TEST)
