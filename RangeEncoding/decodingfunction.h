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
 * \short Function READ_LITTLE_ENDIAN
 * \details Reading for encoding in little endian.
 * \param &is Input file.
 * \param nByte Number of bytes read.
 * \return Occurrences read.
 * \see decoding(), headerExtraction(), setRangeValue(), setControl(), 
 *      shift_and_control(), decodeAlgorithm(), syntax(), encoding()
 */
uint_32 read_little_endian(ifstream& is, uint_32 nByte);

/*!
 * \short Function HEADEREXTRACTION
 * \details Extraction header for decoding.
 * \param &is Input file.
 * \param &Dcoppie Vector of symbols, probability and starting range.
 * \param &n_caratteri How many symbols have to decode in the first byte of file.
 * \return Write to vector symbols and probability.
 * \see decoding(), read_little_endian(), setRangeValue(), setControl(), 
 *      shift_and_control(), decodeAlgorithm(), syntax(), encoding()
 */
void headerExtraction(ifstream& is, vector<coppia2>& Dcoppie, uint_32& n_caratteri);

/*!
 * \short Function SETRANGEVALUE
 * \details Insertion probabilities and ranges starting.
 * \param &Dcoppie Vector of symbols, probability and starting range.
 * \return Vector with the right probability.
 * \see decoding(), read_little_endian(), headerExtraction(), setControl(),
 *      shift_and_control(), decodeAlgorithm(), syntax(), encoding()
 */
void setRangeValue(vector<coppia2>& Dcoppie);

/*!
 * \short Function SETCONTROL
 * \details Searching range value.
 * \param &is Input file.
 * \param &controllo Value that will be searched between top and low range.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \return Value that will be searched.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(),
 *      shift_and_control(), decodeAlgorithm(), syntax(), encoding()
 */
void setControl(ifstream& is, uint_32& controllo, bitreader& br);

/*!
 * \short Function SHIFT_AND_CONTROL
 * \details If Top and Low has the same first digit, then it is not significant and removed. We add a new digit to the right to improve the precision.
 * \param &sotto Bottom range.
 * \param &sopra Top range.
 * \param &contacaratteri Symbols count.
 * \param &rangecont Range of decoding.
 * \param &controllo Symbols input.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \param &flagEndCode Flag for end code.
 * \return New values used by RangeEncoding algorithm for decoding.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(), 
 *      setControl(), decodeAlgorithm(), syntax(), encoding()
 */
void shift_and_control(uint_32& sotto, uint_32& sopra, uint_32& contacaratteri,
	uint_32& rangecont, uint_32& controllo, bitreader& br, bool& flagEndCode);

/*!
 * \short Function DECODEALGORITHM
 * \details Writing encoding on file.
 * \param &os Output file.
 * \param &n_caratteri How many symbols have to decode in the first byte of file.
 * \param &controllo Value that will be searched between top and low range.
 * \param &Dcoppie Vector of symbols, probability and starting range.
 * \param &br Bitreader for reading the digits stored in 4 bits.
 * \return Write the encoded input file to output file.
 * \see decoding(), read_little_endian(), headerExtraction(), setRangeValue(),
 *      setControl(), shift_and_control(), syntax(), encoding()
 */
void decodeAlgorithm(ofstream& os, uint_32& n_caratteri, uint_32& controllo, vector<coppia2>& Dcoppie, bitreader& br, bitwriter& bw, char* Type);

#endif // DECODINGFUNCTION_H