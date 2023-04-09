#include "World.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

World::World(string const& arg0, float arg1):passed(true)
{
	ifstream in (arg0);

	if(in)
	{
		string line;
		int j (0);
		while(getline(in, line))
		{
			Vector2f size_buffer;
			for(int i (0); i < line.size(); i++)
			{
				if(line.at(i) == '1')
				{
					bodies.push_back(new Body(Vector2f(i * arg1, j * arg1), Vector2f(arg1, arg1)));
				
				}
				else if(line.at(i) == '2')
				{
					bodies.push_back(new Player(Vector2f(i * arg1, j * arg1), Vector2f(arg1, arg1)));
					cameras.push_back(Camera(Vector2f(0.f, 0.f), Vector2f(800.f, 450.f)));
					(cameras.end()-1)->follow(*(*(bodies.end()-1)));
				}
			}

			j ++;
		}
	}
	else
	{
		cout << "/!\\ Failed to load world from " << arg0 << " /!\\" << endl;
	}

	in.close();

}

World::~World()
{
	for(Body * body_ptr : bodies)
		delete body_ptr;
}

void World::work(RenderWindow & arg)
{

	if(cameras.size() >= 0)
		arg.setView(cameras.at(0));

	for(Camera & cam_ref : cameras)
	{
		cam_ref.update();
	}

	for(Body * body_ptr : bodies)
	{
		if(!passed)
		{
			if(body_ptr->toString().find("Player") != string::npos)
			{
				Player * player ((Player*) body_ptr);

				player->reactTo(to_pass);
				passed = true;
			}
		}

		for(Body * body_ptr1 : bodies)
		{
			if(body_ptr == body_ptr1)
				continue;

			body_ptr->collides(*body_ptr1);
		}

		body_ptr->update();

		visual.setPosition(body_ptr->getCenter() - body_ptr->getSize() / 2.f);
		visual.setSize(body_ptr->getSize());
		
		arg.draw(visual);
	}
}

void World::passEvent(Event const& arg)
{
	if(arg.type == Event::KeyPressed && cameras.size() > 0)
	{
		if(arg.key.code == Keyboard::B)
			cameras.at(0).doStunt(Stunt::BarrelRoll);
		else if(arg.key.code == Keyboard::E)
			cameras.at(0).doStunt(Stunt::EarthQuake);
		else if(arg.key.code == Keyboard::K)
			cameras.at(0).doStunt(Stunt::QuickZoom);
	}

	to_pass = arg;
	passed = false;
}