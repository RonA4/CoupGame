# ronamsalem4@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

MAIN_SRC = main.cpp game/Game.cpp game/Player.cpp roles/*.cpp
GUI_SRC = GUI/gui.cpp game/Game.cpp game/Player.cpp roles/*.cpp
TEST_SRC = test/test.cpp game/Game.cpp game/Player.cpp roles/*.cpp

INCLUDES = -Igame -Iroles

BIN_MAIN = main
BIN_TEST = test_game


all: Main

.PHONY: Main GUI test clean valgrind

# Running the main file
Main:
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(INCLUDES) -o $(BIN_MAIN)
	./$(BIN_MAIN)

#Running the test file
test:
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(INCLUDES) -o $(BIN_TEST)
	./$(BIN_TEST)

#Memory leak test
valgrind: test
	valgrind --leak-check=full ./$(BIN_TEST)

#They didn't ask for it in the assignment instructions, but it's for the convenience of running the GUI.
run_gui:
	g++ GUI/gui.cpp game/*.cpp roles/*.cpp -Igame -Iroles -IGUI  -o coup_game -lsfml-graphics -lsfml-window -lsfml-system
	./coup_game

#Deletes all irrelevant files after running
clean:
	rm -f $(BIN_MAIN) $(BIN_GUI) $(BIN_TEST)
