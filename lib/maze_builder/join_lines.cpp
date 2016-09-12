#include "def.h"
#include "join_lines.h"

#include <iostream>
using std::cout;
using std::endl;

void join_end_end(Wall2& main, Wall2& joiner, const Point2& cpoint,
                  bool join_at_a)
{
	cout << "main: " << main.segment << ", joiner: " << joiner.segment << endl;

	Point2& mainPointCloserToCrossPoint(main.segment.getEndCloserTo(cpoint));

	// Cut these lines at the point of intersection
	mainPointCloserToCrossPoint.moveTo(cpoint);
	joiner.segment.getEndCloserTo(cpoint).moveTo(cpoint);

	if (join_at_a)
		joiner.segment.a.crossRoad = joiner.opposite->segment.a.crossRoad =
		    mainPointCloserToCrossPoint.crossRoad;
	else
		joiner.segment.b.crossRoad = joiner.opposite->segment.b.crossRoad =
		    mainPointCloserToCrossPoint.crossRoad;
}

void join_end_middle(Wall2& middle, Wall2& end, Point2 ipMiddleEnd,
                     Point2 ipMiddleEndOpposite, Point2 ipMiddleOppositeEnd,
                     Point2 ipMiddleOppositeEndOpposite)
{
	cout << "Middle: " << middle.segment << ", end: " << end.segment << endl;
}
