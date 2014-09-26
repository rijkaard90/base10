#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<array>
#include<vector>
#include<algorithm>
#include <functional>

using namespace std;
typedef unsigned char byte;

struct coppia {
	byte _b;
	double _prob;
	double _start;

	coppia(byte b, double prob, double start=0) : _b(b), _prob(prob),_start(start) {}
	bool operator> (const coppia& c) const { return _prob > c._prob; }
};

void encode_symbol(byte b,vector<coppia>& x,unsigned& low,unsigned& range){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_start, it->_prob,low,range);
			break;
		}
	cout << "symbol not found!";
}
//da finire
void encode(int start, int size, unsigned& low, unsigned& range){
	int total = 10;


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
	char tmp;
	unsigned int tot= 0;//numero totali simboli letti dallo stream
	while (is.get(tmp)){
		tot++;
		unsigned char a = (unsigned char)tmp;
		myarray[a]++;
	}
 
	for (int i = 0; i<256; ++i)
		if (myarray[i] != 0){
			//cout << i << '\t' << myarray[i] << '\n';
			coppie.push_back(coppia(i, myarray[i] / (double)tot * 10));
		}

	sort(coppie.begin(), coppie.end(), greater<coppia>());
	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	int prob_range = 0;
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

}