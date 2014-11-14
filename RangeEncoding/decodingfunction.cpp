#include "decodingfunction.h"

uint_32 read_little_endian(ifstream& is, uint_32 nByte){
	uint_32 result = 0;
	for (uint_32 i = 0; i < nByte; i++)
		result += static_cast<uint_32>(is.get() << (8 * i));

	return result;
}

void headerExtraction(ifstream& is, vector<u_couple>& decodingCouple, uint_32& numSymbolsFirstByte){
	// leggo quanti caratteri devo decodificare nel primo byte del file
	numSymbolsFirstByte = read_little_endian(is, sizeof(uint_32));

	// leggo numero totale di simboli presenti nell'header con probabilità maggiore di zero
	uint_32 numSymbolsHeader = is.get();
	// controllo errore nel caso siano presenti tutti i valori del codice ascii.
	if (numSymbolsHeader == 0) numSymbolsHeader = 256;
	// cout << endl << endl << "n_simboli: " << n_simboli << endl;

	// leggo simboli con probabilità e ricreo array coppie
	for (uint_32 i = 0; i < numSymbolsHeader; i++){
		byte value = is.get();
		// leggo 3 byte per come è impostato l'header
		uint_32 uProb = read_little_endian(is, 3);
		decodingCouple.push_back(u_couple(value, uProb));
	}
}

void setRangeValue(vector<u_couple>& decodingCouple){
	uint_32 prob = 0;
	for (auto it = decodingCouple.begin(); it != decodingCouple.end(); ++it){
		it->_Fa = prob;
		prob += it->_fa;
		// cout << "symbol: " << (uint_32)it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}
}

void setControl(ifstream& is, uint_32& control, bitreader& br){
	for (uint_32 i = 0; i < 9; ++i)
		control = control * 10 + br(4);
	// cout << endl << "Starting control: " << controllo << endl;
}

void shift_and_control(uint_32& below, uint_32& over, uint_32& contCharacter,
	uint_32& rangeCont, uint_32& control, bitreader& br, bool& flagEndCode){

	// variabile d'appoggio e uso di 64bit per non perdere precisione
	uint_64 belowTemp = below;
	uint_64 overTemp = over;
	belowTemp *= 10;
	overTemp *= 10;

	// calcolo modulo
	below = uint_32(belowTemp % (uint_64)(pow(10.0, contCharacter + 1)));
	over = uint_32(overTemp % (uint_64)(pow(10.0, contCharacter + 1)));
	rangeCont = over - below;
	control = (control % (uint_32)(pow(10.0, contCharacter))) * 10;

	// controllo fine codice
	uint_32 tmp = br(4);
	if (tmp <= 9 && !flagEndCode)
		control += tmp;
	else flagEndCode = true;
}

void decodeAlgorithm(ofstream& os, uint_32& numSymbolsFirstByte, uint_32& control,
	vector<u_couple>& decodingCouple, bitreader& br, bitwriter& bw, bool isTextFile){
	// valori per il decoding
	uint_32 rangeCont = uint_32(pow(10.0, 9.0));
	uint_32 over = rangeCont;
	uint_32 below = 0;
	bool flagEndCode = false;
	uint_32 contCharacter = 8;

	// ciclo per ogni carattere
	while (numSymbolsFirstByte > 0){

		// calcolo della probabilità rispetto al range
		uint_32 prob = uint_32(round((float_64)(control - below) / (rangeCont)* 100000));

		// ciclo per controllare a quale simbolo di riferisce la probabilità
		for (auto it = decodingCouple.begin(); it != decodingCouple.end(); ++it){
			if (prob >= it->_Fa && prob < (it->_fa + it->_Fa)){
				// cout << it->_b;  //<< "\t";
				char out = it->_b;
				bw(out, 8);
				// per riscrivere il ritorno a capo, necessaria aggiunta (file di testo)
				if (it->_b == 0x0D && isTextFile){
					byte space = 0x0A;
					os << space;
				}
				// aggiornamento valori
				below = uint_32(below + ceil((float_64)(it->_Fa)*((float_64)rangeCont / 100000)));
				rangeCont = uint_32(((float_64)(it->_fa)*((float_64)rangeCont / 100000)));
				over = below + rangeCont;
				//cout << below << "\t" << rangeCont << "\t" << over << endl;
				break;
			}
		}

		// cout << control << "\t" << below << "\t" << rangeCont << "\t" << over << "\t" << numSymbolsFirstByte << endl;

		// se c'è da shiftare il top e low aggiorno
		while (((below / (uint_32)(pow(10.0, contCharacter))) == (over / (uint_32)(pow(10.0, contCharacter)))))
			shift_and_control(below, over, contCharacter, rangeCont, control, br, flagEndCode);

		// caso particolare nel caso di range piccoli
		if (rangeCont < 1000){
			// verifica quando entra nel caso particolare
			// cout << "entra";
			shift_and_control(below, over, contCharacter, rangeCont, control, br, flagEndCode);
			shift_and_control(below, over, contCharacter, rangeCont, control, br, flagEndCode);
			rangeCont = 1000000000 - below;
			over = below + rangeCont;
		}

		// cout << control << "\t" << below << "\t" << rangeCont << "\t" << over << endl;
		numSymbolsFirstByte--;
	}
}

void decodedSize(ofstream& os){
	os.seekp(0, ios::end);
	uint_32 decodedLength = uint_32(os.tellp()); // calcolo dimensione file decodificato
	cout << "Size of DecodedFile is:\t " << decodedLength << " bytes." << endl << endl << endl;
}