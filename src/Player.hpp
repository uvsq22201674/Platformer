#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "RigidBody.hpp"
#include "EventListener.hpp"


enum Action
{
	NoAction,
	Right,
	Left,
	Up,
	Down
};

class Player : public RigidBody, public EventListener
{
public:

	Player(sf::Vector2f const& arg0 = sf::Vector2f(0.f, 0.f), sf::Vector2f const& arg1 = sf::Vector2f(100.f, 100.f));


	void bind(Action, sf::Keyboard::Key);

	//Setters
	void setCelerity(float);
	void setJumpForce(float);
	void setMomentumFactor(float);
	void setSpawn(sf::Vector2f);

	//Override RigidBody
	virtual void update();
	virtual bool collides(Body const&);
	virtual std::string toString() const;
	//Override EventListener
	virtual void process(sf::Event const&);

private:

	float celerity;
	float jump_force;
	float momentum_factor;
	float fast_fall_force;
	float dir;

	sf::Vector2f spawn;

	std::map<sf::Keyboard::Key, Action> bindings;
};

#endif