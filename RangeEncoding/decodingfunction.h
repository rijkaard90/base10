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
 * \short Function READ_LITTLE_ENDIAN
 * \details Reading for encoding in little endian.
 * \param &is Input file.
 * \param nByte Number of bytes read.
 * \return Occurrences read.
 * \see syntax(), decoding(), encoding(), double_to_unsigned()
 */
unsigned read_little_endian(ifstream& is, unsigned nByte);

/*!
 * \short Function DOUBLE_TO_UNSIGNED
 * \details Allows switching from double to unsigned without loosing sign.
 * \param &sottoV Double for better rounding bottom range.
 * \param &sotto Bottom range.
 * \param &sopraV Double for better rounding top range.
 * \param &sopra Top range.
 * \param &contacaratteri Characters count.
 * \param &rangecont Range of decoding.
 * \param &controllo Characters input.
 * \param &br Bit reader.
 * \return Unsigned range and data.
 * \see syntax(), decoding(), encoding(), read_little_endian();
 */
void double_to_unsigned(double& sottoV, unsigned& sotto, double& sopraV, unsigned& sopra,
	unsigned& contacaratteri, double& rangecont, unsigned& controllo, bitreader& br);

#endif // DECODINGFUNCTION_H