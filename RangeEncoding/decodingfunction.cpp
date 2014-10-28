#include "decodingfunction.h"

unsigned read_little_endian (ifstream& is, unsigned nByte){
	unsigned result = 0;
	for (unsigned i = 0; i < nByte; i++)
		result += static_cast<unsigned>(is.get() << (8 * i));

	return result;
}

void convert_and_shift(double& sottoV, unsigned& sotto, double& sopraV, unsigned& sopra,
	unsigned& contacaratteri, double& rangecont, unsigned& controllo, bitreader& br, bool& flagEndCode){
	//operazioni separate per permettere passaggio da double a unsigned senza perdita di segno e per fare il modulo
	sottoV = sottoV * 10;
	//variabile d'appoggio e uso del long long per non perdere precisione
	unsigned long long sottoP = (unsigned long long)round(sottoV);
	//calcolo modulo
	sotto = sottoP % (unsigned long long)(pow(10.0, contacaratteri + 1));
	sopraV = sopraV * 10;
	unsigned long long sopraP = (unsigned long long)round(sopraV);
	sopra = sopraP % (unsigned long long)(pow(10.0, contacaratteri + 1));
	rangecont = sopra - sotto;
	controllo = (controllo % (unsigned)(pow(10.0, contacaratteri))) * 10;
	//controllo fine codice
	unsigned tmp = br(4);
	if (tmp <= 9 && !flagEndCode){
		controllo += tmp;
	}
	else flagEndCode = true;	

}