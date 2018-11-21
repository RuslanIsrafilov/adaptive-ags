#include <cmath>
#include <string>
#include <cstdlib>
#include <stdexcept>

#include "grishagins/grishagin_function.hpp"
#include "grishagins/grishagin_function_data.hpp"

namespace gf {

grishagin_function::grishagin_function(int number) {
   if (number < GF_MIN_FUNCTION_NUMBER ||
       number > GF_MAX_FUNCTION_NUMBER) {
      throw std::invalid_argument("Grishagin's function number must be between " +
                                 std::to_string(GF_MIN_FUNCTION_NUMBER) + " and " +
                                 std::to_string(GF_MAX_FUNCTION_NUMBER));
   }

   mNumber = number;
   randomize(number);
}

grishagin_function::~grishagin_function() { }


double grishagin_function::calculate(double x, double y) const {
   double d1, d2, sx1, cx1, sy1, cy1;
   double snx[7], csx[7], sny[7], csy[7];

   d1 = gf_pi_value * x;
   d2 = gf_pi_value * y;

   sx1 = std::sin(d1);
   cx1 = std::cos(d1);
   sy1 = std::sin(d2);
   cy1 = std::cos(d2);

   snx[0] = sx1;
   csx[0] = cx1;
   sny[0] = sy1;
   csy[0] = cy1;

   for (int i = 0; i < 6; i++) {
      snx[i + 1] = snx[i] * cx1 + csx[i] * sx1;
      csx[i + 1] = csx[i] * cx1 - snx[i] * sx1;
      sny[i + 1] = sny[i] * cy1 + csy[i] * sy1;
      csy[i + 1] = csy[i] * cy1 - sny[i] * sy1;
   }

   d1 = 0;
   d2 = 0;
   for (int i = 0; i < 7; i++) {
      double csxI = csx[i];
      double snxI = snx[i];
      for (int j = 0; j < 7; j++) {
         double snxIsnyJ = snxI * sny[j];
         double csxIcsyJ = csxI * csy[j];
         d1 += af[i * 7 + j] * snxIsnyJ + bf[i * 7 + j] * csxIcsyJ;
         d2 += cf[i * 7 + j] * snxIsnyJ - df[i * 7 + j] * csxIcsyJ;
      }
   }

   return -std::sqrt(d1 * d1 + d2 * d2);
}

double grishagin_function::minima() const {
   return gf_minimums[mNumber - 1];
}

void grishagin_function::minimizer(double &x, double &y) const {
   int xIndex = 2 * (mNumber - 1);
   int yIndex = xIndex + 1;
   x = gf_minimizers[xIndex];
   y = gf_minimizers[yIndex];
}

void grishagin_function::randomize(int nf) {
   int i1, i2, i3;
   int lst = 10;
   i1 = (nf - 1) / lst;
   i2 = i1 * lst;

   for (int j = 0; j < 45; j++)
      icnf[j] = gf_matcon[i1][j];

   if (i2 != (nf - 1)) {
      i3 = nf - 1 - i2;
      for (int j = 1; j <= i3; j++) {
         for (int i = 0; i < 196; i++)
            random_20(icnf);
      }
   }

   for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
         af[j * 7 + i] = 2 * random_20(icnf) - 1;
         cf[j * 7 + i] = 2 * random_20(icnf) - 1;
      }
   }

   for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
         bf[j * 7 + i] = 2 * random_20(icnf) - 1;
         df[j * 7 + i] = 2 * random_20(icnf) - 1;
      }
   }
}

double grishagin_function::random_20(unsigned char k[]) {
   unsigned char k1[45];
   for (int i = 0; i < 38; i++)
      k1[i] = k[i + 7];

   for (int i = 38; i < 45; i++)
      k1[i] = 0;

   for (int i = 0; i < 45; i++)
      k[i] = std::abs(k[i] - k1[i]);

   for (int i = 27; i < 45; i++)
      k1[i] = k[i - 27];

   for (int i = 0; i < 27; i++)
      k1[i] = 0;

   generate(k, k1, 9, 44);
   generate(k, k1, 0, 8);

   double rndm = 0;
   double de2 = 1;
   for (int i = 0; i < 36; i++) {
      de2 /= 2;
      rndm += k[i + 9] * de2;
   }

   return rndm;
}

void grishagin_function::generate(unsigned char k[], unsigned char k1[], int kap1, int kap2) {
   int jct = 0;
   for (int i = kap2; i >= kap1; i--) {
      int tmpJ = k[i] + k1[i] + jct;
      jct = tmpJ / 2;
      k[i] = tmpJ - 2 * jct;
   }

   if (jct != 0) {
      for (int i = kap2; i >= kap1; i--) {
         int tmpJ = k[i] + jct;
         jct = tmpJ / 2;
         k[i] = tmpJ - jct * 2;
      }
   }
}

} // namespace gf
