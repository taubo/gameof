CXXFLAGS = -std=c++17 -g

game_of: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 
