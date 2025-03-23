#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>

class Button 
{
private:
	sf::RectangleShape button;
	sf::Text text;

	float btnWidth;
	float btnHeight;

	std::function<void()> onClick;
public:
	Button();

	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor);

	void setFont(sf::Font& font);

	void setPosition(sf::Vector2f point);

	sf::Vector2f getPosition() const;

	void drawTo(sf::RenderWindow& window) const;

	bool isMouseOver(sf::Vector2f& point);

	void hover(sf::Color back_color, sf::Color text_color);

	void setOnClick(std::function<void()> callback);

	void click();
};