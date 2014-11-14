#include "decoding.h"
#include "decodingfunction.h"

void decoding(ifstream& is, ofstream& os, bool isTextFile){
	
	vector<u_couple> decodingCouple;
	uint_32 numSymbolsFirstByte;
	bitreader br(is); // bitreader per leggere le cifre salvate in 4 bit
	bitwriter bw(os); // bitwriter per scrivere le cifre salvate in 4 bit

	// header extraction
	headerExtraction(is, decodingCouple, numSymbolsFirstByte);

	// inserimento probabilità e range di partenza
	setRangeValue(decodingCouple);	
	
	// inizializzo variabile controllo con le prime 9 cifre del file input perchè il range è 10^9
	// questa variabile rappresenterà il valore che sarà cercato nel range tra sotto e sopra
	uint_32 control = 0;
	setControl(is, control, br);

	// decodifica vera e propria
	decodeAlgorithm(os, numSymbolsFirstByte, control, decodingCouple, br, bw, isTextFile);

	// calcolo dimensioni file ricreato dopo la codifica per controllo
	decodedSize(os);

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}