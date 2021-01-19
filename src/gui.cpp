#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>

int width = 700;
int height = 750;

sf::RenderWindow window;
int              offSetX;
int              offSetY;
int              tileSize;
sf::Color        bgColor(255, 255, 255);
sf::Font         font;
bool             pause = false;
int              speed = 1000;
int              speedIndex = 1;
int              headerHeight;
int              footerHeight;
sf::Clock        timeClock;
sf::Time         time1;
sf::Time         time2;

void updateDimensions()
{
	headerHeight = std::max(50.0, 0.09 * height);
	footerHeight = std::max(70.0, 0.15 * height);
	tileSize = (std::min(width, height - headerHeight - footerHeight) - 50) / Puzzle::getSize();
	offSetX = (width - tileSize * Puzzle::getSize()) / 2;
	offSetY = (height - headerHeight - footerHeight - tileSize * Puzzle::getSize()) / 2;
}

void init()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2i  windowPos;
	window.create(sf::VideoMode(width, height), "N-PUZZLE", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	updateDimensions();
	windowPos.x = static_cast<int>(desktop.width) / 2 - width / 2;
	windowPos.y = static_cast<int>(desktop.height) / 2 - (height - headerHeight - footerHeight) / 2;
	window.setPosition(windowPos);
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
	int      textSize;
	int      textWidth;
	int      textHeight;

	textSize = 0.70 * headerHeight;
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(bgColor);
	text.setString("N-PUZZLE");
	textWidth = text.getLocalBounds().width;
	textHeight = 1.15 * textSize;
	text.setPosition((width - textWidth) / 2, (headerHeight - textHeight) / 2);
	window.draw(text);
}

void drawFooter(int index, int indexMax)
{
	int footerY = height - footerHeight;

	sf::RectangleShape rectangle(sf::Vector2f(width, footerHeight));
	rectangle.setFillColor({ 50, 48, 49 });
	rectangle.setPosition(0, footerY);
	window.draw(rectangle);

	sf::Text text;
	int      textSize;
	int      textWidth;
	int      textHeight;
	bool     smallWidth = width < 400;

	textSize = 0.25 * footerHeight;
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setFillColor({ 255, 255, 255 });
	text.setString(catArgs("Speed ", speedIndex, smallWidth ? "\n" : "    ", "Puzzle ", index, "/", indexMax));
	textWidth = text.getLocalBounds().width;
	textHeight = 1.15 * text.getLocalBounds().height;
	text.setPosition((width - textWidth) / 2, (footerHeight - textHeight) / 2 + footerY);
	window.draw(text);
}

void drawTile(int x, int y, int nb)
{
	Position           tilePos;
	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	tile.setFillColor({ 96, 199, 144 });
	tile.setOutlineThickness(-5);
	tile.setOutlineColor(bgColor);
	tilePos.x = tileSize * x + offSetX;
	tilePos.y = tileSize * y + offSetY + headerHeight;
	tile.setPosition(tilePos.x, tilePos.y);
	window.draw(tile);

	sf::Text text;
	int      textSize;
	int      textWidth;
	int      textHeight;

	textSize = 0.70f * tileSize;
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(bgColor);
	text.setString(std::to_string(nb));
	textWidth = text.getLocalBounds().width;
	textHeight = 1.1 * textSize;
	text.setPosition(tilePos.x + (tileSize - 5 - textWidth) / 2, tilePos.y + (tileSize - 5 - textHeight) / 2);
	window.draw(text);
}

void renderFrame(const Puzzle &puzzle, int index, int indexMax)
{
	int size = Puzzle::getSize();

	drawHeader();
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int nb = puzzle.getAt(y, x);
			if (nb != 0)
				drawTile(x, y, nb);
		}
	}
	drawFooter(index, indexMax);
}

void handleKeyEvent(const sf::Event &event, int &index, int indexMax)
{
	if (event.key.code == sf::Keyboard::Space)
		pause = !pause;
	if (event.key.code == sf::Keyboard::Right && index < indexMax - 1)
		index++;
	if (event.key.code == sf::Keyboard::Left && index > 0)
		index--;
	if (event.key.code == sf::Keyboard::Up && speed > 100)
	{
		speed -= 100;
		speedIndex++;
	}
	if (event.key.code == sf::Keyboard::Down && speed < 1000)
	{
		speed += 100;
		speedIndex--;
	}
	if (event.key.code == sf::Keyboard::R)
	{
		index = 0;
		time1 = timeClock.getElapsedTime();
		time2 = timeClock.getElapsedTime();
	}
}

void handleWindowResizeEvent(const sf::Event &event)
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
	updateDimensions();
}

void render(const std::vector<Puzzle> &puzzles)
{
	init();
	sf::Event event;
	int       nbPuzzle = static_cast<int>(puzzles.size());
	int       index = 0;

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
				handleKeyEvent(event, index, nbPuzzle);
			if (event.type == sf::Event::Resized)
				handleWindowResizeEvent(event);
		}
		window.clear(bgColor);
		renderFrame(puzzles[index], index + 1, nbPuzzle);
		window.display();
		if (!pause)
		{
			time2 = timeClock.getElapsedTime();
			if (time2.asMilliseconds() - time1.asMilliseconds() >= speed && index < nbPuzzle - 1)
			{
				index++;
				time1 = timeClock.getElapsedTime();
			}
		}
	}
}
