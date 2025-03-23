#include <iostream>
#include <SFML/Graphics.hpp>

#include "Button.h"

Button::Button() {

}

Button::Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
	button.setSize(buttonSize);
	button.setFillColor(bgColor);
	button.setOutlineThickness(3);
	button.setOutlineColor(textColor);

	// Get these for later use:
	btnWidth = buttonSize.x;
	btnHeight = buttonSize.y;

	text.setString(btnText);
	text.setCharacterSize(charSize);
	text.setFillColor(textColor);
}

void Button::setFont(sf::Font& font) {
	text.setFont(font);
}

void Button::setPosition(sf::Vector2f point) {
	button.setPosition(point);

	// Center text on button
	float xPos = point.x + (btnWidth - text.getLocalBounds().width) / 2;
	float yPos = point.y + (btnHeight - text.getLocalBounds().height) / 2 - text.getLocalBounds().top;

	text.setPosition(xPos, yPos);
}

sf::Vector2f Button::getPosition() const
{
	return button.getPosition();
}

void Button::drawTo(sf::RenderWindow& window) const 
{
	window.draw(button);
	window.draw(text);
}

bool Button::isMouseOver(sf::Vector2f& point)
{
	return button.getGlobalBounds().contains(point);
}

void Button::hover(sf::Color back_color, sf::Color text_color)
{
	button.setFillColor(back_color);
	text.setFillColor(text_color);
}

void Button::setOnClick(std::function<void()> callback)
{
	onClick = callback;
}

void Button::click()
{
	if (onClick) {
		onClick();
	}
}