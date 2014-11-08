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
 * \details Allows algorithm to calculate new values to be used in decoding.
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

#endif // DECODINGFUNCTION_H