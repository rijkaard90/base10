#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<array>
#include<vector>
#include<algorithm>
#include<functional>

using namespace std;
typedef unsigned char byte;

struct coppia {
	byte _b;
	double _prob;
	double _start;

	coppia(byte b, double prob, double start=0) : _b(b), _prob(prob),_start(start) {}
	bool operator> (const coppia& c) const { return _prob > c._prob; }
};

void encode(int start, int size, unsigned& low, unsigned& range){
	int total = 10;
	// adjust the range based on the symbol interval
	range /= total;
	low += start * range;
	range *= size;
}

void encode_symbol(byte b,vector<coppia>& x,unsigned& low,unsigned& range){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_start, it->_prob,low,range);
			return;
		}
	cout << "symbol not found!";
}

int main(){
	array<unsigned int, 256> myarray;
	vector<coppia> coppie;

	ifstream is("prova.txt", ifstream::binary);
	if (!is) return -1;

	//mi assicuro che l'array sia inizializzato a zero
	for (auto it = myarray.begin(); it != myarray.end(); ++it){
		*it = 0;
	}
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
		it->_start = prob_range;
		prob_range += it->_prob;
		//cout << "symbol: " << it->_b << "\t probability: " << it->_cnt << "\n";
		cout << "symbol: " << it->_b << "\t start_range: " << it->_start << "\t probability: " << it->_prob << "\n";
	}

	/* prova encoding aabaz di wikipedia*/
	unsigned range = 100000;
	unsigned low = 0;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;
	while (is.get(reinterpret_cast<char&>(b)))
		encode_symbol(b,coppie,low,range);
	cout << "low: " << low << "\t top: " << range+low << "\n";

}
