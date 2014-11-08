#ifndef BITSTREAMS_H
#define BITSTREAMS_H

#include <ostream>

typedef unsigned char byte;
typedef int int_32;
typedef unsigned int uint_32;

/*!
 * \short Class BitWriter
 * \details Writes on bitstream count least significant bits of u.
 */
class bitwriter {
	std::ostream& _os;
	byte _buffer;
	int_32 _bits;

	// Aggiunge il bit meno significativo di u al buffer
	void write_bit(uint_32 u) {
		_buffer = (_buffer << 1) | (u & 1);
		if (++_bits == 8) {
			_os.put(_buffer);
			_bits = 0;
		}
	}

	// Non si deve né copiare né assegnare!
	bitwriter(const bitwriter&);
	bitwriter& operator= (const bitwriter&);

public:
	bitwriter(std::ostream& os) : _os(os), _bits(0) {}

	// Scrive sul bitstream i count bit meno significativi di u
	void operator() (uint_32 u, uint_32 count) {
		while (count > 0)
			write_bit(u >> --count);
	}

	// Svuota il buffer sul file
	void flush(uint_32 fill_bit = 1) {
		while (_bits > 0)
			write_bit(fill_bit);
	}

	~bitwriter() {
		flush();
	}
};

/*!
 * \short Class BitReader
 * \details Reads on stream required count bit of u and puts them in the least significant bits of the result.
 */
class bitreader {
	std::istream& _is;
	byte _buffer;
	int_32 _bits;

	// Aggiunge il bit meno significativo di u al buffer
	uint_32 read_bit() {
		if (_bits == 0) {
			_is.get(reinterpret_cast<char&>(_buffer));
			_bits = 8;
		}
		return (_buffer >> --_bits) & 1;
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	bitreader(std::istream& is) : _is(is), _bits(0) {}

	operator std::istream&() { return _is; }

	// Legge dal bitstream i count bit richiesti e li mette nei bit meno significativi del risultato
	uint_32 operator() (uint_32 count) {
		uint_32 u = 0;
		while (count-- > 0)
			u = (u << 1) | read_bit();
		return u;
	}
};

#endif // BITSTREAMS_H