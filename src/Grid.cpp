#include "Grid.h"
#include <iomanip>

Grid::Grid()
	:width(0), height(0)
{
}

Grid::Grid(unsigned int w, unsigned int h)
	:width(w), height(h)
{
}

bool Grid::within(const Location & loc) const
{
	return loc.x < width && loc.y < height;
}

bool Grid::operational(const Location & loc) const
{
	// Remarks: unordered_set doesn't allow same mutiple values
	//          so count() method returns 1 when val is found | 0 otherwise
	return !walls.count(loc);
}

std::vector<Grid::Location> Grid::neighbours(const Location & loc) const
{
	std::array<Location, 4> potential = 
	{
		Location(loc.x + 1, loc.y),
		Location(loc.x, loc.y + 1),
		Location(loc.x - 1, loc.y),
		Location(loc.x, loc.y - 1)
	};

	std::vector<Location> nburs;
	for (auto &loc : potential)
	{
		if (within(loc) && operational(loc))
			nburs.push_back(loc);
	}
	return std::move(nburs);
}

void Grid::unsetWalls()
{
	walls.clear();
}

void Grid::setWalls(const std::vector<Location>& new_walls)
{
	for (const auto &wall : new_walls)
	{
		walls.emplace(wall);
	}
}

void Grid::print(std::ostream & out) const
{
	const std::streamsize size = 4;

	out << "Grid[" << width << 'x' << height << "]\n"
		<< "X-axis horizontally; Y-axis vertically\n\n"
		<< std::setw(size) << ' ';

	// Draw grid horizontal-axis indicators
	for (unsigned int x = 0; x < width; ++x) {
		out << std::setw(size) << x;
	}
	out << '\n';

	for (unsigned int y = 0; y < height; ++y)
	{
		out << std::setw(size) << y;
		for (unsigned int x = 0; x < width; ++x) {
			out << std::setw(size) << (operational(Grid::Location(x, y)) ? "[ ]" : "[X]");
		}
		out << '\n';
	}
	out << std::endl;
}

void Grid::print(const std::unordered_set<Location, Location>& track, std::ostream & out) const
{
	const std::streamsize size = 4;

	out << "Grid[" << width << 'x' << height << "]\n"
		<< "X-axis horizontally; Y-axis vertically\n" << std::endl
		<< std::setw(size) << ' ';

	// Draw grid horizontal-axis indicators
	for (unsigned int x = 0; x < width; ++x) {
		out << std::setw(size) << x;
	}
	out << '\n';

	for (unsigned int y = 0; y < height; ++y)
	{
		out << std::setw(size) << y;
		for (unsigned int x = 0; x < width; ++x) 
		{
			Grid::Location loc(x, y);
			out << std::setw(size) << (track.count(loc) ? "[@]" : (operational(loc) ? "[ ]" : "[X]"));
		}
		out << std::endl;
	}
	out << std::endl;
}

Grid::Location::Location()
	:x(0), y(0)
{
}

Grid::Location::Location(unsigned int x, unsigned y)
	:x(x), y(y)
{
}

Grid::Location & Grid::Location::operator=(const Location & temp)
{
	x = temp.x;
	y = temp.y;
	return *this;
}

bool Grid::Location::operator==(const Location & loc) const
{
	return x == loc.x && y == loc.y;
}

bool Grid::Location::operator!=(const Location & loc) const
{
	return x != loc.x || y != loc.y;
}

size_t Grid::Location::operator()(const Location & loc) const
{
	// Remarks: http://en.cppreference.com/w/cpp/utility/hash
	const size_t h1(std::hash<unsigned int>{}(loc.x));
	const size_t h2(std::hash<unsigned int>{}(loc.y));
	return h1 ^ (h2 << 1);
}