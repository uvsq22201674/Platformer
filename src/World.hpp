#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "Body.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class World
{
public:
	World() = delete;
	World(std::string const&, float);
	virtual ~World();

	void work(sf::RenderWindow & arg);
	void passEvent(sf::Event const&);

private:

	sf::Event to_pass;
	bool passed;
	sf::RectangleShape visual;

	std::vector<Body*> bodies;
	std::vector<Camera> cameras;

};

#endif