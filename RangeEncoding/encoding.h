#ifndef ENCODING_H
#define ENCODING_H

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
 * \short Function ENCODING
 * \details Primary function for encoding algorithm.
 * \param &is Input file.
 * \param &os Output file.
 * \return The encoded input file.
 * \see encode(), emit_digit(), encode_symbol(), decoding(), syntax()
 */
void encoding (ifstream& is, ofstream& os);

#endif // ENCODING_H