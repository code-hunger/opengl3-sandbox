#ifndef GEOMETRY_H_NTEYZYI7
#define GEOMETRY_H_NTEYZYI7

struct Point2 {
	short int x, y;

	inline bool operator==(const Point2 other) const
	{
		return this->x == other.x && this->y == other.y;
	}
};

struct Vector2 {
	int x, y;
};

struct Point3 {
	int x, y, z;
};

struct Vector3 {
	int x, y, z;
};

struct Line2 {
	Point2 a, b;
};

struct WeightLine2 {
	Point2 a;
	int weight_a;
	Point2 b;
	int weight_b;

	int len() const
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}

	inline bool operator==(const WeightLine2 other) const
	{
		return (this->a == other.a && this->b == other.b) ||
		       (this->a == other.b && this->b == other.a);
	}
};

#endif /* end of include guard: GEOMETRY_H_NTEYZYI7 */
