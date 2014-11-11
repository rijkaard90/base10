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
 *         1. encode()
 *         2. emit_digit()
 *         3. encode_symbol()
 *    - \c decoding()
 *         1. read_little_endian()
 *         2. convert_and_shift()
 *
 *  More details will be provided in definition.
 */

/*!
 * \short Function READ_LITTLE_ENDIAN
 * \details Reading for encoding in little endian.
 * \param &is Input file.
 * \param nByte Number of bytes read.
 * \return Occurrences read.
 * \see syntax(), encoding(), decoding(), convert_and_shift()
 */
uint_32 read_little_endian(ifstream& is, uint_32 nByte);

/*!
 * \short Function CONVERT_AND_SHIFT
 * \details If Top and Low has the same first digit, then it is not significant and removed. We add a new digit to the right to improve the precision.
 * \param &sotto Bottom range.
 * \param &sopra Top range.
 * \param &contacaratteri Characters count.
 * \param &rangecont Range of decoding.
 * \param &controllo Characters input.
 * \param &br Bit reader.
 * \param &flagEndCode End
 * \return New values used by RangeEncoding algorithm for decoding.
 * \see syntax(), encoding(), decoding(), read_little_endian();
 */
void convert_and_shift(uint_32& sotto, uint_32& sopra, uint_32& contacaratteri,
	uint_32& rangecont, uint_32& controllo, bitreader& br, bool& flagEndCode);

void decodeAlgorithm(ofstream& os, uint_32& n_caratteri, uint_32& controllo, vector<coppia2>& Dcoppie, bitreader& br);

void rangeFinder(ifstream& is, uint_32& controllo, bitreader& br);

void headerExtraction(ifstream& is, vector<coppia2>& Dcoppie, uint_32& n_caratteri);

void probRange(vector<coppia2>& Dcoppie);
#endif // DECODINGFUNCTION_H