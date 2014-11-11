#include "decoding.h"
#include "decodingfunction.h"

void decoding(ifstream& is, ofstream& os){
	
	vector<coppia2> Dcoppie;
	uint_32 n_caratteri;

	//Header extraction
	headerExtraction(is, Dcoppie, n_caratteri);

	//inserimento probabilità e range di partenza
	probRange(Dcoppie);

	//verifica n_caratteri
	cout << n_caratteri << '\n';	

	//inizializzo variabile controllo con le prime 9 cifre del file input perchè il range è 10^9
	//questa variabile rappresenterà il valore che sarà cercato nel range tra sotto e sopra
	uint_32 controllo = 0;

	//bitreader per leggere le cifre salvate in 4 bit
	bitreader br(is);
	
	rangeFinder(is, controllo, br);

	//decodifica vera e propria
	decodeAlgorithm(os, n_caratteri, controllo, Dcoppie, br);	

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}