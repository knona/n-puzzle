#ifndef GUI_HPP
#define GUI_HPP

#include "Position.struct.hpp"
#include "Puzzle.class.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Gui
{
	private:
	sf::RenderWindow _window;
	sf::Font         _font;
	sf::Color        _bgColor;

	int _width;
	int _height;
	int _offSetX;
	int _offSetY;
	int _tileSize;
	int _headerHeight;
	int _footerHeight;

	bool _pause;
	int  _speed;
	int  _speedIndex;

	sf::Clock _timeClock;
	sf::Time  _time1;
	sf::Time  _time2;

	void displayCommands() const;
	void updateDimensions();
	void drawHeader();
	void drawFooter(int index, int indexMax);
	void drawTile(int x, int y, int nb);
	void renderFrame(const Puzzle &puzzle, int index, int indexMax);
	void handleKeyEvent(const sf::Event &event, int &index, int indexMax);
	void handleWindowResizeEvent(const sf::Event &event);

	public:
	Gui();
	Gui(const Gui &) = delete;
	Gui &operator=(const Gui &) = delete;
	Gui(Gui &&) = delete;
	Gui &operator=(Gui &&) = delete;
	~Gui();

	void init();
	void render(const std::vector<Puzzle> &puzzles);
};

#endif // !GUI_HPP
