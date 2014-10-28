#include "encoding.h"
#include "encodingfunction.h"

void encoding(ifstream& is, ofstream& os){

	array<unsigned, 256> myarray;
	vector<coppia> coppie;

	//mi assicuro che l'array sia inizializzato a zero
	myarray.fill(0);

	//calcolo le ricorrenze di ogni simbolo
	unsigned tot_symbol = 0;//numero totali simboli letti dallo stream
	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){
		//gestione ritorno a capo(è presente uno spazio in più)
		if (tmp == 0x0D){
			is.get();
		}
		tot_symbol++;
		myarray[tmp]++;
	}

	for (int i = 0; i < 256; ++i){
		if (myarray[i] != 0)
			coppie.push_back(coppia(i, (double)myarray[i] / (double)tot_symbol));
	}

	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	double prob_range = 0.0;
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		it->_Fa = prob_range;
		prob_range += it->_fa;
		cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}

	/* prova encoding */
	double range = pow(10.0, 6.0);
	double low = 0.0;
	double up = range;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;

	/*	SCRITTURA HEADER		 */

	//scrivo in output lunghezza file iniziale
	os.write(reinterpret_cast<char*>(&tot_symbol), sizeof(unsigned));

	//scrivo in output numero simboli ascii con probabilità maggiore di zero
	unsigned char numcoppie = coppie.size();
	os.write(reinterpret_cast<char*>(&numcoppie), 1);

	//stampiamo per ogni simbolo le sue occorrenze ed alla fine il numero totale di symbol letti
	//in decodifica calcoleremo fa,Fa di nuovo
	for (int i = 0; i < 256; ++i){
		if (myarray[i] != 0){
			os << (char)i;
			//per salvare spazio nell'header scrivo solo 3 byte dell'unsigned perchè 4 non sono praticamente mai utilizzati
			os.write(reinterpret_cast<char*>(&myarray[i]), 3);
		}
	}
	/*	FINE HEADER		 */

	//il bitwriter serve per scrivere in output le cifre da 0 a 9, quindi bastano 4 bit per cifra per codificarle
	bitwriter bw(os);

	//codifica vera e propria
	while (is.get(reinterpret_cast<char&>(b))){
		//if (tmp != 0x0D)
			encode_symbol(b, coppie, low, range, up, os, bw);
	}

	unsigned differenza = 1;
	double appoggio;
	double appoggioU;
	double cnt = 0.0;
	//variabili d'appoggio, necessarie entrambe per il caso particolare che lo zero sia prima cifra di uno dei due
	appoggio = low;
	appoggioU = up;
	//cerchiamo l'ordine di grandezza di low e top
	while (appoggio > 10 || appoggioU >10){
		appoggio /= 10;
		appoggioU /= 10;
		cnt++;
	}

	//calcolo quante cifre ha il low e salvo in miao per il for successivo
	appoggio = 0.0;
	unsigned miao = 0;
	while (differenza == 1){
		appoggio = low / pow(10.0, cnt);
		differenza = (unsigned)(up / (unsigned)pow(10, cnt) - (unsigned)appoggio);
		cnt--;
		miao++;
	}

	//calcolo valore finale da aggiungere al file
	unsigned final = (unsigned)(appoggio + differenza / 2);
	cout << "finale:" << final << endl;

	//estrazione cifre finali e scrittura tramite bitwriter sul file
	unsigned x;
	for (unsigned i = 0; i < miao; ++i){
		x = (final *(unsigned)pow(10, i)) % (unsigned)pow(10, miao) / (unsigned)pow(10, miao - 1);
		bw(x, 4);
	}

	//per scrivere non solo gli uno finali del flush ma anche l ultimo valore se non riempe il byte
	bw.flush();

	// chiude is e os per poterli eventualemente riusare
	is.close();
	os.close();
}