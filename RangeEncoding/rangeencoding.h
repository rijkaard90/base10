#ifndef RANGEENCODING_H
#define RANGEENCODING_H

/*!< Encoding and Decoding for RangeEncoding algorithm. */

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

#include "encoding.h"
#include "decoding.h"

#endif // RANGEENCODING_H