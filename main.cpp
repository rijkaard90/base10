#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<array>
#include<vector>
#include<algorithm>
#include<functional>
#include<math.h>
#include<string>


using namespace std;
typedef unsigned char byte;

struct coppia {
	byte _b;
	double _fa; //prob
	double _Fa; //start

	coppia(byte b, double prob, double start=0) : _b(b), _fa(prob),_Fa(start) {}
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

void encode(double start, double size, double& low, double& range){
	// adjust the range based on the symbol interval
	//double total = 100;
	//range /= total;
	//ATTENZIONE all'arrotondamento che puo essere sbagliato, DA RIVEDERE
	low = (unsigned long)(low + start * range);
	range = range* size;
	range = (unsigned long)(range + 0.99);
	//low = (unsigned long)(low + start * range);
	//range = (unsigned long)(range* size);
}

void encode_symbol(byte b, vector<coppia>& x, double& low, double& range){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa,low,range);
			//verifica di ogni passaggio
			cout << "low: " << low << "\t top: " << range + low << endl;
			//modifica per range bassi disponibili
			if (range < 100.0){
				range *= 10.0;
				low *= 10.0;
			}
			return;
		}
	cout << "symbol not found!";
}

void syntax() {
	cout << "Syntax:" << endl;
	cout << "Range encoding <input filename> <output filename>" << endl;
}

string trasforma_string(double top, double low, string first, string last){
	//comparazione top e low per trovare le cifre sufficienti per la codifica
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

	double raggio=top - low;
	raggio /= 2;
	raggio += low;
	//aggiunta delle due cifre finali
	string asd = to_string(raggio);
	asd.erase(0, length - 1);
	codifica.append(asd, 0, 2);
	return codifica;
}

int main(int argc, char *argv[]){

	array<unsigned int, 256> myarray;
	vector<coppia> coppie;

	if (argc != 3) {
		syntax();
		return EXIT_FAILURE;
	}

	string InputFileName = argv[1];
	string OutputFileName = argv[2];

	ifstream is(InputFileName, ifstream::binary);
	if (!is) return -1;

	ofstream os(OutputFileName, ofstream::binary);
	if (!os) return -1;

	//mi assicuro che l'array sia inizializzato a zero
	myarray.fill(0);

	/* SPUNTONI DA TOGLIERE
	//calcolo le ricorrenze di ogni simbolo
	unsigned int tot = 0;//numero totali simboli letti dallo stream
	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){
		tot++;
		myarray[tmp]++;
	}

	for (int i = 0; i<256; ++i)
		if (myarray[i] != 0)
			coppie.push_back(coppia(i, myarray[i] / (double)tot * 10));
			//cout << i << '\t' << myarray[i] << '\n';

	sort(coppie.begin(), coppie.end(), greater<coppia>());
	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	double prob_range = 0;
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		it->_fa = prob_range;
		prob_range += it->_Fa;
		//cout << "symbol: " << it->_b << "\t probability: " << it->_cnt << "\n";
		cout << "symbol: " << it->_b << "\t start_range: " << it->_fa << "\t probability: " << it->_Fa << "\n";
	}
	SPUNTONI DA TOGLIERE*/ 

	/* prova encoding */
	double range = pow(10.0, 3.0);
	double low = 0;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;
	
	//prova esempio pdf
	coppie.push_back(coppia('K', 0.10, 0.0));
	coppie.push_back(coppia('L', 0.21, 0.10));
	coppie.push_back(coppia('M', 0.27, 0.31));
	coppie.push_back(coppia('N', 0.42, 0.58));
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		cout << it->_b << " " << it->_Fa << " " << it->_fa << endl;
	}

	//codifica vera e propria
	while (is.get(reinterpret_cast<char&>(b)))
		encode_symbol(b,coppie,low,range);
	double top = range + low;
	
	cout << "finale " << "low: " << low << "\t top: " << top << endl;

	//trasformazione in stringhe
	string first = to_string(low);
	string last = to_string(top);
	string codifica=trasforma_string(top,low,first,last);
	//FARE CASO CIFRE UGUALI E NON NE PRENDE DUE MA SOLO UNA!!//

	//stampa
	cout << first << endl << last << endl;
	cout << codifica << endl;
	os << codifica;
}
