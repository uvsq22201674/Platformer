#ifndef RIGID_BODY_HPP_INCLUDED
#define RIGID_BODY_HPP_INCLUDED

#include "Body.hpp"

class RigidBody : public Body
{
public:

	static const float GRAVITY;

	RigidBody(sf::Vector2f arg0 = sf::Vector2f(0.f, 0.f), 
			  sf::Vector2f arg1 = sf::Vector2f(100.f, 100.f));

	//Override
	virtual void update();
	virtual bool collides(Body const&);
	virtual std::string toString() const;

private:

	sf::Vector2f ref_speed;
};


#endif