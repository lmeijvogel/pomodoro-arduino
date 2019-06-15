CXX = g++
CXXFLAGS = -std=c++0x -g -Wall -Wextra -pedantic

APP_FILES_O = obj/GuiLight.o obj/CountdownState.o obj/PomodoroState.o obj/BreakState.o obj/BlinkingState.o obj/WaitingState.o obj/Context.o obj/RandomGenerator.o
TEST_FILES_O = obj/PomodoroStateTests.o obj/ContextTests.o $(APP_FILES_O)

default: bin bin/main

arduino:
	$(MAKE) -C src

upload:
	$(MAKE) -C src upload

all: default arduino

bin/main: main.cpp $(APP_FILES_O) obj/BareGui.o obj/NCursesGui.o
	$(CXX) main.cpp $(APP_FILES_O) obj/BareGui.o obj/NCursesGui.o $(CXXFLAGS) -lncurses -o $@

clean:
	rm -rf bin
	rm -f $(APP_FILES_O) $(TEST_FILES_O)
	$(MAKE) -C src clean

purge: clean
	rm -rf obj

test: bin bin/run_tests
	bin/run_tests

obj:
	mkdir -p obj

obj/catch.o:  __tests__/suite.cpp
	$(CXX) __tests__/suite.cpp -c -o $@ $(CXXFLAGS)

obj/PomodoroStateTests.o: __tests__/PomodoroStateTests.cpp obj/PomodoroState.o
	$(CXX) __tests__/PomodoroStateTests.cpp -c -o $@ $(CXXFLAGS)

obj/ContextTests.o: __tests__/ContextTests.cpp obj/Context.o
	$(CXX) __tests__/ContextTests.cpp -c -o $@ $(CXXFLAGS)

obj/PomodoroState.o: obj/CountdownState.o src/PomodoroState.cpp
	$(CXX) src/PomodoroState.cpp -c -o $@ $(CXXFLAGS)

obj/BlinkingState.o: obj/CountdownState.o src/BlinkingState.cpp
	$(CXX) src/BlinkingState.cpp -c -o $@ $(CXXFLAGS)

obj/RandomBlinkingState.o: src/RandomBlinkingState.cpp obj/CountdownState.o  obj/RandomGenerator.o
	$(CXX) src/RandomBlinkingState.cpp -c -o $@ $(CXXFLAGS)

obj/RandomGenerator.o: src/RandomGenerator.cpp src/RandomGenerator.h
	$(CXX) src/RandomGenerator.cpp -c -o $@ $(CXXFLAGS)

obj/BreakDoneState.o: obj/BlinkingState.o src/BreakDoneState.cpp
	$(CXX) src/BreakDoneState.cpp -c -o $@ $(CXXFLAGS)

obj/BreakState.o: obj/CountdownState.o src/BreakState.cpp
	$(CXX) src/BreakState.cpp -c -o $@ $(CXXFLAGS)

obj/WaitingState.o: src/State.hpp src/WaitingState.cpp
	$(CXX) src/WaitingState.cpp -c -o $@ $(CXXFLAGS)

obj/Context.o: src/Context.cpp obj/PomodoroState.o obj/BlinkingState.o obj/BreakState.o obj/WaitingState.o
	$(CXX) src/Context.cpp -c -o $@ $(CXXFLAGS)

obj/CountdownState.o: src/CountdownState.cpp
	$(CXX) src/CountdownState.cpp -c -o $@ $(CXXFLAGS)

obj/StateMachine.o: src/StateMachine.hpp src/StateMachine.cpp src/State.hpp
	$(CXX) src/StateMachine.cpp -c -o $@ $(CXXFLAGS)

obj/BareGui.o: BareGui.cpp Gui.hpp
	$(CXX) BareGui.cpp -c -o $@ $(CXXFLAGS)

obj/NCursesGui.o: NCursesGui.cpp Gui.hpp
	$(CXX) NCursesGui.cpp -c -o $@ $(CXXFLAGS)

obj/GuiLight.o: src/GuiLight.cpp src/Light.hpp
	$(CXX) src/GuiLight.cpp -c -o $@ $(CXXFLAGS)

bin: obj
	mkdir -p bin

bin/run_tests: $(APP_FILES_O) $(TEST_FILES_O) obj/catch.o
	$(CXX) $(TEST_FILES_O) obj/catch.o -o $@ $(CXXFLAGS)
