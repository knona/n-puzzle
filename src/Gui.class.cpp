#include "Gui.class.hpp"

Gui::Gui()
{
	_width = 700;
	_height = 750;
	_bgColor = sf::Color({ 255, 255, 255 });
	_pause = false;
	_speed = 1000;
	_speedIndex = 1;
	_time1 = _timeClock.getElapsedTime();
	_time2 = _timeClock.getElapsedTime();
}

Gui::~Gui()
{}

void Gui::updateDimensions()
{
	_headerHeight = std::max(50.0, 0.09 * _height);
	_footerHeight = std::max(70.0, 0.15 * _height);
	_tileSize = (std::min(_width, _height - _headerHeight - _footerHeight) - 50) / Puzzle::getSize();
	_offSetX = (_width - _tileSize * Puzzle::getSize()) / 2;
	_offSetY = (_height - _headerHeight - _footerHeight - _tileSize * Puzzle::getSize()) / 2;
}

void Gui::init()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2i  windowPos;
	_window.create(sf::VideoMode(_width, _height), "N-PUZZLE",
	               sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	this->updateDimensions();
	windowPos.x = static_cast<int>(desktop.width) / 2 - _width / 2;
	windowPos.y = static_cast<int>(desktop.height) / 2 - (_height - _headerHeight - _footerHeight) / 2;
	_window.setPosition(windowPos);
	if (!_font.loadFromFile("resources/font/modular.otf"))
		throw std::runtime_error("Cannot open the font");
}

void Gui::drawHeader()
{
	sf::RectangleShape rectangle(sf::Vector2f(_width, _headerHeight));
	rectangle.setFillColor({ 50, 48, 49 });
	rectangle.setPosition(0, 0);
	_window.draw(rectangle);

	sf::Text text;
	int      textSize;
	int      textWidth;
	int      textHeight;

	textSize = 0.70 * _headerHeight;
	text.setFont(_font);
	text.setCharacterSize(textSize);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(_bgColor);
	text.setString("N-PUZZLE");
	textWidth = text.getLocalBounds().width;
	textHeight = 1.15 * textSize;
	text.setPosition((_width - textWidth) / 2, (_headerHeight - textHeight) / 2);
	_window.draw(text);
}

void Gui::drawFooter(int index, int indexMax)
{
	int footerY = _height - _footerHeight;

	sf::RectangleShape rectangle(sf::Vector2f(_width, _footerHeight));
	rectangle.setFillColor({ 50, 48, 49 });
	rectangle.setPosition(0, footerY);
	_window.draw(rectangle);

	sf::Text text;
	int      textSize;
	int      textWidth;
	int      textHeight;
	bool     smallWidth = _width < 400;

	textSize = 0.25 * _footerHeight;
	text.setFont(_font);
	text.setCharacterSize(textSize);
	text.setFillColor({ 255, 255, 255 });
	text.setString(catArgs("Speed ", _speedIndex, smallWidth ? "\n" : "    ", "Puzzle ", index, "/", indexMax));
	textWidth = text.getLocalBounds().width;
	textHeight = 1.15 * text.getLocalBounds().height;
	text.setPosition((_width - textWidth) / 2, (_footerHeight - textHeight) / 2 + footerY);
	_window.draw(text);
}

void Gui::drawTile(int x, int y, int nb)
{
	Position           tilePos;
	sf::RectangleShape tile(sf::Vector2f(_tileSize, _tileSize));
	tile.setFillColor({ 96, 199, 144 });
	tile.setOutlineThickness(-5);
	tile.setOutlineColor(_bgColor);
	tilePos.x = _tileSize * x + _offSetX;
	tilePos.y = _tileSize * y + _offSetY + _headerHeight;
	tile.setPosition(tilePos.x, tilePos.y);
	_window.draw(tile);

	sf::Text text;
	int      textSize;
	int      textWidth;
	int      textHeight;

	textSize = 0.70f * _tileSize;
	text.setFont(_font);
	text.setCharacterSize(textSize);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(_bgColor);
	text.setString(std::to_string(nb));
	textWidth = text.getLocalBounds().width;
	textHeight = 1.1 * textSize;
	text.setPosition(tilePos.x + (_tileSize - 5 - textWidth) / 2, tilePos.y + (_tileSize - 5 - textHeight) / 2);
	_window.draw(text);
}

void Gui::renderFrame(const Puzzle &puzzle, int index, int indexMax)
{
	int size = Puzzle::getSize();

	this->drawHeader();
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int nb = puzzle.getAt(y, x);
			if (nb != 0)
				this->drawTile(x, y, nb);
		}
	}
	this->drawFooter(index, indexMax);
}

void Gui::handleKeyEvent(const sf::Event &event, int &index, int indexMax)
{
	if (event.key.code == sf::Keyboard::Space)
		_pause = !_pause;
	if (event.key.code == sf::Keyboard::Right && index < indexMax - 1)
		index++;
	if (event.key.code == sf::Keyboard::Left && index > 0)
		index--;
	if (event.key.code == sf::Keyboard::Up && _speed > 100)
	{
		_speed -= 100;
		_speedIndex++;
	}
	if (event.key.code == sf::Keyboard::Down && _speed < 1000)
	{
		_speed += 100;
		_speedIndex--;
	}
	if (event.key.code == sf::Keyboard::R)
	{
		index = 0;
		_time1 = _timeClock.getElapsedTime();
		_time2 = _timeClock.getElapsedTime();
	}
}

void Gui::handleWindowResizeEvent(const sf::Event &event)
{
	_width = event.size.width;
	_height = event.size.height;
	if (_width < 300 && _height < 400)
		_window.setSize(sf::Vector2u(300, 400));
	else if (_width < 300)
		_window.setSize(sf::Vector2u(300, _height));
	else if (_height < 400)
		_window.setSize(sf::Vector2u(_width, 400));

	sf::FloatRect visibleArea(0, 0, _width, _height);
	_window.setView(sf::View(visibleArea));
	this->updateDimensions();
}

void Gui::render(const std::vector<Puzzle> &puzzles)
{
	sf::Event event;
	int       nbPuzzle = static_cast<int>(puzzles.size());
	int       index = 0;

	while (_window.isOpen())
	{
		while (_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
			    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				_window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed)
				this->handleKeyEvent(event, index, nbPuzzle);
			if (event.type == sf::Event::Resized)
				this->handleWindowResizeEvent(event);
		}
		_window.clear(_bgColor);
		this->renderFrame(puzzles[index], index + 1, nbPuzzle);
		_window.display();
		if (!_pause)
		{
			_time2 = _timeClock.getElapsedTime();
			if (_time2.asMilliseconds() - _time1.asMilliseconds() >= _speed && index < nbPuzzle - 1)
			{
				index++;
				_time1 = _timeClock.getElapsedTime();
			}
		}
	}
}
