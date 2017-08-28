#include "Color.h"
#include <vector>

const Color& getColor()
{
	static const std::vector<Color> colors = {
	    {1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
	    {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
	    {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
	    {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
	    {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
	    {0, 0.5f, 0, "D green"}};
	static unsigned long color_count = colors.size(), color = 0;

	return colors[(color++) % color_count];
}
