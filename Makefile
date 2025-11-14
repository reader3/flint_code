# Makefile to compile projects

CC = gcc
CFLAGS = -lm
LDFLAGS = -lflint

# Output binaries
CHK_AUTOS = check_autoset
CHK_NORM = check_norm
CHK_COMPLEX = check_complex

# Test flag files
TEST_AUTOS = .test_autoset_done
TEST_NORM = .test_norm_done
TEST_COMPLEX = .test_complex_done

.PHONY: all autoset norm complex debug_complex debug_autoset clean help clean_norm clean_complex clean_autoset

# Default target
all: autoset norm complex

# Help target
help:
	@echo "Makefile Targets:"
	@echo "  all               - Compile and test autoset, norm, and complex"
	@echo "  help              - Display this help message"
	@echo "  autoset           - Compile and test autoset"
	@echo "  norm              - Compile and test norm"
	@echo "  complex           - Compile and test complex"
	@echo "  debug_complex     - Compile complex with debug enabled"
	@echo "  debug_autoset     - Compile autoset with debug enabled"
	@echo "  test_autoset      - Test autoset_bin_prec program"
	@echo "  test_norm         - Test acb_norm program"
	@echo "  test_complex      - Test acb_set_str program"
	@echo "  clean             - Remove all compiled binaries and test files"
	@echo "  clean_norm        - Remove norm binaries and test files"
	@echo "  clean_complex     - Remove complex binaries and test files"
	@echo "  clean_autoset     - Remove autoset binaries and test files"

# Target for autoset
autoset:
	@if [ ! -f $(CHK_AUTOS) ]; then echo "Compiling autoset_bin_prec.c..."; fi
	@$(CC) -o $(CHK_AUTOS) autoset_bin_prec_main.c autoset_bin_prec.c $(CFLAGS)
	@if [ ! -f $(TEST_AUTOS) ]; then \
		echo "Running auto set tests..."; \
		bash test_prec.sh; \
		touch $(TEST_AUTOS); \
	else \
		echo "Auto set tests already run."; \
	fi

# Target for norm
norm:
	@if [ ! -f $(CHK_NORM) ]; then echo "Compiling acb_norm.c..."; fi
	@$(CC) -o $(CHK_NORM) acb_norm_main.c acb_norm.c acb_set_str.c autoset_bin_prec.c $(LDFLAGS) $(CFLAGS)
	@if [ ! -f $(TEST_NORM) ]; then \
		echo "Running norm tests..."; \
		bash test_mod.sh; \
		bash test_norm.sh; \
		touch $(TEST_NORM); \
	else \
		echo "Norm tests already run."; \
	fi

# Target for complex
complex:
	@if [ ! -f $(CHK_COMPLEX) ]; then echo "Compiling acb_set_str..."; fi
	@$(CC) -o $(CHK_COMPLEX) acb_set_str_main.c acb_set_str.c autoset_bin_prec.c $(LDFLAGS) $(CFLAGS)
	@if [ ! -f $(TEST_COMPLEX) ]; then \
		echo "Running complex tests..."; \
		bash test_complex.sh; \
		touch $(TEST_COMPLEX); \
	else \
		echo "Acb Complex tests already run."; \
	fi

# Debug target for complex
debug_complex:
	@vim acb_set_str.c -s enable_debug.vim
	@echo "Compiling complex with debug enabled..."
	@$(CC) -o $(CHK_COMPLEX) acb_set_str_main.c acb_set_str.c autoset_bin_prec.c $(LDFLAGS) $(CFLAGS)
	@echo "Running complex debug tests..."
	@bash test_complex.sh
	@vim acb_set_str.c -s disable_debug.vim
	@touch acb_set_str.c
	@rm .test_complex_done

# Debug target for autoset
debug_autoset:
	@vim autoset_bin_prec.c -s enable_debug.vim
	@echo "Compiling autoset with debug enabled..."
	@$(CC) -o $(CHK_AUTOS) autoset_bin_prec_main.c autoset_bin_prec.c $(CFLAGS)
	@echo "Running autoset debug tests..."
	@bash test_prec.sh
	@vim autoset_bin_prec.c -s disable_debug.vim
	@touch autoset_bin_prec.c
	@rm .test_autoset_done

test_autoset:
	@bash test_prec.sh

test_norm:
	@bash test_mod.sh
	@bash test_norm.sh

test_complex:
	@bash test_complex.sh

# Clean target
clean:
	rm -f $(CHK_AUTOS) $(CHK_NORM) $(CHK_COMPLEX) $(TEST_AUTOS) $(TEST_NORM) $(TEST_COMPLEX)

# Selective clean targets
clean_norm:
	rm -f $(CHK_NORM) $(TEST_NORM)

clean_complex:
	rm -f $(CHK_COMPLEX) $(TEST_COMPLEX)

clean_autoset:
	rm -f $(CHK_AUTOS) $(TEST_AUTOS)

