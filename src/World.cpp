#include "World.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

World::World(string const& arg0, float arg1):passed(true), file_path(arg0), unit(arg1)
{
	int ext_ind (file_path.find("."));
	if(ext_ind == string::npos)
		ext_ind = file_path.size();
	for(int i (file_path.size()-1);i >= 0;i--)
	{
		if(i < ext_ind)
		{
			if((i > 0 && file_path.at(i - 1) == '/') || i == 0)
			{
				string a;
				a += toupper(file_path.at(i));
				name =  a + name;
				break;
			}
			else if(i < file_path.size() - 1 && file_path.at(i) == '_')
				name = " " + name;
			else
				name = file_path.at(i) + name;
		}
	}
}

World::~World()
{
	unload();
}

bool World::load()
{
	unload();

	ifstream in (file_path);

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
					bodies.push_back(new Body(Vector2f(i * unit, j * unit), Vector2f(unit, unit)));
				
				}
				else if(line.at(i) == '2')
				{
					bodies.push_back(new Player(Vector2f(i * unit, j * unit), Vector2f(unit, unit)));
					cameras.push_back(Camera(Vector2f(0.f, 0.f), Vector2f(800.f, 450.f)));
					(cameras.end()-1)->follow(*(*(bodies.end()-1)));
				}
			}

			j ++;
		}

		cout << "World \"" << name << "\" loaded successfully from file \"" << file_path << "\"" << endl;
		in.close();
		return true;
	}
	else
	{
		cout << "/!\\ World \"" << name << "\" failed to load from file \"" << file_path << "\" /!\\" << endl;
		in.close();
		return false;
	}
}

void World::work(RenderWindow & arg)
{

	if(cameras.size() > 0)
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

string World::getName() const
{
	return name;
}

void World::unload()
{
	for(Body * body_ptr : bodies)
		delete body_ptr;
	bodies.clear();
	cameras.clear();

	passed = true;
}