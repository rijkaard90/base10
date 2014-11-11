#include "encodingfunction.h"

void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top){
	//calcoli valori top,range e low
	//arrotondamento in alto per il low e in basso per il range
	low = low + ceil((float_64)(start)*((float_64)range / 100000));
	range = ((float_64)(size)*((float_64)range / 100000));
	top = low + range;
}

void emit_digit(uint_32& low, bitwriter& bw){
	//shift del low e uscita della cifra
	uint_32 lowU = (uint_32)round(low) % 100000000;
	uint_32 out = low / 100000000;
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
			while (((uint_32)low / 100000000) == ((uint_32)top / 100000000)){
				emit_digit(low, bw);
				uint_32 topU = (uint_32)round(top) % 100000000;
				top = topU * 10;
				range = top - low;
			}
			//modifica per range bassi disponibili
			if (range < 1000){
				emit_digit(low, bw);
				emit_digit(low, bw);
				range = 1000000000 - low;
			}
			return;
		}
	//cout << "Symbol not found!";
}

void calcOccurrences(ifstream& is, uint_32& tot_symbol, array<uint_32, 256>& myarray){
	//mi assicuro che l'array sia inizializzato a zero
	myarray.fill(0);

	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){ //gestione ritorno a capo (è presente uno spazio in più)		
		if (tmp == 0x0D)
			is.get();
		tot_symbol++;
		myarray[tmp]++;
	}
}

void calcProbability(vector<coppia>& coppie, uint_32& tot_symbol, array<uint_32, 256>& myarray){
	for (uint_32 i = 0; i < 256; ++i){
		if (myarray[i] != 0)
			coppie.push_back(coppia(i, (float_64)myarray[i] / (float_64)tot_symbol));
	}
}

void rounding(vector<coppia>& coppie, vector<coppia2>& coppie2){
	uint_32 flagSfora = true; //flag per verificare il caso della probabilità che sfora 1
	uint_32 contaprob = 0;   // conta la probabilità

	while (true){
		for (auto it = coppie.begin(); it != coppie.end(); ++it){
			float_64 x = it->_fa;
			x = x * 100000;
			//variabile dove è salvato il valore senza virgola della prob
			uint_32 u = x;
			//livellamento delle probabilità dei valori per evitare errori di calcolo
			if (flagSfora){
				if (u < 100){
					u = 100;
				}
				if (u > 3000){
					u = 3000;
				}
			}
			//ci entriamo se il livellamento supera 1 di probabilità totale.
			//tutti i valori avranno uguale probabilità
			else
				u = 100000 / coppie.size();
			contaprob += u;
			//inserimento della probabilità corretta
			coppie2.push_back(coppia2(it->_b, u));
		}
		//ci entriamo se il livellamento supera 1 di probabilità totale.
		//tutti i valori avranno uguale probabilità
		if (contaprob > 100000){
			flagSfora = false;
			coppie2.clear();
			contaprob = 0;
		}
		else break;
	}
}

void rangeAssignment(vector<coppia>& coppie, vector<coppia2>& coppie2){
	uint_32 prob_range = 0;
	for (auto it = coppie2.begin(); it != coppie2.end(); ++it){
		it->_Fa = prob_range;
		prob_range += it->_fa;
		cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << endl;
	}

	//correzione valore finale per arrivare a 1 di probabilità
	coppie.at(coppie.size() - 1)._fa = 100000 - coppie.at(coppie.size() - 1)._Fa;
}

void headerCreation(vector<coppia2>& coppie2, uint_32& tot_symbol, ofstream& os){
	//scrivo in output lunghezza file iniziale
	os.write(reinterpret_cast<char*>(&tot_symbol), sizeof(uint_32));

	//scrivo in output numero simboli ascii con probabilità maggiore di zero
	byte numcoppie = coppie2.size();
	os.write(reinterpret_cast<char*>(&numcoppie), 1);

	//stampiamo per ogni simbolo il valore della probabilità senza virgola
	for (auto it = coppie2.begin(); it != coppie2.end(); ++it){
		os << it->_b;
		uint_32 u = round(it->_fa);
		os.write(reinterpret_cast<char*>(&u), 3); //non sono necessari più di 3 byte
	}
}

void encodeAlgorithm(ifstream& is, ofstream& os, vector<coppia2>& coppie2){
	// valori per l'encoding
	uint_32 range = pow(10.0, 9.0);
	uint_32 low = 0;
	uint_32 up = range;
	byte b;

	//il bitwriter serve per scrivere in output le cifre da 0 a 9, quindi bastano 4 bit per cifra per codificarle
	bitwriter bw(os);

	while (is.get(reinterpret_cast<char&>(b)))
		encode_symbol(b, coppie2, low, range, up, os, bw);

	//variabili per i calcoli successivi
	uint_32 differenza = 1;
	float_64 appoggio;
	float_64 appoggioU;
	float_64 cnt = 0.0;

	//variabili d'appoggio, necessarie entrambe per il caso particolare che lo zero sia prima cifra di uno dei due
	appoggio = low;
	appoggioU = up;

	//cerchiamo l'ordine di grandezza di low e top
	while (appoggio > 10 || appoggioU >10){
		appoggio /= 10;
		appoggioU /= 10;
		cnt++;
	}

	//calcolo quante cifre ha il low e salvo in contLow per il for successivo
	appoggio = 0.0;
	uint_32 contLow = 0;
	while (differenza == 1){
		appoggio = low / pow(10.0, cnt);
		differenza = (uint_32)(up / (uint_32)pow(10, cnt) - (uint_32)appoggio);
		cnt--;
		contLow++;
	}

	//calcolo valore finale da aggiungere al file
	uint_32 final = (uint_32)(appoggio + differenza / 2);
	cout << "finale:" << final << endl;

	//estrazione cifre finali e scrittura tramite bitwriter sul file
	uint_32 x;
	for (uint_32 i = 0; i < contLow; ++i){
		x = (final *(uint_32)pow(10, i)) % (uint_32)pow(10, contLow) / (uint_32)pow(10, contLow - 1);
		bw(x, 4);
	}

	//per scrivere non solo gli uno finali del flush ma anche l ultimo valore se non riempe il byte
	bw.flush();
}