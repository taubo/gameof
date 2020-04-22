#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
#include <utility>

#include <boost/functional/hash.hpp>

enum State {
    DEAD = 0,
    ALIVE = 1,
};

struct Cell {
    State state;
    // std::vector<Cell> neighbours;
};

struct GameGrid {
	GameGrid();

	void set_alive(int x, int y);
	void set_dead(int x, int y);
	bool is_alive(int x, int y);
	void update();

    private:
		std::unordered_map<std::pair<int, int>, Cell, boost::hash<std::pair<int, int>>> alive_cells;
		// std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> neighbours_count;
		std::map<std::pair<int, int>, int> neighbours_count;
};

GameGrid::GameGrid()
{
	// neighbours_count.
}

void GameGrid::set_alive(int x, int y)
{
	alive_cells.insert({{x, y}, {ALIVE}});
}

void GameGrid::set_dead(int x, int y)
{
	alive_cells.erase({x, y});
}

bool GameGrid::is_alive(int x, int y)
{
	return (alive_cells.find({x, y}) != alive_cells.end());
}

std::vector<std::pair<int, int>> neighbours(int x, int y)
{
	std::vector<std::pair<int, int>> neighbours_vec;

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			if (i == 0 && j == 0) {
				continue;
			}
			neighbours_vec.push_back({x + j, y + i});
		}
	}

	return neighbours_vec;
}

void GameGrid::update(void)
{
	int x, y;
	std::unordered_map<std::pair<int, int>, Cell, boost::hash<std::pair<int, int>>> alive_cells_local;
	neighbours_count.clear();

	for (alive : alive_cells) {
		x = alive.first.first;
		y = alive.first.second;
		for (neighbour : neighbours(x, y)) {
			// std::cout << neighbour.first << ", " << neighbour.second << "\n";
			auto elem = neighbours_count.find(neighbour);
			// std::cout << "count: " << neighbours_count.count({x, y}) << "\n";
			if (elem != neighbours_count.end()) {
				// std::cout << "found: (" << neighbour.first << ", " << neighbour.second << ")\n";
				elem->second++;
			} else {
				// std::cout << "inserting: (" << neighbour.first << ", " << neighbour.second << ")\n";
				neighbours_count.insert({neighbour, {1}});
			}
		}
	}

	for (elem : neighbours_count) {
		// std::cout << "(" << elem.first.first << ", " << elem.first.second << ") -> " << elem.second << "\n";
		int x = elem.first.first;
		int y = elem.first.second;
		int count = elem.second;
		if (count == 3 && alive_cells.find({x, y}) == alive_cells.end()) {
			alive_cells_local.insert({{x, y}, {ALIVE}});
		} else if ((count == 2 || count == 3) && alive_cells.find({x, y}) != alive_cells.end()) {
			alive_cells_local.insert({{x, y}, {ALIVE}});
		}
	}

	alive_cells = alive_cells_local;
}

template <typename T> struct Grid {
	unsigned width;
	unsigned height;

	Grid();
	Grid(unsigned w, unsigned h);
	void add(const T &cell);
	T get(int x, int y);
	void set(const T& cell, int x, int y);
	std::vector<T> get_neighbours(int x, int y);

	private:
	std::vector<T> grid;
};

	template <typename T>
Grid<T>::Grid(unsigned w, unsigned h)
	:width(20), height(20)
{
	width = w;
	height = h;
}

	template <typename T>
void Grid<T>::add(const T &cell)
{
	grid.push_back(cell);
}

	template <typename T>
T Grid<T>::get(int x, int y)
{
	if (x < width && y < height) {
		return grid[x + y * width];
	} else {
		throw "out of bound\n";
	}
}

	template <typename T>
void Grid<T>::set(const T& cell, int x, int y)
{
	grid[x + y * width] = cell;
}

	template <typename T>
std::vector<T> Grid<T>::get_neighbours(int x, int y)
{
	std::vector<T> neighbours;

	T elem;

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			if (i == 0 && j == 0)
				continue;
			try {
				elem = get(x + i, y + j);
				neighbours.push_back(elem);
			} catch (const char *msg) {
				// std::cout << msg;
			}
		}
	}

	return neighbours;
}

struct Game {
	private:
		unsigned frame_rate;

	public:
		void init(unsigned frame_rate);
		void update(void);
};

void Game::init(unsigned frame_rate)
{
	std::cout << "Game::init\n";

	this->frame_rate = frame_rate;
}

void display_grid(Grid<Cell> grid)
{
	for (unsigned rows = 0; rows < grid.width; rows++) {
		for (unsigned cols = 0; cols < grid.height; cols++) {
			if (grid.get(rows, cols).state == DEAD) {
				std::cout << " .";
			} else {
				std::cout << " O";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void update_grid(Grid<Cell> &grid, GameGrid &g_grid)
{
	Grid<Cell> _grid = grid;

	for (unsigned rows = 0; rows < grid.width; rows++) {
		for (unsigned cols = 0; cols < grid.height; cols++) {
			/*
			   unsigned alive = 0;
			   std::vector<Cell> neighbours_vec = grid.get_neighbours(cols, rows);
			   for (Cell c : neighbours_vec) {
			   if (c.state == ALIVE) {
			   alive++;
			   }
			   }
			   Cell c = grid.get(cols, rows);
			   if (c.state == ALIVE) {
			   if (alive < 2 || alive > 3) {
			   c.state = DEAD;
			   _grid.set(c, cols, rows);
			   }
			   } else if (c.state == DEAD) {
			   if (alive == 3) {
			   c.state = ALIVE;
			   _grid.set(c, cols, rows);
			   }
			   }
			   */
			if (g_grid.is_alive(cols, rows)) {
				_grid.set({ALIVE}, cols, rows);
			} else {
				_grid.set({DEAD}, cols, rows);
			}
		}
	}

	grid = _grid;
}

const unsigned frame_period = 100;
const unsigned default_w = 200;
const unsigned default_h = 200;

int main(void)
{
	std::cout << "game_of\n";
	bool is_exit = false;

	Grid<Cell> grid(60, 60);
	std::cout << grid.width << " " << grid.height << "\n";

	GameGrid g_grid;

	for (unsigned i = 0; i < 60 * 60; ++i) {
		Cell cell;
		cell.state = DEAD;
		grid.add(cell);
	}

	Cell alive_cell;
	alive_cell.state = ALIVE;

	/*
	   grid.set(alive_cell, 9, 9);
	   grid.set(alive_cell, 9, 10);
	   grid.set(alive_cell, 9, 11);

	   grid.set(alive_cell, 8, 8);
	   grid.set(alive_cell, 8, 9);
	   grid.set(alive_cell, 8, 10);

	   grid.set(alive_cell, 13, 4);
	   grid.set(alive_cell, 14, 5);
	   grid.set(alive_cell, 15, 3);
	   grid.set(alive_cell, 15, 4);
	   grid.set(alive_cell, 15, 5);
	   */

	g_grid.set_alive(9, 9);
	g_grid.set_alive(9, 10);
	g_grid.set_alive(9, 11);

	g_grid.set_alive(8, 8);
	g_grid.set_alive(8, 9);
	g_grid.set_alive(8, 10);

	g_grid.set_alive(13, 4);
	g_grid.set_alive(14, 5);
	g_grid.set_alive(15, 3);
	g_grid.set_alive(15, 4);
	g_grid.set_alive(15, 5);

	/*
	for (int i = 0; i < 40; ++i) {
		g_grid.set_alive(i, i);
		g_grid.set_alive(i, 39 - i);
	}
	*/

	char c;
while (!is_exit) {
	std::this_thread::sleep_for(std::chrono::milliseconds(frame_period));
	display_grid(grid);
	g_grid.update();
	update_grid(grid, g_grid);
	// std::cin >> c;
}
}
