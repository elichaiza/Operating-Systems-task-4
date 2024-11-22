# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Targets
all: OS_EX4

run: OS_EX4
	./OS_EX4 -v 5 -e 6 -s 42

OS_EX4: main.cpp
	$(CXX) $(CXXFLAGS) -o OS_EX4 main.cpp

clean:
	rm -f OS_EX4 *.o *.gcno *.gcda *.gcov gmon.out callgrind.out coverage.info
	rm -rf out

.PHONY: clean run all

# Code coverage
coverage: clean
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -o OS_EX4 main.cpp
	./OS_EX4 -v 5 -e 6 -s 42
	gcov main.cpp
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out

# Profiling with gprof
profile: clean
	$(CXX) $(CXXFLAGS) -pg -o OS_EX4 main.cpp
	./OS_EX4 -v 5 -e 6 -s 42
	gprof OS_EX4 gmon.out > analysis.txt

# Valgrind/memcheck
memcheck: OS_EX4
	valgrind --leak-check=full --log-file=valgrind_memcheck.txt ./OS_EX4 -v 3 -e 3 -s 42

# Valgrind call graph
callgrind: OS_EX4
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./OS_EX4 -v 3 -e 3 -s 42
	kcachegrind callgrind.out