#include "encoding.h"
#include "encodingfunction.h"

void encoding(ifstream& is, ofstream& os){

	array<uint_32, 256> myarray;
	vector<coppia> coppie;
	vector<coppia2> coppie2;

	//mi assicuro che l'array sia inizializzato a zero
	myarray.fill(0);

	//calcolo le ricorrenze di ogni simbolo
	uint_32 tot_symbol = 0;//numero totali simboli letti dallo stream
	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){ //gestione ritorno a capo (è presente uno spazio in più)		
		if (tmp == 0x0D)
			is.get();
		tot_symbol++;
		myarray[tmp]++;
	}

	for (uint_32 i = 0; i < 256; ++i){
		if (myarray[i] != 0)
			coppie.push_back(coppia(i, (float_64)myarray[i] / (float_64)tot_symbol));
	}

	uint_32 flagSfora = true; //flag per verificare il caso della probabilità che sfora 1
	uint_32 contaprob = 0;   // conta la probabilità
	uint_32 indice = 0; //ricorda il valore con probabil. piu grande
	uint_32 max = 0; //ricorda valore con probab. piu grande
	uint_32 i = 0;  //indice iteratore
	//arrotondamento a 4 cifre decimali della probabilità
	while (true){
		for (auto it = coppie.begin(); it != coppie.end(); ++it){
			float_64 x = it->_fa;
			x = x * 10000;
			//variabile dove è salvato il valore senza virgola della prob
			int_16 u = x;
			//livellamento delle probabilità dei valori per evitare errori di calcolo
			if (flagSfora){
				if (u < 50){
					u = 50;
				}
				if (u > 300){
					u = 300;
				}
			}
			//ci entriamo se il livellamento supera 1 di probabilità totale.
			//tutti i valori hanno uguale probabilità
			else
				u = 10000 / coppie.size();

			contaprob += u;
			coppie2.push_back(coppia2(it->_b, u));
			i++;
		}
		//ci entriamo se il livellamento supera 1 di probabilità totale.
		//tutti i valori avranno uguale probabilità
		if (contaprob > 10000){
			flagSfora = false;
			coppie2.clear();
			contaprob = 0;
		}
		else break;
	}

	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	uint_32 prob_range = 0;
	for (auto it = coppie2.begin(); it != coppie2.end(); ++it){
		it->_Fa = prob_range;
		prob_range += it->_fa;
		cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << endl;
	}

	//correzione valore finale per arrivare a 1
	coppie.at(coppie.size() - 1)._fa = 10000 - coppie.at(coppie.size() - 1)._Fa;

	/* prova encoding */
	uint_32 range = pow(10.0, 6.0);
	uint_32 low = 0;
	uint_32 up = range;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;

	/*!< Header creation. */

	//scrivo in output lunghezza file iniziale
	os.write(reinterpret_cast<char*>(&tot_symbol), sizeof(uint_32));

	//scrivo in output numero simboli ascii con probabilità maggiore di zero
	byte numcoppie = coppie2.size();
	os.write(reinterpret_cast<char*>(&numcoppie), 1);

	//stampiamo per ogni simbolo il valore della probabilità senza virgola
	for (auto it = coppie2.begin(); it != coppie2.end(); ++it){
		os << it->_b;
		int_16 u = round(it->_fa);
		os.write(reinterpret_cast<char*>(&u), 2);
	}

	/*!< Header end. */

	//il bitwriter serve per scrivere in output le cifre da 0 a 9, quindi bastano 4 bit per cifra per codificarle
	bitwriter bw(os);

	//codifica vera e propria
	while (is.get(reinterpret_cast<char&>(b)))
		encode_symbol(b, coppie2, low, range, up, os, bw);

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

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}