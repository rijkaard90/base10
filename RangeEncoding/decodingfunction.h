#ifndef DECODINGFUNCTION_H
#define DECODINGFUNCTION_H

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
 * \short Function READ_LITTLE_ENDIAN
 * \details Reading for encoding in little endian.
 * \param &is Input file.
 * \param nByte Number of bytes read.
 * \return Occurrences read.
 * \see decoding(), headerExtraction(), setRangeValue(), setControl(), shift_and_control(), 
 *      decodeAlgorithm(), decodedSize(), syntax(), encoding()
 */
uint_32 read_little_endian(ifstream& is, uint_32 nByte);

/*!
 * \short Function HEADEREXTRACTION
 * \details Extraction header for decoding.
 * \param &is Input file.
 * \param &decodingCouple Vector of symbols, probability and starting range, unsigned.
 * \param &numSymbolsFirstByte How many symbols have to decode in the first byte of file.
 * \return Write to vector symbols and probability.
 * \see decoding(), read_little_endian(), setRangeValue(), setControl(), shift_and_control(),
 *      decodeAlgorithm(), decodedSize(), syntax(), encoding()
 */
void headerExtraction(ifstream& is, vector<u_couple>& decodingCouple, uint_32& numSymbolsFirstByte);

/*!
 * \short Function SETRANGEVALUE
 * \details Insertion probabilities and ranges starting.
 * \param &decodingCouple Vector of symbols, probability and starting range, unsigned.
 * \return Vector with the right probability.
 * \see decoding(), read_little_endian(), headerExtraction(), setControl(), shift_and_control(),
 *      decodeAlgorithm(), decodedSize(), syntax(), encoding()
 */
void setRangeValue(vector<u_couple>& decodingCouple);

/*!
 * \short Function SETCONTROL
 * \details Searching range value.
 * \param &is Input file.
 * \param &control Value that will be searched between top and low range.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \return Value that will be searched.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(), shift_and_control(),
 *      decodeAlgorithm(), decodedSize(), syntax(), encoding()
 */
void setControl(ifstream& is, uint_32& control, bitreader& br);

/*!
 * \short Function SHIFT_AND_CONTROL
 * \details If Top and Low has the same first digit, then it is not significant and removed. 
 *          We add a new digit to the right to improve the precision.
 * \param &below Bottom range.
 * \param &over Top range.
 * \param &contCharacter Symbols count.
 * \param &rangeCont Range of decoding.
 * \param &control Value that will be searched between top and low range.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \param &flagEndCode Flag for end code.
 * \return New values used by RangeEncoding algorithm for decoding.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(), setControl(),
 *      decodeAlgorithm(), decodedSize(), syntax(), encoding()
 */
void shift_and_control(uint_32& below, uint_32& over, uint_32& contCharacter,
	uint_32& rangeCont, uint_32& control, bitreader& br, bool& flagEndCode);

/*!
 * \short Function DECODEALGORITHM
 * \details Writing encoding on file.
 * \param &os Output file.
 * \param &numSymbolsFirstByte How many symbols have to decode in the first byte of file.
 * \param &control Value that will be searched between top and low range.
 * \param &decodingCouple Vector of symbols, probability and starting range, unsigned.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \param &bw Bitwriter for writing the digits stored in 4 bits.
 * \param isTextFile Specify if we have to compress a text file or another type.
 * \return Write the encoded input file to output file.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(), 
 *      setControl(), shift_and_control(), decodedSize(), syntax(), encoding()
 */
void decodeAlgorithm(ofstream& os, uint_32& numSymbolsFirstByte, uint_32& control,
	vector<u_couple>& decodingCouple, bitreader& br, bitwriter& bw, bool isTextFile);

/*!
 * \short Function DECODEDSIZE
 * \details Calculate decoded file size.
 * \param &os Output file.
 * \return Number of bytes in the decoded file.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(), 
 *      setControl(), shift_and_control(), decodeAlgorithm(), syntax(), encoding()
 */
void decodedSize(ofstream& os);

#endif // DECODINGFUNCTION_H