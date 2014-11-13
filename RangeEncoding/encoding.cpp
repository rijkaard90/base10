#include "encoding.h"
#include "encodingfunction.h"

void encoding(ifstream& is, ofstream& os, bool isTextFile){

	array<uint_32, 256> myarray;
	vector<coppia> coppie;
	vector<coppia2> coppie2;
	uint_32 tot_symbol = 0; //numero totali simboli letti dallo stream

	//calcolo le ricorrenze di ogni simbolo
	setOccurrences(is, tot_symbol, myarray, isTextFile);

	//calcolo delle probabilità e inserimento nel vettore 
	setProbability(coppie, tot_symbol, myarray);		

	//arrotondamento a 5 cifre decimali della probabilità
	rounding(coppie, coppie2);		

	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	setRange(coppie, coppie2);	
	
	//calcolo dimensione input file
	uint_32 inputLength = inputSize(is);

	//Header creation
	headerCreation(coppie2, tot_symbol, os);
	
	//codifica vera e propria
	encodeAlgorithm(is, os, coppie2);

	//calcolo dimensione file codificato e rapporto di compressione
	printCompressionRatio(os, inputLength);

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}