/*!
 * \mainpage Range Encoding Description
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
 * \image html "../renc.jpg"
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

#include "rangeencoding.h"

/*! 
 * \short Function SYNTAX
 * \details Specifies the correct syntax for using RangeEncoding algorithm.
 * \return Correct syntax.
 * \see encoding(), decoding()
 */
void syntax() {
	cout << "Syntax:" << endl;
	cout << "Range Encoding <input filename> <output filename>" << endl;
}

/*!
 * \short Function MAIN
 * \details Something for running RangeEncoding algorithm.
 * \see syntax(), encoding(), decoding()
 */
int main(int argc, char *argv[]){
	
	if (argc != 3) {
		syntax();
		return EXIT_FAILURE;
	}

	string InputFileName = argv[1];
	string OutputFileName = argv[2];

	/*!< Encoding. */
	{
		ifstream is(InputFileName, ifstream::binary);
		if (!is) return -1;
		ofstream os(OutputFileName, ofstream::binary);
		if (!os) return -1;

		encoding(is, os);
	}
	
	/*!< Decoding. */
	{	
		ifstream is(OutputFileName, ifstream::binary);
		if (!is) return -1;
		ofstream os("decodifica.txt", ofstream::binary);
		if (!os) return -1;

		decoding(is, os);
	}
}