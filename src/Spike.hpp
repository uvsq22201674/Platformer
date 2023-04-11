#ifndef SPIKE_HPP_INCLUDED
#define SPIKE_HPP_INCLUDED

#include "Body.hpp"

class Spike : public Body
{
public:

	Spike(sf::Vector2f arg0 = sf::Vector2f(0.f, 0.f), sf::Vector2f arg1 = sf::Vector2f(100.f, 100.f));

	//Override
	std::string toString() const;
	
};


#endif