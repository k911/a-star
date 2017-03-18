#pragma once

#include <iostream>
#include <utility>
#include <unordered_set>
#include <vector>
#include <array>

// Grid structure for A* algorithm
class Grid
{
public:
	// Location structure for Grid()
	class Location
	{
	public:
		unsigned int x, y;

		Location();
		Location(unsigned int x, unsigned y);

		Location& operator=(const Location &temp);

		// Simple comparision operator overload
		bool operator==(const Location& loc) const;
		bool operator!=(const Location& loc) const;

		// Operator used for std::hash<Grid::Location> (needed by std::unordered_set)
		size_t operator()(const Location& loc) const;
	};

	/// Constructors etc.
	Grid();
	Grid(unsigned int w, unsigned int h);


	// Check if Location is within Grid
	bool within(const Location& loc) const;

	// Check if Location is not a wall
	bool operational(const Location& loc) const;

	// Returns set of locations which are in neighbourhood of @param loc
	std::vector<Location> neighbours(const Location& loc) const;

	// Empty walls set @unused
	void unsetWalls();
	
	// Mark set of locations as walls
	void setWalls(const std::vector<Location> &new_walls);

	// Simply show the grid in console
	void print(std::ostream& out = std::cout) const;
	
	// Print the grid with showing '@' character when location was part of track
	void print(const std::unordered_set<Location, Location> &track, std::ostream& out = std::cout) const;


private:
	unsigned int width, height;
	std::unordered_set<Location, Location> walls;
};