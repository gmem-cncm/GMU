const floating pi = 3.1415926535897932384626433832795028841971693993751058209749445923;

// Some useful functions to assist in problem solving
// --------------------------------------------------

// Generates tables for polynomial computation.
class PolynomialEvaluatorCoeftable {
public:
	// Factorial table:
	//  __y = Term order in source polynomial
	// |
	// v  x = Term order in result
	//   ___0___1___2___3___4___5___6___
	// 0|  (1)                         
	// 1|   1  (1)                       factorials[0]
	// 2|   2   2  (1)                   factorials[1..2]
	// 3|   6   6   3  (1)               factorials[3..5]
	// 4|  24  24  12   4  (1)           factorials[6..9]
	// 5| 120 120  60  20   5  (1)       factorials[10..14]
	// 6| 720 720 360 120  30   6  (1)   factorials[15..20]   	
	// ... upto table order. Obvious stuff marked with () is not tabled and must be optimized
	floating *factorials;

    // Binomial table, not currently implemented:
	//  __y = Term order in source polynomial
	// |
	// v  x = Term order in result
	//   ___0___1___2___3___4___5___6___
	// 0|  (1)                           
	// 1|  (1) (1)                       
	// 2|  (1)  2  (1)                   binomials[0]
	// 3|  (1)  3   3  (1)               binomials[1..2]
	// 4|  (1)  4   6   4  (1)           binomials[3..5]
	// 5|  (1)  5  10  10   5  (1)       binomials[6..9]
	// 6|  (1)  6  15  20  15   6  (1)   binomials[10..14]
	// ... upto table order. Obvious stuff marked with () is not tabled and must be optimized

	int maxorder; // Maximum order (or y) these tables are capable of

	floating getFactorial(int x, int y) const {
		return factorials[((y-1)*(y-1)+(y-1))/2+x]; // (y == x) is illegal!
	}

	// Ensure the maximum order this table is capable of
	void ensureMaxOrder(int maxorder) {
		if (this->maxorder < maxorder) {
			// Re-create tables
			delete[] factorials;
			factorials = new floating[(maxorder*maxorder+maxorder)/2];
			// Fill tables
			int i, y;
			for (i = 0, y = 1; (y <= maxorder); y++) {
				for (int x = 0; (x < (y-1)); x++) {
					factorials[i++] = factorials[i-y+1]*y;
				}
				factorials[i++] = y;
			}
			this->maxorder = maxorder;
		}
	}

	// Constructor - marks table empty
	PolynomialEvaluatorCoeftable() {
		maxorder = 0;
		factorials = NULL;
	}

	// Destructor - deletes table
	~PolynomialEvaluatorCoeftable() {
		delete[] factorials;
	}	
};

// A class for evaluating polynomials
class PolynomialEvaluator {
private:
	static PolynomialEvaluatorCoeftable coeftable;
	int maxorder;
	floating *x1powers; // Note! Tables start at power = 1
	floating *x2powers;
	floating *winvpowers;
public:

	// Evaluate value of polynomial.
	// Polynomial = c[0] x^e[0] + c[1] x^e[1] + ... + c[terms-1] x^e[terms-1]
	//
	// c[]    = Polynomial coefficients 
	// e[]    = polynomial exponents
	// terms  = Number of terms in polynomial
	// x      = Position to calculate value at
	//
	// Returns: value of polynomial at x
	floating y(const floating *c, const int *e, int terms, floating x) {
		floating accu = 0;
		floating xpow = x;
		int xexp = 0;
		for (int term = 0; (term < terms); term++) {
			for(;(xexp < e[term]); xexp++) {
				x1powers[xexp] = xpow;
				xpow *= x;
			}
			if (e[term]) accu += x1powers[e[term]-1] * c[term];
			else accu += c[term];
		}
		return accu;
	};

	// Integrate polynomial section
	// Polynomial = c[0] x^e[0] + c[1] x^e[1] + ... + c[terms-1] x^e[terms-1]
	//
	// c[]    = Polynomial coefficients 
	// e[]    = polynomial exponents
	// terms  = Number of terms in polynomial
	// x1..x2 = Section to integrate
	//
	// Returns: Integral of polynomial, over x1..x2
	floating integrate(const floating *c, const int *e, int terms, floating x1, floating x2) {
		floating accu = 0;
		floating x1pow = x1, x2pow = x2;
		int xexp = 0;
		for (int term = 0; (term < terms); term++) {
			for(;(xexp < e[term]); xexp++) {
				x1powers[xexp] = x1pow *= x1;
				x2powers[xexp] = x2pow *= x2;
			}
			if (e[term]) accu += c[term]*(x2powers[e[term]-1]-x1powers[e[term]-1])/(e[term]+1);
			else accu += c[term]*(x2-x1);
		}
		return accu;
	};
	
	// Evaluate first derivative of polynomial.
	// Polynomial = c[0] x^e[0] + c[1] x^e[1] + ... + c[terms-1] x^e[terms-1]
	//
	// c[]    = Polynomial coefficients 
	// e[]    = polynomial exponents
	// terms  = Number of terms in polynomial
	// x      = Position to calculate value at
	//
	// Returns: value of differential at x
	floating d1(const floating *c, const int *e, int terms, floating x) {
		floating accu = 0;
		floating xpow = x;
		int xexp = 0;
		for (int term = 0; (term < terms); term++) {
			for(;(xexp < (e[term]-1)); xexp++) {
				x1powers[xexp] = xpow;
				xpow *= x;
			}
			if (e[term]) {
				if (e[term] == 1) accu += c[term];
				else accu += x1powers[e[term]-2] * c[term] * e[term];
			}
		}
		return accu;
	};

	// Evaluate nth derivative of polynomial.
	// Polynomial = c[0] x^e[0] + c[1] x^e[1] + ... + c[terms-1] x^e[terms-1]
	//
	// c[]    = Polynomial coefficients 
	// e[]    = polynomial exponents
	// terms  = Number of terms in polynomial
	// x      = Position to calculate value at
	// n      = derivative number (for example 2 for second derivative)
	//
	// Returns: value of differential at x
	floating dn(const floating *c, const int *e, int terms, floating x, int n) {
		if (n == 0) return y(c, e, terms, x);
		if (n == 1) return d1(c, e, terms, x);
		floating accu = 0;
		floating xpow = x;
		int xexp = 0;
		for (int term = 0; (term < terms); term++) {
			for(;(xexp < (e[term]-n)); xexp++) {
				x1powers[xexp] = xpow;
				xpow *= x;
			}
			if (e[term] >= n) {
				if (e[term] == n) accu += c[term] * coeftable.getFactorial(e[term]-n, e[term]);
				else accu += x1powers[e[term]-1-n] * c[term] * coeftable.getFactorial(e[term]-n, e[term]);
			}
		}
		return accu;
	};

	// Evaluate Fourier transform of polynomial section.
	// Polynomial = c[0] x^e[0] + c[1] x^e[1] + ... + c[terms-1] x^e[terms-1]
	// 
	// c[]    = Polynomial coefficients 
	// e[]    = polynomial exponents
	// terms  = Number of terms in polynomial
    // x1..x2 = Section to Fourier integrate
	// w      = Angular frequency in radians
	// 
	// Returns: realresp and imagresp are summed with the transform result.
	// Please initialize them to zero by yourself before calling. 
	// Magnitude can be calculated as hypot(realresp, imagresp)
	void fourierTransform(const floating *c, const int *e, int terms, floating x1, floating x2, floating w, floating &realresp, floating &imagresp) {
		// If w = 0, we can (and must) use direct integration
		// Also, numerical problems arise when w is small.
		if (w == 0) realresp += integrate(c, e, terms, x1, x2);
		else {
			// Calculate required powers
			floating x1pow = x1, x2pow = x2, winv = 1.0/w, winvpow = winv;
			int term, xexp = 0;
			for (term = 0; (term < terms); term++) {
				for(;(xexp < e[term]); xexp++) {
					x1powers[xexp] = x1pow;
					x2powers[xexp] = x2pow;
					winvpowers[xexp] = winvpow *= winv;
					x1pow *= x1;
					x2pow *= x2;
				}
			}
			// Now xpowers[i] = x^(i+1) and winvpowers[i] = 1/w^(i+2)
			floating realcosaccu1 = 0, realsinaccu1 = 0;
			floating realcosaccu2 = 0, realsinaccu2 = 0;
			for (term = 0; (term < terms); term++) {
				if (e[term] == 0) {
					// Calculate 0th order term with optimizations
					realsinaccu1 += winv*c[term];
					realsinaccu2 += winv*c[term];
				} 
				else {
					// Calculate higher order term
					// Calcualte xexp = 0 separately with optimizations
					floating pa1 = coeftable.getFactorial(0, e[term])*winvpowers[e[term]-1];
					floating pa2 = pa1;
					// Calcualte xexp = e[term] separately with optimizations
					floating pb1, pb2;
					if (e[term] & 1) {
						if (e[term] & 2) { // sub pb
							pb1 = -winv*x1powers[e[term]-1];
							pb2 = -winv*x2powers[e[term]-1];
						} else {           // add pb
							pb1 = +winv*x1powers[e[term]-1];
							pb2 = +winv*x2powers[e[term]-1];
						}
					} else {
						if (e[term] & 2) { // sub pa
							pa1 -= winv*x1powers[e[term]-1];
							pa2 -= winv*x2powers[e[term]-1];
							pb1 = 0;
							pb2 = 0;
						} else {           // add pa
							pa1 += winv*x1powers[e[term]-1];
							pa2 += winv*x2powers[e[term]-1];
							pb1 = 0;
							pb2 = 0;
						}
					}
					// Calcualte xexp = 1 .. e[term]-1
					for (xexp = 1; (xexp <= e[term]-1); xexp++) {
						floating coef = coeftable.getFactorial(xexp, e[term])*winvpowers[e[term]-xexp-1];
						floating temp1 = coef*x1powers[xexp-1];
						floating temp2 = coef*x2powers[xexp-1];
						if (xexp & 1) {
							if (xexp & 2) { // sub pb
								pb1 -= temp1;
								pb2 -= temp2;
							} else {        // add pb
								pb1 += temp1;
								pb2 += temp2;
							}
						} else {
							if (xexp & 2) { // sub pa
								pa1 -= temp1;
								pa2 -= temp2;
							} else {        // add pa
								pa1 += temp1;
								pa2 += temp2;
							}
						}
					}
					pa1 *= c[term]; pb1 *= c[term];
					pa2 *= c[term]; pb2 *= c[term];
					if (e[term] & 1) {
						// a for cos, b for sin
						if (e[term] & 2) {
							// na nb
							realcosaccu1 -= pa1; realsinaccu1 -= pb1; 
							realcosaccu2 -= pa2; realsinaccu2 -= pb2; 
						} else {
							// pa pb
							realcosaccu1 += pa1; realsinaccu1 += pb1; 
							realcosaccu2 += pa2; realsinaccu2 += pb2; 
						}
					} else {
						// b for cos, a for sin
						if (e[term] & 2) {
							// pb na
							realcosaccu1 += pb1; realsinaccu1 -= pa1; 
							realcosaccu2 += pb2; realsinaccu2 -= pa2; 
						} else {
							// nb pa
							realcosaccu1 -= pb1; realsinaccu1 += pa1; 
							realcosaccu2 -= pb2; realsinaccu2 += pa2; 
						}
					}
				}
			}
			floating coswx1 = cos(w*x1), sinwx1 = sin(w*x1);
			floating coswx2 = cos(w*x2), sinwx2 = sin(w*x2);
			// Add transform result to original values, scale by 1/(2pi)
			realresp += (realcosaccu2*coswx2 + realsinaccu2*sinwx2) - (realcosaccu1*coswx1 + realsinaccu1*sinwx1);
			imagresp += (realcosaccu1*sinwx1 - realsinaccu1*coswx1) - (realcosaccu2*sinwx2 - realsinaccu2*coswx2);
		}
	}

	// Constructor
	//
	// maxorder = Maximum orders of the polynomials to be transformed
	//            (for example maxorder = 3 for 2x^3 + 5x^2 and 3x^2 + 1)
	PolynomialEvaluator(int maxorder) {
		coeftable.ensureMaxOrder(maxorder);
		this->maxorder = maxorder;
		x1powers = new floating[maxorder];
		x2powers = new floating[maxorder];
		winvpowers = new floating[maxorder-1];
	}

	// Destructor
	~PolynomialEvaluator() {
		delete[] x1powers;
		delete[] x2powers;
		delete[] winvpowers;
	}
};

// The coefficient table for PolynomialEvaluator
PolynomialEvaluatorCoeftable PolynomialEvaluator::coeftable;