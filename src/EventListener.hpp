#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP


#include <SFML/Graphics.hpp>

class EventListener
{
	
public:

	EventListener();

	void reactTo(sf::Event const&);
	virtual void process(sf::Event const&) = 0;

protected:

	bool in_focus;
};

#endif