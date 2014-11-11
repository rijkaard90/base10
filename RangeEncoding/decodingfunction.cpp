#include "decodingfunction.h"

uint_32 read_little_endian(ifstream& is, uint_32 nByte){
	uint_32 result = 0;

	for (uint_32 i = 0; i < nByte; i++)
		result += static_cast<uint_32>(is.get() << (8 * i));

	return result;
}

void convert_and_shift(uint_32& sotto, uint_32& sopra, uint_32& contacaratteri,
	uint_32& rangecont, uint_32& controllo, bitreader& br, bool& flagEndCode){

	//variabile d'appoggio e uso del long long per non perdere precisione
	uint_64 sottoP = sotto;
	uint_64 sopraP = sopra;
	sottoP *= 10;
	sopraP *= 10;

	//calcolo modulo
	sotto = sottoP % (uint_64)(pow(10.0, contacaratteri + 1));
	sopra = sopraP % (uint_64)(pow(10.0, contacaratteri + 1));
	rangecont = sopra - sotto;
	controllo = (controllo % (uint_32)(pow(10.0, contacaratteri))) * 10;

	//controllo fine codice
	uint_32 tmp = br(4);
	if (tmp <= 9 && !flagEndCode)
		controllo += tmp;
	else flagEndCode = true;
}

void decodeAlgorithm(ofstream& os, uint_32& n_caratteri, uint_32& controllo, vector<coppia2>& Dcoppie, bitreader& br){
	//valori per il decoding
	uint_32 rangecont = pow(10.0, 9.0);
	uint_32 sopra = rangecont;
	uint_32 sotto = 0;
	bool flagEndCode = false;
	uint_32 contacaratteri = 8;

	//ciclo per ogni carattere
	while (n_caratteri > 0){

		//calcolo della probabilità rispetto al range
		uint_32 prob = round((float_64)(controllo - sotto) / (rangecont)* 100000);

		//ciclo per controllare a quale simbolo di riferisce la probabilità
		for (auto it = Dcoppie.begin(); it != Dcoppie.end(); ++it){
			if (prob >= it->_Fa && prob < (it->_fa + it->_Fa)){
				//cout << it->_b;  //<< "\t";
				os << it->_b;
				//per riscrivere il ritorno a capo, necessaria aggiunta
				if (it->_b == 0x0D){
					byte spazio = 0x0A;
					os << spazio;
				}
				//aggiornamento valori
				sotto = sotto + ceil((float_64)(it->_Fa)*((float_64)rangecont / 100000));
				rangecont = ((float_64)(it->_fa)*((float_64)rangecont / 100000));
				sopra = sotto + rangecont;
				//cout << sotto << "\t" << rangecont << "\t" << sopra << endl;
				break;
			}
		}

		//cout << controllo << "\t" << sotto << "\t" << rangecont << "\t" << sopra << "\t" << n_caratteri << endl;

		//se c'è da shiftare il top e low aggiorno
		while (((sotto / (uint_32)(pow(10.0, contacaratteri))) == (sopra / (uint_32)(pow(10.0, contacaratteri)))))
			convert_and_shift(sotto, sopra, contacaratteri, rangecont, controllo, br, flagEndCode);

		//caso particolare nel caso di range piccoli
		if (rangecont < 1000){
			//verifica quando entra nel caso particolare
			//cout << "entra";
			convert_and_shift(sotto, sopra, contacaratteri, rangecont, controllo, br, flagEndCode);
			convert_and_shift(sotto, sopra, contacaratteri, rangecont, controllo, br, flagEndCode);
			rangecont = 1000000000 - sotto;
			sopra = sotto + rangecont;
		}

		//cout << controllo << "\t" << sotto << "\t" << rangecont << "\t" << sopra << endl;
		n_caratteri--;
	}
}

void rangeFinder(ifstream& is, uint_32& controllo, bitreader& br){
	for (uint_32 i = 0; i < 9; ++i)
		controllo = controllo * 10 + br(4);
	cout << endl << "controllo: " << controllo << endl;
}

void headerExtraction(ifstream& is, vector<coppia2>& Dcoppie, uint_32& n_caratteri){
	//leggo quanti caratteri devo decodificare nel primo byte del file
	n_caratteri = read_little_endian(is, sizeof(uint_32));

	//leggo numero totale di simboli presenti nell'header con probabilità maggiore di zero
	uint_32 n_simboli = is.get();
	//controllo errore nel caso siano presenti tutti i valori del codice ascii.
	if (n_simboli == 0) n_simboli = 256;
	cout << endl << endl << "n_caratteri: " << n_caratteri << endl << "n_simboli: " << n_simboli << endl;

	//leggo simboli con probabilità e ricreo array coppie
	for (uint_32 i = 0; i < n_simboli; i++){
		byte valore = is.get();
		//leggo 3 byte per come è impostato l'header
		uint_32 u_prob = read_little_endian(is, 3);
		Dcoppie.push_back(coppia2(valore, u_prob));
	}
}

void probRange(vector<coppia2>& Dcoppie){
	uint_32 prob = 0;
	for (auto it = Dcoppie.begin(); it != Dcoppie.end(); ++it){
		it->_Fa = prob;
		prob += it->_fa;
		cout << "symbol: " << (uint_32)it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}
}