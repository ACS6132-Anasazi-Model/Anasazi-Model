include ./env

.PHONY: create_folders
create_folders:
	mkdir -p objects
	mkdir -p bin
	mkdir -p tests

.PHONY: clean_compiled_files
clean_compiled_files:
	rm -f ./objects/*.o
	rm -f ./bin/*.exe

.PHONY: clean
clean:
	rm -rf objects
	rm -rf bin

.PHONY: compile
compile: clean_compiled_files
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Main.cpp -o ./objects/Main.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Model.cpp -o ./objects/Model.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Field.cpp -o ./objects/Field.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Household.cpp -o ./objects/Household.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Location.cpp -o ./objects/Location.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./bin/main.exe  ./objects/Main.o ./objects/Model.o ./objects/Field.o ./objects/Household.o ./objects/Location.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: all
all: clean create_folders compile

.PHONY: test1
test1:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test1.cpp -o ./tests/test1.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test1.exe  ./tests/test1.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: test2
test2:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test2.cpp -o ./tests/test2.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test2.exe  ./tests/test2.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: test3
test3:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test3.cpp -o ./tests/test3.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test3.exe  ./tests/test3.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: test4
test4:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test4.cpp -o ./tests/test4.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test4.exe  ./tests/test4.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: test5
test5:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test5.cpp -o ./tests/test5.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test5.exe  ./tests/test5.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: test6
test6:
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./tests/test6.cpp -o ./tests/test6.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./tests/test6.exe  ./tests/test6.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY: all_tests
all_tests: test1 test2 test3 test4 test5 test6
