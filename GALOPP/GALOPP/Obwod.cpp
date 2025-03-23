/**
 @file Obwod.cpp
*/

#define _USE_MATH_DEFINES

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <cmath>
#include <memory>
#include <complex>
#include <iomanip>

#include "Analiza.h"
#include "Obwod.h"


int Obwod::generateUniqueId()
{
	return nextNodeId++;
}

void Obwod::clearContainers()
{
	elementy.clear();
	wezly.clear();
	macierz_potencjalow.clear();
	stare2nowe.clear();
	potencjaly.clear();
}

std::pair<double, double> Obwod::toPair(const sf::Vector2f& vec)
{
	return std::make_pair(static_cast<double>(vec.x), static_cast<double>(vec.y));
}

void Obwod::updateConnectedLines(const std::vector<std::shared_ptr<element_graficzny>>& elem_graf, const int nodeId)
{
	for (auto& otherElem : elem_graf)
	{
		if (otherElem->typ == 'W')
		{
			auto leftNodePair = toPair(otherElem->leftNode);
			auto rightNodePair = toPair(otherElem->rightNode);

			if (nodeMap.find(leftNodePair) != nodeMap.end() || nodeMap.find(rightNodePair) != nodeMap.end())
			{
				if (nodeMap[leftNodePair] == nodeId || nodeMap[rightNodePair] == nodeId)
				{
					if (nodeMap[leftNodePair] != nodeId) {
						nodeMap[leftNodePair] = nodeId;
						updateConnectedLines(elem_graf, nodeId);
					}
					if (nodeMap[rightNodePair] != nodeId) {
						nodeMap[rightNodePair] = nodeId;
						updateConnectedLines(elem_graf, nodeId);
					}
				}
			}
		}
	}
}

void Obwod::showNodes()
{
	if (nodeMap.empty())
		return;
	for (auto& i : nodeMap)
	{
		std::cout << i.first.first << " " << i.first.second << ": " << i.second << std::endl;
	}
	std::cout << std::endl;
}

void Obwod::set_frequency(double freq)
{
	frequency = freq;
}

double Obwod::get_frequency() const
{
	return frequency;
}

void Obwod::konwersja(const std::vector<std::shared_ptr<element_graficzny>>& elem_graf)
{
	if (elem_graf.empty())
		return;
	clearContainers();
	nodeMap.clear();
	nextNodeId = 0;
	// Przypisanie wezlow liniom
	for (auto& elem : elem_graf)
	{
		if (elem->typ == 'W')
		{
			auto leftNodePair = toPair(elem->leftNode);
			auto rightNodePair = toPair(elem->rightNode);

			if (nodeMap.find(leftNodePair) == nodeMap.end() && nodeMap.find(rightNodePair) == nodeMap.end())
			{
				int uniqueId = generateUniqueId();
				nodeMap[leftNodePair] = uniqueId;
				nodeMap[rightNodePair] = uniqueId;
				updateConnectedLines(elem_graf, uniqueId);
			}
		}
	}
	// Przypisanie wezlow innym elementom
	for (auto& elem : elem_graf)
	{
		if (elem->typ != 'W')
		{
			auto leftNodePair = toPair(elem->leftNode);
			auto rightNodePair = toPair(elem->rightNode);

			if (nodeMap.find(leftNodePair) == nodeMap.end()) {
				nodeMap[leftNodePair] = generateUniqueId();
			}
			if (nodeMap.find(rightNodePair) == nodeMap.end()) {
				nodeMap[rightNodePair] = generateUniqueId();
			}
			// Dodanie elementu do wektora
			int id = elem->id;
			char typ = elem->typ;
			double wartosc = elem->wartosc;
			std::pair<int, int> miejsce{ nodeMap[leftNodePair], nodeMap[rightNodePair] };
			if (typ == 'E')
				elementy.push_back(std::shared_ptr<element>(new E(id, typ, miejsce, wartosc, std::dynamic_pointer_cast<zrodlo_sem>(elem)->fi)));
			else if (typ == 'I')
				elementy.push_back(std::shared_ptr<element>(new I(id, typ, miejsce, wartosc, std::dynamic_pointer_cast<zrodlo_spm>(elem)->fi)));
			else if (typ == 'R')
				elementy.push_back(std::shared_ptr<element>(new R(id, typ, miejsce, wartosc)));
			else if (typ == 'C')
				elementy.push_back(std::shared_ptr<element>(new C(id, typ, miejsce, wartosc)));
			else if (typ == 'L')
				elementy.push_back(std::shared_ptr<element>(new L(id, typ, miejsce, wartosc)));
		}
	}
	//wypisz_elementy(elementy);
	//showNodes();
}

void Obwod::solve()
{
	if (elementy.empty())
		return;
	wektor_wezlow();
	dodaj_rez_obok_sem();
	wektor_wezlow();
	for (auto& i : elementy)
	{
		i->impedancja = i->wyznacz_Z(frequency);
		i->admitancja = i->wyznacz_Y();
		i->czest_rez = i->wyznacz_czest_rez(elementy);
	}
	mapujStare();
	coltri();
	potencjaly = gauss(macierz_potencjalow, stare2nowe);
	for (auto& i : elementy)
	{
		if (i->typ == 'R')
		{
			i->wyznacz_I(elementy, potencjaly);
			i->wyznacz_V(potencjaly);
			i->wyznacz_moce();
		}
	}
	for (auto& i : elementy)
	{
		if (i->typ != 'R')
		{
			i->wyznacz_I(elementy, potencjaly);
			i->wyznacz_V(potencjaly);
			i->wyznacz_moce();
		}
	}
	//zapis_wyjscia(plik_wyjscia);
}

double Obwod::getWartosc(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->wartosc;
	}
	return 0;
}

double Obwod::getMocCzynna(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->moc_czynna;
	}
	return 0;
}

double Obwod::getMocBierna(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->moc_bierna;
	}
	return 0;
}

double Obwod::getCzestRez(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->czest_rez;
	}
	return 0;
}

std::complex<double> Obwod::getNapiecie(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->napiecie;
	}
	return { 0,0 };
}

std::complex<double> Obwod::getPrad(int id)
{
	for (auto& i : elementy)
	{
		if (i->id == id)
			return i->prad;
	}
	return { 0,0 };
}

void wypisz_elementy(const std::vector<std::shared_ptr<element>>& elementy)
{
	std::cout << std::endl;
	for (auto& i : elementy)
	{
		std::cout << i->typ << " " << i->umiejscowienie.first << " " << i->umiejscowienie.second << " " << i->wartosc << std::endl;
	}
}

void Obwod::wektor_wezlow()
{
	for (auto& el : elementy)
	{
		wezly.insert(el->umiejscowienie.first);
		wezly.insert(el->umiejscowienie.second);
	}
}

void Obwod::dodaj_rez_obok_sem()
{
	int max = 0;
	for (auto& i : wezly)
	{
		if (i > max)
		{
			max = i;
		}
	}
	int wolny_wezel = max;
	for (int i = 0; i < elementy.size(); i++)
	{
		if (elementy[i]->typ == 'E')
		{
			wolny_wezel++;
			elementy.push_back(std::shared_ptr<element>(new R(-1, 'R', { elementy[i]->umiejscowienie.second, wolny_wezel }, -1))); // rezystor -1 dodany na gałąź ze źródłem SEM
			elementy[i]->umiejscowienie.second = wolny_wezel;
		}
	}
}
 
void Obwod::mapujStare()
{
	int nowywezel = 0;
	for (auto& i : wezly)
	{
		stare2nowe[i] = nowywezel;
		nowywezel++;
	}
}

void Obwod::coltri()
{
 	auto iter = stare2nowe.begin();
	iter++;  // omijamy uziemiony wezel (zawsze to bedzie najmniejszy wezel znaleziony w pliku wejscia)
 	for ( ; iter != stare2nowe.end(); iter++)
	{
		std::vector<std::complex<double>> wiersz(stare2nowe.size() + 1);
		for (auto& j : elementy)
		{
			if ((j->typ == 'R' || j->typ == 'C' || j->typ == 'L') && j->umiejscowienie.first == iter->first)
			{
				wiersz[iter->second] += j->admitancja;
				wiersz[stare2nowe[j->umiejscowienie.second]] -= j->admitancja;
			}
			else if ((j->typ == 'R' || j->typ == 'C' || j->typ == 'L') && j->umiejscowienie.second == iter->first)
			{
				wiersz[iter->second] += j->admitancja;
				wiersz[stare2nowe[j->umiejscowienie.first]] -= j->admitancja;
			}
			else if (j->typ == 'E' && j->umiejscowienie.first == iter->first)
			{
				wiersz[iter->second] += {1, 0};
				wiersz[stare2nowe[j->umiejscowienie.second]] -= {1, 0};
			}
			else if (j->typ == 'E' && j->umiejscowienie.second == iter->first)
			{
				wiersz[iter->second] += {1, 0};
				wiersz[stare2nowe[j->umiejscowienie.first]] -= {1, 0};
			}
		}
		for (auto& j : elementy)
		{
			if ((j->typ == 'E' || j->typ == 'I') && j->umiejscowienie.first == iter->first)
			{
				wiersz[stare2nowe.size()] += j->impedancja;
			}
			else if ((j->typ == 'E' || j->typ == 'I') && j->umiejscowienie.second == iter->first)
			{
				wiersz[stare2nowe.size()] -= j->impedancja;
			}
		}
		wiersz.erase(wiersz.begin());
		macierz_potencjalow.push_back(wiersz);
		wiersz.clear();
	}
	//wypisz(macierz_potencjalow);
}

//void wypisz(const macierz& macierz)
//{
//	std::vector<std::complex<double>> wiersz;
//	std::cout << std::endl << macierz.size() << " x " << macierz[0].size() << std::endl;
//	for (auto& i : macierz)
//	{
//		wiersz = i;
//		for (auto& j : wiersz)
//		{
//			std::cout << j << " ";
//		}
//		std::cout << std::endl;
//	}
//}

void Obwod::zapis_wyjscia(const std::string& nazwa_pliku)
{
	std::ofstream plik(nazwa_pliku);
	if (plik)
	{
		double bilans_czynnej = 0, bilans_biernej = 0;
		plik << "Elementy obwodu:" << std::endl << "TYP | WĘZEŁ POCZĄTKOWY | WĘZEŁ KOŃCOWY | WARTOŚĆ | PRĄD | NAPIĘCIE | CZESTOTLIWOSC REZONANSOWA | MOC CZYNNA | MOC BIERNA" << std::endl;
		for (auto& i : elementy)
		{
			if (i->typ == 'R' && i->wartosc == -1)
				continue;
			plik << std::setprecision(5) << i->typ << " " << i->umiejscowienie.first << " " << i->umiejscowienie.second << " " << i->wartosc << " "
				//<< i->prad << " [A] " << i->napiecie << " [V] "
				 << sqrt(pow(i->prad.real(), 2) + pow(i->prad.imag(), 2)) << "e^j(" << atan(imag(i->prad) / real(i->prad)) * 180 / M_PI << ") [A] "
				 << sqrt(pow(i->napiecie.real(), 2) + pow(i->napiecie.imag(), 2)) << "e^j(" << atan(imag(i->napiecie) / real(i->napiecie)) * 180 / M_PI << ") [V] "
				 << std::setprecision(8) << i->czest_rez << "Hz " << i->moc_czynna << "[W] " << i->moc_bierna << "[VA]" << std::endl;
			bilans_czynnej += i->moc_czynna;
			bilans_biernej += i->moc_bierna;
		}
		plik << "Bilans mocy układu (moc czynna, moc bierna): " << std::setprecision(4) << bilans_czynnej << "[W] " << bilans_biernej << "[VA]";
		//std::cout << "Bilans mocy ukladu: " << bilans << "W";
	}
}