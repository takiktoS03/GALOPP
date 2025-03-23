#pragma once
/**
@file sfml.h
*/

#define _USE_MATH_DEFINES

//#include <thread>
#include <iostream>
#include <sstream>
#include <SFML\Graphics.hpp>

class element_graficzny : public sf::Drawable
{
public:
    double wartosc;
    char typ;
    int id;
    sf::RectangleShape ksztalt;
    sf::Texture texture;
    sf::Vector2f leftNode, rightNode;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    virtual std::shared_ptr<element_graficzny> clone() const = 0;
    virtual bool mouse_over(sf::Vector2f& pozycja) const = 0;
    // Metoda serializacji
    virtual void serialize(std::ostringstream& oss) const;
    // Metoda deserializacji
    virtual void deserialize(std::istringstream& iss);
    virtual ~element_graficzny() {};
};

class linia : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    //sf::VertexArray _vertices;
    float _thickness;
    sf::Color _color1;
    sf::Color _color2;
public:
    sf::VertexArray _vertices;
    bool LeftHold;
    linia(sf::Color color1, sf::Color color2, float thickness);
    void setStartPoint(const sf::Vector2f& start);
    void setEndPoint(const sf::Vector2f& end);
    void setPositions();
    void changeLeftColor(const sf::Color& color);
    void changeRightColor(const sf::Color& color);
    bool mouse_over(sf::Vector2f& pozycja) const;
    std::shared_ptr<element_graficzny> clone() const override;
    void serialize(std::ostringstream& oss) const override;
    void deserialize(std::istringstream& iss) override;
};

class rezystor : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    rezystor(const sf::Color& color);
    bool mouse_over(sf::Vector2f& pozycja) const;
    std::shared_ptr<element_graficzny> clone() const override;
};

class pole_robocze : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    pole_robocze(double width, double height);
    bool mouse_over(sf::Vector2f& pozycja) const;
    std::shared_ptr<element_graficzny> clone() const override;
};

class zrodlo_sem : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    zrodlo_sem(const sf::Color& color);
    double fi;
    bool mouse_over(sf::Vector2f& pozycja) const;
    void serialize(std::ostringstream& oss) const;
    void deserialize(std::istringstream& iss);
    std::shared_ptr<element_graficzny> clone() const override;
};

class zrodlo_spm : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    zrodlo_spm(const sf::Color& color);
    double fi;
    bool mouse_over(sf::Vector2f& pozycja) const;
    void serialize(std::ostringstream& oss) const;
    void deserialize(std::istringstream& iss);
    std::shared_ptr<element_graficzny> clone() const override;
};

class kondensator : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    kondensator(const sf::Color& color);
    bool mouse_over(sf::Vector2f& pozycja) const;
    std::shared_ptr<element_graficzny> clone() const override;
};

class cewka : public element_graficzny
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    cewka(const sf::Color& color);
    bool mouse_over(sf::Vector2f& pozycja) const;
    std::shared_ptr<element_graficzny> clone() const override;
};