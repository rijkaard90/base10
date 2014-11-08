#ifndef DECODING_H
#define DECODING_H

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
 * \short Function DECODING
 * \details Primary function for decoding algorithm.
 * \param &is Input file.
 * \param &os Output file.
 * \return The decoded input file.
 * \see syntax(), encoding(), read_little_endian(), convert_and_shift()
 */
void decoding(ifstream& is, ofstream& os);

#endif // DECODING_H