#include "World.hpp"
#include "Spike.hpp"
#include "Checkpoint.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

World::World(string const& arg0, float arg1):file_path(arg0), unit(arg1)
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
		int state (0);

		string line;

		while(getline(in, line))
		{
			string cop;
			for(char c : line)
			{
				if(c != ' ' and c != '\t')
					cop += c;
			}
			line = cop;
			
			if(state == 0)
			{
				
				if(line == "Platform")
				{
					bodies.push_back(new Body());
				}
				else if(line == "Spike")
				{
					bodies.push_back(new Spike());
				}
				else if(line == "Checkpoint")
				{
					bodies.push_back(new Checkpoint());
				}
				else if(line == "Player")
				{
					bodies.push_back(new Player());
					cameras.push_back(Camera(Vector2f(0.f, 0.f), Vector2f(800.f, 450.f)));
					cameras.at(cameras.size() - 1).follow(*bodies.at(bodies.size() - 1));
				}

				state ++;
			}
			else if(state == 1)
			{
				if(line.find('}') != string::npos)
				{
					bodies.at(bodies.size()-1)->setCenter(bodies.at(bodies.size()-1)->getCenter() + bodies.at(bodies.size()-1)->getSize()/2.f);

					state = 0;
					continue;
				}

				string buffer;
				string attrib;
				int stat (0);

				for(int i (0); i < line.size(); i++)
				{
					if(line.at(i) == ':')
					{
						if(buffer == "position" || buffer == "size")
							i++;
						attrib = buffer;
						buffer = "";
						stat = 1;
					}
					else if(line.at(i) == ',' && stat == 1)
					{
						if(attrib == "options")
						{
							//TODO
							break;
						}

						if(attrib == "position")
						{
							bodies.at(bodies.size()-1)->setCenter(Vector2f(stoi(buffer), 0));
						}
						else if(attrib == "size")
							bodies.at(bodies.size()-1)->setSize(Vector2f(stoi(buffer), 0));

						stat = 2;

						buffer = "";
					}
					else if((line.at(i) == ',' || i == line.size() - 1) && stat == 2)
					{
						if(attrib == "position")
						{
							bodies.at(bodies.size()-1)->setCenter(bodies.at(bodies.size()-1)->getCenter() + Vector2f(0, stoi(buffer)));
							
							if((*(bodies.end()-1))->toString().find("Player") != string::npos)
							{
								Player * p ((Player*) *(bodies.end()-1));
								p->setSpawn(p->getCenter());
							}
						}
						else if(attrib == "size")
							bodies.at(bodies.size()-1)->setSize(bodies.at(bodies.size()-1)->getSize() + Vector2f(0, stoi(buffer)));

						break;
					}
					else
						buffer += line.at(i);
				}
			}

		}

		cout << "Fichier " << file_path << " chargé !" << endl;
		
		return true;
	}
	else
	{
		cout << "/!\\ Erreur : Fichier " << file_path << " illisible !" << endl;
		
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
		FloatRect box0 ({body_ptr->getCenter().x - body_ptr->getSize().x/2.f, body_ptr->getCenter().y - body_ptr->getSize().y/2.f, body_ptr->getSize().x, body_ptr->getSize().y});
		FloatRect box1 ({cameras[0].getCenter().x - cameras[0].getSize().x/2.f, cameras[0].getCenter().y - cameras[0].getSize().y/2.f, cameras[0].getSize().x, cameras[0].getSize().y});
		

		if(box0.left > box1.left + box1.width
		|| box0.left + box0.width < box1.left
		|| box0.top > box1.top + box1.height
		|| box0.top + box0.height < box1.top)
			continue;

		if(body_ptr->toString().find("Player") != string::npos)
		{
			Player * player ((Player*) body_ptr);

			for(Event const& e:event_queue)
				player->reactTo(e);
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
		visual.setFillColor(Color::White);
		if(body_ptr->toString().find("Spike")!=string::npos)
			visual.setFillColor(Color::Red);
		else if(body_ptr->toString().find("Player")!=string::npos)
			visual.setFillColor(Color::Yellow);
		else if(body_ptr->toString().find("Checkpoint")!=string::npos)
			visual.setFillColor(Color::Green);

		arg.draw(visual);
	}

	event_queue.clear();
}

void World::addToEventQueue(Event const& arg)
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

	event_queue.push_back(arg);
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
	event_queue.clear();
}