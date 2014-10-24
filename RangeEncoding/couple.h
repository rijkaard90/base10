#ifndef COUPLE_H
#define COUPLE_H

typedef unsigned char byte;

/*!
 * \short Class coppia
 * \details Characters, probability and starting range management.
 */
struct coppia {
	byte _b;	/*!< Unsigned char. */
	double _fa; /*!< Probability. */
	double _Fa; /*!< Starting range. */

	/*!
	 * \short Struct initialization
	 * \details Passaggio e assegnazione dei valori.
	 */
	coppia(byte b, double prob, double start = 0) : _b(b), _fa(prob), _Fa(start) {}

	/*!
	 * \short Operator >
	 * \details Overloading operator >.
	 */
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

#endif // COUPLE_H