struct Color
{
	const float r, g, b;
	char const* const name;
	operator const char*() const { return name; }
};

const Color& getColor();

