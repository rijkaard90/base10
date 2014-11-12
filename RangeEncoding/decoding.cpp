#include "decoding.h"
#include "decodingfunction.h"

void decoding(char* Type, ifstream& is, ofstream& os){
	
	vector<coppia2> Dcoppie;
	uint_32 n_caratteri;

	//Header extraction
	headerExtraction(is, Dcoppie, n_caratteri);

	//inserimento probabilit� e range di partenza
	setRangeValue(Dcoppie);

	//verifica n_caratteri
	cout << n_caratteri << '\n';	

	//bitreader per leggere le cifre salvate in 4 bit
	bitreader br(is);
	
	//inizializzo variabile controllo con le prime 9 cifre del file input perch� il range � 10^9
	//questa variabile rappresenter� il valore che sar� cercato nel range tra sotto e sopra
	uint_32 controllo = 0;
	setControl(is, controllo, br);

	bitwriter bw(os);

	//decodifica vera e propria
	decodeAlgorithm(os, n_caratteri, controllo, Dcoppie, br, bw,Type);	

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}