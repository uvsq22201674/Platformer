#include "Body.hpp"
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

Body::Body(sf::Vector2f arg0, sf::Vector2f arg1):center(arg0), size(arg1), on_ground(false), on_wall(false), physics (false), collided_ground(false), collided_wall(false)
{}

void Body::update()
{
	center += speed;

	collided_wall = false;	

	collided_ground = false;
}
bool Body::collides(Body const& arg)
{
	FloatRect dis ({center.x - size.x/2 + speed.x, center.y - size.y / 2 + speed.y, size.x, size.y});
	FloatRect oth ({
		arg.getCenter().x - arg.getSize().x / 2.f + arg.getSpeed().x,
		arg.getCenter().y - arg.getSize().y / 2.f + arg.getSpeed().y,
		arg.getSize().x,
		arg.getSize().y 
	});

	if(dis.left + dis.width <= oth.left
	|| dis.top + dis.height <= oth.top
	|| dis.left >= oth.left + oth.width
	|| dis.top >= oth.top + oth.height)
	{
		if(!collided_ground)
		{
			on_ground = false;
		}
		if(!collided_wall)
		{
			on_wall = false;
		}
		
		return false;
	}
	else if(physics)
	{

		
		if(dis.left + dis.width < oth.left + speed.x + 10.f && dis.top + dis.height * 9.f/10.f >= oth.top)
		{
			speed.x = 0;
			center.x = oth.left - size.x / 2.f ;

			on_wall = true;
			collided_wall = true;
		}
		else if(dis.left > oth.left + oth.width + speed.x - 10.f && dis.top + dis.height * 9.f/10.f >= oth.top)
		{
			speed.x = 0;
			center.x = oth.left + oth.width + size.x / 2.f;

			on_wall = true;
			collided_wall = true;
		}
		else if(dis.top > oth.top + oth.height + speed.y - 10.f)
		{
			speed.y = 0;
			center.y = oth.top + oth.height + size.y / 2.f;
		}
		else if(dis.top + dis.height < oth.top + speed.y + 10.f)
		{
			speed.y = 0;
			center.y = oth.top - size.y / 2.f;

			on_ground = true;
			collided_ground = true;
		}

		return true;
	}
	else
		return true;
}

string Body::toString() const
{
	return "Body";
}

//Getters

Vector2f Body::getCenter() const
{
	return center;
}
Vector2f Body::getSize() const
{
	return size;
}
Vector2f Body::getSpeed() const
{
	return speed;
}
bool Body::isOnGround() const
{
	return on_ground;
}
bool Body::isOnWall() const
{
	return on_wall;
}

//Setters

void Body::setCenter(Vector2f const& arg)
{
	center = arg;
}
void Body::setSize(Vector2f const& arg)
{
	size = arg;
}
void Body::setSpeed(Vector2f const& arg)
{
	speed = arg;
}