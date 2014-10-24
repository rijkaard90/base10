#include "decodingfunction.h"

unsigned read_little_endian (ifstream& is, unsigned nByte){
	unsigned result = 0;
	for (unsigned i = 0; i < nByte; i++)
		result += static_cast<unsigned>(is.get() << (8 * i));

	return result;
}

void double_to_unsigned(double& sottoV, unsigned& sotto, double& sopraV, unsigned& sopra, 
	unsigned& contacaratteri, double& rangecont, unsigned& controllo, bitreader& br){
	//operazioni separate per permettere passaggio da double a unsigned senza perdita di segno e per fare il modulo
	sottoV = sottoV * 10;
	sotto = (unsigned)ceil(sottoV) % (unsigned)(pow(10.0, contacaratteri + 1));
	sopraV = sopraV * 10;
	sopra = (unsigned)sopraV % (unsigned)(pow(10.0, contacaratteri + 1));
	rangecont = sopra - sotto;
	controllo = (controllo % (unsigned)(pow(10.0, contacaratteri))) * 10;
	//controllo fine codice
	controllo += br(4);
}