include config.mk

.PHONY: default dirs clean libsr
default: dirs libsr

dirs:
	@mkdir -p ${BLD_DIR}
	@mkdir -p ${BLD_DIR}/tests

clean:
	@rm -rf $(BLD_DIR)
	@rm -rf ${BLD_DIR}/tests

libsr:
	@make -s -C sr
	@make -s -C sr/tests
