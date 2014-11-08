#ifndef RANGEENCODING_H
#define RANGEENCODING_H

/*!< Encoding and Decoding for RangeEncoding algorithm. */

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

#include "encoding.h"
#include "decoding.h"

#endif // RANGEENCODING_H