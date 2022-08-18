export EXECUTABLE := executable 

.PHONY: default
default: $(EXECUTABLE)

export DEBUG_FLAGS := -o3
export SOURCE_FILES := $(wildcard *.cpp)
export OBJ_FILES := $(patsubst %.cpp, %.o, $(SOURCE_FILES))
export DEP_FILES := $(patsubst %.cpp, %.dep, $(SOURCE_FILES))
export COV_FILES := $(patsubst %.cpp, %, $(SOURCE_FILES))
export TEMP_FILES := $(OBJ_FILES) $(DEP_FILES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJ_FILES)
	@echo "Linking the executable"
	@gcc --std=c++2a $(DEBUG_FLAGS) $^  /usr/lib/gcc/x86_64-linux-gnu/9/libstdc++.a -o $@ -lm

%.o:
	@echo "Compiling $<"
	gcc $(DEBUG_FLAGS) -c $< -o $@

%.dep: %.cpp
	@echo "Generating depdendencies for $<"
	@gcc -MM $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning the temprorary files"
	@-rm -f $(TEMP_FILES)
	@-rm -f *.gcov
	@-rm -f *.gcda
	@-rm -f *.gcno

.PHONY: test
test: $(EXECUTABLE)
	@echo "Running the tests"
	@time ./$(EXECUTABLE)

.PHONY: debug
debug: clean
	@echo "Building the debug version"
	@$(MAKE) DEBUG_FLAGS="-g"
	@echo "Running GDB"
	@gdb ./$(EXECUTABLE)

.PHONY: release
release: clean
	@echo "Building the release version"
	@$(MAKE) DEBUG_FLAGS="-o3"

.PHONY: profile
profile: clean
	@echo "Building the profile version"
	@$(MAKE) DEBUG_FLAGS="-pg"
	@echo "Running the application in profile mode"
	@./$(EXECUTABLE)
	@echo "Running the profiler to see the output"
	@gprof ./$(EXECUTABLE)

.PHONY: coverage
coverage: clean
	@echo "Building with gcov "
	@$(MAKE) DEBUG_FLAGS="-g --coverage"
	@echo "Running the app in coverage mode"
	@./$(EXECUTABLE)
	@echo "Running the coverage tool"
	@gcov $(COV_FILES)

include $(DEP_FILES)
