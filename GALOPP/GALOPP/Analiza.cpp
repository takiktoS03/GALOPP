#pragma once
/**
@file klasy.cpp
*/

#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "Analiza.h"

//element::element(char nazwa, std::pair<int, int> para, double wart)
//{
//	typ = nazwa;
//	umiejscowienie = para;
//	wartosc = wart;
//}

E::E(int id, char typ, std::pair<int, int> miejsce, double wartosc, double fi)
{
	this->id = id;
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
	this->fi = fi;
}

I::I(int id, char typ, std::pair<int, int> miejsce, double wartosc, double fi)
{
	this->id = id;
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
	this->fi = fi;
}

R::R(int id, char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->id = id;
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

C::C(int id, char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->id = id;
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

L::L(int id, char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->id = id;
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

E::~E() {};
I::~I() {};
R::~R() {};
C::~C() {};
L::~L() {};

std::complex<double> E::wyznacz_Z(double& freq)
{
	return { wartosc * cos(E::fi * M_PI / 180),wartosc * sin(E::fi * M_PI / 180) };
	
}
std::complex<double> I::wyznacz_Z(double& freq)
{
	return { wartosc * cos(I::fi * M_PI / 180),wartosc * sin(I::fi * M_PI / 180) };
}
std::complex<double> R::wyznacz_Z(double& freq)
{
	return { wartosc,0 };
}
std::complex<double> C::wyznacz_Z(double& freq)
{
	return { e,-1 / (2 * M_PI * freq * wartosc) + e };
}
std::complex<double> L::wyznacz_Z(double& freq)
{
	return { e,(2 * M_PI * freq * wartosc) + e};
}

std::complex<double> element::wyznacz_Y() const
{
	std::complex<double> jeden{ 1, 0 };
	return jeden / element::impedancja;
}

double E::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double I::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double R::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double C::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	for (auto& i : elementy)
	{
		if (i->typ == 'L' && (i->umiejscowienie.first == C::umiejscowienie.second || i->umiejscowienie.first == C::umiejscowienie.first || i->umiejscowienie.second == C::umiejscowienie.first || i->umiejscowienie.second == C::umiejscowienie.second))
		{
			return 1 / (2 * M_PI * sqrt(i->wartosc * C::wartosc));
		}
	}
	return 0;
}
//((i->umiejscowienie == C::umiejscowienie || (i->umiejscowienie.first == C::umiejscowienie.second && i->umiejscowienie.second == C::umiejscowienie.first)) //rownolegle polaczenie
double L::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	for (auto& i : elementy)
	{
		if (i->typ == 'C' && (i->umiejscowienie.first == L::umiejscowienie.second || i->umiejscowienie.first == L::umiejscowienie.first || i->umiejscowienie.second == L::umiejscowienie.first || i->umiejscowienie.second == L::umiejscowienie.second))
		{
			return 1 / (2 * M_PI * sqrt(i->wartosc * L::wartosc));
		}
	}
	return 0;
}

void E::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly) 
{
	for (auto& j : elementy)
	{
		if (j->typ == 'R' && j->wartosc == -1 && E::umiejscowienie.second == j->umiejscowienie.second)
		{
			E::prad = j->prad; // prad na zrodle SEM jest taki jak prad do niego doplywajacy (z dodanego rezystora -1)
			E::umiejscowienie.second = j->umiejscowienie.first; // zamiana numerow wezlow spowrotem
		}
	}
}
void I::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	I::prad = -I::impedancja; // prad na zrodle SPM jest taki jak jego wartosc
}
void R::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	if (R::typ == 'R' && R::wartosc == -1)
	{
		R::prad = (potencjaly[R::umiejscowienie.second] - potencjaly[R::umiejscowienie.first]) * R::admitancja;
	}
	else
		R::prad = (potencjaly[R::umiejscowienie.first] - potencjaly[R::umiejscowienie.second]) * R::admitancja;
}
void C::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	C::prad = (potencjaly[C::umiejscowienie.first] - potencjaly[C::umiejscowienie.second]) * C::admitancja;
}
void L::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	L::prad = (potencjaly[L::umiejscowienie.first] - potencjaly[L::umiejscowienie.second]) * L::admitancja;
}


void element::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	napiecie = potencjaly[element::umiejscowienie.first] - potencjaly[element::umiejscowienie.second];
}

void element::wyznacz_moce()
{
	moc_czynna = sqrt(pow(napiecie.real(), 2) + pow(napiecie.imag(), 2)) * sqrt(pow(prad.real(), 2) + pow(prad.imag(), 2)) * cos(arg(napiecie) - arg(prad));
	moc_bierna = sqrt(pow(napiecie.real(), 2) + pow(napiecie.imag(), 2)) * sqrt(pow(prad.real(), 2) + pow(prad.imag(), 2)) * sin(arg(napiecie) - arg(prad));
}