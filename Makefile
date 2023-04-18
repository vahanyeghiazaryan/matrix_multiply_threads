export EXE_FILE := bin/exe
export OUTPUT_FILE := test/output.txt
export GOUT_FILE := test/gout.txt
export SOURCE_FILES := $(wildcard src/*.cpp)
export T_SOURCE_FILES := $(wildcard test/src/*.cpp)
export OBJ_FILES := $(patsubst src/%.cpp, bin/%.o, $(SOURCE_FILES))
export T_OBJ_FILES := $(patsubst test/src/%.cpp,test/bin/%.o,$(T_SOURCE_FILES))
export DEP_FILES := $(patsubst src/%.cpp, bin/%.dep, $(SOURCE_FILES))
export T_DEP_FILES := $(patsubst test/src/%.cpp,test/bin/%.dep,$(T_SOURCE_FILES))

$(EXE_FILE) : $(OBJ_FILES)
	@-gcc $^ -lm -lstdc++ -o $@
	@-echo "made!"

./bin/%.o : ./src/%.cpp
	@-gcc -c $< -o $@

./bin/%.dep : ./src/%.cpp
	@-mkdir -p bin
	@-gcc -M $< -o $@

.PHONY : clean
clean :
	@-rm -rf bin output.txt
	@-echo "cleaned!"

.PHONY : run
run : $(EXE_FILE)
	@-./$(EXE_FILE) 16 ./input1.txt ./input2.txt

-include $(DEP_FILES)
