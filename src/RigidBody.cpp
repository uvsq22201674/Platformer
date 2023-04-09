#include "RigidBody.hpp"

using namespace sf;
using namespace std;

const float RigidBody::GRAVITY = 1.f;

RigidBody::RigidBody(Vector2f arg0, Vector2f arg1):Body::Body(arg0, arg1)
{
	physics = true;
}

void RigidBody::update()
{
	speed.y += GRAVITY;

	speed += ref_speed;
	Body::update();
	speed -= ref_speed;
}
bool RigidBody::collides(Body const& arg)
{
	if(Body::collides(arg))
	{

		ref_speed = arg.getSpeed();

		return true;
	}
	
	return false;
}
string RigidBody::toString() const
{
	return Body::toString() + " RigidBody";
}