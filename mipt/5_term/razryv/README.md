"All-in-one":
	make all

Compile:
	make

Calculate results and drop them to the results/ folder (Unix only)
	./gen_results.sh

Cleanup:
	make clean - to remove binaries only
	make clean_all - to remove binaries and results

General help (also available using -h option):
	Available options:
	-1, -2 - select variant (1 by default)
	-v - print debugging information
	-h - print this help and exit
	-q - don't promt for input
	-c - only calculate an print coefs.
	-i - only calculate and print intervals

	Data format:
	a/b for gamma (for example, "5/3"),
	a.bEc for everything else (for example 1.2E3).
