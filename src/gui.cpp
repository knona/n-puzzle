#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>

int width = 700;
int height = 750;

sf::Event        event;
sf::RenderWindow window;
int              offSetX;
int              offSetY;
int              cellSize;
sf::Color        bgColor(255, 255, 255);
sf::Font         font;
bool             pause = false;
int              speed = 1000;
int              speedIndex = 1;
int              headerHeight;
int              footerHeight;
sf::Clock        cloock;

void computeVariables()
{
	headerHeight = std::max(50.0, 0.09 * height);
	footerHeight = std::max(70.0, 0.15 * height);
	cellSize = (std::min(width, height - headerHeight - footerHeight) - 50) / Puzzle::getSize();
	offSetX = (width - cellSize * Puzzle::getSize()) / 2;
	offSetY = (height - headerHeight - footerHeight - cellSize * Puzzle::getSize()) / 2;
}

void init()
{
	window.create(sf::VideoMode(width, height), "N-PUZZLE", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2i  windowPos;
	computeVariables();
	windowPos.x = static_cast<int>(desktop.width) / 2 - width / 2;
	windowPos.y = static_cast<int>(desktop.height) / 2 - (height - headerHeight - footerHeight) / 2;
	window.setPosition(windowPos);
	window.pollEvent(event);
	if (!font.loadFromFile("resources/font/modular.otf"))
		throw std::runtime_error("Cannot open the font");
}

void drawHeader()
{
	sf::RectangleShape rectangle(sf::Vector2f(width, headerHeight));
	rectangle.setFillColor({ 50, 48, 49 });
	rectangle.setPosition(0, 0);
	window.draw(rectangle);

	sf::Text text;
	int      textSize = 0.70 * headerHeight;
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(bgColor);
	text.setString("N-PUZZLE");

	int textWidth = text.getLocalBounds().width;
	int textHeight = 1.15 * textSize;
	text.setPosition((width - textWidth) / 2, (headerHeight - textHeight) / 2);
	window.draw(text);
}

void drawFooter(int index, int indexMax)
{
	int                footerY = height - footerHeight;
	sf::RectangleShape rectangle(sf::Vector2f(width, footerHeight));
	rectangle.setFillColor({ 50, 48, 49 });
	rectangle.setPosition(0, footerY);
	window.draw(rectangle);

	sf::Text text;
	int      textSize = 0.25 * footerHeight;
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setFillColor({ 255, 255, 255 });

	bool newLine = width < 400;

	text.setString(catArgs("Speed ", speedIndex, newLine ? "\n" : "    ", "Puzzle ", index, "/", indexMax));
	int textWidth = text.getLocalBounds().width;
	int textHeight = 1.15 * text.getLocalBounds().height;
	text.setPosition((width - textWidth) / 2, (footerHeight - textHeight) / 2 + footerY);
	window.draw(text);
}

void renderFrame(const Puzzle &puzzle, int index, int indexMax)
{
	int size = Puzzle::getSize();

	drawHeader();
	drawFooter(index, indexMax);
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
			squarePos.y = cellSize * y + offSetY + headerHeight;

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
			int textHeight = 1.1 * textSize;
			text.setPosition(squarePos.x + (cellSize - 5 - textWidth) / 2,
			                 squarePos.y + (cellSize - 5 - textHeight) / 2);
			window.draw(text);
		}
	}
}

void render(const std::vector<Puzzle> &puzzles)
{
	init();
	int index = 0;

	sf::Time time1 = cloock.getElapsedTime();
	sf::Time time2 = cloock.getElapsedTime();

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
				if (event.key.code == sf::Keyboard::Space)
					pause = !pause;

				if (event.key.code == sf::Keyboard::Right)
				{
					if (index < static_cast<int>(puzzles.size()) - 1)
						index++;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					if (index > 0)
						index--;
				}

				if (event.key.code == sf::Keyboard::Up)
				{
					if (speed > 100)
					{
						speed -= 100;
						speedIndex++;
					}
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					if (speed < 1000)
					{
						speed += 100;
						speedIndex--;
					}
				}
				if (event.key.code == sf::Keyboard::R)
				{
					index = 0;
					time1 = cloock.getElapsedTime();
					time2 = cloock.getElapsedTime();
				}
			}
			if (event.type == sf::Event::Resized)
			{
				width = event.size.width;
				height = event.size.height;
				if (width < 300 && height < 400)
					window.setSize(sf::Vector2u(300, 400));
				else if (width < 300)
					window.setSize(sf::Vector2u(300, height));
				else if (height < 400)
					window.setSize(sf::Vector2u(width, 400));

				sf::FloatRect visibleArea(0, 0, width, height);
				window.setView(sf::View(visibleArea));
				computeVariables();
			}
		}
		window.clear(bgColor);
		renderFrame(puzzles[index], index + 1, puzzles.size());
		window.display();
		if (!pause)
		{
			time2 = cloock.getElapsedTime();
			if (time2.asMilliseconds() - time1.asMilliseconds() >= speed &&
			    index < static_cast<int>(puzzles.size()) - 1)
			{
				index++;
				time1 = cloock.getElapsedTime();
			}
		}
	}
}
