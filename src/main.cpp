#include "Body.hpp"
#include "Player.hpp"
#include "World.hpp"
#include <iostream>

using namespace sf;
using namespace std;

int main()
{

	RenderWindow win (VideoMode(800, 450), "Plateform - Sama Rive", Style::Close);
	win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(true);

	win.setPosition(Vector2i(100, 100));

	World test ("test.world", 32.f);


	while(win.isOpen())
	{
		Event event;

		while(win.pollEvent(event))
		{
			switch(event.type)
			{
				case Event::Closed:
					win.close();
				break;
				case Event::KeyPressed:
					test.passEvent(event);
				break;
				case Event::KeyReleased:
					test.passEvent(event);
				break;
			}
		}

		win.clear();

		test.work(win);

		win.display();

	}

	
	return 0;
};