#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "Body.hpp"

enum Stunt
{
	NoStunt,
	BarrelRoll,
	QuickZoom,
	EarthQuake
};

class Camera : public sf::View
{
public:

	Camera();
	Camera(sf::Vector2f const& arg0, sf::Vector2f const& arg1);

	void update();

	void follow(Body const&);

	void doStunt(Stunt const&);

private:

	void stopStunt();

	int frame_count;
	Stunt doing;
	Body const* following;
	sf::Vector2f initial_size;

};


#endif