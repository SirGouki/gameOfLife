/*
	Programmed by: SirGouki
	Version: 1.1 - 
		moved to its own project
		broke draw sections out to functions.
		broke main loop out to Update function.
		switched from byte arrays to unsigned char arrays
		removed un-needed braces
		removed un-needed bools for left and right mouse buttons
		removed grid.  Will re-add next update.
		removed original check code - implemented screen wrap.
	Last Updated: 2220 06AUG2019
*/
#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <Windows.h>

//function prototypes
void entry();
void Init(sf::RenderWindow&, sf::RenderTexture&);
void Update();

void DrawSurface(sf::RenderTexture & surface, sf::Shape& s, float x, float y);
void DrawScreen(sf::RenderWindow&, sf::RenderTexture&);

//global Vars... cannot settup RenderWindow here, do it in the loop
const int window_w = 640;
const int window_h = 640;
const int cellSize = 8;

#if defined(_WINDOWS)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	entry();

	return 0;
}

#endif

int main(int argc, char* argv[])
{
	entry();

	return 0;
}

void entry()
{
	//this function should initialize any necessary globals, then start the loop
	Update();
}

void Init(sf::RenderWindow& rw, sf::RenderTexture& rt)
{
	rw.create(sf::VideoMode(window_w, window_h), "");

	if (!rt.create(window_w, window_h))
	{

	}
}

void Update()
{
	//this is the main loop
	//set up local vars here
	sf::RenderWindow window;
	sf::RenderTexture surface;

	Init(window, surface);

	window.setTitle("Conway's Game of Life by SirGouki version: 1.0");

	const int bounds_x = window_w / cellSize;
	const int bounds_y = window_h / cellSize;

	bool paused = true;
	int fps = 60;

	//current generation and next gen vars
	unsigned char cells[bounds_x][bounds_y];
	unsigned char nextGen[bounds_x][bounds_y];

	enum state
	{
		dead,
		alive
	};

	//init the arrays
	for (int x = 0; x < bounds_x; x++)
	{
		for (int y = 0; y < bounds_y; y++)
		{
			cells[x][y] = dead;
			nextGen[x][y] = dead;
		}
	}

	//controls
	sf::Keyboard::Key kb_Start = sf::Keyboard::Space;
	sf::Mouse::Button m_left = sf::Mouse::Left;
	sf::Mouse::Button m_right = sf::Mouse::Right;

	bool start_Pressed = false;
	bool start_WasPressed = false;
	bool LMB_Pressed = false;
	bool RMB_Pressed = false;

	int m_x = 0;
	int m_y = 0;

	//drawing stuff
	sf::RectangleShape sh_dead;
	sf::RectangleShape sh_alive;
	sf::RectangleShape sh_selector;
	sf::Color clearColor = sf::Color::Black;

	//Set up our "Sprites"
	sh_dead.setFillColor(sf::Color::Black); //some kind of gray Color(64,64,64,255)
	sh_dead.setSize(sf::Vector2f((float)cellSize, (float)cellSize));
	sh_alive.setFillColor(sf::Color::Green);
	sh_alive.setSize(sf::Vector2f((float)cellSize, (float)cellSize));
	sh_selector.setOutlineColor(sf::Color::Red);
	sh_selector.setFillColor(sf::Color(255, 255, 255, 128));
	sh_selector.setSize(sf::Vector2f((float)cellSize, (float)cellSize));
	//init done

	
	while (window.isOpen())
	{
		//update
		m_x = sf::Mouse::getPosition(window).x;
		m_y = sf::Mouse::getPosition(window).y;

		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == kb_Start)
					start_Pressed = true;
				else
					start_Pressed = false;
				
			}
			else if (e.type == sf::Event::KeyReleased)
			{
				if (e.key.code == kb_Start)
					start_Pressed = false;
			}

			if (e.type == sf::Event::MouseButtonPressed)
			{

				if (e.mouseButton.button == m_left)
					LMB_Pressed = true;
				if (e.mouseButton.button == m_right)
					RMB_Pressed = true;
			}
			else if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.mouseButton.button == m_left)
					LMB_Pressed = false;
				if (e.mouseButton.button == m_right)
					RMB_Pressed = false;
			}

			if (e.type == sf::Event::Closed)
				window.close();
		}

		if (start_Pressed && !start_WasPressed)
			paused = !paused;

		if (LMB_Pressed)
			cells[m_x / cellSize][m_y / cellSize] = alive;

		if (RMB_Pressed)
			cells[m_x / cellSize][m_y / cellSize] = dead;

		if (!paused)
		{
			for (int x = 0; x < bounds_x; x++)
			{
				for (int y = 0; y < bounds_y; y++)
				{
					//check for living neighbors
					int neighbors = 0;

					//set up bounds
					int left = x - 1;
					//if (left < 0)
						//left = 0;

					int right = x + 1;
					//if (right > 49)
						//right = 49;

					int top = y - 1;
					//if (top < 0)
						//top = 0;

					int bottom = y + 1;
					//if (bottom > 49)
						//bottom = 49;


					for (int xx = left; xx <= right; xx++)
					{
						for (int yy = top; yy <= bottom; yy++)
						{
							//these are for checking the opposite side of the field
							int cx;
							int cy;

							//if xx is going passed the array, set cx to the opposite side of the array
							if (xx < 0)
							{
								cx = bounds_x - 1;
							}
							else if (xx == bounds_x)
							{
								cx = 0;
							}
							else
							{
								cx = xx;
							}

							//if yy is going passed the array, set cy to the opposite side of the array
							if (yy < 0)
							{
								cy = bounds_y - 1;
							}
							else if (yy == bounds_y)
							{
								cy = 0;
							}
							else
							{
								cy = yy;
							}

							if (!(x == xx && y == yy) && (cells[cx][cy] == alive))
							{
								neighbors++;
							}
						}
					}

					if (cells[x][y] == alive)
					{
						//if we have fewer than 2 neighbors, or 4+ neighbors
						if (neighbors < 2)
							nextGen[x][y] = dead;
						else if (neighbors > 3)
							nextGen[x][y] = dead;
						else
							nextGen[x][y] = alive;
					}
					else //this cell is dead
					{
						//if we have 3 neighbors, reproduction occurs
						if (neighbors == 3)
							nextGen[x][y] = alive;
						else
							nextGen[x][y] = dead;
					}
				}
			}

			//finally, update our current gen to the next gen
			for (int x = 0; x < bounds_x; x++)
			{
				for (int y = 0; y < bounds_y; y++)
				{
					cells[x][y] = nextGen[x][y];
				}
			}
		}
		else
		{
			//don't update or check for next gen while paused
		}
		//end update

		//draw step
		surface.clear(clearColor);

		for (int x = 0; x < bounds_x; x++)
		{
			for (int y = 0; y < bounds_y; y++)
			{
				if (cells[x][y] == dead)
					DrawSurface(surface, sh_dead, x*cellSize, y*cellSize);
				else
					DrawSurface(surface, sh_alive, x*cellSize, y*cellSize);
			}
		}
		DrawSurface(surface, sh_selector, m_x - (m_x % 8), m_y - (m_y % 8));
		surface.display();
		DrawScreen(window, surface);
		//end draw
		start_WasPressed = start_Pressed;

		Sleep(1000 / fps);
	}
	

}

void DrawSurface(sf::RenderTexture& surface, sf::Shape& s, float x, float y)
{
	//draw a single object to the surface
	s.setPosition(sf::Vector2f(x, y));
	surface.draw(s);

}

void DrawScreen(sf::RenderWindow& window, sf::RenderTexture& surface)
{
	window.clear(sf::Color::Black);

	const sf::Texture& tR_surface = surface.getTexture();
	sf::Sprite spr_surface(tR_surface);

	window.draw(spr_surface);
	window.display();
}
