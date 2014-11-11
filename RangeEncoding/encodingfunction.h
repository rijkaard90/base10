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
 * \short Function ENCODE
 * \details Calculates the values of top and bottom range, and new range.
 * \param start Starting range.
 * \param size Probability.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \return Range for encoding.
 * \see syntax(), encoding(), decoding(), emit_digit(), encode_symbol()
 */
void encode(uint_32 start, uint_32 size, uint_32& low, uint_32& range, uint_32& top);

/*!
 * \short Function EMIT_DIGIT
 * \details Change for low range available.
 * \param &low Bottom range.
 * \param &bw Bit writer.
 * \return Bottom range adjusted.
 * \see syntax(), encoding(), decoding(), encode(), encode_symbol()
 */
void emit_digit(uint_32& low, bitwriter& bw);

/*!
 * \short Function ENCODE_SYMBOL
 * \details Core function for encoding algorithm, for each element manages the range.
 * \param b Unsigned char.
 * \param &x Vector of font, probability and starting range.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \param &os Output file.
 * \param &bw Bit writer.
 * \return Encoding symbol.
 * \see syntax(), encoding(), decoding(),  encode(), emit_digit()
 */
void encode_symbol(byte b, vector<coppia2>& x, uint_32& low, uint_32& range, uint_32& top, ofstream& os, bitwriter& bw);

void calcOccurrences(ifstream& is, uint_32& tot_symbol, array<uint_32, 256>& myarray);

void calcProbability(vector<coppia>& coppie, uint_32& tot_symbol, array<uint_32, 256>& myarray);

void rounding(vector<coppia>& coppie, vector<coppia2>& coppie2);

void rangeAssignment(vector<coppia>& coppie, vector<coppia2>& coppie2);

void headerCreation(vector<coppia2>& coppie2, uint_32& tot_symbol, ofstream& os);

void encodeAlgorithm(ifstream& is, ofstream& os, vector<coppia2>& coppie2);

#endif // ENCODINGFUNCTION_H