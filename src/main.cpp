/**
 * A star search algorithm implemented for grid surface
 * @author Konrad Obal
 * @copyright Konrad Obal 2016
 */

#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <functional>
#include "Grid.h"

using namespace std;

// Get set of locations which creates rectangle with given parameters
void add_rectangle(std::vector<Grid::Location> &rect, unsigned int x_start, unsigned int x_end, unsigned int y_start, unsigned int y_end)
{
	if (x_start > x_end)
		std::swap(x_start, x_end);
	if (y_start > y_end)
		std::swap(y_start, y_end);

	for (unsigned int x = x_start; x <= x_end; ++x)
		for (unsigned int y = y_start; y <= y_end; ++y)
			rect.push_back(Grid::Location(x, y));
}

// Generate random walls on grid
std::vector<Grid::Location> generateWalls(unsigned int width, unsigned int height, unsigned int number = 4)
{
	std::vector<Grid::Location> walls;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<unsigned int> x(0, width-1), y(0, height-1), w(1, width/3), h(1, height/3);
	unsigned int x1, x2, y1, y2;

	while (number > 0)
	{
		x1 = x(mt);
		x2 = x1 + w(mt);
		y1 = y(mt);
		y2 = y1 + h(mt);

		add_rectangle(walls, x1, x2, y1, y2);
		number--;
	}
	return std::move(walls);
}


unsigned int diff(unsigned int a, unsigned int b)
{
	return (a > b) ? (a - b) : (b - a);
}


bool operator<(const pair<unsigned int, Grid::Location> &el1, const pair<unsigned int, Grid::Location> &el2)
{
	return el1.first < el2.first;
}

// Heuristic function
unsigned int hx(Grid::Location first, Grid::Location second)
{
	// Simply calculate distance in straight line on the grid
	return diff(first.x, second.x) + diff(first.y, second.y);
}

/**
 * A star search algorithm in Grid
 * @param Grid structure
 * @param Grid::Location starting point
 * @param Grid::Location goal point
 * @param unordered_map which keeps previous Location (value) which
 *        led us to current Location (key)
 * @return unsigned int total cost of the found track 
 */
unsigned int a_search
(
	const Grid &grid, 
	const Grid::Location &start, 
	const Grid::Location &goal, 
	unordered_map<Grid::Location, Grid::Location, Grid::Location> &previous_location)
{
	// Early exit when start or goal point are located outside grid or on a wall
	if (!grid.within(start) || !grid.within(goal) || !grid.operational(start) || !grid.operational(goal))
		return 0;

	// Min priority queue container which keeps seeking always for location
	// with lowest estimated cost to the goal point
	priority_queue<
		pair<unsigned int, Grid::Location>,
		vector<pair<unsigned int, Grid::Location>>,
		greater<pair<unsigned int, Grid::Location>>
	> min_queue;

	// Add starting location to initialize A* search process
	min_queue.emplace(0, start);

	// Keeps minimal summarized costs
	unordered_map<Grid::Location, unsigned int, Grid::Location> costs;

	// Main iterator
	Grid::Location current;

	while (!min_queue.empty() && (current = min_queue.top().second) != goal)
	{
		min_queue.pop();

		for (auto &nb : grid.neighbours(current)) 
		{

			// Caculate summarized cost to next (neighbour) location
			unsigned int cost = costs[current];
			++cost; // Cost to neighbour location is always '1' unless we set other type of surface
			        // e.g. forest would be  good example

			// Create costs to this location or change it if from somewhere else are lower
			if (!costs.count(nb) || cost < costs[nb]) 
			{
				// Save cost
				costs[nb] = cost;

				// Place in the min priority que, with estimated cost as priority
				min_queue.emplace(cost + hx(nb, goal), nb);

				// Store location from where cost where lower
				previous_location[nb] = current;
			}
		}
	}

	// Return costs to the goal coordiante from the starting point
	// if @return 0; road not found
	return costs[goal];
}

// Reconsruction track from the visited coordinates
unordered_set<Grid::Location, Grid::Location> get_track(
	const Grid::Location &start,
	const Grid::Location &goal,
	unordered_map<Grid::Location, Grid::Location, Grid::Location> &came_from)
{
	unordered_set<Grid::Location, Grid::Location> track = { start, goal };

	Grid::Location current = goal;
	while (current != start) 
	{
		current = came_from[current];
		track.emplace(current);
	}

	return move(track);
}

int main()
{
	// Load grid dimensions
	unsigned int width, height, walls, total_cost;
	Grid::Location start, goal;
	cout << "Width: "; cin >> width;
	cout << "Height: "; cin >> height;
	cout << "How many walls: "; cin >> walls;

	if (width <= 1 || height <= 1)
	{
		cerr << "Grid must be at least 2x2" << endl;
		exit(1);
	}

	// Show generated grid
	Grid grid(width, height);
	grid.setWalls(generateWalls(width, height, walls));
	grid.print();

	// Load coordinates
	cout << "Start coordinates [x, y]: "; cin >> start.x >> start.y;
	cout << "Goal coordinates [x, y]: "; cin >> goal.x >> goal.y;
	cout << endl << endl;

	if (start == goal)
	{
		cerr << "Start and goal coordinates must be different." << endl;
		exit(2);
	}

	// Map storing as value Location from which we get into key Location 
	unordered_map<Grid::Location, Grid::Location, Grid::Location> previous_location;

	// Start a star search algorithm
	if (total_cost = a_search(grid, start, goal, previous_location))
	{
		grid.print(get_track(start, goal, previous_location));
		cout << "Total cost: " << total_cost << endl;
	}
	else
	{
		cout << "Couldn't find track from 'start' to 'goal'." << endl;
	}

	system("PAUSE");
	return 0;
}
