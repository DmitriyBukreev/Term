SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)
CFLAGS = -fsanitize=address -c
LDFLAGS = -fsanitize=address -o

all: dirs term

dirs:
	@mkdir -p obj bin

term: precompiled.h.pch $(OBJ)
	gcc $(LDFLAGS) bin/$@ $(OBJ)
	#------------------------------------------

$(OBJ): obj/%.o: src/%.c
	check/checkpatch.pl --no-tree --fix -f $^
	#------------------------------------------
	cppcheck $^
	#------------------------------------------
	gcc $(CFLAGS) $^ -o $@
	#------------------------------------------

precompiled.h.pch: src/precompiled.h
	gcc $< -o $@
	#------------------------------------------