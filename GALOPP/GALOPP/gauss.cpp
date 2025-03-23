/**
@file gauss.cpp
*/

#include <iostream>
#include <unordered_map>
#include <set>
#include <cmath>

#include "Analiza.h"
#include "Obwod.h"

std::unordered_map<int, std::complex<double>> Obwod::gauss(macierz& gauss, std::unordered_map<int, int>& stare2nowe)
{
	std::unordered_map<int, std::complex<double>> nowe2stare;
	if (gauss.empty())
		return nowe2stare;
	std::complex<double> a, zz = { 0,0 }, jz = { 1,0 }, zj = { 0,1 }, jj = { 1,1 };
	int n = gauss.size();
	for (int i = 0; i < n; i++)
	{
		int ielmax = i;// indeks wiersza, w ktorym znajduje sie element max
		double elmax = sqrt(pow(real(gauss[i][i]), 2) + pow(imag(gauss[i][i]), 2)); // element max to wartosc bezwzgledna z jakiegos* elementu w kolumnie
		for (int j = i; j < n; j++)
		{// *jakis, znaczy najwiekszy w kolumnie pod wzgledem modulu
			if (sqrt(pow(real(gauss[j][i]), 2) + pow(imag(gauss[j][i]), 2)) > elmax)
			{
				elmax = sqrt(pow(real(gauss[j][i]), 2) + pow(imag(gauss[j][i]), 2));
				ielmax = j;
			}
		}
		if (elmax == 0)
		{
			//std::cout << "Macierz osobliwa (wyznacznik = 0), nie da sie obliczyc zmiennych\n";
			return nowe2stare;
		}
		if (ielmax != i)
		{// Zamiana wierszy (r�wna�) "i" z "ielmax"
			std::vector<std::complex<double>> temp = gauss[i];
			gauss[i] = gauss[ielmax];
			gauss[ielmax] = temp;
			/*int temp2 = nr_wiersza[i];
			nr_wiersza[i] = nr_wiersza[ielmax];
			nr_wiersza[ielmax] = temp2;*/
		}
		for (int j = i + 1; j < n; j++)
		{// postepowanie proste gaussa
			a = gauss[j][i] / gauss[i][i];
			gauss[j][i] = zz;
			for (int k = i + 1; k <= n; k++)
			{
				gauss[j][k] -= a * gauss[i][k];
			}
		}
	}
	if (gauss[n - 1][n - 1] == zz)
	{
		//std::cout << "Macierz osobliwa, nie da sie obliczyc zmiennych\n"; // Nie mieszamy logiki aplikacji z interfejsem uzytkownika
		return nowe2stare;
	}
	for (int i = n - 1; i > 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{// postepowanie odwrotne gaussa
			a = gauss[j][i] / gauss[i][i];
			for (int k = n; k >= 0; k--)
			{
				gauss[j][k] -= a * gauss[i][k];
			}
		}
	}
	for (int w = 0; w < n; w++)
	{// normalizowanie wierszy
		if (gauss[w][w] != jz)
		{
			a = jz / gauss[w][w];
			for (int j = 0; j <= n; j++)
			{
				gauss[w][j] *= a;
			}
		}
	}
	//wypisz(gauss);
	//std::cout << std::endl;
	std::vector<std::complex<double>> napiecia{ {0, 0} };
	for (int i = 0; i < n; i++)
	{
		napiecia.push_back(gauss[i][n]);
	}
	for (auto iter = stare2nowe.begin(); iter != stare2nowe.end(); iter++)
	{
		nowe2stare[iter->first] = napiecia[iter->second];
	}
	return nowe2stare;
}