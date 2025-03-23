#include <iostream>
#include <sstream>
#include <regex>
#include <SFML/Graphics.hpp>

#include "Textbox.h"

void Textbox::deleteLastChar()
{
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++)
	{
		newT += t[i];
	}
	text.str("");
	text << newT;
	textbox.setString(text.str() + "_");
}

void Textbox::inputLogic(int charTyped)
{
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
	{
		text << static_cast<char>(charTyped);
	}
	else if (charTyped == DELETE_KEY) 
	{
		if (text.str().length() > 0)
			deleteLastChar();
	}
	textbox.setString(text.str() + "_");
}

Textbox::Textbox(int size, sf::Color color, bool sel) {
	textbox.setCharacterSize(size);
	textbox.setFillColor(color);
	isSelected = sel;

	// Wybranie textboxu doda '_' dla informacji
	if (isSelected)
		textbox.setString("_");
	else
		textbox.setString("");
}

void Textbox::setFont(sf::Font& fonts) 
{
	textbox.setFont(fonts);
}

void Textbox::setPosition(sf::Vector2f point) 
{
	textbox.setPosition(point);
}

void Textbox::setString(double wartosc)
{
	text.str("");
	textbox.setString("");
	if (!std::isnan(wartosc))
	{
		text << wartosc;
		textbox.setString(text.str());
	}
	else
		textbox.setString(text.str());
}

void Textbox::setLimit(bool ToF) {
	hasLimit = ToF;
}

void Textbox::setLimit(bool ToF, int lim) {
	hasLimit = ToF;
	limit = lim - 1;
}

void Textbox::setSelected(bool sel) {
	isSelected = sel;
	// Dodawanie znaku '_' na koncu zaleznie od wybrania
	std::string newText = getText();
	if (sel)
		newText.append("_");
	else if (!newText.empty() && newText.back() == '_')
		newText.pop_back();
	textbox.setString(newText);
}

std::string Textbox::getText()
{
	return text.str();
}

void Textbox::drawTo(sf::RenderWindow& window) const
{
	window.draw(textbox);
}

// Funkcja do wpisywania podczas petli
void Textbox::typedOn(sf::Event input) 
{
	if (isSelected) 
	{
		int charTyped = input.text.unicode;
		std::regex digits("[0-9.0-9]");
		std::regex singleLetter("[Mkmun]");
		std::string charStr(1, static_cast<char>(charTyped));
		if (std::regex_match(charStr, digits) || charTyped == DELETE_KEY ||
			(std::regex_match(charStr, singleLetter) && !endsWithLetter()))
		{
			if (hasLimit) 
			{
				if (getText().length() <= limit)
					inputLogic(charTyped);
				// mozliwosc usuwania znakow
				else if (getText().length() > limit && charTyped == DELETE_KEY)
					deleteLastChar();
			}
			else
				inputLogic(charTyped);
		}
	}
}

bool Textbox::endsWithLetter()
{
	std::string currentText = text.str();
	if (currentText.empty()) return false;

	char lastChar = currentText.back();
	return std::regex_match(std::string(1, lastChar), std::regex("[Mkmun]"));
}

double Textbox::convertToValue()
{
	std::string str = getText();
	double value = 0.0;

	if (str.empty())
		return 0;
	if (endsWithLetter())
	{
		char lastChar = str.back();
		str.pop_back(); // usuwanie ostatnie znaku przed konwersja
		double baseValue = std::stod(str); // string na double

		switch (lastChar)
		{
		case 'M': value = baseValue * pow(10, 6); break;
		case 'k': value = baseValue * pow(10, 3); break;
		case 'm': value = baseValue * pow(10, -3); break;
		case 'u': value = baseValue * pow(10, -6); break;
		case 'n': value = baseValue * pow(10, -9); break;
		default: value = baseValue; break;
		}
	}
	else
		value = std::stod(str);
	return value;
}

void Textbox::appendHz()
{
	std::string str = getText();
	str.append("H");
	str.append("z");
	textbox.setString(str);
}
