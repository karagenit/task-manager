TESTS=builds/test_trim

ARGS=-Wall -Werror -std=c++17

BUILD=g++

all: testall task_monitor

testall: $(TESTS)

task_monitor:

.PHONY: clean
clean:
	@rm -rf builds

builds/test_trim: builds/helper_functions.o builds/tests/test_trim.o
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(BUILD) $(ARGS) -o $@ $^

builds/helper_functions.o: helper_functions.cpp helper_functions.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(BUILD) $(ARGS) -c -o $@ helper_functions.cpp


builds/tests/test_trim.o: tests/test_trim.cpp helper_functions.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds/tests
	@$(BUILD) $(ARGS) -c -o $@ tests/test_trim.cpp

builds/file-system/file_system.o: file-system/file_system.cpp file-system/file_system.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds/file-system
	@$(BUILD) $(ARGS) -c -o $@ file-system/file_system.cpp
