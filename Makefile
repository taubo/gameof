CXXFLAGS = -std=c++17

game_of: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 
