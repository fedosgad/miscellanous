"All-in-one" (compile and generate results for all variants):

	make all

Compile:

	make

Compile with debug support:

	make debug

Calculate results and drop them to the results/ folder (Unix only)

	./gen_results.sh

Cleanup:

	make clean - to remove binaries only
	make clean_all - to remove binaries and results

General help (also available using -h option):

	Available options:
	-1, -2 - select variant (1 by default)
	-v - print intermediate information
	-h - print this help and exit
	-q - don't promt for input
	-c - only calculate an print coefs.
	-i - only calculate and print intervals
	-r - only calculate and print roots
	-s [int] - divide initial guess into [int] segments (10000 by default)
	-e [float] - find roots with [float] precesion (1e-6 by default)

	Data format:
	a/b for gamma (for example, "5/3"),
	a.bEc for everything else (for example 1.2E3).

	In the first var. you enter U, then P.
	In the second var. you first enter P, then U.
