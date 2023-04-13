#include "Body.hpp"
#include "Player.hpp"
#include "World.hpp"
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

int main(int argc, char ** argv)
{

	RenderWindow win (VideoMode(800, 450), "Plateform - Sama Rive", Style::Close);
	win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(true);

	win.setPosition(Vector2i(100, 100));

	string to_load ("worlds/test.lvl");

	if(argc > 1)
	{
		to_load = argv[1];
	}

	World test (to_load, 32.f);
	test.load();

	while(win.isOpen())
	{
		Event event;

		while(win.pollEvent(event))
		{
			test.addToEventQueue(event);
			switch(event.type)
			{
				case Event::Closed:
					win.close();
				break;
				case Event::KeyPressed:
					//test.addToEventQueue(event);
				break;
				case Event::KeyReleased:
					//test.addToEventQueue(event);
				break;
			}
		}

		win.clear();

		test.work(win);

		win.display();

	}

	
	return 0;
};