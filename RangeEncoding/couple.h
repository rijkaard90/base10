#ifndef COUPLE_H
#define COUPLE_H

typedef unsigned char byte;
typedef unsigned int uint_32;
typedef double float_64;

/*!
 * \short Class coppia
 * \details Symbol, probability and starting range management - Double.
 */
struct coppia {
	byte _b;	  /*!< Unsigned char. */
	float_64 _fa; /*!< Double probability. */
	float_64 _Fa; /*!< Double starting range. */

	/*!
	 * \short Struct initialization
	 * \details Allocation and initialization values - Double.
	 */
	coppia(byte b, float_64 prob, float_64 start = 0) : _b(b), _fa(prob), _Fa(start) {}

	/*!
	 * \short Operator >
	 * \details Overloading operator >.
	 */
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

/*!
 * \short Class coppia2
 * \details Symbol, probability and starting range management - Unsigned.
 */
struct coppia2 {
	byte _b;	 /*!< Unsigned char. */
	uint_32 _fa; /*!< Unsigned probability. */
	uint_32 _Fa; /*!< Unsigned starting range. */

	/*!
	 * \short Struct initialization
	 * \details Allocation and initialization values - Unsigned.
	 */
	coppia2(byte b, uint_32 prob, uint_32 start = 0) : _b(b), _fa(prob), _Fa(start) {}

	/*!
	 * \short Operator >
	 * \details Overloading operator >.
	 */
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

#endif // COUPLE_H