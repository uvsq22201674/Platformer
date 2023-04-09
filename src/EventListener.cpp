#include "EventListener.hpp"

using namespace sf;
using namespace std;

EventListener::EventListener():in_focus(false)
{}

void EventListener::reactTo(Event const& e)
{
	if(in_focus)
		process(e);
}
