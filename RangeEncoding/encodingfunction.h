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
 *		   5. headerCreation()
 *         6. encode()
 *         7. emit_digit()
 *		   8. encode_symbol()
 *		   9. encodeAlgorithm()
 *    - \c decoding()
 *         1. read_little_endian()
 *         2. headerExtraction()
 *         3. setRangeValue()
 *         4. setControl()
 *         5. shift_and_control()
 *         6. decodeAlgorithm()
 *
 *  More details will be provided in definition.
 */

/*!
 * \short Function SETOCCURRENCES
 * \details Calculating the occurrences of each symbol.
 * \param &is Input file.
 * \param &tot_symbol Total number of symbols read from the stream.
 * \param &myarray Array of occurrences.
 * \return Array occurrences depending on the input file.
 * \see encoding(), setProbability(), rounding(), setRange(), headerCreation(), 
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void setOccurrences(ifstream& is, uint_32& tot_symbol, array<uint_32, 256>& myarray, bool isTextFile);

/*!
 * \short Function SETPROBABILITY
 * \details Calculating the probability of each symbol.
 * \param &coppie Vector of symbols, probability and starting range. 
 * \param &tot_symbol Total number of symbols read from the stream.
 * \param &myarray Array of occurrences.
 * \return Array probability depending on the input file.
 * \see encoding(), setOccurrences(), rounding(), setRange(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void setProbability(vector<coppia>& coppie, uint_32& tot_symbol, array<uint_32, 256>& myarray);

/*!
 * \short Function ROUNDING
 * \details Rounding to 5 decimal digits for probability.
 * \param &coppie Vector of symbols, probability and starting range.
 * \param &coppie2 Vector with correct probability.
 * \return Vector with probability smoothed to avoid calculation errors.
 * \see encoding(), setOccurrences(), setProbability(), setRange(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void rounding(vector<coppia>& coppie, vector<coppia2>& coppie2);

/*!
 * \short Function SETRANGE
 * \details Assignment start of the range for each symbol.
 * \param &coppie Vector of symbols, probability and starting range.
 * \param &coppie2 Vector with correct probability.
 * \return Vector with final correction value for up to 1 probability.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void setRange(vector<coppia>& coppie, vector<coppia2>& coppie2);

uint_32 inputSize(ifstream& is);
/*!
 * \short Function HEADERCREATION
 * \details Creating header for encoding.
 * \param &coppie2 Vector with correct probability.
 * \param &tot_symbol Total number of symbols read from the stream.
 * \param &os Output file.
 * \return Write to output file initial length, number of ascii symbols with probability greater than zero, probability value without a decimal point.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), setRange(), encode(), 
 *      emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void headerCreation(vector<coppia2>& coppie2, uint_32& tot_symbol, ofstream& os);

/*!
 * \short Function ENCODE
 * \details Calculates the values of top and bottom range, and new range.
 * \param start Starting range.
 * \param size Probability.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \return Range for encoding.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), setRange(), headerCreation(),
 *      emit_digit(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top);

/*!
 * \short Function EMIT_DIGIT
 * \details Change for low range available.
 * \param &low Bottom range.
 * \param &bw Bit writer.
 * \return Bottom range adjusted.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), setRange(), headerCreation(),
 *      encode(), encode_symbol(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void emit_digit(uint_32& low, bitwriter& bw);

/*!
 * \short Function ENCODE_SYMBOL
 * \details Core function for encoding algorithm, for each element manages the range.
 * \param b Unsigned char.
 * \param &x Vector of symbols, probability and starting range.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \param &os Output file.
 * \param &bw Bit writer.
 * \return Encoding symbol.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), setRange(), headerCreation(),
 *      encode(), emit_digit(), encodeAlgorithm(), syntax(), decodeAlgorithm()
 */
void encode_symbol(byte b, vector<coppia2>& x, uint_32& low, uint_32& range, uint_32& top, ofstream& os, bitwriter& bw);

/*!
 * \short Function ENCODEALGORITHM
 * \details Writing encoding on file.
 * \param &is Input file.
 * \param &os Output file.
 * \param &coppie2 Vector with correct probability.
 * \return Write the encoded input file to output file.
 * \see encoding(), setOccurrences(), setProbability(), rounding(), setRange(), headerCreation(),
 *      encode(), emit_digit(), encode_symbol(), syntax(), decodeAlgorithm()
 */
void encodeAlgorithm(ifstream& is, ofstream& os, vector<coppia2>& coppie2);

void printCompressionRatio(ofstream& os, uint_32& inputLength);

#endif // ENCODINGFUNCTION_H