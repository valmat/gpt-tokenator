# CPP   = g++
# LD    = g++
# C     = gcc
# LD_C  = gcc

CPP   = clang++
LD    = clang++
C     = clang
LD_C  = clang

# CPP_STD  = -std=c++2a
CPP_STD  = -std=c++20


INCLUDES   = -I. -I../include
OPT_FLAG   = -O3 -g  -pedantic -Wall -Wextra -Wsequence-point

CPP_FLAGS  = ${CPP_STD} ${OPT_FLAG}
LD_FLAGS   = ${OPT_FLAG}

LIBS_CPP   = ../libs/libtokenator_cpp.a -licuuc -licui18n
LIBS_C     = ../libs/libtokenator.a -licuuc -licui18n -lm -lstdc++


STRIPBIN = true

version_color= \033[1;33m

ifeq ($(BUILD),release)   
    CPP   = g++
    LD    = g++
    CPP_STD  = -std=c++2a

	CPP_FLAGS  = ${CPP_STD} -O3 ${OPT_FLAG} ${INCLUDES} -flto -DNDEBUG -c
	LD_FLAGS   = ${CPP_STD} ${OPT_FLAG} -Wl,--no-as-needed -flto

	STRIPBIN = strip --strip-all

    version_color = \033[1;32m
endif

############################################################
.PHONY: clean release




all: bin/tok_count.bin bin/tok_count_c.bin
	@echo
	@echo "\033[1;36mBuild complite\033[0m"

# Compiling
o/tok_count.o : tok_count.cpp
	${CPP} ${CPP_STD} ${OPT_FLAG} -fno-rtti ${INCLUDES} -c tok_count.cpp -o o/tok_count.o

o/tok_count_c.o : tok_count_c.c
	${C} ${OPT_FLAG} ${INCLUDES} -c tok_count_c.c -o o/tok_count_c.o

# Linking
bin/tok_count.bin: o/tok_count.o
	${LD} ${LD_FLAGS} o/tok_count.o ${LIBS_CPP} -o bin/tok_count.bin
	@strip --strip-all bin/tok_count.bin
	@echo ""

bin/tok_count_c.bin: o/tok_count_c.o
	${LD_C} ${LD_FLAGS} o/tok_count_c.o ${LIBS_C} -o bin/tok_count_c.bin
	@strip --strip-all bin/tok_count_c.bin
	@echo ""



# make release version
release:
	make "BUILD=release" -j`nproc`

clean:
	@rm -f o/*.o bin/*.bin
	@echo "\033[0;33mClean complite \033[0m"


# 	find "o"  -type f -name '*.o'   -delete
# 	find "bin" -type f -name '*.bin' -delete
# 	@echo "\033[0;33mClean complite \033[0m"