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
 * \short Function ENCODE
 * \details Calculates the values of top and bottom range, and new range.
 * \param start Starting range.
 * \param size Probability.
 * \param &low Bottom range.
 * \param &range Size of range.
 * \param &top Top range.
 * \return Range for encoding.
 * \see syntax(), encoding(), emit_digit(), encode_symbol(), decoding()
 */
void encode(double start, double size, double& low, double& range, double& top);

/*!
 * \short Function EMIT_DIGIT
 * \details Change for low range available.
 * \param &low Bottom range.
 * \param &bw Bit writer.
 * \return Bottom range adjusted.
 * \see encoding(), encode(), encode_symbol(), decoding(), syntax()
 */
void emit_digit(double& low, bitwriter& bw);

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
 * \see encoding(), encode(), emit_digit(), decoding(), syntax()
 */
void encode_symbol(byte b, vector<coppia>& x, double& low, double& range, double& top, ofstream& os, bitwriter& bw);

#endif // ENCODINGFUNCTION_H