#include "encodingfunction.h"

void setOccurrences(ifstream& is, uint_32& totInputSymbols, array<uint_32, 256>& symbolsOccurrences, bool isTextFile){
	// mi assicuro che l'array sia inizializzato a zero
	symbolsOccurrences.fill(0);

	byte symbol;
	while (is.get(reinterpret_cast<char&>(symbol))){ // gestione ritorno a capo (è presente uno spazio in più)		
		if (symbol == 0x0D && isTextFile)
			is.get();
		totInputSymbols++;
		symbolsOccurrences[symbol]++;
	}
}

void setProbability(vector<f_couple>& floatCouple, uint_32& totInputSymbols, array<uint_32, 256>& symbolsOccurrences){
	for (uint_32 i = 0; i < 256; ++i){
		if (symbolsOccurrences[i] != 0)
			floatCouple.push_back(f_couple(i, (float_64)symbolsOccurrences[i] / (float_64)totInputSymbols));
	}
}

void rounding(vector<f_couple>& floatCouple, vector<u_couple>& unsignedCouple){
	bool HigherThanOne = true; // flag per verificare il caso della probabilità che sfora 1
	uint_32 countProbability = 0;   // conta la probabilità

	while (true){
		for (auto it = floatCouple.begin(); it != floatCouple.end(); ++it){
			float_64 fProbability = it->_fa;
			fProbability = fProbability * 100000;
			// variabile dove è salvato il valore senza virgola della prob
			uint_32 uProbability = (uint_32)fProbability;
			// livellamento delle probabilità dei valori per evitare errori di calcolo
			if (HigherThanOne){
				if (uProbability < 100){
					uProbability = 100;
				}
				if (uProbability > 3000){
					uProbability = 3000;
				}
			}
			// ci entriamo se il livellamento supera 1 di probabilità totale.
			// tutti i valori avranno uguale probabilità
			else
				uProbability = 100000 / floatCouple.size();
			countProbability += uProbability;
			// inserimento della probabilità corretta
			unsignedCouple.push_back(u_couple(it->_b, uProbability));
		}
		// ci entriamo se il livellamento supera 1 di probabilità totale.
		// tutti i valori avranno uguale probabilità
		if (countProbability > 100000){
			HigherThanOne = false;
			unsignedCouple.clear();
			countProbability = 0;
		}
		else break;
	}
}

void setRange(vector<f_couple>& floatCouple, vector<u_couple>& unsignedCouple){
	uint_32 probRange = 0;
	for (auto it = unsignedCouple.begin(); it != unsignedCouple.end(); ++it){
		it->_Fa = probRange;
		probRange += it->_fa;
		// cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << endl;
	}
	// correzione valore finale per arrivare a 1 di probabilità
	floatCouple.at(floatCouple.size() - 1)._fa = 100000 - floatCouple.at(floatCouple.size() - 1)._Fa;
}

uint_32 inputSize(ifstream& is){
	uint_32 inputLength;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(0, ios::end);
	inputLength = uint_32(is.tellg());
	is.seekg(ios_base::beg); // Torno all'inizio
	return inputLength;
}

void headerCreation(vector<u_couple>& unsignedCouple, uint_32& totInputSymbols, ofstream& os){
	// scrivo in output lunghezza file iniziale
	os.write(reinterpret_cast<char*>(&totInputSymbols), sizeof(uint_32));

	// scrivo in output numero simboli ascii con probabilità maggiore di zero
	byte numCouple = unsignedCouple.size();
	os.write(reinterpret_cast<char*>(&numCouple), 1);

	// stampiamo per ogni simbolo il valore della probabilità senza virgola
	for (auto it = unsignedCouple.begin(); it != unsignedCouple.end(); ++it){
		os << it->_b;
		uint_32 uProb = (uint_32)round(it->_fa);
		os.write(reinterpret_cast<char*>(&uProb), 3); //non sono necessari più di 3 byte
	}
}

void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top){
	// calcoli valori top,range e low
	// arrotondamento in alto per il low e in basso per il range
	low = uint_32(low + ceil((float_64)(start)*((float_64)range / 100000)));
	range = uint_32(((float_64)(size)*((float_64)range / 100000)));
	top = low + range;
}

void emit_digit(uint_32& low, bitwriter& bw){
	// shift del low e uscita della cifra
	uint_32 lowU = (uint_32)round(low) % 100000000;
	uint_32 out = low / 100000000;
	bw(out, 4);
	low = lowU * 10;
}

void encode_symbol(byte b, vector<u_couple>& unsignedCouple, uint_32& low, uint_32& range, uint_32& top, ofstream& os, bitwriter& bw){
	for (auto it = unsignedCouple.begin(); it != unsignedCouple.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa, low, range, top);
			// cout << "low: " << low << "\t range: " << range << "\t top: " << top << endl;
			// se la prima cifra del low e del top sono uguali la sputo fuori
			while (((uint_32)low / 100000000) == ((uint_32)top / 100000000)){
				emit_digit(low, bw);
				uint_32 topU = (uint_32)round(top) % 100000000;
				top = topU * 10;
				range = top - low;
			}
			// modifica per range bassi disponibili
			if (range < 1000){
				emit_digit(low, bw);
				emit_digit(low, bw);
				range = 1000000000 - low;
			}
			return;
		}
	// cout << "Symbol not found!";
}

void encodeAlgorithm(ifstream& is, ofstream& os, vector<u_couple>& unsignedCouple){
	// valori per l'encoding
	uint_32 range = uint_32(pow(10.0, 9.0));
	uint_32 low = 0;
	uint_32 up = range;
	byte b;

	// il bitwriter serve per scrivere in output le cifre da 0 a 9, quindi bastano 4 bit per cifra per codificarle
	bitwriter bw(os);

	while (is.get(reinterpret_cast<char&>(b)))
		encode_symbol(b, unsignedCouple, low, range, up, os, bw);

	// variabili per i calcoli successivi
	uint_32 difference = 1;
	float_64 tempLow;
	float_64 tempTop;
	float_64 cont = 0.0;

	// variabili d'appoggio, necessarie entrambe per il caso particolare che lo zero sia prima cifra di uno dei due
	tempLow = low;
	tempTop = up;

	// cerchiamo l'ordine di grandezza di low e top
	while (tempLow > 10 || tempTop > 10){
		tempLow /= 10;
		tempTop /= 10;
		cont++;
	}

	// calcolo quante cifre ha il low e salvo in contLow per il for successivo
	tempLow = 0.0;
	uint_32 contLow = 0;
	while (difference == 1){
		tempLow = low / pow(10.0, cont);
		difference = (uint_32)(up / (uint_32)pow(10, cont) - (uint_32)tempLow);
		cont--;
		contLow++;
	}

	// calcolo valore finale da aggiungere al file
	uint_32 final = (uint_32)(tempLow + difference / 2);
	//cout << "finale:" << final << endl;

	// estrazione cifre finali e scrittura tramite bitwriter sul file
	uint_32 lastDigit;
	for (uint_32 i = 0; i < contLow; ++i){
		lastDigit = (final *(uint_32)pow(10, i)) % (uint_32)pow(10, contLow) / (uint_32)pow(10, contLow - 1);
		bw(lastDigit, 4);
	}

	// per scrivere non solo gli uno finali del flush ma anche l ultimo valore se non riempe il byte
	bw.flush();
}

void printCompressionRatio(ofstream& os, uint_32& inputLength){
	os.seekp(0, ios::end);
	uint_32 encodedLength = uint_32(os.tellp());
	cout << "Size of InputFile is:\t " << inputLength << " bytes.";
	cout << endl << "Size of EncodedFile is:\t " << encodedLength << " bytes."; // Ho tutte le dimensioni, calcolo rapporto compressione
	cout << endl << endl << "Compress Ratio:\t " << 100 - ((float_64)encodedLength / inputLength * 100) << "%" << endl << endl;
}