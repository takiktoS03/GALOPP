#pragma once
/**
@file sfml.cpp
*/

#include "sfml.h"

void element_graficzny::serialize(std::ostringstream& oss) const
{
    oss << typ << " " << ksztalt.getPosition().x << " " << ksztalt.getPosition().y << " " << ksztalt.getRotation() << " " << wartosc << "\n";
}

void element_graficzny::deserialize(std::istringstream& iss)
{
    float rotation;
    sf::Vector2f position;
    iss >> position.x >> position.y >> rotation >> wartosc;
    ksztalt.setRotation(rotation);
    ksztalt.setPosition(position);
}

void linia::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_vertices, states);
}

linia::linia(sf::Color color1, sf::Color color2, float thickness)
{
    typ = 'W';
    _color1 = color1;
    _color2 = color2;
    _thickness = thickness;
    LeftHold = true;
}

void linia::setStartPoint(const sf::Vector2f& start) {
    leftNode = start;
}

void linia::setEndPoint(const sf::Vector2f& end) {
    rightNode = end;
    setPositions();
}

void linia::setPositions()
{
    sf::Vector2f direction = rightNode - leftNode;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x); // wektor prostopadly
    sf::Vector2f offset = (_thickness / 2.f) * unitPerpendicular; // nadanie grubosci linii

    _vertices = sf::VertexArray(sf::PrimitiveType::Quads, 4);
    _vertices[0] = sf::Vertex(leftNode + offset, _color1);
    _vertices[1] = sf::Vertex(rightNode + offset, _color2);
    _vertices[2] = sf::Vertex(rightNode - offset, _color2);
    _vertices[3] = sf::Vertex(leftNode - offset, _color1);
}

void linia::changeLeftColor(const sf::Color& color)
{
    _color1 = color;
}

void linia::changeRightColor(const sf::Color& color)
{
    _color2 = color;
}

bool linia::mouse_over(sf::Vector2f& pozycja) const
{
    return _vertices.getBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> linia::clone() const
{
    return std::make_shared<linia>(sf::Color(100, 107, 120), sf::Color(100, 107, 120), 4);
}

void linia::serialize(std::ostringstream& oss) const
{
    oss << typ << " " << leftNode.x << " " << leftNode.y << " "
        << rightNode.x << " " << rightNode.y << " " << "\n";
}

void linia::deserialize(std::istringstream& iss)
{
    iss >> leftNode.x >> leftNode.y >> rightNode.x >> rightNode.y;
    setPositions();
}

void rezystor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

rezystor::rezystor(const sf::Color& color)
{
    typ = 'R';
    wartosc = 0;
    texture.loadFromFile("../GALOPP/images/rezystor.png");
    ksztalt.setSize(sf::Vector2f(90, 35));
    ksztalt.setTexture(&texture);
    ksztalt.setFillColor(color);
}

bool rezystor::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> rezystor::clone() const
{
    return std::make_shared<rezystor>(*this);
}

void pole_robocze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

pole_robocze::pole_robocze(double width, double height)
{
    typ = 'P';
    ksztalt.setSize(sf::Vector2f(width, height));
    ksztalt.setFillColor(sf::Color::Transparent);
}

bool pole_robocze::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> pole_robocze::clone() const
{
    return nullptr;
}

void zrodlo_sem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

zrodlo_sem::zrodlo_sem(const sf::Color& color)
{
    typ = 'E';
    wartosc = 0;
    fi = 0;
    texture.loadFromFile("../GALOPP/images/zrodlo_sem.png");
    ksztalt.setSize(sf::Vector2f(90, 61));
    ksztalt.setTexture(&texture);
    ksztalt.setFillColor(color);
}

bool zrodlo_sem::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);

}

std::shared_ptr<element_graficzny> zrodlo_sem::clone() const
{
    return std::make_shared<zrodlo_sem>(*this);
}

void zrodlo_sem::serialize(std::ostringstream& oss) const
{
    oss << typ << " " << ksztalt.getPosition().x << " " << ksztalt.getPosition().y << " " << ksztalt.getRotation() << " " << fi << " " << wartosc << "\n";
}

void zrodlo_sem::deserialize(std::istringstream& iss)
{
    float rotation;
    sf::Vector2f position;
    iss >> position.x >> position.y >> rotation >> fi >> wartosc;
    ksztalt.setRotation(rotation);
    ksztalt.setPosition(position);
}

void zrodlo_spm::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

zrodlo_spm::zrodlo_spm(const sf::Color& color)
{
    typ = 'I';
    wartosc = 0;
    fi = 0;
    texture.loadFromFile("../GALOPP/images/zrodlo_spm.png");
    ksztalt.setSize(sf::Vector2f(90, 61));
    ksztalt.setTexture(&texture);
    ksztalt.setFillColor(color);
}

bool zrodlo_spm::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> zrodlo_spm::clone() const
{
    return std::make_shared<zrodlo_spm>(*this);
}

void zrodlo_spm::serialize(std::ostringstream& oss) const
{
    oss << typ << " " << ksztalt.getPosition().x << " " << ksztalt.getPosition().y << " " << ksztalt.getRotation() << " " << fi << " " << wartosc << "\n";
}

void zrodlo_spm::deserialize(std::istringstream& iss)
{
    float rotation;
    sf::Vector2f position;
    iss >> position.x >> position.y >> rotation >> fi >> wartosc;
    ksztalt.setRotation(rotation);
    ksztalt.setPosition(position);
}

void kondensator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

kondensator::kondensator(const sf::Color& color)
{
    typ = 'C';
    wartosc = 0;
    texture.loadFromFile("../GALOPP/images/kondensator.png");
    ksztalt.setSize(sf::Vector2f(90, 35));
    ksztalt.setTexture(&texture);
    ksztalt.setFillColor(color);
}

bool kondensator::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> kondensator::clone() const
{
    return std::make_shared<kondensator>(*this);
}

void cewka::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(ksztalt, states);
}

cewka::cewka(const sf::Color& color)
{
    typ = 'L';
    wartosc = 0;
    texture.loadFromFile("../GALOPP/images/cewka.png");
    ksztalt.setSize(sf::Vector2f(90, 35));
    ksztalt.setTexture(&texture);
    ksztalt.setFillColor(color);
}

bool cewka::mouse_over(sf::Vector2f& pozycja) const
{
    return ksztalt.getGlobalBounds().contains(pozycja);
}

std::shared_ptr<element_graficzny> cewka::clone() const
{
    return std::make_shared<cewka>(*this);
}
