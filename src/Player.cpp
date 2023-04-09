#include "Player.hpp"
#include <utility>
#include <cmath>

using namespace sf;
using namespace std;

Player::Player(Vector2f const& arg0, Vector2f const& arg1):RigidBody::RigidBody(arg0, arg1),
														   bindings({
														   		{Keyboard::Right, Action::Right},
														   		{Keyboard::Left, Action::Left},
														   		{Keyboard::Space, Action::Up},
														   		{Keyboard::Down, Action::Down}
														   }),
														   celerity(7.5f),
														   momentum_factor(0.4f),
														   jump_force(16.f),
														   fast_fall_force(5.f),
														   dir(0)
{
	in_focus = true;
}

void Player::bind(Action arg0, Keyboard::Key arg1)
{
	try
	{
		bindings.at(arg1) = arg0;
	}catch(exception const& e)
	{
		bindings.insert(pair<Keyboard::Key, Action>(arg1, arg0));
	}
}


//Setters
void Player::setCelerity(float arg)
{
	celerity = arg;
}
void Player::setJumpForce(float arg)
{
	jump_force = arg;
}
void Player::setMomentumFactor(float arg)
{
	momentum_factor = arg;
}


//Override
void Player::update()
{
	speed.x = celerity * dir;

	RigidBody::update();
}

string Player::toString() const
{
	return RigidBody::toString() + " Player";
}
void Player::process(sf::Event const& event)
{
	if(event.type == Event::KeyPressed)
	{
		Action act;
		try
		{
			act = bindings.at(event.key.code);
		}catch(exception const& e)
		{
			act = Action::NoAction;
		}

		switch(act)
		{
		case Action::Right:
			dir = 1;
		break;
		case Action::Left:
			dir = -1;
		break;
		case Action::Up:
			if(isOnGround())
				speed.y -=  jump_force + abs(speed.x) * momentum_factor;

		break;
		case Action::Down:
			speed.y += fast_fall_force;
		break;
		}
	}
	else if(event.type == Event::KeyReleased)
	{
		Action act;
		try
		{
			act = bindings.at(event.key.code);
		}catch(exception const& e)
		{
			act = Action::NoAction;
		}

		switch(act)
		{
		case Action::Right:
			if(dir > 0.f)
				dir = 0;

		break;
		case Action::Left:
			if(speed.x < 0.f)
				dir = 0;
		break;
		case Action::Up:
			if(speed.y < 0.f)
				speed.y /= 2.f;
		break;
		}
	}
}