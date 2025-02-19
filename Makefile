TESTS=builds/test_trim builds/test_file_system

ARGS=-Wall -Werror

BUILD=g++
DEFINES=-DQT_CHARTS_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
FLAGS=-std=c++17 -Wall -fPIC $(DEFINES) -pipe -g
BUILD=g++
QT_BASE=/usr/include/x86_64-linux-gnu/qt5
QTLIBS=-lQt5Core -lQt5Gui -lQt5Widgets -lQt5Gui -lQt5Charts -lGL -lpthread
LIBS=
QTINCLUDES=-I$(QT_BASE) -I$(QT_BASE)/QtCharts -I$(QT_BASE)/QtWidgets -I$(QT_BASE)/QtGui -I$(QT_BASE)/QtCore -I/usr/include/libdrm
LFLAGS=-Wall -Werror -g

LINK=$(BUILD) $(LFLAGS)
MOC=moc

all: tests task_monitor

tests: $(TESTS)


task_monitor: builds/main.o builds/systemtab.o builds/resourcestab.o \
	builds/moc_systemtab.o builds/moc_resourcestab.o builds/moc_file_system_tab.o \
	builds/file_system_tab.o builds/moc_processes_tab.o builds/processes_tab.o \
	builds/moc_menu_bar.o builds/menu_bar.o builds/running_process.o builds/helper_functions.o \
	builds/file_system.o
	$(LINK) -o $@ $^ $(QTLIBS) $(LIBS)

.PHONY: clean
clean:
	@rm -rf builds

builds/test_trim: builds/helper_functions.o builds/tests/test_trim.o
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -o $@ $^

builds/test_file_system: builds/file_system.o builds/tests/test_file_system.o builds/helper_functions.o
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(LINK) -o $@ $^ $(LIBS)

builds/helper_functions.o: helper_functions.cpp helper_functions.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c -o $@ helper_functions.cpp

builds/running_process.o: running_process.cpp running_process.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) -o $@ running_process.cpp

builds/tests/test_trim.o: tests/test_trim.cpp helper_functions.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds/tests
	@$(BUILD) $(ARGS) -c -o $@ tests/test_trim.cpp

builds/file_system.o: file_system.cpp file_system.h helper_functions.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds/file-system
	@$(BUILD) $(ARGS) -c -o $@ file_system.cpp

builds/main.o: main.cpp systemtab.h resourcestab.h file_system_tab.h processes_tab.h\
	menu_bar.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) main.cpp -o $@

builds/moc_menu_bar.o: menu_bar.h
	@mkdir -p builds
	@$(MOC) $(QTINCLUDES) -I./builds/qt $< -o builds/moc_menu_bar.cc
	@$(BUILD) $(FLAGS) $(QTINCLUDES) builds/moc_menu_bar.cc -c -o $@

builds/moc_systemtab.o: systemtab.h
	@mkdir -p builds
	@$(MOC) $(QTINCLUDES) -I./builds/qt $< -o builds/moc_systemtab.cc
	@$(BUILD) $(FLAGS) $(QTINCLUDES) builds/moc_systemtab.cc -c -o $@

builds/moc_resourcestab.o: resourcestab.h
	@mkdir -p builds
	@$(MOC) $(QTINCLUDES) -I./builds/qt $< -o builds/moc_resourcestab.cc
	@$(BUILD) $(FLAGS) $(QTINCLUDES) builds/moc_resourcestab.cc -c -o $@

builds/moc_file_system_tab.o: file_system_tab.h
	@mkdir -p builds
	@$(MOC) $(QTINCLUDES) -I./builds/qt $< -o builds/moc_file_system_tab.cc
	@$(BUILD) $(FLAGS) $(QTINCLUDES) builds/moc_file_system_tab.cc -c -o $@

builds/moc_processes_tab.o: processes_tab.h
	@mkdir -p builds
	@$(MOC) $(QTINCLUDES) -I./builds/qt $< -o builds/moc_processes_tab.cc
	@$(BUILD) $(FLAGS) $(QTINCLUDES) builds/moc_processes_tab.cc -c -o $@

builds/systemtab.o: systemtab.cpp systemtab.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) systemtab.cpp -o $@

builds/resourcestab.o: resourcestab.cpp resourcestab.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) resourcestab.cpp -o $@

builds/file_system_tab.o: file_system_tab.cpp file_system_tab.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) $< -o $@

builds/processes_tab.o: processes_tab.cpp processes_tab.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) $< -o $@

builds/menu_bar.o: menu_bar.cpp menu_bar.h
	@mkdir -p builds
	@$(BUILD) $(FLAGS) -c $(QTINCLUDES) $< -o $@

builds/tests/test_file_system.o: tests/test_file_system.cpp file_system.h
	@printf "\033[32mBuilding: "
	@printf $@
	@printf "\033[0m\n"
	@mkdir -p builds/tests
	@$(BUILD) $(ARGS) -c -o $@ tests/test_file_system.cpp