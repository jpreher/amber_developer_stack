/*------------------------------------------------------------------------------

   HXA7241 General library.
   Harrison Ainsworth / HXA7241 : 2004-2011

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include <math.h>

#include "alt_math_utilities/PowFast.hpp"




/// implementation -------------------------------------------------------------

namespace alt_math_utilities
{

/**
 * Following the bit-twiddling idea in:
 *
 * 'A Fast, Compact Approximation of the Exponential Function'
 * Technical Report IDSIA-07-98
 * Nicol N. Schraudolph;
 * IDSIA,
 * 1998-06-24.
 *
 * [Rewritten for floats by HXA7241, 2007.]
 *
 * and the adjustable-lookup idea in:
 *
 * 'Revisiting a basic function on current CPUs: A fast logarithm implementation
 * with adjustable accuracy'
 * Technical Report ICSI TR-07-002;
 * Oriol Vinyals, Gerald Friedland, Nikki Mirghafori;
 * ICSI,
 * 2007-06-21.
 *
 * [Improved (doubled accuracy) and rewritten by HXA7241, 2007.]
 */


const float _2p23 = 8388608.0f;


/**
 * Initialize powFast lookup table.
 *
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 18
 */
void powFastSetTable
(
   unsigned int* const pTable,
   const unsigned int  precision
)
{
   // step along table elements and x-axis positions
   float zeroToOne = 1.0f / (static_cast<float>(1 << precision) * 2.0f);
   for( int i = 0;  i < (1 << precision);  ++i )
   {
      // make y-axis value for table element
      const float f = (::powf( 2.0f, zeroToOne ) - 1.0f) * _2p23;
      pTable[i] = static_cast<unsigned int>( f < _2p23 ? f : (_2p23 - 1.0f) );

      zeroToOne += 1.0f / static_cast<float>(1 << precision);
   }
}


/**
 * Get pow (fast!).
 *
 * @val        power to raise radix to
 * @ilog2      one over log, to required radix, of two
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 18
 */
inline
float powFastLookup
(
   const float         val,
   const float         ilog2,
   unsigned int* const pTable,
   const unsigned int  precision
)
{
   // build float bits
   const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

   // replace mantissa with lookup
   const int it = (i & 0xFF800000) | pTable[(i & 0x7FFFFF) >> (23 - precision)];

   // convert bits to float
   union { int i; float f; } pun;
   return pun.i = it,  pun.f;
}




/// wrapper class --------------------------------------------------------------

PowFast::PowFast
(
   const unsigned int precision
)
 : precision_m( precision <= 18u ? precision : 18u )
 , pTable_m   ( new unsigned int[ 1 << precision_m ] )
{
   powFastSetTable( pTable_m, precision_m );
}


PowFast::~PowFast()
{
   delete[] pTable_m;
}


float PowFast::two
(
   const float f
) const
{
   return powFastLookup( f, 1.0f, pTable_m, precision_m );
}


float PowFast::e
(
   const float f
) const
{
   return powFastLookup( f, 1.44269504088896f, pTable_m, precision_m );
}


float PowFast::ten
(
   const float f
) const
{
   return powFastLookup( f, 3.32192809488736f, pTable_m, precision_m );
}


float PowFast::r
(
   const float logr,
   const float f
) const
{
   return powFastLookup( f, (logr * 1.44269504088896f), pTable_m, precision_m );
}


unsigned int PowFast::precision() const
{
   return precision_m;
}




/// default instance -----------------------------------------------------------
const PowFast& POWFAST()
{
   static const PowFast k( 17 );
   return k;
}


}
