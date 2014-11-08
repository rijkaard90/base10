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
	uint_64 sottoP = sotto * 10;
	uint_64 sopraP = sopra * 10;

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