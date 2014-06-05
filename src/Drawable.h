#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics.hpp>

class Drawable
{
public:
	virtual ~Drawable() {}

	virtual void Draw(sf::RenderWindow& window) = 0;

protected:
	Drawable() {}
	Drawable(const Drawable&) {}
};

#endif // DRAWABLE_H