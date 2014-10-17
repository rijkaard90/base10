#ifndef CODIFICA_H
#define CODIFICA_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;
typedef unsigned char byte;

#include "coppia.h"

/*!
\brief	Function ENCODE
\details Adjust the range based on the symbol interval.
\param start descrizione parametro.
\param size descrizione parametro.
\param &low descrizione parametro.
\param &range descrizione parametro.
\param &top descrizione.
\return Adjusted values of low and range.
\see encode_symbol(), trasforma_string(), syntax()
*/
void encode(double start, double size, double& low, double& range, double& top){
	low = low + start * range;
	top = (unsigned long)(low + size* range);
	low = (unsigned long)low;
	range = top - low;
}

/*!
\brief	Function ENCODE_SYMBOL
\details dettagli della funzione encode simboli.
\param b descrizione parametro
\param &x descrizione parametro
\param &low descrizione parametro
\param &range descrizione parametro
\param &top descrizione.
\return Ritorna la coppia di valori.
\see encode, trasforma_string(), syntax()
*/
void encode_symbol(byte b, vector<coppia>& x, double& low, double& range, double& top){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa, low, range, top);
			//verifica di ogni passaggio
			cout << "low: " << low << "\t range: " << range << "\t top: " << top << endl;
			//modifica per range bassi disponibili
			if (range < 100000000.0){
				range *= 10.0;
				low *= 10.0;
			}
			return;
		}
	cout << "Symbol not found!";
}

/*!
\brief	Function TRASFORMA_STRING
\details Comparazione top e low per trovare le cifre sufficienti per la codifica.
\param top descrizione parametro
\param low descrizione parametro
\param first descrizione parametro
\param last descrizione parametro
\return Che cosa ritorna.
\see encode(), encode_symbol(), syntax()
*/
string trasforma_string(double top, double low, string first, string last){
	string codifica;
	unsigned length = 1;
	while (true){
		if ((first.compare(0, length, last, 0, length)) != 0){
			break;
		}
		codifica.push_back(first.at(length - 1));
		cout << codifica << endl;
		length++;
	}
	double raggio = top - low;
	raggio /= 2.0;
	raggio += low;
	//aggiunta delle due cifre finali
	string asd = to_string(raggio);
	asd.erase(0, length - 1);
	codifica.append(asd, 0, 2);
	return codifica;
}

#endif // CODIFICA_H