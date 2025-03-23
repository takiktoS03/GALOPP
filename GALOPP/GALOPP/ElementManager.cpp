#pragma once
/**
@file ElementManager.cpp
*/

#include "ElementManager.h"


double ElementManager::getFrequency() const
{
    return frequency;
}

void ElementManager::setFrequency(double freq)
{
    frequency = freq;
}

void ElementManager::dodaj_menu(int& width, int& height)
{
    sf::Color green(21, 207, 51);
    // Pole robocze, na ktorym mozna rysowac elementy
    std::shared_ptr<pole_robocze> pole = std::make_shared<pole_robocze>(width * 0.83 + 3, height - 56);
    pole->ksztalt.setPosition(0, 56);
    dodajElement(pole);

    // Linia na panelu menu
    std::shared_ptr<linia> line = std::make_shared<linia>(green, green, 5);
    line->setStartPoint(sf::Vector2f(width - 133, height * 0.20));
    line->setEndPoint(sf::Vector2f(width - 43, height * 0.20));
    line->setPositions();
    dodajElement(line);

    // Rezystor na panelu menu
    std::shared_ptr<rezystor> rez = std::make_shared<rezystor>(green); //width * 0.07, height * 0.04,
    rez->ksztalt.setPosition(width - 133, height * 0.30);
    dodajElement(rez);

    // Zrodlo sem na panelu menu
    std::shared_ptr<zrodlo_sem> sem = std::make_shared<zrodlo_sem>(green);
    sem->ksztalt.setPosition(width - 133, height * 0.42);
    dodajElement(sem);

    // Zrodlo spm na panelu menu
    std::shared_ptr<zrodlo_spm> spm = std::make_shared<zrodlo_spm>(green);
    spm->ksztalt.setPosition(width - 133, height * 0.56);
    dodajElement(spm);

    // Kondensator na panelu menu
    std::shared_ptr<kondensator> kond = std::make_shared<kondensator>(green);
    kond->ksztalt.setPosition(width - 133, height * 0.70);
    dodajElement(kond);

    // Cewka na panelu menu
    std::shared_ptr<cewka> cew = std::make_shared<cewka>(green);
    cew->ksztalt.setPosition(width - 133, height * 0.81);
    dodajElement(cew);
}

int ElementManager::generateUniqueId()
{
    return newId++;
}

void ElementManager::dodajElement(std::shared_ptr<element_graficzny> element)
{
    if (element->typ != 'W')
        element->id = generateUniqueId();
    elementy_wizualne.push_back(element);
}

void ElementManager::usunElement(std::shared_ptr<element_graficzny> element)
{
    elementy_wizualne.erase(
        std::remove_if(elementy_wizualne.begin(), elementy_wizualne.end(),
            [&element](const std::shared_ptr<element_graficzny>& e) { return e == element; }),
        elementy_wizualne.end()
    );
}

void ElementManager::usunElementy()
{
    elementy_wizualne.clear();
}

std::vector<std::shared_ptr<element_graficzny>> ElementManager::getElementyWizualne()
{
    return elementy_wizualne;
}

bool ElementManager::czyWewnatrz(std::shared_ptr<element_graficzny> element)
{
    sf::FloatRect pole_robocze = elementy_wizualne[0]->ksztalt.getGlobalBounds();
    sf::FloatRect element_klikniety;
    if (std::dynamic_pointer_cast<linia>(element) != nullptr)
    {
        element_klikniety = std::dynamic_pointer_cast<linia>(element)->_vertices.getBounds();
    }
    else
        element_klikniety = element->ksztalt.getGlobalBounds();

    return (element_klikniety.left >= pole_robocze.left) &&
        (element_klikniety.top >= pole_robocze.top) &&
        ((element_klikniety.left + element_klikniety.width) <= (pole_robocze.left + pole_robocze.width)) &&
        ((element_klikniety.top + element_klikniety.height) <= (pole_robocze.top + pole_robocze.height));
}

bool ElementManager::moznaUstawic(std::shared_ptr<element_graficzny> element)
{
    sf::FloatRect element_bounds = element->ksztalt.getGlobalBounds();
    for (auto& el : elementy_wizualne)
    {
        if (element_bounds.intersects(el->ksztalt.getGlobalBounds()))
        {
            return false;
        }
    }
    return true;
}

void ElementManager::ustaw_wezly(std::shared_ptr<element_graficzny> elem)
{
    if (std::dynamic_pointer_cast<linia>(elem) == nullptr)
    {
        sf::FloatRect bounds = elem->ksztalt.getLocalBounds();
        elem->leftNode = elem->ksztalt.getTransform().transformPoint(bounds.left, bounds.top + bounds.height / 2);
        elem->rightNode = elem->ksztalt.getTransform().transformPoint(bounds.left + bounds.width, bounds.top + bounds.height / 2);
    }
}

float ElementManager::odleglosc(const sf::Vector2f& myszka, const sf::Vector2f& wezel)
{
    return sqrt(pow(myszka.x - wezel.x, 2) + pow(myszka.y - wezel.y, 2));
}

void ElementManager::przystaw_element(std::shared_ptr<element_graficzny> element, const sf::Vector2f& myszka)
{
    float snapDistance = 20.0f; // Odleg³oœæ, przy której elementy siê przystawiaj¹
    ustaw_wezly(element);
    for (auto& elem : elementy_wizualne)
    {
        ustaw_wezly(elem);
        // Przystawianie elementu
        if (std::dynamic_pointer_cast<linia>(element) != nullptr && std::dynamic_pointer_cast<linia>(element)->LeftHold)
        {
            if (odleglosc(myszka, elem->leftNode) < snapDistance)
            {
                std::dynamic_pointer_cast<linia>(element)->setStartPoint(elem->leftNode);
                return;
            }
            else if (odleglosc(myszka, elem->rightNode) < snapDistance)
            {
                std::dynamic_pointer_cast<linia>(element)->setStartPoint(elem->rightNode);
                return;
            }
        }
        else if (std::dynamic_pointer_cast<linia>(element) != nullptr)
        {
            if (odleglosc(myszka, elem->leftNode) < snapDistance)
            {
                std::dynamic_pointer_cast<linia>(element)->setEndPoint(elem->leftNode);
                return;
            }
            else if (odleglosc(myszka, elem->rightNode) < snapDistance)
            {
                std::dynamic_pointer_cast<linia>(element)->setEndPoint(elem->rightNode);
                return;
            }
        }
        else
        {
            if (odleglosc(myszka, elem->leftNode) < snapDistance)
            {
                element->ksztalt.setPosition(elem->leftNode - (element->leftNode - element->ksztalt.getPosition()));
                return;
            }
            else if (odleglosc(myszka, elem->rightNode) < snapDistance)
            {
                element->ksztalt.setPosition(elem->rightNode - (element->rightNode - element->ksztalt.getPosition()));
                return;
            }
        }
    }
}

void ElementManager::rysujElementy(sf::RenderWindow& window)
{
    if (window.isOpen())
    {
        for (auto& element : elementy_wizualne)
        {
            window.draw(*element);
        }
    }
}

void ElementManager::ustawWezly()
{
    if (elementy_wizualne.empty())
        return;
    for (auto& element : elementy_wizualne)
    {
        ustaw_wezly(element);
    }
}

std::shared_ptr<element_graficzny> ElementManager::wybrano_obiekt(sf::Vector2f myszka)
{
    for (auto& element : elementy_wizualne)
    {
        if (element->mouse_over(myszka))
        {
            return element;
        }
    }
    return nullptr; // Zwrócenie nullptr, jeœli ¿aden element nie zosta³ klikniêty
}

int ElementManager::getElemId(sf::Vector2f myszka) const
{
    for (auto& element : elementy_wizualne)
    {
        if (element->typ != 'W' && element->mouse_over(myszka))
        {
            return element->id;
        }
    }
    return 0;
}

void ElementManager::saveElementsToFile(const std::string& filename)
{
    std::ofstream ofs(filename);
    ofs << frequency << "\n";
    std::ostringstream oss;
    for (const auto& element : elementy_wizualne)
    {
        element->serialize(oss);
    }

    ofs << oss.str();
    ofs.close();
}

void ElementManager::loadElementsFromFile(const std::string& filename, ElementManager& menu_elementow, int width, int height)
{
    std::ifstream ifs(filename);
    ifs >> frequency;
    usunElementy();
    std::string line;
    while (std::getline(ifs, line))
    {
        std::istringstream iss(line);
        char typ;
        iss >> typ;
        std::shared_ptr<element_graficzny> element;
        if (typ == 'W')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.20 + 1))->clone();
        }
        else if (typ == 'R')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.32))->clone();
        }
        else if (typ == 'E')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.42))->clone();
        }
        else if (typ == 'I')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.57))->clone();
        }
        else if (typ == 'C')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.72))->clone();
        }
        else if (typ == 'L')
        {
            element = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.82))->clone();
        }

        if (element)
        {
            element->deserialize(iss);
            dodajElement(element);
        }
    }
    ifs.close();
}
