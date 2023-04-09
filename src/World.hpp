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

	bool load();
	void unload();

	void work(sf::RenderWindow & arg);
	void passEvent(sf::Event const&);

	std::string getName() const;

private:

	bool passed;

	sf::Event to_pass;
	sf::RectangleShape visual;

	std::string name;
	std::string file_path;
	float unit;

	std::vector<Body*> bodies;
	std::vector<Camera> cameras;

};

#endif