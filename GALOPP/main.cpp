/**
@file main.cpp
*/
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <filesystem>

#include "Analiza.h"
#include "Obwod.h"
#include "sfml.h"
#include "ElementManager.h"
#include "Window.h"
#include "Textbox.h"
#include "Button.h"

import toString;

int main()
{
    Window window(sf::VideoMode(1024, 768), "GALOPP"); /// sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    //window.setKeyRepeatEnabled(false);
    //window.setActive(false);
    //sf::Thread tlo(wyswietl_tlo, &window);
    //tlo.launch();
    //sf::Thread rysuj(&ElementManager::rysujElementy, &menu_elementow);
    ////menu_elementow.rysujElementy(&window);
    //tlo.wait();
    //rysuj.launch();
    //rysuj.wait();

    int width = window.getSize().x;
    int height = window.getSize().y;
    // Przycisk do symulowania
    Button sym_button("SYMULUJ", sf::Vector2f(150, 30), 20, window.dark_blue, window.neon_blue);
    sym_button.setFont(window.get_font());
    sym_button.setPosition(sf::Vector2f(width - 166, height * 0.92 - 3));

    // Przycisk potwierdzania
    Button ok_button("OK", sf::Vector2f(50, 25), 20, window.dark_blue, window.neon_blue);
    ok_button.setFont(window.get_font());
    ok_button.setPosition(sf::Vector2f(width / 3.5 + 225, height / 2.5 + 150));

    // Przycisk anulowania
    Button cancel_button("ANULUJ", sf::Vector2f(90, 25), 20, window.dark_blue, window.neon_blue);
    cancel_button.setFont(window.get_font());
    cancel_button.setPosition(sf::Vector2f(width / 3.5 + 50, height / 2.5 + 150));

    // Przycisk plik do otwierania menu
    Button plik("Plik", sf::Vector2f(80, 50), 20, window.dark_blue, window.neon_blue);
    plik.setFont(window.get_font());
    plik.setPosition(sf::Vector2f(0, 3));

    // Przycisk zapisywania wynikow
    Button eksport_wynikow("Zapisz wynik", sf::Vector2f(140, 25), 18, window.dark_blue, window.neon_blue);
    eksport_wynikow.setFont(window.get_font());
    eksport_wynikow.setPosition(sf::Vector2f(0, 56));

    // Przycisk zapisywania obwodu
    Button save("Zapisz", sf::Vector2f(140, 25), 18, window.dark_blue, window.neon_blue);
    save.setFont(window.get_font());
    save.setPosition(sf::Vector2f(0, 84));

    // Przycisk odczytywania obwodu
    Button open("Otwórz", sf::Vector2f(140, 25), 18, window.dark_blue, window.neon_blue);
    open.setFont(window.get_font());
    open.setPosition(sf::Vector2f(0, 112));

    Textbox wartosc(16, window.neon_blue, true);
    wartosc.setFont(window.get_font());
    wartosc.setLimit(true, 9);
    wartosc.setPosition(sf::Vector2f(width / 3.5 + 130, height / 2.5 + 65));

    sf::RectangleShape input_value(sf::Vector2f(100, 18));
    input_value.setFillColor(window.tlo);
    input_value.setPosition(sf::Vector2f(width / 3.5, height / 2.5) + sf::Vector2f(120, 65));

    Textbox przesuniecie(16, window.neon_blue, false);
    przesuniecie.setFont(window.get_font());
    przesuniecie.setLimit(true, 9);
    przesuniecie.setPosition(sf::Vector2f(width / 3.5 + 130, height / 2.5 + 95));

    sf::Text text_fi(L"K¹t fazowy", window.get_font());
    text_fi.setFillColor(window.green);
    text_fi.setCharacterSize(16);
    text_fi.setPosition(sf::Vector2f(width / 3.5, height / 2.5) + sf::Vector2f(20, 93));

    sf::RectangleShape input_fi(sf::Vector2f(100, 18));
    input_fi.setFillColor(window.tlo);
    input_fi.setPosition(sf::Vector2f(width / 3.5, height / 2.5) + sf::Vector2f(120, 95));

    Textbox czestotliwosc(16, window.neon_blue, false);
    czestotliwosc.setFont(window.get_font());
    czestotliwosc.setLimit(true, 7);
    czestotliwosc.setPosition(sf::Vector2f(width - 128, height * 0.13 + 20));

    sf::RectangleShape input_freq(sf::Vector2f(90, 18));
    input_freq.setFillColor(window.tlo);
    input_freq.setPosition(sf::Vector2f(width - 133, height * 0.13 + 18));

    sf::Text value(L"0", window.get_font());
    value.setFillColor(window.green);
    value.setCharacterSize(16);
    value.setPosition(sf::Vector2f(160, height - 180));

    sf::Text moc_czynna(L"0", window.get_font());
    moc_czynna.setFillColor(window.green);
    moc_czynna.setCharacterSize(16);
    moc_czynna.setPosition(sf::Vector2f(160, height - 160));

    sf::Text mob_bierna(L"0", window.get_font());
    mob_bierna.setFillColor(window.green);
    mob_bierna.setCharacterSize(16);
    mob_bierna.setPosition(sf::Vector2f(160, height - 140));

    sf::Text czest_rez(L"0", window.get_font());
    czest_rez.setFillColor(window.green);
    czest_rez.setCharacterSize(16);
    czest_rez.setPosition(sf::Vector2f(160, height - 120));

    sf::Text napiecie(L"0", window.get_font());
    napiecie.setFillColor(window.green);
    napiecie.setCharacterSize(16);
    napiecie.setPosition(sf::Vector2f(160, height - 100));

    sf::Text prad(L"0", window.get_font());
    prad.setFillColor(window.green);
    prad.setCharacterSize(16);
    prad.setPosition(sf::Vector2f(160, height - 80));

    ElementManager menu_elementow, obwod;
    window.stworz_tlo(); //watek?
    menu_elementow.dodaj_menu(width, height);
    sf::Vector2f localPosition;
    std::shared_ptr<element_graficzny> obiekt_klikniety; // LPM
    std::shared_ptr<element_graficzny> obiekt_edytowany; // PPM
    Obwod analizowany_obwod;
    bool zasymulowany = false;
    bool menu_otwarte = false;

    // Ustawienie delegacji dla przyciskow
    sym_button.setOnClick([&obwod, &analizowany_obwod, &zasymulowany]() {
        zasymulowany = true;
        obwod.ustawWezly();
        analizowany_obwod.konwersja(obwod.getElementyWizualne());
        analizowany_obwod.solve();
        });

    //ok.setOnClick([&czestotliwosc, &analizowany_obwod]() {
    //    //analizowany_obwod.set_frequency(czestotliwosc.getText().);
    //    std::cout << "potwierdzam!" << std::endl;
    //    });

    ok_button.setOnClick([&obiekt_edytowany, &wartosc, &przesuniecie]() {
        if (std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany) != nullptr)
            std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany)->fi = przesuniecie.convertToValue();
        else if(std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany) != nullptr)
            std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany)->fi = przesuniecie.convertToValue();

        obiekt_edytowany->wartosc = wartosc.convertToValue();
        obiekt_edytowany = nullptr;
        });

    cancel_button.setOnClick([&obiekt_edytowany]() {
        obiekt_edytowany = nullptr;
        });

    plik.setOnClick([&menu_otwarte]() {
        if (menu_otwarte)
            menu_otwarte = false;
        else
            menu_otwarte = true;
        });

    std::string export_results = "plik1.txt";
    eksport_wynikow.setOnClick([&menu_otwarte, &analizowany_obwod, &export_results]() {
        // Œcie¿ka do folderu "Wyniki"
        std::filesystem::path output_folder("../Wyniki");

        // Tworzenie folderu, jeœli nie istnieje
        if (!std::filesystem::exists(output_folder))
            std::filesystem::create_directory(output_folder);
        // Ustawienie pe³nej œcie¿ki do pliku
        std::filesystem::path output_file = output_folder / export_results;

        // Zapisanie pliku
        analizowany_obwod.zapis_wyjscia(output_file.string());

        menu_otwarte = false;
        });

    std::string export_file = "obwod.txt";
    save.setOnClick([&menu_otwarte, &obwod, &analizowany_obwod, &export_file]() {
        // Œcie¿ka do folderu "Wyniki"
        std::filesystem::path output_folder("../Obwody");

        // Tworzenie folderu, jeœli nie istnieje
        if (!std::filesystem::exists(output_folder))
            std::filesystem::create_directory(output_folder);
        // Ustawienie pe³nej œcie¿ki do pliku
        std::filesystem::path output_file = output_folder / export_file;

        // Zapisanie pliku
        obwod.setFrequency(analizowany_obwod.get_frequency());
        obwod.saveElementsToFile(output_file.string());

        menu_otwarte = false;
        });

    open.setOnClick([&menu_otwarte, &zasymulowany, &czestotliwosc, &obwod, &menu_elementow, &analizowany_obwod, &export_file, &width, &height]() {
        // Œcie¿ka do folderu "Wyniki"
        std::filesystem::path output_folder("../Obwody");

        // Tworzenie folderu, jeœli nie istnieje
        if (!std::filesystem::exists(output_folder))
            std::filesystem::create_directory(output_folder);
        // Ustawienie pe³nej œcie¿ki do pliku
        std::filesystem::path output_file = output_folder / export_file;

        // Zapisanie pliku
        obwod.loadElementsFromFile(output_file.string(), menu_elementow, width, height);
        analizowany_obwod.set_frequency(obwod.getFrequency());
        czestotliwosc.setString(analizowany_obwod.get_frequency());
        czestotliwosc.appendHz();
        menu_otwarte = false;
        zasymulowany = false;
        });

    czestotliwosc.setString(analizowany_obwod.get_frequency());
    czestotliwosc.appendHz();
    int id = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                /*case sf::Event::Resized:
                    menu_elementow.usunElementy();
                    dodaj_do_menu(menu_elementow, event.size.width, event.size.height);
                    window.setSize(sf::Vector2u(event.size.width, event.size.height));
                    break;*/
                case sf::Event::KeyPressed:
                    if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        obiekt_klikniety = nullptr;
                        if (obiekt_edytowany != nullptr && wartosc.isSelected || przesuniecie.isSelected)
                        {
                            wartosc.setSelected(false);
                            przesuniecie.setSelected(false);
                        }
                        else if (obiekt_edytowany != nullptr)
                            cancel_button.click();
                        czestotliwosc.setSelected(false);
                        czestotliwosc.setString(analizowany_obwod.get_frequency());
                        czestotliwosc.appendHz();
                    }
                    else if (obiekt_edytowany == nullptr && !czestotliwosc.isSelected)
                    {
                        if (event.key.scancode == sf::Keyboard::Scan::W)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.20 + 1))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::R)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.32))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::E)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.42))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::I)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.57))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::C)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.72))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::L)
                        {
                            obiekt_klikniety = menu_elementow.wybrano_obiekt(sf::Vector2f(width * 0.93, height * 0.82))->clone();
                        }
                        else if (event.key.scancode == sf::Keyboard::Scan::Enter)
                        {
                            sym_button.click();
                        }
                    }
                    else if (event.key.scancode == sf::Keyboard::Scan::Enter)
                    {
                        if (czestotliwosc.isSelected)
                        {
                            analizowany_obwod.set_frequency(czestotliwosc.convertToValue());
                            czestotliwosc.setSelected(false);
                            czestotliwosc.appendHz();
                        }
                        else if (obiekt_edytowany != nullptr)
                            ok_button.click();
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    // obsluga rysowania elementow po ekranie
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (sym_button.isMouseOver(localPosition))
                            sym_button.click();
                        else if (obiekt_edytowany != nullptr && ok_button.isMouseOver(localPosition))
                            ok_button.click();
                        else if (obiekt_edytowany != nullptr && cancel_button.isMouseOver(localPosition))
                            cancel_button.click();
                        else if (plik.isMouseOver(localPosition))
                            plik.click();
                        else if (eksport_wynikow.isMouseOver(localPosition) && menu_otwarte)
                            eksport_wynikow.click();
                        else if (save.isMouseOver(localPosition) && menu_otwarte)
                            save.click();
                        else if (open.isMouseOver(localPosition) && menu_otwarte)
                            open.click();
                        else if (input_freq.getGlobalBounds().contains(localPosition))
                        {
                            wartosc.setSelected(false);
                            przesuniecie.setSelected(false);
                            czestotliwosc.setString(analizowany_obwod.get_frequency());
                            czestotliwosc.setSelected(true);
                        }
                        else if (input_value.getGlobalBounds().contains(localPosition))
                        {
                            wartosc.setSelected(true);
                            przesuniecie.setSelected(false);
                            czestotliwosc.setSelected(false);
                            czestotliwosc.appendHz();
                        }
                        else if (input_fi.getGlobalBounds().contains(localPosition) && std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany) != nullptr || std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany) != nullptr)
                        {
                            wartosc.setSelected(false);
                            przesuniecie.setSelected(true);
                            czestotliwosc.setSelected(false);
                            czestotliwosc.appendHz();
                        }
                        else if (obiekt_edytowany == nullptr)
                        {
                            if (std::dynamic_pointer_cast<linia>(obiekt_klikniety) != nullptr) // obsluga linii
                            {
                                if (std::dynamic_pointer_cast<linia>(obiekt_klikniety)->LeftHold == false && menu_elementow.czyWewnatrz(obiekt_klikniety))
                                {
                                    obwod.dodajElement(obiekt_klikniety);
                                    obiekt_klikniety = nullptr;
                                }
                                if (obiekt_klikniety != nullptr)
                                    std::dynamic_pointer_cast<linia>(obiekt_klikniety)->LeftHold = false;
                            }
                            else if (obiekt_klikniety != nullptr && menu_elementow.czyWewnatrz(obiekt_klikniety) && obwod.moznaUstawic(obiekt_klikniety)) // dodanie elementu do obwodu
                            {
                                zasymulowany = false;
                                obiekt_klikniety->ksztalt.setFillColor(sf::Color(21, 207, 51));
                                obwod.dodajElement(obiekt_klikniety);
                                obiekt_klikniety = nullptr;
                            }
                            else if (obiekt_klikniety == nullptr && obwod.wybrano_obiekt(localPosition) != nullptr) // wybranie elementu z obwodu
                            {
                                obiekt_klikniety = obwod.wybrano_obiekt(localPosition);
                                obwod.usunElement(obiekt_klikniety);
                            }
                            else if (menu_elementow.wybrano_obiekt(localPosition) != nullptr) // wybranie elementu z menu
                            {
                                if (menu_elementow.wybrano_obiekt(localPosition)->clone() != nullptr)
                                    obiekt_klikniety = menu_elementow.wybrano_obiekt(localPosition)->clone();
                            }
                        }
                        
                    }
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        if (obiekt_klikniety != nullptr)
                        {
                            obiekt_klikniety->ksztalt.rotate(90.f);
                        }
                        else if (obiekt_klikniety == nullptr && obiekt_edytowany == nullptr && obwod.wybrano_obiekt(localPosition) != nullptr)
                        {
                            // otworzenie panelu z wlasciwosciami elementu
                            if (obwod.wybrano_obiekt(localPosition)->typ != 'W')
                            {
                                obiekt_edytowany = obwod.wybrano_obiekt(localPosition);
                                if (std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany) != nullptr)
                                    przesuniecie.setString(std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany)->fi);
                                else if(std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany) != nullptr)
                                    przesuniecie.setString(std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany)->fi);

                                wartosc.setString(obiekt_edytowany->wartosc);
                            }
                        }
                    }
                    break;
                case sf::Event::MouseMoved:
                    localPosition = sf::Vector2f(sf::Mouse::getPosition(window));
                    if (sym_button.isMouseOver(localPosition))
                        sym_button.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        sym_button.hover(window.dark_blue, window.neon_blue);
                    if (ok_button.isMouseOver(localPosition))
                        ok_button.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        ok_button.hover(window.dark_blue, window.neon_blue);
                    if (cancel_button.isMouseOver(localPosition))
                        cancel_button.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        cancel_button.hover(window.dark_blue, window.neon_blue);
                    if (plik.isMouseOver(localPosition))
                        plik.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        plik.hover(window.dark_blue, window.neon_blue);
                    if (eksport_wynikow.isMouseOver(localPosition))
                        eksport_wynikow.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        eksport_wynikow.hover(window.dark_blue, window.neon_blue);
                    if (save.isMouseOver(localPosition))
                        save.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        save.hover(window.dark_blue, window.neon_blue);
                    if (open.isMouseOver(localPosition))
                        open.hover(sf::Color(200, 200, 50), window.dark_blue);
                    else
                        open.hover(window.dark_blue, window.neon_blue);
                    id = obwod.getElemId(localPosition);
                    break;
                case sf::Event::TextEntered:
                    if (czestotliwosc.isSelected)
                        czestotliwosc.typedOn(event);
                    else if (wartosc.isSelected && obiekt_edytowany != nullptr)
                        wartosc.typedOn(event);
                    else if (przesuniecie.isSelected && std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany) != nullptr || std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany) != nullptr)
                        przesuniecie.typedOn(event);
                    break;
            }
        }
        window.rysuj_banery();
        menu_elementow.rysujElementy(window);
        plik.drawTo(window);
        sym_button.drawTo(window);
        window.draw(input_freq);
        czestotliwosc.drawTo(window);
        window.rysuj_teksty();
        obwod.rysujElementy(window);
        if (obiekt_edytowany != nullptr)
        {
            window.rysuj_okno_dialogowe();
            ok_button.drawTo(window);
            cancel_button.drawTo(window);
            window.draw(input_value);
            wartosc.drawTo(window);
            if (std::dynamic_pointer_cast<zrodlo_sem>(obiekt_edytowany) != nullptr || std::dynamic_pointer_cast<zrodlo_spm>(obiekt_edytowany) != nullptr)
            {
                window.draw(text_fi);
                window.draw(input_fi);
                przesuniecie.drawTo(window);
            }
        }
        else
        {
            if (id != 0 && zasymulowany)
            {
                window.rysuj_wartosci();
                value.setString(doubleToString(analizowany_obwod.getWartosc(id)));
                moc_czynna.setString(doubleToString(analizowany_obwod.getMocCzynna(id)));
                mob_bierna.setString(doubleToString(analizowany_obwod.getMocBierna(id)));
                czest_rez.setString(doubleToString(analizowany_obwod.getCzestRez(id)));
                napiecie.setString(complexToString(analizowany_obwod.getNapiecie(id)));
                prad.setString(complexToString(analizowany_obwod.getPrad(id)));
                window.draw(value);
                window.draw(moc_czynna);
                window.draw(mob_bierna);
                window.draw(czest_rez);
                window.draw(napiecie);
                window.draw(prad);
            }
        }
        if (menu_otwarte)
        {
            eksport_wynikow.drawTo(window);
            save.drawTo(window);
            open.drawTo(window);
        }
        // zmiana koloru i polozenia wybranego elementu
        if (obiekt_klikniety != nullptr)
        {
            obiekt_klikniety->ksztalt.setFillColor(sf::Color(21, 207, 51, 150));
            if (!menu_elementow.czyWewnatrz(obiekt_klikniety) || !obwod.moznaUstawic(obiekt_klikniety))
            {
                obiekt_klikniety->ksztalt.setFillColor(sf::Color::Red);
                if (std::dynamic_pointer_cast<linia>(obiekt_klikniety) != nullptr)
                {
                    std::dynamic_pointer_cast<linia>(obiekt_klikniety)->changeLeftColor(sf::Color::Red);
                    std::dynamic_pointer_cast<linia>(obiekt_klikniety)->changeRightColor(sf::Color::Red);
                }
            }
            else if (std::dynamic_pointer_cast<linia>(obiekt_klikniety) != nullptr)
            {
                std::dynamic_pointer_cast<linia>(obiekt_klikniety)->changeLeftColor(sf::Color(100, 107, 120));
                std::dynamic_pointer_cast<linia>(obiekt_klikniety)->changeRightColor(sf::Color(100, 107, 120));
            }
            if (std::dynamic_pointer_cast<linia>(obiekt_klikniety) != nullptr && std::dynamic_pointer_cast<linia>(obiekt_klikniety)->LeftHold)
            {
                std::dynamic_pointer_cast<linia>(obiekt_klikniety)->setStartPoint(localPosition);
            }
            else if (std::dynamic_pointer_cast<linia>(obiekt_klikniety) != nullptr)
            {
                std::dynamic_pointer_cast<linia>(obiekt_klikniety)->setEndPoint(localPosition);
            }
            else
                obiekt_klikniety->ksztalt.setPosition(localPosition);
            obwod.przystaw_element(obiekt_klikniety, localPosition);
            window.draw(*obiekt_klikniety);
        }
        window.display();
    }
    return 0;
}