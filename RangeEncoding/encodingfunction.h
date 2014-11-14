#ifndef ENCODINGFUNCTION_H
#define ENCODINGFUNCTION_H

#include <iostream>
#include <array>
#include <vector>
#include <fstream>

#include "typedef.h"
#include "couple.h"
#include "bitstreams.h"

/*!
 *  List of RangeEncoding algorithm function:
 *    - \c syntax()
 *    - \c encoding()
 *		   1. setOccurrences()
 *         2. setProbability()
 *         3. rounding()
 *         4. setRange()
 *		   5. inputSize()
 *		   6. headerCreation()
 *         7. encode()
 *         8. emit_digit()
 *		   9. encode_symbol()
 *		   10. encodeAlgorithm()
 *		   11. printCompressionRatio()
 *    - \c decoding()
 *         1. read_little_endian()
 *         2. headerExtraction()
 *         3. setRangeValue()
 *         4. setControl()
 *         5. shift_and_control()
 *         6. decodeAlgorithm()
 *		   7. decodedSize()
 *
 *  More details will be provided in definition.
 */

/*!
 * \short Function SETOCCURRENCES
 * \details Calculating the occurrences of each symbol.
 * \param &is Input file.
 * \param &totInputSymbols Total number of symbols read from the stream.
 * \param &symbolsOccurrences Array of occurrences.
 * \param isTextFile Specify if we have to compress a text file or another type.
 * \return Array occurrences depending on the input file.
 * \see syntax(), encoding(), setProbability(), rounding(), setRange(), inputSize(), headerCreation(), 
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void setOccurrences(ifstream& is, uint_32& totInputSymbols, array<uint_32, 256>& symbolsOccurrences, bool isTextFile);

/*!
 * \short Function SETPROBABILITY
 * \details Calculating the probability of each symbol.
 * \param &floatCouple Vector of symbols, probability and starting range, float.
 * \param &totInputSymbols Total number of symbols read from the stream.
 * \param &symbolsOccurrences Array of occurrences.
 * \return Array probability depending on the input file.
 * \see syntax(), encoding(), setOccurrences(), rounding(), setRange(), inputSize(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void setProbability(vector<f_couple>& floatCouple, uint_32& totInputSymbols, array<uint_32, 256>& symbolsOccurrences);

/*!
 * \short Function ROUNDING
 * \details Rounding to 5 decimal digits for probability.
 * \param &floatCouple Vector of symbols, probability and starting range, float.
 * \param &unsignedCouple Vector with correct probability, unsigned.
 * \return Vector with probability smoothed to avoid calculation errors.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), setRange(), inputSize(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void rounding(vector<f_couple>& floatCouple, vector<u_couple>& unsignedCouple);

/*!
 * \short Function SETRANGE
 * \details Assignment start of the range for each symbol.
 * \param &floatCouple Vector of symbols, probability and starting range, float.
 * \param &unsignedCouple Vector with correct probability, unsigned.
 * \return Vector with final correction value for up to 1 probability.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), inputSize(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void setRange(vector<f_couple>& floatCouple, vector<u_couple>& unsignedCouple);

/*!
 * \short Function INPUTSIZE
 * \details Calculate input file size.
 * \param &is Input file.
 * \return Number of bytes in the input file.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
uint_32 inputSize(ifstream& is);

/*!
 * \short Function HEADERCREATION
 * \details Creating header for encoding.
 * \param &unsignedCouple Vector with correct probability, unsigned.
 * \param &totInputSymbols Total number of symbols read from the stream.
 * \param &os Output file.
 * \return Write to output file initial length, number of ascii symbols with probability greater than zero, probability value without a decimal point.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void headerCreation(vector<u_couple>& unsignedCouple, uint_32& totInputSymbols, ofstream& os);

/*!
 * \short Function ENCODE
 * \details Calculates the values of top and bottom range, and new range.
 * \param start Starting range.
 * \param size Probability.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \return Range for encoding.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), 
 *       headerCreation(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top);

/*!
 * \short Function EMIT_DIGIT
 * \details Change for low range available.
 * \param &low Bottom range.
 * \param &bw Bit writer.
 * \return Bottom range adjusted.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), 
 *      headerCreation(), encode(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void emit_digit(uint_32& low, bitwriter& bw);

/*!
 * \short Function ENCODE_SYMBOL
 * \details Core function for encoding algorithm, for each element manages the range.
 * \param b Unsigned char.
 * \param &unsignedCouple Vector with correct probability, unsigned.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \param &os Output file.
 * \param &bw Bit writer.
 * \return Encoding symbol.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), 
 *      headerCreation(), encode(), emit_digit(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void encode_symbol(byte b, vector<u_couple>& unsignedCouple, uint_32& low, uint_32& range, uint_32& top, ofstream& os, bitwriter& bw);

/*!
 * \short Function ENCODEALGORITHM
 * \details Writing encoding on file.
 * \param &is Input file.
 * \param &os Output file.
 * \param &unsignedCouple Vector with correct probability, unsigned.
 * \return Write the encoded input file to output file.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), 
 *      headerCreation(), encode(), emit_digit(), encode_symbol(), printCompressionRatio(), decoding()
 */
void encodeAlgorithm(ifstream& is, ofstream& os, vector<u_couple>& unsignedCouple);

/*!
 * \short Function INPUTSIZE
 * \details Calculate decoded file size and compression ratio.
 * \param &os Output file.
 * \param &inputLength Size of input file.
 * \return Number of bytes in the decoded file and % of compression ratio.
 * \see syntax(), encoding(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), 
 *      headerCreation(), encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), decoding()
 */
void printCompressionRatio(ofstream& os, uint_32& inputLength);

#endif // ENCODINGFUNCTION_H