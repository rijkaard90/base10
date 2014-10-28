#include "encodingfunction.h"

void encode(double start, double size, double& low, double& range, double& top){
	low = low + start * range;
	top = (low + size* range);
	top = (unsigned)round(top);
	low = (unsigned)round(low);
	range = top - low;
}

void emit_digit(double& low, bitwriter& bw){
	unsigned lowU = (unsigned)round(low) % 100000;
	unsigned out = low / 100000;
	bw(out, 4);
	low = lowU * 10;
}

void encode_symbol(byte b, vector<coppia>& x, double& low, double& range, double& top, ofstream& os, bitwriter& bw){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa, low, range, top);
			//verifica di ogni passaggio
			cout << "low: " << low << "\t range: " << range << "\t top: " << top << endl;
			//se la prima cifra del low e del top sono uguali la sputo fuori
			while (((unsigned)low / 100000) == ((unsigned)top / 100000)){
				//emit_digit(low, os);
				emit_digit(low, bw);
				unsigned topU = (unsigned)round(top) % 100000;
				top = topU * 10;
				range = top - low;
			}
			//modifica per range bassi disponibili
			if (range < 1000){
				emit_digit(low, bw);
				emit_digit(low, bw);
				range = 1000000.0 - low;
				top = low + range;
			}
			return;
		}
	cout << "Symbol not found!";
}