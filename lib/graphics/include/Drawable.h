#ifndef DRAWABLE_H_DEZHC0SK
#define DRAWABLE_H_DEZHC0SK

struct Drawable
{
	virtual void draw(unsigned int mode) = 0;
	virtual ~Drawable() = default;
};

#endif /* end of include guard: DRAWABLE_H_DEZHC0SK */
