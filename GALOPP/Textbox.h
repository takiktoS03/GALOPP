#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

// Define keys:
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox
{
private:
	sf::Text textbox;

	std::ostringstream text;
	bool hasLimit = false;
	int limit = 0;

	void deleteLastChar();

	void inputLogic(int charTyped);
public:
	Textbox(int size, sf::Color color, bool sel);

	void setFont(sf::Font& fonts);

	void setPosition(sf::Vector2f point);

	void setString(double wartosc);

	void setLimit(bool ToF);

	void setLimit(bool ToF, int lim);

	bool isSelected = false;

	void setSelected(bool sel);

	std::string getText();

	void drawTo(sf::RenderWindow& window) const;

	void typedOn(sf::Event input);

	bool endsWithLetter();

	double convertToValue();

	void appendHz();
};