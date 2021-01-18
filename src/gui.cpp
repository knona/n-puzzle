#include "Position.struct.hpp"
#include "Puzzle.class.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>

int width = 600;
int height = 600;

sf::Event        event;
sf::RenderWindow window;
int              offSetX;
int              offSetY;
int              cellSize;
sf::Color        bgColor(255, 255, 255);
sf::Font         font;
bool             manualMode = false;
int              speed = 1000;

void computeVariables()
{
	cellSize = (std::min(width, height) - 50) / Puzzle::getSize();
	offSetX = (width - cellSize * Puzzle::getSize()) / 2;
	offSetY = (height - cellSize * Puzzle::getSize()) / 2;
}

void init()
{
	window.create(sf::VideoMode(width, height), "N-PUZZLE", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2i  windowPos;
	windowPos.x = static_cast<int>(desktop.width) / 2 - width / 2;
	windowPos.y = static_cast<int>(desktop.height) / 2 - height / 2;
	window.setPosition(windowPos);
	// window.setKeyRepeatEnabled(false);
	window.pollEvent(event);
	if (!font.loadFromFile("resources/font/modular.otf"))
		throw std::runtime_error("Cannot open the font");
	computeVariables();
}

void renderFrame(const Puzzle &puzzle)
{
	int size = Puzzle::getSize();

	sf::RectangleShape square(sf::Vector2f(cellSize, cellSize));
	square.setFillColor({ 96, 199, 144 });
	square.setOutlineThickness(-5);
	square.setOutlineColor(bgColor);
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int nb = puzzle.getAt(y, x);
			if (nb == 0)
				continue;
			Position squarePos;
			squarePos.x = cellSize * x + offSetX;
			squarePos.y = cellSize * y + offSetY;

			square.setPosition(squarePos.x, squarePos.y);
			window.draw(square);

			sf::Text text;
			int      textSize = 0.70f * cellSize;
			text.setFont(font);
			text.setCharacterSize(textSize);
			text.setStyle(sf::Text::Bold);
			text.setFillColor(bgColor);
			text.setString(std::to_string(nb));

			int textWidth = text.getLocalBounds().width;
			int textHeight = textSize + 0.1 * textSize;
			text.setPosition(squarePos.x + (cellSize - 5 - textWidth) / 2,
			                 squarePos.y + (cellSize - 5 - textHeight) / 2);
			window.draw(text);
		}
	}
}

void render(const std::list<Puzzle> &list)
{
	init();
	std::list<Puzzle>::const_iterator it = list.cbegin();

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
			    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::M)
					manualMode = !manualMode;
				if (manualMode)
				{
					if (event.key.code == sf::Keyboard::Right)
					{
						if (std::next(it) != list.cend())
							it = std::next(it);
					}
					if (event.key.code == sf::Keyboard::Left)
					{
						if (it != list.begin())
							it = std::prev(it);
					}
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					if (speed > 100)
						speed -= 100;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					if (speed < 1000)
						speed += 100;
				}
				if (event.key.code == sf::Keyboard::R)
					it = list.cbegin();
			}
			if (event.type == sf::Event::Resized)
			{
				width = event.size.width;
				height = event.size.height;
				sf::FloatRect visibleArea(0, 0, width, height);
				window.setView(sf::View(visibleArea));
				computeVariables();
			}
		}
		window.clear(bgColor);
		renderFrame(*it);
		window.display();
		if (!manualMode)
		{
			if (std::next(it) != list.cend())
			{
				it = std::next(it);
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));
			}
		}
	}
}
