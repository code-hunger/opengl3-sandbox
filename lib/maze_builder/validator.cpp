#include "validator.h"

#include <iostream>
using std::cout;
using std::endl;

void validate_cross_road(CrossRoad* crossRoad)
{
	if (crossRoad == nullptr) {
		throw "CrossRoad pointer is empty!";
	}
	if (crossRoad->points.size() < 1) {
		throw "CrossRoads should have at least one point!";
	}
	if (crossRoad->lines.size() < 2) {
		throw "CrossRoads can't have less than 2 lines!";
	}
}

void validate_walls(Walls& walls, bool force)
{
	if (!force) {
		return;
	}
	cout << "Validating walls...";
	bool valid = true;
	for (const auto& wall : walls) {
		// First check the opposite
		if (wall.opposite == nullptr) {
			cout << "\nOpposite is nullptr: " << wall.segment << endl;
			valid = false;
		} else if (wall.opposite->opposite != &wall) {
			cout << "\nThe opposite's opposite is not the current wall!";
			valid = false;
		} else {
			if (wall.segment.a.crossRoad !=
			    wall.opposite->segment.a.crossRoad) {
				puts("\npointA's crossRoad is not the same as opposite's "
				     "A.crossRoad");
				valid = false;
			}
			if (wall.segment.b.crossRoad !=
			    wall.opposite->segment.b.crossRoad) {
				std::cout << "\npointB's crossRoad is not the same as "
				             "opposite's B.crossRoad"
				          << endl;
				valid = false;
			}
		}

		try {
			validate_cross_road(wall.segment.a.crossRoad);
			validate_cross_road(wall.segment.b.crossRoad);
		} catch (const char* err) {
			cout << "\n" << err << endl;
			valid = false;
		}
	}
	if (!valid) {
		cout << endl;
		throw "Walls' state invalid!";
	}
	cout << " ready!" << endl;
}
