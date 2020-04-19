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
    // std::vector<Cell> neighbours;
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

template <typename T>
std::vector<T> Grid<T>::get_neighbours(unsigned x, unsigned y)
{
    std::vector<T> neighbours;

    T elem;

    elem = get(x - 1, y - 1);
    neighbours.push_back(elem);
    elem = get(x, y - 1);
    neighbours.push_back(elem);
    elem = get(x + 1, y - 1);
    neighbours.push_back(elem);

    elem = get(x - 1, y);
    neighbours.push_back(elem);
    elem = get(x + 1, y);
    neighbours.push_back(elem);

    elem = get(x - 1, y + 1);
    neighbours.push_back(elem);
    elem = get(x, y + 1);
    neighbours.push_back(elem);
    elem = get(x + 1, y + 1);
    neighbours.push_back(elem);

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

void update_grid(Grid<Cell> &grid)
{
    Grid<Cell> _grid = grid;

    for (unsigned rows = 1; rows < grid.width; rows++) {
        for (unsigned cols = 1; cols < grid.height; cols++) {
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
        }
    }

    grid = _grid;
}

const unsigned frame_period = 1000;
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

    while (!is_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_period));
        display_grid(grid);
        update_grid(grid);
    }

}
