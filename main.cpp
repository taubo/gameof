#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

enum State {
    DEAD = 0,
    ALIVE = 1,
};

struct Cell {
    State state;
    std::vector<Cell> neighbours;
};

template <typename T> struct Grid {
    unsigned width;
    unsigned height;

    Grid();
    void add(const T &cell);
    T get(unsigned x, unsigned y);
    void set(const T& cell, unsigned x, unsigned y);
    std::vector<T> get_neighbours(unsigned x, unsigned y);

    private:
        std::vector<T> grid;
};

template <typename T>
Grid<T>::Grid()
    :width(20), height(20)
{
    // grid.
}

template <typename T>
void Grid<T>::add(const T &cell)
{
    grid.push_back(cell);
}

template <typename T>
T Grid<T>::get(unsigned x, unsigned y)
{
    return grid[x + y * width];
}

template <typename T>
void Grid<T>::set(const T& cell, unsigned x, unsigned y)
{
    grid[x + y * width] = cell;
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

const unsigned frame_period = 333;
const unsigned default_w = 200;
const unsigned default_h = 200;

int main(void)
{
    std::cout << "game_of\n";
    bool is_exit = false;

    Grid<Cell> grid;
    std::cout << grid.width << " " << grid.height << "\n";

    for (unsigned i = 0; i < 20 * 20; ++i) {
        Cell cell;
        cell.state = DEAD;
        grid.add(cell);
    }

    Cell alive_cell;
    alive_cell.state = ALIVE;
    grid.set(alive_cell, 10, 10);

    Cell cell = grid.get(10, 10);
    cell = grid.get(11, 10);

    while (!is_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_period));
        display_grid(grid);
    }

}
