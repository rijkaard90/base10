/*! 
 * \mainpage Range Encoding
 * \section range_encoding Introduction
 *
 * Range encoding is an entropy coding method defined by G. Nigel N. 
 * Martin in a 1979 paper, which effectively rediscovered the FIFO arithmetic
 * code first introduced by Richard Clark Pasco in 1976. Given a stream of symbols
 * and their probabilities, a range coder produces a space efficient stream of bits to 
 * represent these symbols and, given the stream and the probabilities, a range decoder
 * reverses the process. 
 * 
 * Range coding is very similar to arithmetic encoding, except that encoding is done
 * with digits in any base, instead of with bits, and so it is faster when using larger
 * bases (e.g. a byte) at small cost in compression efficiency.
 * After the expiration of the first (1978) arithmetic coding patent, range encoding
 * appeared to clearly be free of patent encumbrances. This particularly drove
 * interest in the technique in the open source community. Since that time, patents on 
 * various well-known arithmetic coding techniques have also expired.
 *
 * \subsection how_works How range encoding works
 * Range encoding conceptually encodes all the symbols of the message into one number,
 * unlike Huffman coding which assigns each symbol a bit-pattern and concatenates all 
 * the bit-patterns together. Thus range encoding can achieve greater compression 
 * ratios than the one-bit-per-symbol lower bound on Huffman encoding and it does not
 * suffer the inefficiencies that Huffman does when dealing with probabilities that are
 * not exact powers of two.
 *
 * The central concept behind range encoding is this: given a large-enough range of
 * integers, and a probability estimation for the symbols, the initial range can easily
 * be divided into sub-ranges whose sizes are proportional to the probability of the
 * symbol they represent. Each symbol of the message can then be encoded in turn, by
 * reducing the current range down to just that sub-range which corresponds to the next
 * symbol to be encoded. The decoder must have the same probability estimation the
 * encoder used, which can either be sent in advance, derived from already transferred
 * data or be part of the compressor and decompressor.
 *
 * When all symbols have been encoded, merely identifying the sub-range is enough to
 * communicate the entire message (presuming of course that the decoder is somehow
 * notified when it has extracted the entire message). A single integer is actually
 * sufficient to identify the sub-range, and it may not even be necessary to transmit
 * the entire integer; if there is a sequence of digits such that every integer
 * beginning with that prefix falls within the sub-range, then the prefix alone is all
 * that's needed to identify the sub-range and thus transmit the message.
 *
 * The central problem may appear to be selecting an initial range large enough that
 * no matter how many symbols we have to encode, we will always have a current range
 * large enough to divide into non-zero sub-ranges. In practice, however, this is not
 * a problem, because instead of starting with a very large range and gradually
 * narrowing it down, the encoder works with a smaller range of numbers at any given
 * time. After some number of digits have been encoded, the leftmost digits will not
 * change.
 */
 
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;
typedef unsigned char byte;

/*! 
	\brief	Classe coppia
	\details Gestione dei caratteri insieme alla probabilità e al range di partenza.
*/
struct coppia {
	byte _b;	/*!< Unsigned char. */
	double _fa; /*!< Probabilità della coppia. */
	double _Fa; /*!< Start della coppia. */

	/*! 
		\brief	Inizializzazione struct
		\details Passaggio e assegnazione dei valori.
	*/
	coppia(byte b, double prob, double start = 0) : _b(b), _fa(prob), _Fa(start) {}
	
	/*! 
		\brief	Operator >.
		\details Overloading operator >.
	*/
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

/*! 
	\brief	Function ENCODE
	\details Adjust the range based on the symbol interval.
	\param start descrizione parametro.
	\param size descrizione parametro.
	\param &low descrizione parametro.
	\param &range descrizione parametro.
	\return Adjusted values of low and range.
	\see encode_symbol(), trasforma_string(), syntax()
*/
void encode(double start, double size, double& low, double& range, double& top){
	//PORCODO ORA VA BENE DIOCANE
	low = low + start * range;
	top = (unsigned long)(low + size* range);
	low = (unsigned long) low;
	range = top - low;
}

/*! 
	\brief	Function ENCODE_SYMBOL
	\details dettagli della funzione encode simboli.
	\param b descrizione parametro
	\param &x descrizione parametro
	\param &low descrizione parametro
	\param &range descrizione parametro
	\return Ritorna la coppia di valori.
	\see encode, trasforma_string(), syntax()
*/
void encode_symbol(byte b, vector<coppia>& x, double& low, double& range, double& top){
	for (auto it = x.begin(); it != x.end(); ++it)
		if (it->_b == b){
			encode(it->_Fa, it->_fa, low, range, top);
			//verifica di ogni passaggio
			cout << "low: " << low << "\t range: " << range << "\t top: " << top << endl;
			//modifica per range bassi disponibili
			if (range < 100000000.0){
				range *= 10.0;
				low *= 10.0;
			}
			return;
		}
	cout << "Symbol not found!";
}

/*! 
	\brief	Function TRASFORMA_STRING
	\details Comparazione top e low per trovare le cifre sufficienti per la codifica.
	\param top descrizione parametro
	\param low descrizione parametro
	\param first descrizione parametro
	\param last descrizione parametro
	\return Che cosa ritorna.
	\see encode(), encode_symbol(), syntax()
*/
string trasforma_string(double top, double low, string first, string last){
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
	double raggio = top - low;
	raggio /= 2.0;
	raggio += low;
	//aggiunta delle due cifre finali
	string asd = to_string(raggio);
	asd.erase(0, length - 1);
	codifica.append(asd, 0, 2);
	return codifica;
}

/*! 
	\brief	Function SYNTAX
	\details Specifies the correct syntax for using Range Engoding.
	\return Correct syntax.
	\see encode(), encode_symbol(), trasforma_string()
*/
void syntax() {
	cout << "Syntax:" << endl;
	cout << "Range encoding <input filename> <output filename>" << endl;
}

//! Function main Range Encoding.
int main(int argc, char *argv[]){

	array<double, 256> myarray;
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

	//calcolo le ricorrenze di ogni simbolo
	unsigned tot_symbol = 0;//numero totali simboli letti dallo stream
	byte tmp;
	while (is.get(reinterpret_cast<char&>(tmp))){
		tot_symbol++;
		myarray[tmp]++;
	}

	for (int i = 0; i < 256; ++i){
		if (myarray[i] != 0){
			coppie.push_back(coppia(i, (double)myarray[i] / (double)tot_symbol));
			//cout << i << "\t"<< myarray[i]<<endl;
		}
	}

	//sort(coppie.begin(), coppie.end(), greater<coppia>());
	//dopo aver ordinato il vector posso assegnare l'inizio del range per ogni simbolo
	double prob_range = 0.0;
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		it->_Fa = prob_range;
		prob_range += it->_fa;
		//cout << "symbol: " << it->_b << "\t probability: " << it->_cnt << "\n";
		cout << "symbol: " << it->_b << "\t start_range: " << it->_Fa << "\t probability: " << it->_fa << "\n";
	}

	/* prova encoding */
	double range = pow(10.0, 10.0);
	double low = 0.0;
	double up = range;
	is.clear(); // Disattivo l'EOF precedente
	is.seekg(ios_base::beg); // Torno all'inizio
	byte b;
	
	//PROVA ESEMPIO PDF
	/*coppie.push_back(coppia('K', 0.10, 0.0));
	coppie.push_back(coppia('L', 0.21, 0.10));
	coppie.push_back(coppia('M', 0.27, 0.31));
	coppie.push_back(coppia('N', 0.42, 0.58));
	for (auto it = coppie.begin(); it != coppie.end(); ++it){
		cout << it->_b << " " << it->_Fa << " " << it->_fa << endl;
	}*/

	//codifica vera e propria
	while (is.get(reinterpret_cast<char&>(b))){
		//tot_symbol++;
		encode_symbol(b, coppie, low, range, up);
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
	string codifica = trasforma_string(top, low, first, last);

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
		unsigned n_caratteri = is.get();
		//+1 perchè cosi nel while posso fermarmi a 0 e posso usare un unsigned invece di una variabile signed
		cout << n_caratteri<<'\n';
		unsigned totalrange = pow(10.0,10.0);
		double rangecont = totalrange;
		unsigned numero[] = {9,4,0,0}; //numero di prova
		//unsigned modulo = 10000;
		unsigned controllo = 940;
		double sopraV = totalrange; //variabile in double per permettere migliore arrotondamento
		unsigned sopra = 10000000000; //necessario unsigned per il valore finale di top e low e per il modulo
		double sottoV = 0.0;
		unsigned sotto = 0;
		bool flagRange = false;  //flag per controllare se shiftare o no il range
		bool flagShift = false;
		unsigned aggiunta = 0;

		//ciclo per ogni carattere
		while (n_caratteri > 0){
		
				//controllo se c'è da shiftare il top e low
				// ((sotto / 100) == (sopra / 100)) ||
				if (sopra-sotto < 100000000){
					flagRange = true;
				}
				
				//calcolo della probabilità rispetto al range
				double prob = (double)(controllo - sotto) / rangecont;
				
				//ciclo per controllare a quale simbolo di riferisce la probabilità
				for (auto it = coppie.begin(); it != coppie.end(); ++it){
					if (prob >= it->_Fa && prob < (it->_fa + it->_Fa)){
						cout << it->_b;
						os << it->_b;
						//aggiornamento valori !!ARROTONDAMENTO ERRATO
						sottoV = sotto + (it->_Fa * rangecont);
						rangecont = (rangecont * it->_fa);
						sopraV = sottoV + rangecont;
						//cout << sottoV << "\t" << rangecont << "\t" << sopraV << endl;
						break;
					}
				}

				//se c'è da shiftare il top e low aggiorno
				if (flagRange){
					//operazioni separate per permettere passaggio da double a unsigned senza perdita di segno e per fare il modulo
					sottoV = sottoV * 10;
					sotto = (unsigned)sottoV % 10000000000;
					sopraV = sopraV * 10;
					sopra = (unsigned)sopraV % 10000000000;
					flagRange = false;
					rangecont = sopra - sotto;
					controllo = (controllo % 1000000000) * 10;
					controllo += numero[3 + aggiunta];
					aggiunta++;
					flagRange = false;
				}
				else{//se non shifto trasformo i double in unsigned
					sotto = (unsigned)sottoV;
					sopra = (unsigned)sopraV;
					rangecont = sopra - sotto;
				}			

				cout << controllo << "\t" << sotto << "\t" << rangecont << "\t" << sopra << endl;
				n_caratteri--;
		}
		//dopo aver scritto in binario la codifica devo gestire la lettura delle cifre, inizio con n_cifre_range_iniziale - 1		
	}
}