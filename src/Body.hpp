#ifndef BODY_HPP_INCLUDED
#define BODY_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Body
{

public:
	Body(sf::Vector2f arg0 = sf::Vector2f(0.f, 0.f), sf::Vector2f arg1 = sf::Vector2f(100.f, 100.f));

	virtual void update();
	virtual bool collides(Body const& arg);
	virtual std::string toString() const;	

	//Getters

	sf::Vector2f getCenter() const;
	sf::Vector2f getSpeed() const;
	sf::Vector2f getSize() const;
	bool isOnGround() const;

	//Setters

	void setCenter(sf::Vector2f const&);
	void setSpeed(sf::Vector2f const&);
	void setSize(sf::Vector2f const&);

protected:

	bool physics;
	sf::Vector2f speed;

private:

	bool new_loop;

	sf::Vector2f center;
	sf::Vector2f size;
	bool on_ground;
};

#endif