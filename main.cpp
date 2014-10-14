#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<array>
#include<vector>
#include<algorithm>
#include<functional>
#include<math.h>
#include<string>
#include<cstdlib>


using namespace std;
typedef unsigned char byte;

struct coppia {
	//! coppia di valori.
	byte _b;
	double _fa; /*!< probabilità della coppia. */
	double _Fa; /*!< start della coppia. */

	coppia(byte b, double prob, double start=0) : _b(b), _fa(prob),_Fa(start) {}
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

void encode(double start, double size, double& low, double& range){
	//! Descrizione di encode
    /*!
      \param start descrizione parametro
      \param size descrizione parametro
	  \param &low descrizione parametro
	  \param &range descrizione parametro
      \return Che cosa ritorna
      \sa encode_symbol(), trasforma_string()
    */
	// adjust the range based on the symbol interval
	//double total = 100;
	//range /= total;
	//ATTENZIONE all'arrotondamento che puo essere sbagliato, DA RIVEDERE
	low = (unsigned long)(low + start * range);
	range = range* size;
	range = (unsigned long)(range + 0.99);
	//low = (unsigned long)(low + start * range);
	//range = (unsigned long)(range* size);
}

void encode_symbol(byte b, vector<coppia>& x, double& low, double& range){
	//! funzione di encode dei simboli
    /*!
      \param b descrizione parametro
      \param &x descrizione parametro
	  \param &low descrizione parametro
	  \param &range descrizione parametro
      \return Che cosa ritorna
      \sa encode(), encode_symbol()
    */
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa,low,range);
			//verifica di ogni passaggio
			cout << "low: " << low << "\t top: " << range + low << endl;
			//modifica per range bassi disponibili
			if (range < 100.0){
				range *= 10.0;
				low *= 10.0;
			}
			return;
		}
	cout << "symbol not found!";
}

void syntax() {
	cout << "Syntax:" << endl;
	cout << "Range encoding <input filename> <output filename>" << endl;
}

string trasforma_string(double top, double low, string first, string last){
	//! Comparazione top e low per trovare le cifre sufficienti per la codifica
    /*!
      \param top descrizione parametro
      \param low descrizione parametro
	  \param first descrizione parametro
	  \param last descrizione parametro
      \return Che cosa ritorna
      \sa encode(), encode_symbol()
    */
	string codifica;
	unsigned length = 1;

	while (true){
		if ((first.compare(0, length, last, 0, length)) != 0){
			break;
		}
		codifica.push_back(first.at(length - 1));
		cout << codifica << endl;
		length++;
	}

	double raggio=top - low;
	raggio /= 2;
	raggio += low;
	//aggiunta delle due cifre finali
	string asd = to_string(raggio);
	asd.erase(0, length - 1);
	codifica.append(asd, 0, 2);
	return codifica;
}

int main(int argc, char *argv[]){
	//! Funzione main del mio range encoding
	array<unsigned int, 256> myarray;
	vector<coppia> coppie;

	if (argc != 3) {
		syntax();
		return EXIT_FAILURE;
	}

	string InputFileName = argv[1];
	string OutputFileName = argv[2];

	ifstream is(InputFileName, ifstream::binary);
	if (!is) return -1;

	ofstream os(OutputFileName, ofstream::binary);
	if (!os) return -1;

	//mi assicuro che l'array sia inizializzato a zero
	myarray.fill(0);

	/* SPUNTONI DA TOGLIERE
	//calcolo le ricorrenze di ogni simbolo
	unsigned int tot_symbol = 0;//numero totali simboli letti dallo stream
	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){
		tot_symbol++;
		myarray[tmp]++;
	}

	for (int i = 0; i<256; ++i)
		if (myarray[i] != 0)
			coppie.push_back(coppia(i, myarray[i] / (double)tot * 10));
			//cout << i << '\t' << myarray[i] << '\n';

	sort(coppie.begin(), coppie.end(), greater<coppia>());
	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	double prob_range = 0;
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		it->_fa = prob_range;
		prob_range += it->_Fa;
		//cout << "symbol: " << it->_b << "\t probability: " << it->_cnt << "\n";
		cout << "symbol: " << it->_b << "\t start_range: " << it->_fa << "\t probability: " << it->_Fa << "\n";
	}
	SPUNTONI DA TOGLIERE*/ 

	/* prova encoding */
	double range = pow(10.0, 3.0);
	double low = 0;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;
	
	//prova esempio pdf
	coppie.push_back(coppia('K', 0.10, 0.0));
	coppie.push_back(coppia('L', 0.21, 0.10));
	coppie.push_back(coppia('M', 0.27, 0.31));
	coppie.push_back(coppia('N', 0.42, 0.58));
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		cout << it->_b << " " << it->_Fa << " " << it->_fa << endl;
	}

	//codifica vera e propria
	unsigned tot_symbol = 0;
	while (is.get(reinterpret_cast<char&>(b))){
		tot_symbol++;
		encode_symbol(b, coppie, low, range);
	}

	//header= n simboli codificati, tabella symbol-fa-Fa
	//os << tot_symbol; 
	os.write(reinterpret_cast<char*>(&tot_symbol), 1);
	/* da mettere a posto la scrittura dei double nell'header
	for (unsigned i = 0; i < coppie.size(); ++i){
		os << coppie[i]._b;
		os.write(reinterpret_cast<char*>(&coppie[i]._fa), sizeof(coppie[i]._fa));
		os.write(reinterpret_cast<char*>(&coppie[i]._Fa), sizeof(coppie[i]._Fa));
	}*/		
	//os << 0xffff; //termino cosi l'header

	double top = range + low;
	
	cout << "finale " << "low: " << low << "\t top: " << top << endl;

	//trasformazione in stringhe
	string first = to_string(low);
	string last = to_string(top);
	string codifica=trasforma_string(top,low,first,last);

	//stampa
	cout << first << endl << last << endl;
	cout << codifica << endl;
	os << codifica;
	//bisogna scrivere in binario la codifica, cosi è in char e non va bene

	// chiude is e os per poterli eventualemente usare nella decodifica
	is.close();
	os.close();

	/*		Decodifica		 */
	{	//graffe per non avere problemi con i nomi is e os di prima
		ifstream is(OutputFileName, ifstream::binary);
		if (!is) return -1;
		ofstream os("decodifica.txt", ofstream::binary);
		if (!os) return -1;

		//leggo quanti caratteri devo decodificare nel primo byte del file
		unsigned n_caratteri = is.get() + 1;
		//+1 perchè cosi nel while posso fermarmi a 0 e posso usare un unsigned invece di una variabile signed
		cout << n_caratteri<<'\n';
		unsigned totalrange = 1000;
		double rangecont = 1000.0;
		unsigned numero[9] = { 7, 4, 3, 2, 0, 3, 9, 0, 0}; //numero di prova
		unsigned modulo = 10000;
		unsigned controllo = 743;
		double sopraV=1000.0; //variabile in double per permettere migliore arrotondamento
		unsigned sopra=1000; //necessario unsigned per il valore finale di top e low e per il modulo
		double sottoV=0.0;
		unsigned sotto=0;
		bool flagRange = false;  //flag per controllare se shiftare o no il range
		bool flagCode = false;
		unsigned aggiunta = 0;

		//ciclo per ogni carattere
		while (n_caratteri > 0){
				//controllo se c'è da shiftare il top e low
				if ((sotto / 100) == (sopra / 100)){
					flagRange = true;
				}
				//calcolo della probabilità rispetto al range
				double prob = (double)(controllo-sotto) / rangecont;
				//ciclo per controllare a quale simbolo di riferisce la probabilità
				for (auto it = coppie.begin(); it != coppie.end(); ++it){
					if (prob >= it->_Fa && prob < (it->_fa + it->_Fa)){
						cout << it->_b;
						os << it->_b;
						//aggiornamento valori !!ARROTONDAMENTO ERRATO
						sottoV = sotto + (it->_Fa*rangecont);
						rangecont = (rangecont * it->_fa)+0.1;
						sopraV = sottoV + rangecont;
						//cout << sottoV << "\t" << rangecont << "\t" << sopraV << endl;
						break;
					}
				}

				//se c'è da shiftare il top e low aggiorno
				if (flagRange){
					//operazioni separate per permettere passaggio da double a unsigned senza perdita di segno
					//e per fare il modulo
					sottoV = sottoV * 10;
					sotto = (unsigned)sottoV % 1000;
					sopraV = sopraV * 10;
					sopra = (unsigned)sopraV % 1000;
					flagRange = false;
					rangecont *= 10;
				}
				else{//se non shifto trasformo i double in unsigned
					sotto = (unsigned)sottoV;
					sopra = (unsigned)sopraV;
				}

				//controllo se devo shiftare il code
				//!!!!ATTENZIONE:ERRORE! ALGORITMO SBAGLIATO, in realtà il code va ogni
				//volta shiftato il turno dopo, lo stesso vale di conseguenza con il TOP e LOW!!
				//al primo step per caso andava bene, ma poi in realtà sballa tantissimo per colpa di questo errore!
				if (controllo <= sotto || controllo > sopra){
					controllo = (controllo % 100) * 10;
					controllo += numero[3 + aggiunta];
					aggiunta++;
				}

				cout << sotto << "\t" << rangecont << "\t" << sopra << endl;
				n_caratteri--;
		}
		//dopo aver scritto in binario la codifica devo gestire la lettura delle cifre, inizio con n_cifre_range_iniziale - 1

		
	}


}
