#include "encoding.h"
#include "encodingfunction.h"

void encoding(ifstream& is, ofstream& os, bool isTextFile){

	array<uint_32, 256> symbolsOccurrences;
	vector<f_couple> floatCouple;
	vector<u_couple> unsignedCouple;
	uint_32 totInputSymbols = 0;

	// calcolo le ricorrenze di ogni simbolo
	setOccurrences(is, totInputSymbols, symbolsOccurrences, isTextFile);

	// calcolo delle probabilità e inserimento nel vettore 
	setProbability(floatCouple, totInputSymbols, symbolsOccurrences);

	// arrotondamento a 5 cifre decimali della probabilità
	rounding(floatCouple, unsignedCouple);

	// dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	setRange(floatCouple, unsignedCouple);
	
	// calcolo dimensione input file
	uint_32 inputLength = inputSize(is);

	// header creation
	headerCreation(unsignedCouple, totInputSymbols, os);
	
	// codifica vera e propria
	encodeAlgorithm(is, os, unsignedCouple);

	// calcolo dimensione file codificato e rapporto di compressione
	printCompressionRatio(os, inputLength);

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}