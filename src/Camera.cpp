#include "Camera.hpp"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

Camera::Camera():View::View(), following(nullptr), frame_count(0)
{}
Camera::Camera(Vector2f const& arg0, Vector2f const& arg1):View::View(arg0, arg1), following(nullptr), initial_size(arg1), doing(Stunt::NoStunt)
{}

void Camera::update()
{
	if(following != nullptr)
	{
		//TODO /!\ WHAT HAPPENS IF "following" IS DELETED ?? /!\
		

		setCenter(getCenter() + (following->getCenter() - getCenter()) / 10.f);

		if(doing != Stunt::NoStunt)
		{
			switch(doing)
			{
			case Stunt::BarrelRoll:
			{
				float angle ((float) ((float) (frame_count) * 0.3f * (float) (frame_count)));
				setRotation(angle);
				if(angle >= 360.f)
					stopStunt();
			}
			break;
			case Stunt::QuickZoom:
			{
				float angle ((float) (frame_count) * (3.1416f/180.f));
				setSize(initial_size - Vector2f(initial_size.x/3.f * sin(angle * 8.f) * 2.f, initial_size.y/3.f * sin(angle * 8.f) * 2.f));
				if(angle >= 3.1416f/8.f)
					stopStunt();
			}
			break;
			case Stunt::EarthQuake:
			{
				float angle ((float) (frame_count) * (3.1416f/180.f));
				setCenter(getCenter() + Vector2f(0.f, (getSize().y/16.f) * cos(angle * 30.f)));

				if(frame_count >= 90)
					stopStunt();
			}
			break; 

			}
		}

		if(doing != Stunt::NoStunt)
			frame_count ++;
	}
}

void Camera::follow(Body const& arg)
{
	following = &arg;
}

void Camera::doStunt(Stunt const& arg)
{
	doing = arg;
}

void Camera::stopStunt()
{
	doing = Stunt::NoStunt;
	frame_count = 0;
	setSize(initial_size);
	setRotation(0);
}