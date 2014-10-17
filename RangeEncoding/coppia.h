#ifndef COPPIA_H
#define COPPIA_H

using namespace std;
typedef unsigned char byte;

/*!
\brief	Classe coppia
\details Gestione dei caratteri insieme alla probabilità e al range di partenza.
*/
struct coppia {
	byte _b;	/*!< Unsigned char. */
	double _fa; /*!< Probabilità della coppia. */
	double _Fa; /*!< Start della coppia. */

	/*!
	\brief	Inizializzazione struct
	\details Passaggio e assegnazione dei valori.
	*/
	coppia(byte b, double prob, double start = 0) : _b(b), _fa(prob), _Fa(start) {}

	/*!
	\brief	Operator >.
	\details Overloading operator >.
	*/
	bool operator> (const coppia& c) const { return _fa > c._fa; }
};

#endif // COPPIA_H