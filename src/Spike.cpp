#include "Spike.hpp"

using namespace sf;
using namespace std;

Spike::Spike(Vector2f arg0, Vector2f arg1):Body::Body(arg0, arg1)
{}

//Override

string Spike::toString() const
{
	return Body::toString() + " Spike";
}
