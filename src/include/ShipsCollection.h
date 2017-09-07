#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "graphics/ShaderProgram.h"
#include "graphics/VertexArray.h"

#include <deque>

struct State;
class Ship;
namespace math { struct Point2; }

class ShipsCollection
{
public:
	ShipsCollection();
	ShipsCollection(ShipsCollection&&)=default;

	ushort addShip(const math::Point2& location);

	Ship& operator[](ushort index);
	const auto& operator*() { return ships; }
	auto* operator-> () { return &ships; }

	void update(const State&, double);

	void draw();

	~ShipsCollection();

private:
	ShaderProgram shaderProgram;
	std::deque<Ship> ships; // Deque, in order to keep pointers to Ships valid
	                        // after push_back-ing. Consider storing indices
	                        // instead of pointers to resolve this problem and
	                        // get back to using vector.

	VertexArray vertexArray;

	void draw(const Ship& ship);
};

#endif /* end of include guard: SHIPSCOLLECTION_H_YWCRVLBE */
