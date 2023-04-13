#ifndef CHECKPOINT_HPP_INCLUDED
#define CHECKPOINT_HPP_INCLUDED

#include "Body.hpp"

class Checkpoint : public Body
{
public:

	Checkpoint(sf::Vector2f const& arg0 = sf::Vector2f(0.f, 0.f), sf::Vector2f const& arg1 = sf::Vector2f(100.f, 100.f));

	//Override
	std::string toString() const;
	
};
#endif