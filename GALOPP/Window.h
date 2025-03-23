#pragma once
#include "sfml.h"

class Window : public sf::RenderWindow
{
private:
	sf::Font _font;
	int width, height;
	std::vector<sf::RectangleShape> baners;
	std::vector<sf::Text> texts;
	//std::vector<Textbox> inputs;
	//std::vector<Button> buttons;
public:
	Window(sf::VideoMode mode, const sf::String& title);

	sf::Color tlo = sf::Color(3, 20, 24, 100);
	sf::Color neon_blue = sf::Color(68, 214, 224);
	sf::Color green = sf::Color(21, 207, 51);
	sf::Color dark_blue = sf::Color(9, 56, 62);

	sf::Font& get_font();

	void centerText(sf::Text& text);

	void stworz_tlo();

	void rysuj_banery();

	void rysuj_teksty();

	void rysuj_okno_dialogowe();

	void rysuj_wartosci();

	//void dodaj_przyciski();

	//void dodaj_inputy();
};