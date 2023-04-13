#include "Checkpoint.hpp"

using namespace sf;
using namespace std;

Checkpoint::Checkpoint(Vector2f const& arg0, Vector2f const& arg1):Body::Body(arg0, arg1)
{}

string Checkpoint::toString() const
{
	return Body::toString() + " Checkpoint";
}