CXX = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic

ifeq ($(OS),Windows_NT)
EXE = .exe
else
EXE =
endif

RUN = ./

FUENTES = src/bigint.cpp src/multiplicacion.cpp
CABECERA = include/bigint.h

.PHONY: tests bench

tests: karatsuba_tests$(EXE)
	$(RUN)karatsuba_tests$(EXE)

bench: karatsuba_bench$(EXE)
	$(RUN)karatsuba_bench$(EXE)

karatsuba_tests$(EXE): $(FUENTES) src/tests.cpp $(CABECERA)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(FUENTES) src/tests.cpp -o $@

karatsuba_bench$(EXE): $(FUENTES) src/benchmark.cpp $(CABECERA)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(FUENTES) src/benchmark.cpp -o $@
