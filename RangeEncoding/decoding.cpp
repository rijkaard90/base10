#include "decoding.h"
#include "decodingfunction.h"

void decoding(ifstream& is, ofstream& os){
	
	vector<coppia> Dcoppie;

	//ESTRAZIONE HEADER

	//leggo quanti caratteri devo decodificare nel primo byte del file
	unsigned n_caratteri = read_little_endian(is, sizeof(unsigned));

	//leggo numero totale di simboli presenti nell'header con probabilità maggiore di zero
	byte n_simboli = is.get();
	cout << endl << endl << "n_caratteri: " << n_caratteri << endl << "n_simboli: " << n_simboli << endl;

	//leggo simboli con probabilità e ricreo array coppie
	for (unsigned i = 0; i < n_simboli; i++){
		byte valore = is.get();
		//leggo 3 byte per come è impostato l'header
		unsigned n_occorrenze = read_little_endian(is, 3);
		Dcoppie.push_back(coppia(valore, (double)n_occorrenze / (double)n_caratteri));
	}

	//inserimento _Fa
	double prob = 0.0;
	for (auto it = Dcoppie.begin(); it != Dcoppie.end(); ++it){
		it->_Fa = prob;
		prob += it->_fa;
		cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}

	//+1 perchè cosi nel while posso fermarmi a 0 e posso usare un unsigned invece di una variabile signed
	//n_caratteri++;

	//bitreader per leggere le cifre salvate in 4 bit
	bitreader br(is);

	cout << n_caratteri << '\n';
	unsigned totalrange = 1000000;
	double rangecont = 1000000.0;
	double sopraV = 1000000.0; //variabile in double per permettere migliore arrotondamento
	unsigned sopra = 1000000; //necessario unsigned per il valore finale di top e low e per il modulo
	double sottoV = 0.0;
	unsigned sotto = 0;
	bool flagRange = false;  //flag per controllare se shiftare o no il range
	bool flagShift = false;
	bool flagTry = false;
	bool flagEndCode = false;
	unsigned contacaratteri = 5;
	unsigned sopraw, sottow;

	//inizializzo controllo con le prime 6 cifre del file input perchè il range è 10^6
	unsigned controllo = 0;
	for (unsigned i = 0; i<6; ++i)
		controllo = controllo * 10 + br(4);
	cout << endl << "controllo: " << controllo << endl;

	//ciclo per ogni carattere
	while (n_caratteri > 0){

		sottow = sotto;
		sopraw = sopra;

		//caso particolare dove range piccolo e first digits diversi, necessario per il giusto calcolo successivo
		while (((sottow / (unsigned)(pow(10.0, contacaratteri))) != (sopraw / (unsigned)(pow(10.0, contacaratteri)))) && rangecont < 1000){
			sotto = sotto * 10;
			sotto = sotto % (unsigned)(pow(10.0, contacaratteri + 1));
			controllo = (controllo % (unsigned)(pow(10.0, contacaratteri))) * 10;
			controllo += br(4);
			rangecont = 1000000 - sotto;
			flagTry = true;
			cout << "\t" << "entra ";
		}

		//calcolo della probabilità rispetto al range
		double prob = (double)(controllo - sotto) / rangecont;

		//ciclo per controllare a quale simbolo di riferisce la probabilità
		for (auto it = Dcoppie.begin(); it != Dcoppie.end(); ++it){
			if (prob >= it->_Fa && prob < (it->_fa + it->_Fa)){
				cout << it->_b << "\t";
				os << it->_b;
				//per riscrivere il ritorno a capo, necessaria aggiunta
				if (it->_b == 0x0D){
					unsigned char spazio = 0x0A;
					os << spazio;
				}
				//aggiornamento valori !!ARROTONDAMENTO ERRATO
				sottoV = sotto + (it->_Fa * rangecont);
				rangecont = (rangecont * it->_fa);
				sopraV = sottoV + rangecont;
				//cout << sottoV << "\t" << rangecont << "\t" << sopraV << endl;
				break;
			}
		}

		//se c'è da shiftare il top e low aggiorno
		while (((sottow / (unsigned)(pow(10.0, contacaratteri))) == (sopraw / (unsigned)(pow(10.0, contacaratteri))))){			
			convert_and_shift(sottoV, sotto, sopraV, sopra, contacaratteri, rangecont, controllo, br, flagEndCode);
			flagShift = true;
			//aggiornamento valori per il while
			sottow = sottow % (unsigned)(pow(10.0, contacaratteri + 1));
			sottow = sottow * 10;
			sopraw = sopraw % (unsigned)(pow(10.0, contacaratteri + 1));
			sopraw = sopraw * 10;
		}

		//entra solo se è già entrato nel while del caso particolare
		if (rangecont < 1000 && flagTry){
			convert_and_shift(sottoV, sotto, sopraV, sopra, contacaratteri, rangecont, controllo, br, flagEndCode);
			flagRange = true;
			cout << "\t" << "entra";
		}

		//se non succede niente precedentemente
		if (!flagShift && !flagRange){
			sotto = (unsigned)round(sottoV);
			sopra = (unsigned)round(sopraV);
			rangecont = sopra - sotto;
		}

		flagShift = false;
		flagRange = false;
		flagTry = false;

		cout << controllo << "\t" << sotto << "\t" << rangecont << "\t" << sopra << endl;
		n_caratteri--;
	}
	//dopo aver scritto in binario la codifica devo gestire la lettura delle cifre, inizio con n_cifre_range_iniziale - 1

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}