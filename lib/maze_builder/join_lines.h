#ifndef JOIN_LINES_H_QVIHRCKA
#define JOIN_LINES_H_QVIHRCKA

#include "def.h"

void join_end_end(Wall2& main, Wall2& joiner, const Point2& cpoint, bool join_at_a);

void join_end_middle(Wall2& middle, Wall2& end, Point2 ipMiddleEnd,
                     Point2 ipMiddleEndOpposite, Point2 ipMiddleOppositeEnd,
                     Point2 ipMiddleOppositeEndOpposite);

#endif /* end of include guard: JOIN_LINES_H_QVIHRCKA */
