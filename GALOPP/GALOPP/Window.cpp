#include "Window.h"

Window::Window(sf::VideoMode mode, const sf::String& title) : sf::RenderWindow(mode, title)
{
    _font.loadFromFile("../GALOPP/fonts/ChakraPetch-Medium.ttf");
    width = getSize().x;
    height = getSize().y;
}

sf::Font& Window::get_font()
{
    return _font;
}

void Window::centerText(sf::Text& text) 
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(getSize().x / 2.0f, 15 + textRect.height / 2.0f));
}

void Window::stworz_tlo()
{
    // Wyswietla glowny napis
    sf::Text tytul(L"Symulator obwodów prądu", _font);
    tytul.setFillColor(neon_blue);
    centerText(tytul);

    // Baner tytulowy
    sf::RectangleShape baner(sf::Vector2f(width, 50));
    baner.setOutlineThickness(3);
    baner.setOutlineColor(neon_blue);
    baner.setFillColor(dark_blue);
    baner.setPosition(0, 3);

    // Menu elementow
    sf::RectangleShape prawy_baner(sf::Vector2f(150, height * 0.80));
    prawy_baner.setOutlineThickness(3);
    prawy_baner.setOutlineColor(neon_blue);
    prawy_baner.setFillColor(dark_blue);
    prawy_baner.setPosition(width - 166, height * 0.13 - 3);


    sf::Text freq(L"Częstotliwość", _font);
    freq.setFillColor(neon_blue);
    freq.setCharacterSize(13);
    freq.setPosition(width - 130, height * 0.13);
    // Litery pod elementami
    sf::Text W("W", _font);
    W.setFillColor(green);
    W.setPosition(width - 102, height * 0.21);

    sf::Text R("R", _font);
    R.setFillColor(green);
    R.setPosition(width - 98, height * 0.35);

    sf::Text E("E", _font);
    E.setFillColor(green);
    E.setPosition(width - 97, height * 0.5);

    sf::Text I("I", _font);
    I.setFillColor(green);
    I.setPosition(width - 92, height * 0.64);

    sf::Text C("C", _font);
    C.setFillColor(green);
    C.setPosition(width - 97, height * 0.75);

    sf::Text L("L", _font);
    L.setFillColor(green);
    L.setPosition(width - 95, height * 0.84);

    // Teksty przyciskow
    /*sf::Text sym("SYMULUJ", _font);
    sym.setCharacterSize(20);
    sym.setFillColor(neon_blue);
    sym.setPosition(width - 135, height * 0.92);*/

    // Wektor z banerami
    baners.push_back(baner);
    baners.push_back(prawy_baner);

    // Wektor tekstow
    texts.push_back(tytul);
    texts.push_back(freq);
    texts.push_back(W);
    texts.push_back(R);
    texts.push_back(E);
    texts.push_back(I);
    texts.push_back(C);
    texts.push_back(L);
    //texts.push_back(sym);
}

void Window::rysuj_banery()
{
    if (isOpen())
    {
        clear(tlo);
        for (auto& i : baners)
        {
            draw(i);
        }
    }
}

void Window::rysuj_teksty()
{
    if (isOpen())
    {
        for (auto& i : texts)
        {
            draw(i);
        }
    }
}

void Window::rysuj_okno_dialogowe()
{
    sf::RectangleShape okno_dialogowe(sf::Vector2f(width / 3, height / 4));
    okno_dialogowe.setOutlineThickness(3);
    okno_dialogowe.setOutlineColor(neon_blue);
    okno_dialogowe.setFillColor(dark_blue);
    okno_dialogowe.setPosition(width / 3.5, height / 2.5);
    sf::RectangleShape tytul(sf::Vector2f(width / 3, 30));
    tytul.setOutlineThickness(3);
    tytul.setOutlineColor(neon_blue);
    tytul.setFillColor(dark_blue);
    tytul.setPosition(width / 3.5, height / 2.5);

    sf::Text edit_values(L"Edytuj wartości", _font);
    edit_values.setFillColor(green);
    edit_values.setCharacterSize(22);
    edit_values.setPosition(tytul.getPosition() + sf::Vector2f(92, 0));

    sf::Text value(L"Wartość", _font);
    value.setFillColor(green);
    value.setCharacterSize(16);
    value.setPosition(okno_dialogowe.getPosition() + sf::Vector2f(20, 63));

    draw(okno_dialogowe);
    draw(tytul);
    draw(edit_values);
    draw(value);
}

void Window::rysuj_wartosci()
{
    sf::RectangleShape okno(sf::Vector2f(width / 4, height / 5));
    okno.setOutlineThickness(3);
    okno.setOutlineColor(neon_blue);
    okno.setFillColor(dark_blue);
    okno.setPosition(50, height - 200);

    sf::Text wartosc(L"Wartość:                                      Ω", _font);
    wartosc.setFillColor(green);
    wartosc.setCharacterSize(16);
    wartosc.setPosition(okno.getPosition() + sf::Vector2f(10, 20));

    sf::Text moc_c(L"Moc czynna:                               W", _font);
    moc_c.setFillColor(green);
    moc_c.setCharacterSize(16);
    moc_c.setPosition(okno.getPosition() + sf::Vector2f(10, 40));

    sf::Text moc_b(L"Moc bierna:                                 VA", _font);
    moc_b.setFillColor(green);
    moc_b.setCharacterSize(16);
    moc_b.setPosition(okno.getPosition() + sf::Vector2f(10, 60));

    sf::Text freq(L"Częst. rez.:                                   Hz", _font);
    freq.setFillColor(green);
    freq.setCharacterSize(16);
    freq.setPosition(okno.getPosition() + sf::Vector2f(10, 80));

    sf::Text napiecie(L"Napięcie:                                       V", _font);
    napiecie.setFillColor(green);
    napiecie.setCharacterSize(16);
    napiecie.setPosition(okno.getPosition() + sf::Vector2f(10, 100));

    sf::Text prad(L"Prąd:                                              A", _font);
    prad.setFillColor(green);
    prad.setCharacterSize(16);
    prad.setPosition(okno.getPosition() + sf::Vector2f(10, 120));

    draw(okno);
    draw(wartosc);
    draw(moc_c);
    draw(moc_b);
    draw(freq);
    draw(napiecie);
    draw(prad);
}

//void Window::dodaj_przyciski()
//{
//    // Przycisk do symulowania
//    Button sym_button("SYMULUJ", sf::Vector2f(150, 30), 20, dark_blue, neon_blue);
//    sym_button.setFont(_font);
//    sym_button.setPosition(sf::Vector2f(width - 166, height * 0.92 - 3));
//    
//    // Przycisk potwierdzania
//    Button ok("OK", sf::Vector2f(50, 20), 20, dark_blue, neon_blue);
//    ok.setFont(_font);
//    ok.setPosition(sf::Vector2f(width / 3.5 + 150, height / 2.5 + 150));
//
//    // Przycisk anulowania
//    Button cancel("ANULUJ", sf::Vector2f(50, 20), 20, dark_blue, neon_blue);
//    cancel.setFont(_font);
//    cancel.setPosition(sf::Vector2f(width / 3.5 + 150, height / 2.5 + 150));
//
//    buttons.push_back(sym_button);
//    buttons.push_back(ok);
//    buttons.push_back(cancel);
//}
//// do main
//void Window::dodaj_inputy()
//{
//    Textbox wartosc(16, neon_blue, true);
//    wartosc.setFont(_font);
//    wartosc.setLimit(true, 15);
//    wartosc.setPosition(sf::Vector2f(width / 3.5 + 90, height / 2.5 + 65));
//
//    Textbox przesuniecie(16, neon_blue, false);
//    przesuniecie.setFont(_font);
//    przesuniecie.setLimit(true, 15);
//    przesuniecie.setPosition(sf::Vector2f(width / 3.5 + 90, height / 2.5 + 95));
//
//    Textbox czestotliwosc(16, neon_blue, false);
//    czestotliwosc.setFont(_font);
//    czestotliwosc.setLimit(true, 15);
//    czestotliwosc.setPosition(sf::Vector2f(width - 130, height * 0.13 + 5));
//
//    inputs.push_back(wartosc);
//    inputs.push_back(przesuniecie);
//    inputs.push_back(czestotliwosc);
//}