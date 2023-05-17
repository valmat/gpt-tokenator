# CPP      = g++
# LD       = g++
# CPP_STD  = -std=c++2a

CPP      = clang++
LD       = clang++
CPP_STD  = -std=c++20


OPT_FLAG = -pedantic -Wall -Wextra -fno-rtti -Wsequence-point
INCLUDES = -I. -I../include
LIBS     = -licuuc -licui18n

CPP_FLAGS = ${CPP_STD} ${OPT_FLAG} ${INCLUDES} -DNDEBUG -c
CPP_LTO   = ${CPP_FLAGS} -flto
LD_FLAGS  = ${OPT_FLAG} 
LD__LTO   = ${LD_FLAGS} -flto

############################################################
.PHONY: clean

# all: ../libs/libtokenator_cpp.a ../libs/libtokenator_cpp_lto.a
all: ../libs/libtokenator_cpp.a
	@echo
	@echo "\033[1;36mBuild complite\033[0m"

# Compiling
o/bpe_ranks.o : bpe_ranks.cpp
	${CPP} -O0 ${CPP_FLAGS} bpe_ranks.cpp -o o/bpe_ranks.o

o/tokenator.o : tokenator.cpp
	${CPP} -O3 ${CPP_FLAGS} tokenator.cpp -o o/tokenator.o

o/bpe_ranks_lte.o : bpe_ranks.cpp
	${CPP} -O0 ${CPP_LTO} bpe_ranks.cpp -o o/bpe_ranks_lte.o

o/tokenator_lte.o : tokenator.cpp
	${CPP} -O3 ${CPP_LTO} tokenator.cpp -o o/tokenator_lte.o


# Linking
../libs/libtokenator_cpp.a: o/bpe_ranks.o o/tokenator.o
	ar rs ../libs/libtokenator_cpp.a o/bpe_ranks.o o/tokenator.o

../libs/libtokenator_cpp_lto.a: o/bpe_ranks_lte.o o/tokenator_lte.o
	ar rs ../libs/libtokenator_cpp_lto.a o/bpe_ranks_lte.o o/tokenator_lte.o

clean:
	@find "./o"      -type f -name '*.o'   -delete
	@find "../libs/" -type f -name '*.a' -delete
	@echo "\033[0;33mClean complite \033[0m"