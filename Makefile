include config.mk

.PHONY: default dirs clean libsr
default: dirs libsr
	@make -s -C sr

dirs:
	@mkdir -p ${BLD_DIR}

clean:
	@rm -rf $(BLD_DIR)
