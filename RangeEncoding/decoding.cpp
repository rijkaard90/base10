#include "decoding.h"
#include "decodingfunction.h"

void decoding(ifstream& is, ofstream& os){
	
	vector<coppia2> Dcoppie;

	/*!< Header extraction. */

	//leggo quanti caratteri devo decodificare nel primo byte del file
	uint_32 n_caratteri = read_little_endian(is, sizeof(uint_32));

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

	/*!< Header end. */

	//inserimento _Fa
	uint_32 prob = 0;
	for (auto it = Dcoppie.begin(); it != Dcoppie.end(); ++it){
		it->_Fa = prob;
		prob += it->_fa;
		cout << "symbol: " << (uint_32)it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}

	//bitreader per leggere le cifre salvate in 4 bit
	bitreader br(is);

	//verifica n_caratteri
	cout << n_caratteri << '\n';

	/* valori per il decoding */
	uint_32 rangecont = pow(10.0, 9.0);
	uint_32 sopra = rangecont; 
	uint_32 sotto = 0;
	bool flagEndCode = false;
	uint_32 contacaratteri = 8;

	//inizializzo variabile controllo con le prime 9 cifre del file input perchè il range è 10^9
	//questa variabile rappresenterà il valore che sarà cercato nel range tra sotto e sopra
	uint_32 controllo = 0;
	for (uint_32 i = 0; i < 9; ++i)
		controllo = controllo * 10 + br(4);
	cout << endl << "controllo: " << controllo << endl;
	
	/* inizio decodifica vera e propria*/
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

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}