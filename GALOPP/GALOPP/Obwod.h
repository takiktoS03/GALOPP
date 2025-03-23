#pragma once
/**
@file Obwod.h
*/

#ifndef OBWOD_H
#define OBWOD_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <utility>
#include <concepts>

#include "Analiza.h"
#include "sfml.h"

template <typename T>
concept Hashable = requires(T a) {
	{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

struct pair_hash {
	template <Hashable T1, Hashable T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const {
		auto hash1 = std::hash<T1>{}(pair.first);
		auto hash2 = std::hash<T2>{}(pair.second);
		return hash1 ^ (hash2 << 1);
	}
};

class Obwod
{
private:
	std::vector<std::shared_ptr<element>> elementy;
	double frequency;
	int nextNodeId;
	int generateUniqueId();
	std::unordered_map<std::pair<double, double>, int, pair_hash> nodeMap;
	void clearContainers();
	std::pair<double, double> toPair(const sf::Vector2f& vec);
	void updateConnectedLines(const std::vector<std::shared_ptr<element_graficzny>>& elem_graf, const int nodeId);
	void showNodes();
	std::set<int> wezly;
	macierz macierz_potencjalow;
	std::unordered_map<int, int> stare2nowe;
	std::unordered_map<int, std::complex<double>> potencjaly;
	/**
	@brief Funkcja tworzy kontener unikalnych wezlow z obwodu.
	@details wpisujac kazdy wezel elementu w sposob posortowany od najmniejszych do najwiekszych wartosci.
	*/
	void wektor_wezlow();
	/**
	@brief Funkcja dodaje wirtualny rezystor na galaz obok SEM.
	@details Funkcja dodaje rezystor o wartosci -1 obok galezi ze zrodlem elektromotorycznym, aby pozniej traktowac galaz z SEM jako galaz z opornoscia 1 om.
	*/
	void dodaj_rez_obok_sem();
	/**
	@brief Funkcja tworzy mape wezlow, tak aby zaczynaly sie od 0 do n.
	@details na podstawie kontenera tworzonego w wektor_wezlow()
	*/
	void mapujStare();
	/**
	@brief Funkcja tworzy macierz (uklad rownan) za pomoca metody Coltriego.
	@details Funkcja wprowadza do macierzy admitancje kazdego potencjalu uzywajac metody potencjalow wezlowych.
	Napotkane galezie ze zrodlem elektromotorycznym traktuje jako galezie z impedancja {1,0}om.
	*/
	void coltri();
	/**
	@brief Funkcja oblicza macierz za pomoca metody eliminacji Gaussa-Jordana-Crouta.
	@details Funkcja sprowadza macierz do postaci trojkatnej (uzyskuje zero nad i pod przekatna macierzy).
	Zamienia wiersze (rownanie) z innym wierszem, w ktorym wystepuje wiekszy wspolczynnik w kolumnie.
	Metoda ta sprowadza macierz rozszerzona ukladu rownan do postaci bazowej (macierzy jednostkowej).
	Z tej postaci mozna wprost odczytac potencjaly w wezlach.
	@return Mapa potencjalow w wezlach
	@param [in] uklad_rownan Macierz stworzona w coltri()
	@param [in] stare2nowe Mapa wezlow
	*/
	std::unordered_map<int, std::complex<double>> gauss(macierz& uklad_rownan, std::unordered_map<int, int>& stare2nowe);
public:
	Obwod() : nextNodeId(0), frequency(0) {};
	void set_frequency(double freq);
	double get_frequency() const;
	void konwersja(const std::vector<std::shared_ptr<element_graficzny>>& elem_graf);
	void solve();
	double getWartosc(int id);
	double getMocCzynna(int id);
	double getMocBierna(int id);
	double getCzestRez(int id);
	std::complex<double> getNapiecie(int id);
	std::complex<double> getPrad(int id);
	/**
	@brief Funkcja wypisuje do pliku wyjscia charakterystyke kazdego elementu.
	@details Funkcja wypisuje wszystkie elementy obwodu, ich parametry i bilans obwodu do pliku tekstowego.
	Prad oraz napiecie funkcja podaje w postaci modulu liczby zespolonej oraz wzoru Eulera na kat przesuniecia fazowego dla kazdego elementu
	Funkcja oblicza bilans mocy na podstawie mocy oddanej lub pobranej dla kazdego elementu.
	@param [in] nazwa_pliku Nazwa pliku wyjsciowego
	*/
	void zapis_wyjscia(const std::string& nazwa_pliku);
};

void wypisz_elementy(const std::vector<std::shared_ptr<element>>& elementy);

//void wypisz(const macierz& potencjaly);
#endif
