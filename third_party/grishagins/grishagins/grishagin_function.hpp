#ifndef __GRISHAGIN_FUNCTION_HPP__
#define __GRISHAGIN_FUNCTION_HPP__

namespace gf {

class grishagin_function {
private:
   int mNumber;
   unsigned char icnf[45];
   double af[49];
   double bf[49];
   double cf[49];
   double df[49];

public:
   grishagin_function(int number);
   ~grishagin_function();

   double calculate(double x, double y) const;
   double minima() const;
   void minimizer(double &x, double &y) const;

private:
   void randomize(int nf);
   double random_20(unsigned char k[]);
   void generate(unsigned char k[], unsigned char k1[], int kap1, int kap2);
};

} // namespace gf
#endif
