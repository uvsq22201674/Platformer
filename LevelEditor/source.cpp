#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace sf;
using namespace std;

#include <vector>

class Widget
{
public:

	FloatRect rect;
	string message;
	bool selected = false;
	bool hidden = false;


	Widget(FloatRect arg = FloatRect({0, 0, 100, 100}), Widget const* arg1 = nullptr):
		rect(arg),
		parent(arg1)
	{
		if(parent == nullptr)
			parent = this;
	}

	void paint(RenderWindow & arg);

	void onEvent(Event e)
	{
		if(hidden)
			return;
		switch(e.type)
		{
		case Event::MouseButtonPressed:
			if(e.mouseButton.x > rect.left
			&& e.mouseButton.x < rect.left + rect.width
			&& e.mouseButton.y > rect.top
			&& e.mouseButton.y < rect.top + rect.height)
			{
				selected = true;
				onClick();
			}
			else
			{
				selected = false;
			}
		break;
	case Event::KeyPressed:
		if(selected)
			onKeyPress(e.key.code);
		break;
		}
	}

	virtual string toString() const
	{
		return "Widget";
	}

protected:
	virtual void draw(RenderWindow & arg) = 0;

	virtual void onClick()
	{}
	virtual void onKeyPress(Keyboard::Key arg)
	{}

	Widget const* parent;

};

class LineEdit : public Widget
{
public:

	Text text;
	RectangleShape field;
	RectangleShape border;
	Font help_me;
	string default_string;

	LineEdit(FloatRect arg0 = FloatRect(0, 0, 100, 100), string arg1 = "LineEdit", Widget const* arg2 = nullptr):
		Widget::Widget(arg0,arg2), default_string(arg1)
	{

		help_me.loadFromFile("HelpMe.ttf");

		text.setFont(help_me);
		text.setCharacterSize(rect.height * 8.f/10.f);
		text.setFillColor(Color(25, 25, 25));
		text.setString(default_string);
		text.setPosition(Vector2f(rect.left + 1.f, rect.top + rect.height/10.f));
		
		field.setSize(Vector2f(rect.width, rect.height));
		field.setPosition(Vector2f(rect.left, rect.top));
		field.setFillColor(Color::White);

		border.setSize(Vector2f(rect.width + 4, rect.height + 4));
		border.setPosition(Vector2f(rect.left - 2, rect.top - 2));
		border.setFillColor(Color::Black);
	}

	virtual string toString() const
	{
		return Widget::toString() + " LineEdit";
	}
private:

	virtual void draw(RenderWindow & arg)
	{
		if(selected)
			border.setFillColor(Color::Red);
		else
			border.setFillColor(Color::Black);
		arg.draw(border);
		arg.draw(field);
		arg.draw(text);
	}

	virtual void onClick()
	{}
	virtual void onKeyPress(Keyboard::Key arg)
	{
		text.setFillColor(Color::Black);
		if(text.getString() == default_string)
			text.setString("");


		if(string(text.getString()).size() > 0 && arg == Keyboard::Backspace)
		{
			text.setString(string(text.getString()).substr(0, string(text.getString()).size()-1));
			
			if(text.getGlobalBounds().width > rect.width)
			{
				field.setSize(Vector2f(text.getGlobalBounds().width, field.getSize().y));
				border.setSize(field.getSize() + Vector2f(4, 4));
			}
			return;
		}

		if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::V))
		{
			text.setString(text.getString() + Clipboard::getString());
		}

		if(arg == Keyboard::Slash)
			text.setString(text.getString() + '/');

		if(arg < 26)
			text.setString(text.getString() + (char) (65 + (int) arg));
		else if(arg > 25 && arg < 36)
			text.setString(text.getString() + (char) (48 + (int) arg - 26));
		else if(arg > 74 && arg < 85)
			text.setString(text.getString() + (char) (48 + (int) arg - 75));
		else if(arg == Keyboard::Period || arg == Keyboard::SemiColon)
			text.setString(text.getString() + ".");
		else if(arg == Keyboard::Space)
			text.setString(text.getString() + " ");

		if(text.getGlobalBounds().width > rect.width)
		{
			field.setSize(Vector2f(text.getGlobalBounds().width, field.getSize().y));
			border.setSize(field.getSize() + Vector2f(4, 4));
		}
	}

};

class Button : public Widget
{
public:

	Font help_me;
	Text text;
	RectangleShape button;
	RectangleShape shadow;
	bool activated = false;

	Button(FloatRect arg0 = FloatRect({0, 0, 100, 100}), string arg1 = "Button", Widget const* arg2 = nullptr):
		Widget::Widget(arg0, arg2),
		frame_count(0)
	{
		help_me.loadFromFile("HelpMe.ttf");

		text.setFont(help_me);
		text.setString(arg1);
		text.setCharacterSize(int(rect.height * 8.f/10.f));
		while(text.getGlobalBounds().width > rect.width - 1.f*rect.width/5)
			text.setCharacterSize(text.getCharacterSize() - 1);
		text.setPosition(Vector2f(rect.left + rect.width/2.f - text.getGlobalBounds().width/2.f, rect.top + rect.height/10.f));
		text.setFillColor(Color::Black);

		button.setFillColor(Color::White);
		button.setPosition(Vector2f(rect.left, rect.top));
		button.setSize(Vector2f(rect.width, rect.height));

		shadow.setFillColor(Color(100, 100, 100));
		shadow.setSize(Vector2f(rect.width, rect.height));
		shadow.setPosition(Vector2f(rect.left, rect.top) - Vector2f(button.getSize().x/20.f, button.getSize().x/20.f));
	}

	string toString() const
	{
		return Widget::toString() + " Button";
	}

protected:
	virtual void draw(RenderWindow & arg)
	{

		if(selected)
		{
			button.setPosition(shadow.getPosition());
			
			frame_count ++;
			if(frame_count >= 3)
			{
				frame_count = 0;
				selected = false;
			}
		}
		else
			button.setPosition(shadow.getPosition() + Vector2f(button.getSize().x/20.f, button.getSize().x/20.f));

		text.setPosition(Vector2f(button.getGlobalBounds().left + button.getGlobalBounds().width/2.f - text.getGlobalBounds().width/2.f, button.getGlobalBounds().top + button.getGlobalBounds().height/10.f));

		arg.draw(shadow);
		arg.draw(button);
		arg.draw(text);
	}

	virtual void onClick()
	{
		activated = !activated;
	}

	int frame_count;
};
void Widget::paint(RenderWindow & arg)
{

	if(parent->toString().find("Button") != string::npos && parent != this)
	{
		Button * bp ((Button*) parent);
		hidden = (!bp->activated);
	}
	if(parent->hidden)
		hidden = true;
	if(hidden)
		return;
	draw(arg);
	
}


class PlaceButton : public Button
{

public:
	PlaceButton(FloatRect arg0 = FloatRect({0, 0, 100, 100}), string arg1 = "Button", Widget const* arg2 = nullptr):
		Button::Button(arg0, arg1, arg2)
	{}

	virtual string toString() const
	{
		return Button::toString() + " PlaceButton";
	}
};

struct Object
{
	string name;
	Vector2f pos;
	Vector2f size;
	string options;
	Color color;

};

int main(int argc, char ** argv)
{

	float unit (16.f);

	RenderWindow win (VideoMode(800, 450), "Platformer Level Editor - Sama Rive", Style::Close);
	win.setPosition(Vector2i(100, 100));
	win.setFramerateLimit(30);

	Button toggle_grid ({20.f, 410.f, 100.f, 30.f}, "Grid");
	Button place ({20.f, 10.f, 100.f, 30.f}, "Place");
		PlaceButton platform ({20.f, 50.f, 100.f, 30.f}, "Platform", &place);
		PlaceButton spike    ({20.f, 90.f, 100.f, 30.f}, "Spike", &place);
		PlaceButton player   ({20.f, 130.f, 100.f, 30.f}, "Player", &place);

	Button save ({130.f, 10.f, 100.f, 30.f}, "Save");
		LineEdit path ({130.f, 50.f, 100.f, 30.f}, "default.lvl", &save);
		Button ok ({130.f, 90.f, 100.f, 30.f}, "OK !", &save);

	Button load ({240.f, 10.f, 100.f, 30.f}, "Load");
		LineEdit lpath ({240.f, 50.f, 100.f, 30.f}, "default.lvl", &load);
		Button lok ({240.f, 90.f, 100.f, 30.f}, "OK !", &load);

	Widget * widgets[] = {&toggle_grid, &place, &platform, &spike, &player, &save, &path, &ok, &load, &lpath, &lok};

	toggle_grid.activated = true;

	VertexArray grid (Lines);

	vector<Vertex> h_vertices;
	vector<Vertex> v_vertices;
	for(float f (0.f); f < 800.f; f += unit)
	{
		h_vertices.push_back({Vector2f(f, 0), Color(100, 0, 0)});
		h_vertices.push_back({Vector2f(f, 450), Color(100, 0, 0)});
	
		grid.append(h_vertices.at(h_vertices.size()-1));
		grid.append(h_vertices.at(h_vertices.size()-2));
	}
	for(float f (0.f); f < 450.f; f += unit)
	{
		v_vertices.push_back({Vector2f(0, f), Color(100, 0, 0)});
		v_vertices.push_back({Vector2f(800, f), Color(100, 0, 0)});

		grid.append(v_vertices.at(v_vertices.size()-1));
		grid.append(v_vertices.at(v_vertices.size()-2));
	}

	View world_view (Vector2f(400, 225), Vector2f(800, 450));
	View hud_view (world_view);

	Object placing ({"None"});

	vector<Object> placed;
	int selected (-1);
	RectangleShape obj_vis;
	RectangleShape selection_rect;

	bool mouse_down (false);
	bool ignored_click (true);

	while(win.isOpen())
	{

		Event event;

		while(win.pollEvent(event))
		{
			bool skip_event (false);
			for(Widget * w : widgets)
			{
				w->onEvent(event);
				
				if(w->toString().find("PlaceButton") != string::npos)
				{
					PlaceButton * b ((PlaceButton*) w);

					if(b->activated)
					{
						placing.name = b->text.getString().toAnsiString();
						ignored_click = false;

						b->activated = false;
						
						skip_event = true;
					}
				}
			}
			if(skip_event)
				continue;


			if(ok.activated)
			{
				ok.activated = false;

				ofstream out (path.text.getString().toAnsiString());

				if(out)
				{
					for(Object const& obj : placed)
					{
						out << obj.name << endl << '{' << endl;
						out << "	position : [" << obj.pos.x << ", " << obj.pos.y << "]," << endl;
						out << "	size : [" << obj.size.x << ", " << obj.size.y << "]";
						if(obj.options != "")
							out << ',' << endl << "    options : \"" << obj.options << "\"";
						out << endl << '}' << endl;
					}

					cout << "Sauvegardé avec succès dans " << path.text.getString().toAnsiString() << endl;
				}
				else
					cout << "/!\\ Erreur : Fichier " << path.text.getString().toAnsiString() << " inutilisable !" << endl;
			}

			if(lok.activated)
			{
				lok.activated = false;

				placed.clear();

				ifstream in (lpath.text.getString().toAnsiString());

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
							placed.push_back(Object({line}));
							
							state ++;
						}
						else if(state == 1)
						{
							if(line.find('}') != string::npos)
							{

								if(placed.at(placed.size()-1).name == "Platform")
									placed.at(placed.size()-1).color = Color::White;
								else if(placed.at(placed.size()-1).name == "Spike")
									placed.at(placed.size()-1).color = Color::Red;
								else if(placed.at(placed.size()-1).name == "Player")
									placed.at(placed.size()-1).color = Color::Yellow;

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
										placed.at(placed.size() - 1).options = buffer;
										break;
									}

									if(attrib == "position")
									{
										placed.at(placed.size() - 1).pos.x = stoi(buffer);
									}
									else if(attrib == "size")
										placed.at(placed.size() - 1).size.x = stoi(buffer);

									stat = 2;

									buffer = "";
								}
								else if((line.at(i) == ',' || i == line.size() - 1) && stat == 2)
								{
									if(attrib == "position")
										placed.at(placed.size() - 1).pos.y = stoi(buffer);
									else if(attrib == "size")
										placed.at(placed.size() - 1).size.y = stoi(buffer);

									break;
								}
								else
									buffer += line.at(i);
							}
						}

					}

					cout << "Fichier " << lpath.text.getString().toAnsiString() << " chargé !" << endl;
				}
				else
					cout << "/!\\ Erreur : Fichier " << lpath.text.getString().toAnsiString() << " illisible !" << endl;
			}

			switch(event.type)
			{
			case Event::Closed:
				win.close();
			break;
			case Event::MouseButtonPressed:
			{
				win.setView(world_view);
				Vector2f mouse (win.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y)));

				if(placing.name != "None" && event.mouseButton.button == Mouse::Left)
				{
					if(toggle_grid.activated)
						placing.pos = Vector2f(unit * (int) (mouse.x / unit),unit * (int) (mouse.y / unit));
					else
						placing.pos = mouse;

					mouse_down = true;
				}
				else if(event.mouseButton.button == Mouse::Left)
				{
					selected = -1;
					for(int i (0); i < placed.size(); i++)
					{
						Object & obj (placed.at(i));

						if(mouse.x > obj.pos.x && mouse.x < obj.pos.x + obj.size.x
						&& mouse.y > obj.pos.y && mouse.y < obj.pos.y + obj.size.y)
							selected = i;
					}
				}
			}
			break;
			case Event::MouseMoved:
			{
				if(mouse_down && placing.name != "None")
				{
					win.setView(world_view);
					Vector2f mouse (win.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y)));

					if(toggle_grid.activated)
						placing.size = Vector2f(unit * (int) (mouse.x / unit),unit * (int) (mouse.y / unit));
					else
						placing.size = mouse;

					placing.size = placing.size - placing.pos;
				}
			}
			break;
			case Event::MouseButtonReleased:
			{
				if(placing.name != "None" && ignored_click)
				{
					if(placing.name == "Platform")
						placing.color = Color::White;
					else if(placing.name == "Spike")
						placing.color = Color::Red;
					else if(placing.name == "Player")
						placing.color = Color::Yellow;

					if(placing.size.x != 0.f && placing.size.y != 0)
					{
						selected = placed.size();
						placed.push_back(placing);
					}

					mouse_down = false;
					ignored_click = false;
					placing.name = "None";
					placing.pos = Vector2f(0.f, 0.f);
					placing.size	 = Vector2f(0.f, 0.f);			
				}
				else if(placing.name != "None")
				{
					ignored_click = true;
				}
			}
			break;
			case Event::KeyPressed:
				switch(event.key.code)
				{
				case Keyboard::Right:
					toggle_grid.activated?world_view.move(unit, 0):world_view.move(4, 0);
				break;
				case Keyboard::Left:
				 	toggle_grid.activated?world_view.move(-unit, 0):world_view.move(-4, 0);
				break;
				case Keyboard::Up:
				 	toggle_grid.activated?world_view.move(0, -unit):world_view.move(0, -4);
				break;
				case Keyboard::Down:
				 	toggle_grid.activated?world_view.move(0, unit):world_view.move(0, 4);
				break;
				case Keyboard::Delete:
					if(selected != -1)
					{
						placed.erase(placed.begin() + selected);
						selected = -1;
					}
				break;
				}
			break; 
			}
		}

		win.clear();
		
		win.setView(hud_view);

		if(toggle_grid.activated)
		{
			world_view.setCenter(Vector2f(unit * (int)(world_view.getCenter().x/unit),unit * (int)(world_view.getCenter().y/unit)));

			win.draw(grid);
		}

		win.setView(world_view);

		for(int i (0); i < placed.size(); i++)
		{
			Object & obj (placed.at(i));

			obj_vis.setPosition(obj.pos);
			obj_vis.setSize(obj.size);
			obj_vis.setFillColor(obj.color);
			
			win.draw(obj_vis);

			if(selected == i)
			{
				selection_rect = obj_vis;
				selection_rect.setFillColor(Color(0, 0, 255, 50));
				win.draw(selection_rect);
			}
		}

		if(placing.name != "None")
		{
			obj_vis.setPosition(placing.pos);
			obj_vis.setSize(placing.size);
			obj_vis.setFillColor(Color(100, 100, 100));
			win.draw(obj_vis);
		}

		
		win.setView(hud_view);
		for(Widget * w : widgets)
			w->paint(win);

		win.display();
	}

	return 0;
}
