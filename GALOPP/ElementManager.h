#pragma once
/**
@file ElementManager.h
*/

#include "sfml.h"
#include <fstream>
#include <string>

class ElementManager
{
private:
    std::vector<std::shared_ptr<element_graficzny>> elementy_wizualne;
    float odleglosc(const sf::Vector2f& point1, const sf::Vector2f& point2);
    int newId;
    double frequency;
public:
    ElementManager(): newId(1), frequency(0) {};

    double getFrequency() const;

    void setFrequency(double freq);

    void dodaj_menu(int& width, int& height);

    int generateUniqueId();

    void dodajElement(std::shared_ptr<element_graficzny> element);

    void usunElement(std::shared_ptr<element_graficzny> element);

    void usunElementy();

    std::vector<std::shared_ptr<element_graficzny>> getElementyWizualne();

    bool czyWewnatrz(std::shared_ptr<element_graficzny> element);

    bool moznaUstawic(std::shared_ptr<element_graficzny> element);

    void ustaw_wezly(std::shared_ptr<element_graficzny> elem);

    void przystaw_element(std::shared_ptr<element_graficzny> element, const sf::Vector2f& myszka);

    void rysujElementy(sf::RenderWindow& window);

    void ustawWezly();

    std::shared_ptr<element_graficzny> wybrano_obiekt(sf::Vector2f myszka);

    int getElemId(sf::Vector2f myszka) const;

    void saveElementsToFile(const std::string& filename);

    void loadElementsFromFile(const std::string& filename, ElementManager& menu_elementow, int width, int height);

    ~ElementManager() {};
};