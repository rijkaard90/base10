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
 * \short Function ENCODING
 * \details Primary function for encoding algorithm.
 * \param &is Input file.
 * \param &os Output file.
 * \param isTextFile Specify if we have to compress a text file or another type.
 * \return The encoded input file.
 * \see syntax(), setOccurrences(), setProbability(), rounding(), setRange(), inputSize(), headerCreation(), 
 *      encode(), emit_digit(), encode_symbol(), encodeAlgorithm(), printCompressionRatio(), decoding()
 */
void encoding(ifstream& is, ofstream& os, bool isTextFile);

#endif // ENCODING_H