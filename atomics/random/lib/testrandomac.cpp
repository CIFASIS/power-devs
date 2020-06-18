/************************   testrandomac.cpp   ******************************
* Author:        Agner Fog
* Date created:  2008-11-16
* Last modified: 2008-12-05
* Project:       randoma and randomc
* Description:
* This program tests that the randoma library and randomc code produce
* the same results.
*
* Instructions:
* Compile for console mode, link with the appropriate version of the 
* randoma function library.
*
* Copyright 1997-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*******************************************************************************/

#include <stdio.h>
#include "randoma.h"
#include "mother.cpp"
#include "mersenne.cpp"
#include "sfmt.cpp"
#include "userintf.cpp"

// Sample size
const int nsamp = 1000000;

// Combine SFMT and Mother-Of-All generators
const int usemother = 1;

// Define random seeds
static const int seed = 9876;
static const int initkey[] = {seed,111,222,333,444};
static const int keylen = sizeof(initkey) / sizeof(*initkey);

int main() {
   int i;
   uint32_t sumb, sumi;
   double sumf;

#if 1
   printf("\nTesting Mother-Of-All generator, three different versions:");

   // Mother-Of-All C++ version
   CRandomMother ran11(seed);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran11.BRandom();
      sumi += ran11.IRandom(1,1000000);
      sumf += ran11.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   // Mother-Of-All Library version, thread safe
   CRandomMotherA ran12(seed);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran12.BRandom();
      sumi += ran12.IRandom(1,1000000);
      sumf += ran12.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   // Mother-Of-All Library version, single threaded
   MotherRandomInit(seed);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += MotherBRandom();
      sumi += MotherIRandom(1,1000000);
      sumf += MotherRandom();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);
#endif

#if 1
   printf("\n\nTesting Mersenne Twister generator, three different versions:");

   // Mersenne Twister C++ version
   CRandomMersenne ran21(seed);
   ran21.RandomInitByArray(initkey, keylen);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran21.BRandom();
      sumi += ran21.IRandomX(1,1000000);
      sumf += ran21.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   // Mersenne Twister Library version, thread safe
   CRandomMersenneA ran22(seed);
   ran22.RandomInitByArray(initkey, keylen);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran22.BRandom();
      sumi += ran22.IRandomX(1,1000000);
      sumf += ran22.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   // Mersenne Twister Library version, single threaded
   MersenneRandomInitByArray(initkey, keylen);
   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += MersenneBRandom();
      sumi += MersenneIRandomX(1,1000000);
      sumf += MersenneRandom();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);
#endif

#if 1
   // SFMT C++ version
   printf("\n\nTesting SFMT generator, three different versions:");
   CRandomSFMT ran31(seed, usemother);
   ran31.RandomInitByArray(initkey, keylen);

   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran31.BRandom();
      sumi += ran31.IRandomX(1,1000000);
      sumf += ran31.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   CRandomSFMTA ran32(seed, usemother);
   ran32.RandomInitByArray(initkey, keylen, usemother);

   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += ran32.BRandom();
      sumi += ran32.IRandomX(1,1000000);
      sumf += ran32.Random();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);

   SFMTgenRandomInit(seed, usemother);
   SFMTgenRandomInitByArray(initkey, keylen, usemother);

   sumb = sumi = 0; sumf = 0.;
   for (i = 0; i < nsamp; i++) {
      sumb += SFMTgenBRandom();
      sumi += SFMTgenIRandomX(1,1000000);
      sumf += SFMTgenRandom();
   }
   printf("\n%08X  %08X  %12.4f", sumb, sumi, sumf);
#endif

   printf("\n");
   EndOfProgram();
   return 0;
}
