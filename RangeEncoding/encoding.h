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
 * \short Function ENCODING
 * \details Primary function for encoding algorithm.
 * \param &is Input file.
 * \param &os Output file.
 * \return The encoded input file.
 * \see encode(), emit_digit(), encode_symbol(), decoding(), syntax()
 */
void encoding (ifstream& is, ofstream& os);

#endif // ENCODING_H