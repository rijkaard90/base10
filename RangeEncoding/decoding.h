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
 * \short Function DECODING
 * \details Primary function for decoding algorithm.
 * \param &is Input file.
 * \param &os Output file.
 * \return The decoded input file.
 * \see read_little_endian(), double_to_unsigned(), syntax(), encoding()
 */
void decoding(ifstream& is, ofstream& os);

#endif // DECODING_H