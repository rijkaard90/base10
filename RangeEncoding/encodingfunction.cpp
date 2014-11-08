#include "encodingfunction.h"

void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top){
	low = low + ceil((float_64)(start)*((float_64)range / 10000));
	range = ((float_64)(size)*((float_64)range / 10000));
	top = low + range;
}

void emit_digit(uint_32& low, bitwriter& bw){
	uint_32 lowU = (uint_32)round(low) % 100000;
	uint_32 out = low / 100000;
	bw(out, 4);
	low = lowU * 10;
}

void encode_symbol(byte b, vector<coppia2>& x, uint_32& low, uint_32& range, uint_32& top, ofstream& os, bitwriter& bw){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa, low, range, top);
			//verifica di ogni passaggio
			//cout << "low: " << low << "\t range: " << range << "\t top: " << top << endl;
			//se la prima cifra del low e del top sono uguali la sputo fuori
			while (((uint_32)low / 100000) == ((uint_32)top / 100000)){
				//emit_digit(low, os);
				emit_digit(low, bw);
				uint_32 topU = (uint_32)round(top) % 100000;
				top = topU * 10;
				range = top - low;
			}
			//modifica per range bassi disponibili
			if (range < 1000){
				emit_digit(low, bw);
				emit_digit(low, bw);
				range = 1000000 - low;
			}
			return;
		}
	cout << "Symbol not found!";
}