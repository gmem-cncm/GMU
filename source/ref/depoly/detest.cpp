#include <conio.h>
#include <stdio.h>
#include <complex>
#include <math.h>
#include <stdlib.h>
#include "de.h"
#include "polyeval.h"
#include "epsplot.h"

#define FORKPRECISION 32

const int optiipterms = 4;
const int optiipsections = 2;

const int optiipoversampling = 2;
const floating optiiptransition = 0; // Extra transition band for 1x

const char optiipname[] = "Optimal 2x";

const int optiipsubsamples = 32; //32 is official
const int optiipstopbands = 4;

PolynomialEvaluator polyeval(10);

struct PolynomialInterpolator {
	const floating *c;
	int terms; // Order = terms-1
	int pxsections;
	const char *name;
	const char *categoryname;
	static const int exp[];
	int oversampling;
	int preemphasis;
	int costypreemphasis;
	int pinked;
	char generatedcategoryname[200];

	void generateCategoryName() {		
		int order = terms-1;
		int points = pxsections*2;
		if (order == 1) {
			sprintf(generatedcategoryname, "%d-point, 1st-order", points);
		} else if (order == 2) {
			sprintf(generatedcategoryname, "%d-point, 2nd-order", points);
		} else if (order == 3) {
			sprintf(generatedcategoryname, "%d-point, 3rd-order", points);
		} else {
			sprintf(generatedcategoryname, "%d-point, %dth-order", points, order);
		}
		categoryname = generatedcategoryname;
	}

	const char *getCategoryName() {
		if (!categoryname) generateCategoryName();
		return categoryname;
	}

	void getFullName(char *buffer) {
		if (pinked) {
			if (preemphasis) {
				if (oversampling == 1) {
					sprintf(buffer, "Pinked, pre-emphasized, %s (%s)", name, getCategoryName());
				} else {
					sprintf(buffer, "%dx-oversampled, pinked, pre-emphasized, %s (%s)", oversampling, name, getCategoryName());
				}
			} else {
				if (oversampling == 1) {
					sprintf(buffer, "Pinked, %s (%s)", name, getCategoryName());
				} else {
					sprintf(buffer, "%dx-oversampled, pinked, %s (%s)", oversampling, name, getCategoryName());
				}
			}
		} else {
			if (preemphasis) {
				if (oversampling == 1) {
					sprintf(buffer, "Pre-emphasized, %s (%s)", name, getCategoryName());
				} else {
					sprintf(buffer, "%dx-oversampled, pre-emphasized, %s (%s)", oversampling, name, getCategoryName());
				}
			} else {
				if (oversampling == 1) {
					sprintf(buffer, "%s (%s)", name, getCategoryName());
				} else {
					sprintf(buffer, "%dx-oversampled, %s (%s)", oversampling, name, getCategoryName());
				}
			}
		}
	}

	PolynomialInterpolator(floating *c, int terms, int pxsections, const char *name) {
		this->c = c;
		this->terms = terms;
		this->pxsections = pxsections;
		this->name = name;
		oversampling = 1;
		preemphasis = 0;
		pinked = 0;
	}

	PolynomialInterpolator() {
		oversampling = 1;
		preemphasis = 0;
		pinked = 0;
	}

	void overSample(int oversampling) {
		this->oversampling = oversampling;
	}

	void preEmphasize() {
		preemphasis = 1;
	}

	void pink() {
		pinked = 1;
	}

	void modify(int oversampling) {
		overSample(oversampling);
		preEmphasize();
		pink();
	}

	void unModify() {
		oversampling = 1;
		preemphasis = 0;
		pinked = 0;
	}

	floating evaluateFrequencyResponse(floating w) const {
		if (w < 0.05) w = 0; // Floating point accuracy fix
		floating wmod = pi-fabs(fmod(w, 2*pi)-pi);
		if ((oversampling > 1) && (wmod > (pi/oversampling))) return 0;
		floating realresp = 0, imagresp = 0;
		for (int s = 0; (s < pxsections); s++) {
			polyeval.fourierTransform(&c[s*terms], exp, terms, s, s+1, w, realresp, imagresp);
		}
		floating resp = 2*realresp;
		if (preemphasis) {
			realresp = 0; imagresp = 0;
			for (int s = 0; (s < pxsections); s++) {
				polyeval.fourierTransform(&c[s*terms], exp, terms, s, s+1, (wmod < 0.05)? 0 : wmod, realresp, imagresp);
			}
			resp /= fabs(2*realresp);
		}
		if (pinked && (w > pi)) {
			if (wmod < (5.0/22050)) wmod = 5.0/22050;
			resp *= sqrt(pi)/sqrt(wmod*oversampling);
		}
		
		return resp;
	}

	void printFunction() {
		for (int section = 0; (section < pxsections); section++) {
			printf("\n[%d..%d] f(x) =", section, section+1);
			for (int term = terms-1; (term >= 0); term--) {
				if (exp[term] == 1) {
					printf("\n%.17f * x +", c[terms*section+term]); 
				} else if (exp[term] == 0) {
					printf("\n%.17f", c[terms*section+term]); 
				} else {
					printf("\n%.17f * x^%d +", c[terms*section+term], exp[term]); 
				}
			}
		}
	}
};

const int PolynomialInterpolator::exp[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

class PolynomialInterpolatorFrequencyResponse: public OneArgumentFunction {
public:
	PolynomialInterpolator *ip;
	// Return function value at x
	floating evaluate(floating w) {
		return fabs(ip->evaluateFrequencyResponse(w));
	}
	PolynomialInterpolatorFrequencyResponse() {};
	PolynomialInterpolatorFrequencyResponse(PolynomialInterpolator *ip) {
		this->ip = ip;
	}

};

class PolynomialInterpolatorRoutinePrint{
public:
	static void zForm(PolynomialInterpolator *ip) {
		int o = ip->terms-1;
		int p = ip->pxsections*2;
		char buffer[200];
		ip->getFullName(buffer);
		printf("// %s (z-form)", buffer);
		printf("\nfloat z = x - 1/2.0;");
		printf("\nfloat even1 = y[1]+y[0], odd1 = y[1]-y[0];");
		if (p > 2) printf("\nfloat even2 = y[2]+y[-1], odd2 = y[2]-y[-1];");
		if (p > 4) printf("\nfloat even3 = y[3]+y[-2], odd3 = y[3]-y[-2];");
		floating a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		floating b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0;
		floating c0 = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;
		{
			a0 = ip->c[0*ip->terms + 0];
			a1 = ip->c[0*ip->terms + 1];
			a2 = ip->c[0*ip->terms + 2];
			if (o > 2) a3 = ip->c[0*ip->terms + 3];
			if (o > 3) a4 = ip->c[0*ip->terms + 4];
			if (o > 4) a5 = ip->c[0*ip->terms + 5];
		}
		if (p > 2) {
			b0 = ip->c[1*ip->terms + 0];
			b1 = ip->c[1*ip->terms + 1];
			b2 = ip->c[1*ip->terms + 2];
			if (o > 2) b3 = ip->c[1*ip->terms + 3];
			if (o > 3) b4 = ip->c[1*ip->terms + 4];
			if (o > 4) b5 = ip->c[1*ip->terms + 5];
		}
		if (p > 4) {
			c0 = ip->c[2*ip->terms + 0];
			c1 = ip->c[2*ip->terms + 1];
			c2 = ip->c[2*ip->terms + 2];
			if (o > 2) c3 = ip->c[2*ip->terms + 3];
			if (o > 3) c4 = ip->c[2*ip->terms + 4];
			if (o > 4) c5 = ip->c[2*ip->terms + 5];
		}
		floating c0y1 = (32*a0+16*a1+8*a2+4*a3+2*a4+a5)/32;
		floating c0y2 = (32*b0+3*(16*b1+3*(8*b2+3*(4*b3+6*b4+9*b5))))/32;
		floating c0y3 = (32*c0+5*(16*c1+5*(8*c2+5*(4*c3+10*c4+25*c5))))/32;
		floating c1y1 = -(16*a1+16*a2+12*a3+8*a4+5*a5)/16;
		floating c1y2 = -(16*b1+3*(16*b2+9*(4*b3+8*b4+15*b5)))/16;
		floating c1y3 = -(16*c1+5*(16*c2+5*(12*c3+5*(8*c4+25*c5))))/16;	
		floating c2y1 = (4*a2+6*a3+6*a4+5*a5)/4;
		floating c2y2 = (4*b2+9*(2*b3+6*b4+15*b5))/4;
		floating c2y3 = (4*c2+5*(6*c3+30*c4+125*c5))/4;
		floating c3y1 = -(2*a3+4*a4+5*a5)/2;
		floating c3y2 = -(2*b3+3*(4*b4+15*b5))/2;
		floating c3y3 = -(2*c3+5*(4*c4+25*c5))/2;
		floating c4y1 = (2*a4+5*a5)/2;
		floating c4y2 = (2*b4+15*b5)/2;
		floating c4y3 = (2*c4+25*c5)/2;
		floating c5y1 = -a5;
		floating c5y2 = -b5;
		floating c5y3 = -c5;
		{
			printf("\nfloat c0 = even1*%.17f", c0y1);
			if (p > 2) printf(" + even2*%.17f", c0y2);
			if (p > 4) printf("\n    + even3*%.17f", c0y3);
			printf(";");
		}
		{
			printf("\nfloat c1 = odd1*%.17f", c1y1);
			if (p > 2) printf(" + odd2*%.17f", c1y2);
			if (p > 4) printf("\n    + odd3*%.17f", c1y3);
			printf(";");
		}
		{
			printf("\nfloat c2 = even1*%.17f1", c2y1);
			if (p > 2) printf(" + even2*%.17f", c2y2);
			if (p > 4) printf("\n    + even3*%.17f", c2y3);
			printf(";");
		}
		if (o > 2) {
			printf("\nfloat c3 = odd1*%.17f", c3y1);
			if (p > 2) printf(" + odd2*%.17f", c3y2);
			if (p > 4) printf("\n    + odd3*%.17f", c3y3);
			printf(";");
		}
		if (o > 3) {
			printf("\nfloat c4 = even1*%.17f", c4y1);
			if (p > 2) printf(" + even2*%.17f", c4y2);
			if (p > 4) printf("\n    + even3*%.17f", c4y3);
			printf(";");
		}
		if (o > 4) {
			printf("\nfloat c5 = odd1*%.17f", c5y1);
			if (p > 2) printf(" + odd2*%.17f", c5y2);
			if (p > 4) printf("\n    + odd3*%.17f", c5y3);
			printf(";");
		}
		if (o < 3) printf("\nreturn (c2*z+c1)*z+c0;");
		if (o == 3) printf("\nreturn ((c3*z+c2)*z+c1)*z+c0;");
		if (o == 4) printf("\nreturn (((c4*z+c3)*z+c2)*z+c1)*z+c0;");
		if (o == 5) printf("\nreturn ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;");
		printf("\n");
	}
};

class PolynomialInterpolatorImpulseResponsePlot {
public:
	int graphright, graphtop, xunity, yunity;
	EPSPlot *eps; // You can change line type here

	void plot(PolynomialInterpolator *ip, int differential) {
		const float maxerror = 0.0625f;
		const float leftsidemul = (differential & 1)? -1.0f : 1.0f;

		float *xplot = new float[xunity+1];
		float *yplot = new float[xunity+1];

		for (int s = 0; (s < ip->pxsections); s++) {
			// x = s..s+1
			int t;
			for (t = 0; (t <= xunity); t++) {
				floating x = s+t/(float)xunity;
				xplot[t] = (float)(xunity*s+t);
				yplot[t] = (float)yunity*(float)polyeval.dn(&(ip->c[s*(ip->terms)]), ip->exp, ip->terms, x, differential);
			}
			eps->drawLines(xplot, yplot, xunity+1, maxerror, (float)-graphright, (float)-graphtop, (float)graphright, (float)graphtop);
			for (t = 0; (t <= xunity); t++) { // mirror
				xplot[t] = -xplot[t];
				yplot[t] = yplot[t]*leftsidemul;
			}
			eps->drawLines(xplot, yplot, xunity+1, maxerror, (float)-graphright, (float)-graphtop, (float)graphright, (float)graphtop);
		}

		delete xplot;
		delete yplot;

		if (differential == 0) eps->drawText((float)(-graphright+2), (float)(graphtop-7), "f(x)");
		if (differential == 1) eps->drawText((float)(-graphright+2), (float)(graphtop-7), "f'(x)");
		if (differential == 2) eps->drawText((float)(-graphright+2), (float)(graphtop-7), "f''(x)");
		if (differential == 3) eps->drawText((float)(-graphright+2), (float)(graphtop-7), "f'''(x)");
		if (differential == 4) eps->drawText((float)(-graphright+2), (float)(graphtop-7), "f''''(x)");
		eps->drawText((float)(-graphright+2), (float)(-graphtop+9), ip->name);
		char buffer[200];
		sprintf(buffer, "(%s)", ip->getCategoryName());
		eps->drawText((float)(-graphright+2), (float)(-graphtop+3), buffer);
	}	

	PolynomialInterpolatorImpulseResponsePlot(const char *filename, int graphright, int graphtop, int xunity, int yunity) {
		eps = new EPSPlot(filename, (float)-graphright, (float)-graphtop, (float)graphright, (float)graphtop);
		this->graphright = graphright;
		this->graphtop = graphtop;
		this->xunity = xunity;
		this->yunity= yunity;
		eps->setRGBColor(0.75f, 0, 0);
		eps->setLineCap(0);
		eps->setLineJoin(0);
		eps->setNoDash();
		eps->setLineWidth(0.5);
		eps->drawBox((float)-graphright, (float)-graphtop, (float)graphright, (float)graphtop);		
		eps->setLineWidth(0.25);
		eps->drawLine((float)-graphright, 0, (float)graphright, 0);
		eps->drawLine(0, (float)-graphtop, 0, (float)graphtop);
		eps->setDash(0.25, 2.25);
		int t;
		if (yunity >= 4) {
			for (t = yunity; (t < graphtop); t += yunity) {
				eps->drawLine((float)-graphright, (float)t, (float)graphright, (float)t);
				eps->drawLine((float)-graphright, (float)-t, (float)graphright, (float)-t);
			}
		}
		for (t = xunity; (t < graphright); t += xunity) {
			eps->drawLine((float)t, (float)-graphtop, (float)t, (float)graphtop);
			eps->drawLine((float)-t, (float)-graphtop, (float)-t, (float)graphtop);
		}
		eps->setLineCap(1);
		eps->setLineJoin(1);
		eps->setLineWidth(0.5);
		eps->setNoDash();
		eps->setGray(0);
	}
	~PolynomialInterpolatorImpulseResponsePlot() {
		delete eps;
	}
};

const char *hermitename = "Hermite";
floating hermite3rdc[] = {1.0, 0.0, -5.0/2.0, 3.0/2.0, 2, -4, 5.0/2.0, -1.0/2.0};
PolynomialInterpolator hermitep4o3(hermite3rdc, 4, 2, hermitename);
floating hermite5thc[] = {1, 0, -25.0/12.0, 5.0/12.0, 13.0/12.0, -5.0/12.0, 1, 5.0/12.0, -35.0/8.0, 35.0/8.0, -13.0/8.0, 5.0/24.0, 3, -29.0/4.0, 155.0/24.0, -65.0/24.0, 13.0/24.0, -1.0/24.0};
PolynomialInterpolator hermitep6o5(hermite5thc, 6, 3, hermitename);
floating hermite3rd6pc[] = {1, 0, -7.0/3.0, 4.0/3.0, 5.0/2.0, -59.0/12.0, 3, -7.0/12.0, -3.0/2.0, 7.0/4.0, -2.0/3.0, 1.0/12.0};
PolynomialInterpolator hermitep6o3(hermite3rd6pc, 4, 3, hermitename);

const char *linearname = "Linear";
floating linear1stc[] = {1, -1};
PolynomialInterpolator linear(linear1stc, 2, 1, linearname);

//Cardinal cubic splines a = alpha
//x^3(a+2)-x^2(a+3)+1       x = 0..1
//ax^2-5ax^2+8ax-4a         x = 1..2
//a = -1/2: Catmull-Rom = Hermite!    a = derivative at x=1
//3/2 x^3 - 5/2 x^2 + 1            x = 0..1 
//-1/2 x^3 + 5/2 x^2 - 4 x + 2     x = 1..2

const char *osculating2oname = "2nd-order-osculating";
floating osculating2o4pc[] = {1, 0, -1, -9.0/2.0, 15.0/2.0, -3, -4, 18, -29, 43.0/2.0, -15.0/2.0, 1};
PolynomialInterpolator o2osculatingp4o5(osculating2o4pc, 6, 2, osculating2oname);
floating osculating2o6pc[] = {1, 0, -5.0/4.0, -35.0/12.0, 21.0/4.0, -25.0/12.0, -4, 75.0/4.0, -245.0/8.0, 545.0/24.0, -63.0/8.0, 25.0/24.0, 18, -153.0/4.0, 255.0/8.0, -313.0/24.0, 21.0/8.0, -5.0/24.0};
PolynomialInterpolator o2osculatingp6o5(osculating2o6pc, 6, 3, osculating2oname);

const char *lagrangename = "Lagrange"; 
floating lagrange3rdc[] = {1, -1.0/2.0, -1, 1.0/2.0, 1, -11.0/6.0, 1, -1.0/6.0};
PolynomialInterpolator lagrangep4o3(lagrange3rdc, 4, 2, lagrangename);
floating lagrange5thc[] = {1, -1.0/3.0, -5.0/4.0, 5.0/12.0, 1.0/4.0, -1.0/12.0, 1, -13.0/12.0, -5.0/8.0, 25.0/24.0, -3.0/8.0, 1.0/24.0, 1.0, -137.0/60.0, 15.0/8.0, -17.0/24.0, 1.0/8.0, -1.0/120.0};
PolynomialInterpolator lagrangep6o5(lagrange5thc, 6, 3, lagrangename);

const char *bsplinename = "B-spline";
floating bspline3rdc[] = {2.0/3.0, 0, -1, 1.0/2.0, 4.0/3.0, -2, 1, -1.0/6.0};
PolynomialInterpolator bsplinep4o3(bspline3rdc, 4, 2, bsplinename);
floating bspline5thc[] = {11.0/20.0, 0, -1.0/2.0, 0, 1.0/4.0, -1.0/12.0, 17.0/40.0, 5.0/8.0, -7.0/4.0, 5.0/4.0, -3.0/8.0, 1.0/24.0, 81.0/40.0, -27.0/8.0, 9.0/4.0, -3.0/4.0, 1.0/8.0, -1.0/120.0};
PolynomialInterpolator bsplinep6o5(bspline5thc, 6, 3, bsplinename);

const char *parabola2xname = "Parabolic 2x";
floating parabola2x2ndc[] = {1.0/2.0, 0, -1.0/4.0, 1, -1, 1.0/4.0};
PolynomialInterpolator parabolic2x(parabola2x2ndc, 3, 2, parabola2xname);

const char *wattetrilinearname = "Watte tri-linear";
floating wattetrilinearc[] = {1, -1.0/2.0, -1.0/2.0, 1, -3.0/2.0, 1.0/2.0};
PolynomialInterpolator wattetrilinear(wattetrilinearc, 3, 2, wattetrilinearname);

//2X-OVERSAMPLING------------------------------------------------------

const char *optimal2xname = "Optimal 2x";

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (2-point, 3rd-order)
floating optimal2xp2o3c[] =
{0.80607906469176971, 0.17594740788514596, -2.35977550974341630, 1.57015627178718420};
PolynomialInterpolator optimal2xp2o3(optimal2xp2o3c, 4, 1, optimal2xname);

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (4-point, 2nd-order)
floating optimal2xp4o2c[] =
{0.50061662213752656, -0.04782068534965925, -0.21343978756177684,
0.92770135528027386, -0.88689658749623701, 0.21303593243799016};
PolynomialInterpolator optimal2xp4o2(optimal2xp4o2c, 3, 2, optimal2xname);

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (4-point, 3rd-order)
floating optimal2xp4o3c[] =
{0.59244492420272321, 0.03573669883299365, -0.78664888597764893, 0.36030925263849456,
1.20220428331406090, -1.60101160971478710, 0.70401463131621556, -0.10174985775982505};
PolynomialInterpolator optimal2xp4o3(optimal2xp4o3c, 4, 2, optimal2xname);

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (4-point, 4th-order)
floating optimal2xp4o4c[] =
{0.58448510036125145, 0.04442540676862300, -0.75864870418278074, 0.29412762852131868, 0.04252164479749607,
1.06598379704160570, -1.16581445347275190, 0.21256821036268256, 0.13781898240764315, -0.04289144034653719};
PolynomialInterpolator optimal2xp4o4(optimal2xp4o4c, 5, 2, optimal2xname);

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (6-point, 4th-order)
floating optimal2xp6o4c[] =
{0.42640922432669054, -0.00525580294341423, -0.20486985491012843, 0.00255494211547300, 0.03134095684084392,
0.30902529029941583, 0.37868437559565432, -0.70564644117967990, 0.31182026815653541, -0.04385804833432710,
1.51897639740576910, -1.83761742915820410, 0.83217835730406542, -0.16695522597587154, 0.01249475765486819};
PolynomialInterpolator optimal2xp6o4(optimal2xp6o4c, 5, 3, optimal2xname);

//2x-oversampled, pinked, pre-emphasized, Optimal 2x (6-point, 5th-order)
floating optimal2xp6o5c[] =
{0.48217702203158502, -0.00127577239632662, -0.32675071713952775, -0.02014846731685776, 0.14640674192652170, -0.04317950185225609,
0.35095903476754237, 0.53534756396439365, -1.22477236472789920, 0.74995484587342742, -0.19234043023690772, 0.01802814255926417,
1.62814578813495040, -2.26168360510917840, 1.22220278720010690, -0.31577407091450355, 0.03768876199398620, -0.00152170021558204};
PolynomialInterpolator optimal2xp6o5(optimal2xp6o5c, 6, 3, optimal2xname);

//4X-OVERSAMPLING------------------------------------------------------

const char *optimal4xname = "Optimal 4x";

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (2-point, 3rd-order)
floating optimal4xp2o3c[] =
{0.88207975731800936, -0.10012219395448523, -1.99054787320203810, 1.32598918957298410};
PolynomialInterpolator optimal4xp2o3(optimal4xp2o3c, 4, 1, optimal4xname);

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (4-point, 2nd-order)
floating optimal4xp4o2c[] =
{0.33820365736567115, 0.21144498075197282, -0.22865399531858188,
1.12014639874555470, -1.01414466618792900, 0.22858390767180370};
PolynomialInterpolator optimal4xp4o2(optimal4xp4o2c, 3, 2, optimal4xname);

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (4-point, 3rd-order)
floating optimal4xp4o3c[] =
{0.60304009430474115, 0.05694012453786401, -0.89223007211175309, 0.42912649274763925,
1.31228823423882930, -1.85072890189700660, 0.87687351895686727, -0.13963062613760227};
PolynomialInterpolator optimal4xp4o3(optimal4xp4o3c, 4, 2, optimal4xname);

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (4-point, 4th-order)
floating optimal4xp4o4c[] =
{0.61340295990566229, 0.06128937679587994, -0.94057832565094635, 0.44922093286355397, 0.00986988334359864,
1.30835018075821670, -1.82814511658458520, 0.81943257721092366, -0.09642760567543440, -0.00989340017126506};
PolynomialInterpolator optimal4xp4o4(optimal4xp4o4c, 5, 2, optimal4xname);

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (6-point, 4th-order)
floating optimal4xp6o4c[] =
{0.20167941634921072, -0.06119274485321008, 0.56468711069379207, -0.42059475673758634, 0.02881527997393852,
-0.64579641436229407, 2.33580825807694700, -1.85350543411307390, 0.51926458031522660, -0.04250898918476453,
2.76228852293285200, -3.09936092833253300, 1.27147464005834010, -0.22283280665600644, 0.01369173779618459};
PolynomialInterpolator optimal4xp6o4(optimal4xp6o4c, 5, 3, optimal4xname);

//4x-oversampled, pinked, pre-emphasized, Optimal 4x (6-point, 5th-order)
floating optimal4xp6o5c[] =
{0.50164509338655083, -0.00256790184606694, -0.36229943140977111, -0.04512026308730401, 0.20620318519804220, -0.06607747864416924,
0.30718330223223800, 0.78336433172501685, -1.66940481896969310, 1.08365113099941970, -0.30560854964737405, 0.03255079211953620,
2.05191571792256240, -3.19403437421534920, 1.99766476840488070, -0.62765808573554227, 0.09909173357642603, -0.00628989632244913};
PolynomialInterpolator optimal4xp6o5(optimal4xp6o5c, 6, 3, optimal4xname);

//8X-OVERSAMPLING------------------------------------------------------

const char *optimal8xname = "Optimal 8x";

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (2-point, 3rd-order)
floating optimal8xp2o3c[] =
{0.94001491168487883, -0.51213628865925998, -1.10319974084152170, 0.73514591836770027};
PolynomialInterpolator optimal8xp2o3(optimal8xp2o3c, 4, 1, optimal8xname);

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (4-point, 2nd-order)
floating optimal8xp4o2c[] =
{0.09224718574204172, 0.59257579283164508, -0.24005206207889518,
1.38828036063664320, -1.17126532964206100, 0.24004281672637814};
PolynomialInterpolator optimal8xp4o2(optimal8xp4o2c, 3, 2, optimal8xname);

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (4-point, 3rd-order)
floating optimal8xp4o3c[] =
{0.60658368706046584, 0.07280793921972525, -0.95149675410360302, 0.46789242171187317,
1.35919815911169020, -1.95618744839533010, 0.94949311590826524, -0.15551896027602030};
PolynomialInterpolator optimal8xp4o3(optimal8xp4o3c, 4, 2, optimal8xname);

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (4-point, 4th-order)
floating optimal8xp4o4c[] =
{0.62095991632974834, 0.06389302461261143, -0.98489647972932193, 0.48698871865064902, 0.00255074537015887,
1.35943398999940390, -1.97277963497287720, 0.95410568622888214, -0.14868053358928229, -0.00255226912537286};
PolynomialInterpolator optimal8xp4o4(optimal8xp4o4c, 5, 2, optimal8xname);

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (6-point, 4th-order)
floating optimal8xp6o4c[] =
{-0.17436452172055789, -0.15190225510786248, 1.87551558979819120, -1.15976496200057480, 0.03401038103941584,
-2.26955357035241170, 5.73320660746477540, -3.92391712129699590, 0.93463067895166918, -0.05090907029392906,
4.84834508915762540, -5.25661448354449060, 2.04584149450148180, -0.32814290420019698, 0.01689861603514873};
PolynomialInterpolator optimal8xp6o4(optimal8xp6o4c, 5, 3, optimal8xname);

//8x-oversampled, pinked, pre-emphasized, Optimal 8x (6-point, 5th-order)
floating optimal8xp6o5c[] =
{0.50513183702821474, -0.00368143670114908, -0.36434084624989699, -0.06070462616102962, 0.22942797169644802, -0.07517133281176167,
0.28281884957695946, 0.88385964850687193, -1.82581238657617080, 1.19588167464050650, -0.34363487882262922, 0.03751837438141215,
2.15756386503245070, -3.42137079071284810, 2.18592382088982260, -0.70370361187427199, 0.11419603882898799, -0.00747588873055296};
PolynomialInterpolator optimal8xp6o5(optimal8xp6o5c, 6, 3, optimal8xname);

//16X-OVERSAMPLING------------------------------------------------------

const char *optimal16xname = "Optimal 16x";

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (2-point, 3rd-order)
floating optimal16xp2o3c[] =
{0.96964782067188493, -0.74617479745643256, -0.57923093055631791, 0.38606621963374965};
PolynomialInterpolator optimal16xp2o3(optimal16xp2o3c, 4, 1, optimal16xname);

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (4-point, 2nd-order)
floating optimal16xp4o2c[] =
{-0.41849525763976203, 1.36361593203840510, -0.24506117865474364,
1.90873339502208310, -1.44144384373471430, 0.24506002360805534};
PolynomialInterpolator optimal16xp4o2(optimal16xp4o2c, 3, 2, optimal16xname);

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (4-point, 3rd-order)
floating optimal16xp4o3c[] =
{0.60844825096346644, 0.07980169577604959, -0.97894238166068270, 0.48601256046234864,
1.37724137476464990, -1.99807048591354810, 0.97870442828560433, -0.16195131297091253};
PolynomialInterpolator optimal16xp4o3(optimal16xp4o3c, 4, 2, optimal16xname);

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (4-point, 4th-order)
floating optimal16xp4o4c[] =
{0.62293049365660191, 0.06443376638262904, -0.99620011474430481, 0.49672182806667398, 0.00064264050033187,
1.37216269878963180, -2.00931632449031920, 0.98847675044522398, -0.16214364417487748, -0.00064273459469381};
PolynomialInterpolator optimal16xp4o4(optimal16xp4o4c, 5, 2, optimal16xname);

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (6-point, 4th-order)
floating optimal16xp6o4c[] =
{-0.94730014688427577, -0.33649680079382827, 4.53807483241466340, -2.64598691215356660, 0.03755086455339280,
-5.55035312316726960, 12.52871168241192600, -7.98288364772738750, 1.70665858343069510, -0.05631219122315393,
8.94785524286246310, -9.37021675593126700, 3.44447036756440590, -0.49470749109917245, 0.01876132424143207};
PolynomialInterpolator optimal16xp6o4(optimal16xp6o4c, 5, 3, optimal16xname);

//16x-oversampled, pinked, pre-emphasized, Optimal 16x (6-point, 5th-order)
floating optimal16xp6o5c[] =
{0.50819303579369868, -0.00387117789818541, -0.36990908725555449, -0.06616250180411522, 0.24139298776307896, -0.07990500783668089,
0.27758734130911511, 0.91870010875159547, -1.89281840112089440, 1.24834464824612510, -0.36203450650610985, 0.03994519162531633,
2.19284545406407450, -3.50786533926449100, 2.26228244623301580, -0.73559668875725392, 0.12064126711558003, -0.00798609327859495};
PolynomialInterpolator optimal16xp6o5(optimal16xp6o5c, 6, 3, optimal16xname);

//32X-OVERSAMPLING------------------------------------------------------

const char *optimal32xname = "Optimal 32x";

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (2-point, 3rd-order)
floating optimal32xp2o3c[] =
{0.98472017575676363, -0.87053863725307623, -0.29667081825572522, 0.19775766248673177};
PolynomialInterpolator optimal32xp2o3(optimal32xp2o3c, 4, 1, optimal32xname);

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (4-point, 2nd-order)
floating optimal32xp4o2c[] =
{-1.42170796824052890, 2.87083485132510450, -0.24755243839713828,
2.91684291662070860, -1.95043794419108290, 0.24755229501840223};
PolynomialInterpolator optimal32xp4o2(optimal32xp4o2c, 3, 2, optimal32xname);

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (4-point, 3rd-order)
floating optimal32xp4o3c[] =
{0.60908264223655417, 0.08298544053689563, -0.99052586766084594, 0.49369595780454456,
1.38455689452848450, -2.01496368680360890, 0.99049753216621961, -0.16455902278580614};
PolynomialInterpolator optimal32xp4o3(optimal32xp4o3c, 4, 2, optimal32xname);

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (4-point, 4th-order)
floating optimal32xp4o4c[] =
{0.62342449465938121, 0.06456923251842608, -0.99904509583176049, 0.49917660509564427, 0.00016095224137360,
1.37534629142898650, -2.01847637982642340, 0.99711292321092770, -0.16553360612350931, -0.00016095810460478};
PolynomialInterpolator optimal32xp4o4(optimal32xp4o4c, 5, 2, optimal32xname);

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (6-point, 4th-order)
floating optimal32xp6o4c[] =
{-2.44391738331193720, -0.69468212315980082, 9.67889243081689440, -5.50592307590218160, 0.03957507923965987,
-11.87524595267807600, 25.58633277328986500, -15.73068663442630400, 3.15288929279855570, -0.05936083498715066,
16.79403235763479100, -17.17264148794549100, 6.05175140696421730, -0.79053754554850286, 0.01978575568000696};
PolynomialInterpolator optimal32xp6o4(optimal32xp6o4c, 5, 3, optimal32xname);

//32x-oversampled, pinked, pre-emphasized, Optimal 32x (6-point, 5th-order)
floating optimal32xp6o5c[] =
{0.52558916128536759, 0.00010896283126635, -0.42682321682847008, -0.04095676092513167, 0.25041444762720882, -0.08349799235675044,
0.33937904183610190, 0.80946953063234006, -1.86228986389877100, 1.27215033630638800, -0.37562266426589430, 0.04174912841630993,
2.13606003964474490, -3.48774662195185850, 2.28912105276248390, -0.75510203509083995, 0.12520821766375972, -0.00834987866042734};
PolynomialInterpolator optimal32xp6o5(optimal32xp6o5c, 6, 3, optimal32xname);

//Test!
const char *testipname = "Test";
floating testipc[] =
{1, -1.0/2.0, 3.0/2.0, -5.0/4.0};
PolynomialInterpolator testip(testipc, 4, 1, testipname);

PolynomialInterpolator optiip;

class OptiIpCost: public CostEvaluator {

	floating evaluateCost(floating *c) {
		floating totalerror = 0;
		floating realresp, imagresp;	
		int section;
		floating f;

		// Normalize
		realresp = 0; imagresp = 0;
		for (section = 0; (section < optiipsections); section++) {
			polyeval.fourierTransform(&c[section*optiipterms], optiip.exp, optiipterms, section, section+1, 0, realresp, imagresp);
		}
		floating dcresp = realresp * 2.0;
		for (int coef = 0; (coef < optiipsections*optiipterms); coef++) {
			c[coef] /= dcresp;
		}

		floating maxmagresp = 0;

		//find maximum stopband ripple by sampling
		for (f = -1.0; (f <= 1.0); f += 2.0/optiipsubsamples) {
			realresp = 0, imagresp = 0;
			for (int section = 0; (section < optiipsections); section++) {
				polyeval.fourierTransform(&c[section*optiipterms], optiip.exp, optiipterms, section, section+1, pi/optiipoversampling*f, realresp, imagresp);
			}
			floating magresp = fabs(realresp * 2);
			floating deemphasis = 1.0/magresp;
			floating pink;
			if (fabs(f) <= (5.0/22050)) pink = 1/sqrt(5.0/22050);
			else pink = 1/sqrt(fabs(f));
			for (int stopband = 1; (stopband < (optiipstopbands+1)); stopband++) {
				if ((optiipoversampling > 1) || (stopband != 1) || (f >= (optiiptransition-1))) {
					realresp = 0; imagresp = 0;
					for (int section = 0; (section < optiipsections); section++) {
						polyeval.fourierTransform(&c[section*optiipterms], optiip.exp, optiipterms, section, section+1, 2*pi*stopband+pi/optiipoversampling*f, realresp, imagresp);
					}
					floating magresp = fabs(realresp * 2) * pink;// * deemphasis;
					if (maxmagresp < magresp) maxmagresp = magresp;
				}
			}
		}

		totalerror += maxmagresp; // some error for stopband ripple
		return totalerror; 
	}
};

class PassBandPolyCost: public CostEvaluator {
private:
	floating *passbandresp;
	const int *polyexp;
	int	terms, oversampling, subsamples;

public:
	floating evaluateCost(const floating *c) {
		int k = 0;
		floating f, dberror, maxdberror = 0;
		for (f = 0; (f <= 1); f += 1.0/subsamples) {
			if (f*pi >= 0.05*oversampling) {
				dberror = fabs(20*log10((fabs(1+polyeval.y(c, polyexp, terms, f*pi))/passbandresp[k++])));
				if (maxdberror < dberror) maxdberror = dberror;
			} else k++;
		}
		return maxdberror; 
	}

	PassBandPolyCost(const PolynomialInterpolator *ip, const int *polyexp, int terms, int oversampling, int subsamples) {
		passbandresp = new floating[subsamples*4];
		int k = 0;
		for (floating f = 0; (f < 1.5); f += 1.0/subsamples) {
			passbandresp[k++] = ip->evaluateFrequencyResponse(f*pi/oversampling)/
				ip->evaluateFrequencyResponse(0);
		}
		passbandresp[0] = 1;
		this->polyexp = polyexp;
		this->terms = terms;
		this->oversampling = oversampling;
		this->subsamples = subsamples;
	}

	~PassBandPolyCost() {
		delete[] passbandresp;
	}
};

struct FIRFilter {
	virtual floating evaluateFrequencyResponse(floating f) = 0;
};

struct ZeroFIRFilter: public FIRFilter{
	const floating *z;
	int zeros;

	void evaluateFrequencyResponse(floating f, floating &realresp, floating &imagresp) {
		floating freqreal = cos(f);
		floating freqimag = sin(f);
		for (int zero = 0; (zero < zeros); zero++) {
			floating zeroreal = cos(z[zero*2+0])*z[zero*2+1];
			floating zeroimag = sin(z[zero*2+0])*z[zero*2+1];
			floating zerorespreal = freqreal-zeroreal;
			floating zerorespimag = freqimag-zeroimag;
			floating newrealresp = realresp*zerorespreal - imagresp*zerorespimag;
			imagresp             = realresp*zerorespimag + zerorespreal*imagresp;
			realresp = newrealresp;

			// Complex conjugate
			zeroimag = -zeroimag;
			zerorespimag = freqimag-zeroimag;
			newrealresp = realresp*zerorespreal - imagresp*zerorespimag;
			imagresp             = realresp*zerorespimag + zerorespreal*imagresp;
			realresp = newrealresp;
		}

	}

	floating evaluateFrequencyResponse(floating f) {
		floating realresp = 1, imagresp = 0;
		evaluateFrequencyResponse(0, realresp, imagresp);
		floating dcresp = hypot(realresp, imagresp);
		floating normalization = 1.0/dcresp;
		evaluateFrequencyResponse(f, realresp = normalization, imagresp = 0);
		return hypot(realresp, imagresp);
	}

	void printZeros() {
		printf("\n");
		for (int zero = 0; (zero < zeros); zero++) {
			printf("(%.13f, %.13f)", z[zero*2+0], z[zero*2+1]);
			if (zero < (zeros-1)) printf(", ");
		}
	}

	ZeroFIRFilter(const floating *z, int zeros) {
		this->z = z;
		this->zeros = zeros;
	}

	ZeroFIRFilter() {
	}
};

int optifirzeros = 10;
int optifirsubsamples = 256;
const floating passbandend = 0.113;
const floating stopbandstart = 0.137;

class LowpassZeroFIR: public CostEvaluator {
	floating evaluateCost(floating *c) {
		ZeroFIRFilter fir(c, optifirzeros);


		for (int zero = 0; (zero < optifirzeros); zero++) {
			if (c[zero*2+0] < -pi) c[zero*2+0] = -pi;
			if (c[zero*2+0] >  pi) c[zero*2+0] =  pi;
			if (c[zero*2+1] < 0) {
				c[zero*2+0] = fmod(c[zero*2+0]+2*pi, 2*pi)-pi;
				c[zero*2+1] = -c[zero*2+1];
			}
		}

		floating maxpassbandripple = 0;
		floating maxstopbandripple = 0; 
		for (floating f = 0; (f <= 1); f += 1.0/optifirsubsamples) {
			if (f <= passbandend) {
				// Passband
				floating resp = fir.evaluateFrequencyResponse(f*pi);
				floating ipresp = 1 - 1.9294750817478504562*f*f;
				resp = resp*ipresp;
				if (resp < 1) resp = 1.0/resp;
				if (resp > maxpassbandripple) maxpassbandripple = resp;
			} else if (f >= stopbandstart) {
				// Stopband
				floating resp = fir.evaluateFrequencyResponse(f*pi);
				if (resp > maxstopbandripple) maxstopbandripple = resp;
			}	
		}		
		floating totalcost = (maxpassbandripple-1);
		totalcost = (totalcost > maxstopbandripple)? totalcost : maxstopbandripple;
		return totalcost; 
	}
};

class FrequencyResponsePlot {
private:
	static const float leftmargin;
	static const float rightmargin;
	static const float bottommargin;
	static const float topmargin;
	float xpi, y12db;
	int xsections, ysectionsup, ysectionsdown;
	int items;
public:
	float lastmodsnr;
	EPSPlot *eps;

	void plot(PolynomialInterpolator *ip) {
		const float maxerror = 0.03125f;
		const int subsections = 512; // 512 is official
		const int iterations = 15;

		int findmax = ip->preemphasis && ip->pinked && (ip->oversampling > 1);
		float *xplot = new float[subsections*xsections+1];
		float *yplot = new float[subsections*xsections+1];

		PolynomialInterpolatorFrequencyResponse freqresp(ip);

		floating maxw, maxa;
		
		if (findmax) {
			maxw = 2*pi;
			maxa = freqresp.evaluate(maxw);
		}

		for (int t = 0; (t < (subsections*xsections+1)); t++) {
			xplot[t] = (float)(xpi*(floating)t/subsections);
			floating w = pi*((floating)t/subsections);
			floating a = freqresp.evaluate(w);
			if (findmax && (w > pi) && (a > maxa)) {
				maxw = w;
				maxa = a; 
				//a = freqresp.evaluate(w);
				a = ExtremeFinder::localMax(freqresp, w-pi/subsections/2.0f, w+pi/subsections/2.0f, iterations);
			}
			/*{
				floating amin = ExtremeFinder::localMin(freqresp, w-pi/subsections/2.0f, w+pi/subsections/2.0f, iterations);
				if (amin < pow(10, -12*(ysectionsdown+1)/20.0)) a = amin;
			}// */
			floating db;			
			if (a == 0) db = -666; else db = (floating)(20.0/12.0*log10(a));
			yplot[t] = (float)(y12db*db);
		}
		
		eps->drawLines(xplot, yplot, subsections*xsections+1, maxerror, 0, -y12db*ysectionsdown, xsections*xpi, y12db*ysectionsup);

		delete xplot;
		delete yplot;

		char buffer[200];

		if (findmax) {
			maxa = ExtremeFinder::localMax(freqresp, maxw-pi/subsections, maxw+pi/subsections, iterations);
			floating db;
			if (maxa == 0) db = -666; else db = (floating)(20.0*log10(maxa));
			lastmodsnr = -db;
			sprintf(buffer, "%.1f dB", (float)db);
			eps->drawText((float)(xpi*maxw/pi), (float)(y12db*db/12.0), buffer, 0.5);
		}

		ip->getFullName(buffer);
		eps->drawLegend(xpi*xsections-3.0f, (float)y12db/2-items++*y12db+(y12db*(ysectionsup-1)), buffer);
	}

	void plot(FIRFilter *fir) {
		plot(fir, 0, 0);
	}

	void plot(FIRFilter *fir, floating stopbandstart, floating stopbandend) {
		const float maxerror = 0.03125f;
		const int subsections = 512; // 512 is official
		const int iterations = 15;

		int findmax = stopbandend-stopbandstart;
		float *xplot = new float[subsections*xsections+1];
		float *yplot = new float[subsections*xsections+1];

		floating maxw, maxa;
		
		if (findmax) {
			maxw = stopbandend;
			maxa = fir->evaluateFrequencyResponse(maxw);
		}

		for (int t = 0; (t < (subsections*xsections+1)); t++) {
			xplot[t] = (float)(xpi*(floating)t/subsections);
			floating w = pi*((floating)t/subsections);
			floating a = fir->evaluateFrequencyResponse(w);
			if (findmax && (w > pi) && (a > maxa)) {
				maxw = w;
				maxa = a; 
				a = fir->evaluateFrequencyResponse(w);
			}
			floating db;			
			if (a == 0) db = -666; else db = (floating)(20.0/12.0*log10(a));
			yplot[t] = (float)(y12db*db);
		}
		
		eps->drawLines(xplot, yplot, subsections*xsections+1, maxerror, 0, -y12db*ysectionsdown, xsections*xpi, y12db*ysectionsup);

		delete xplot;
		delete yplot;

		char *buffer = "FIR";

		if (findmax) {
			floating db;
			if (maxa == 0) db = -666; else db = (floating)(20.0*log10(maxa));
			sprintf(buffer, "%.1f dB", (float)db);
			eps->drawText((float)(xpi*maxw/pi), (float)(y12db*db/12.0), buffer, 0.5);
		}

		eps->drawLegend(xpi*xsections-3.0f, (float)y12db/2-items++*y12db+(y12db*(ysectionsup-1)), buffer);
	}

	FrequencyResponsePlot(const char *filename, float xpi, float y12db, int xsections, int ysectionsup, int ysectionsdown) {
		eps = new EPSPlot(filename, -leftmargin, -y12db*ysectionsdown-bottommargin, xsections*xpi+rightmargin, y12db*ysectionsup+topmargin);
		this->xpi = xpi;
		this->y12db = y12db;
		this->xsections = xsections;
		this->ysectionsup = ysectionsup;
		this->ysectionsdown = ysectionsdown;
		eps->setRGBColor(0.75f, 0, 0);
		eps->setLineCap(0);
		eps->setLineJoin(0);
		eps->setNoDash();
		eps->setLineWidth(0.5);
		eps->drawBox(0, -y12db*ysectionsdown, xpi*xsections, y12db*ysectionsup);		
		eps->setLineWidth(0.25);
		eps->drawLine(0, 0, xpi*xsections, 0);
		eps->setDash(0.25, 2.25);
		int t;
		for (t = -ysectionsdown+1; (t < ysectionsup); t++) {
			if (t != 0) eps->drawLine(0, (float)t*y12db, (float)xpi*xsections, (float)t*y12db);
		}
		for (t = 1; (t < xsections); t++) {
			eps->drawLine((float)t*xpi, -y12db*ysectionsdown, (float)t*xpi, y12db*ysectionsup);
		}
		for (t = -ysectionsdown; (t <= ysectionsup); t++) {
			char buffer[200];
			sprintf(buffer, "%d", t*12);
			eps->drawText(-1, (float)t*y12db, buffer, 1, 0.5);
		}
		for (t = 0; (t <= xsections); t++) {
			char buffer[200];
			sprintf(buffer, "%d", t);
			eps->drawText((float)t*xpi, -y12db*ysectionsdown-1.5, buffer, 0.5, 1);
		}
		eps->drawText(-leftmargin+7, y12db*(ysectionsup-ysectionsdown)/2.0f, "Magnitude (dB)", 0.5, 0, 90);
		eps->drawText(xpi*xsections/2.0f, -y12db*ysectionsdown-bottommargin, "Angular frequency (pi)", 0.5, 0);
		eps->setLineCap(0);
		eps->setLineJoin(2);
		eps->setLineWidth(0.25);
		eps->setNoDash();
		eps->setGray(0);
		items = 0;
	}
	~FrequencyResponsePlot() {
		delete eps;
	}
};

class FreqRespEvaluator {
public:
	virtual std::complex<floating> evaluateFreqResp(floating) = 0;
};

class PhaseRespPlot {
	float xsteppoints, ysteppoints, xstepfreq, ystepphase;
	int xstepsleft, xstepsright, ystepsdown, ystepsup;
public:
	EPSPlot *eps;

	void plot(FreqRespEvaluator *evaluator) {
		const substeps = 100;
		float *xplot = new float[substeps*(xstepsleft+xstepsright)+1];
		float *yplot = new float[substeps*(xstepsleft+xstepsright)+1];
		int vertex = 0;
		for (int substep = -xstepsleft*substeps; (substep <= xstepsright*substeps); substep++) {
			xplot[vertex] = substep*xsteppoints/substeps;
			yplot[vertex] = ysteppoints*std::arg(evaluator->evaluateFreqResp(substep*xstepfreq/substeps))/ystepphase;
			vertex++;
		}
		eps->drawLines(xplot, yplot, substeps*(xstepsleft+xstepsright)+1, 0.05f, -xsteppoints*xstepsleft, -ysteppoints*ystepsdown, xsteppoints*xstepsright, ysteppoints*ystepsup);
		delete[] xplot;
		delete[] yplot;
	}

	PhaseRespPlot(const char *filename, float xsteppoints, float ysteppoints, float xstepfreq, float ystepphase, int xstepsleft, int xstepsright, int ystepsdown, int ystepsup) {
		const int leftmargin = 20;
		const int rightmargin = 20;
		const int bottommargin = 20;
		const int topmargin = 20;

		this->xsteppoints = xsteppoints;
		this->ysteppoints = ysteppoints;
		this->xstepfreq = xstepfreq;
		this->ystepphase = ystepphase;
		this->xstepsleft = xstepsleft;
		this->xstepsright = xstepsright;
		this->ystepsdown = ystepsdown;
		this->ystepsup = ystepsup;
		eps = new EPSPlot(filename, -xsteppoints*xstepsleft-leftmargin, -ysteppoints*ystepsdown-bottommargin, xsteppoints*xstepsright+rightmargin, ysteppoints*ystepsup+topmargin);
		eps->setRGBColor(0.75f, 0, 0);
		eps->setLineCap(0);
		eps->setLineJoin(0);
		eps->setNoDash();
		eps->setLineWidth(0.5);
		eps->drawBox(-xsteppoints*xstepsleft, -ysteppoints*ystepsdown, xsteppoints*xstepsright, ysteppoints*ystepsup);		
		eps->setLineWidth(0.25);
		eps->drawLine(0, -ysteppoints*ystepsdown, 0, ysteppoints*ystepsup);
		eps->drawLine(-xsteppoints*xstepsleft, 0, xsteppoints*xstepsright, 0);
		eps->setDash(0.25, 2.25);
		char buffer[200];
		for (int x = -xstepsleft; (x <= xstepsright); x++) {
			if (x) eps->drawLine(xsteppoints*x, -ysteppoints*ystepsdown, xsteppoints*x, ysteppoints*ystepsup);
			sprintf(buffer, "%1.3f", x*xstepfreq);
			eps->drawText(xsteppoints*x, -ysteppoints*ystepsdown-1, buffer, (x == xstepsright)? 1 : 0.5, 1);
		}
		for (int y = -ystepsdown; (y <= ystepsup); y++) {
			if (y) eps->drawLine(-xsteppoints*xstepsleft, ysteppoints*y, xsteppoints*xstepsright, ysteppoints*y);
			sprintf(buffer, "%1.3f", y*ystepphase);
			eps->drawText(-xsteppoints*xstepsleft-1, ysteppoints*y, buffer, 1, (y == ystepsup)? 1 : ((y == -ystepsdown)? 0 : 0.5));
		}
		eps->setLineJoin(2);
		eps->setNoDash();
		eps->setGray(0);
	}

	~PhaseRespPlot() {
		delete eps;
	}
};

class IIRHilbertPair: public CostEvaluator, public FreqRespEvaluator {
	floating bandbottom;
	floating bandwidth;
	int allpasses1, allpasses2;
	floating *c;
	int delay;

	void constraint(floating *c) {
		for (int allpass = 0; (allpass < (allpasses1+allpasses2)); allpass++) {
			if ((allpass != (allpasses1-1)) && (allpass != (allpasses1+allpasses2-1))) {
				if (c[allpass*2+0] > c[(allpass+1)*2+0]) {
					floating temp0 = c[allpass*2+0];
					floating temp1 = c[allpass*2+1];
					c[allpass*2+0] = c[(allpass+1)*2+0];
					c[allpass*2+1] = c[(allpass+1)*2+1];
					c[(allpass+1)*2+0] = temp0;
					c[(allpass+1)*2+1] = temp1;
				}
			}
			if (c[allpass*2+1] < 0.001) c[allpass*2+1] = 0.001-c[allpass*2+1];
			if (c[allpass*2+1] >= 1) c[allpass*2+1] = 2-c[allpass*2+1];
			if (c[allpass*2+0] < 0) c[allpass*2+0] = -c[allpass*2+0];
			if (c[allpass*2+0] > pi) c[allpass*2+0] = 2*pi-c[allpass*2+0];
		}
	}

	std::complex<floating> evaluateSingleAllpassResp(const floating *c, std::complex<floating> z) {
		std::complex<floating> pole1 = std::polar(c[0], c[1]);
		std::complex<floating> pole2 = std::conj(pole1);
		std::complex<floating> zero1 = std::complex<floating>(1)/pole1;
		std::complex<floating> zero2 = std::conj(zero1);
		return (z-zero1)*(z-zero2)/((z-pole1)*(z-pole2));
	}

	std::complex<floating> evaluateCascadedAllpassesResp(const floating *c, int allpasses, floating w, int delay)
	{
		std::complex<floating> z = std::complex<floating>(cos(w), sin(w));
		std::complex<floating> totalresp(std::pow(z, delay));
		for (int allpass = 0; (allpass < allpasses); allpass++) {
			totalresp *= evaluateSingleAllpassResp(c+allpass*2, z);
		}
		return totalresp;
	}

	void print() {
		int allpass;
		printf("\n0 degrees allpass z-plane polar coordinates:");
		for (allpass = 0; (allpass < allpasses1); allpass++) {
			printf("\nPolar(%1.13f, %1.13f) Rect(%1.13f, %1.13f)", c[allpass*2+0], c[allpass*2+1], cos(c[allpass*2+0])*c[allpass*2+1], sin(c[allpass*2+0])*c[allpass*2+1]);
		}
		printf("\n+90 degrees allpass z-plane polar coordinates (angle, radius):");
		for (allpass = allpasses1; (allpass < (allpasses1+allpasses2)); allpass++) {
			printf("\nPolar(%1.13f, %1.13f) Rect(%1.13f, %1.13f)", c[allpass*2+0], c[allpass*2+1], cos(c[allpass*2+0])*c[allpass*2+1], sin(c[allpass*2+0])*c[allpass*2+1]);
		}
	}

public:
	std::complex<floating> evaluateFreqResp(floating w) {
		return evaluateCascadedAllpassesResp(c+(2*allpasses1), allpasses2, w, delay)
			/ evaluateCascadedAllpassesResp(c, allpasses1, w, 0);
	}

	// Returns the maximum angle difference
	floating evaluateCost(floating *c) {
		this->c = c;

		constraint(c);

		const int subsections = 128;
		floating maxcost = 0;
		for (int subsection = 0; (subsection <= subsections); subsection++) {
//			floating w = bandbottom + bandwidth*subsection/subsections;
//			floating cost = fabs(std::arg(evaluateFreqResp(w))-pi/2);
//			if (cost > pi/2) return 10 + (subsections-subsection); // Early out
//			if (cost > maxcost) maxcost = cost;
			floating w = bandbottom + bandwidth/2 + (bandwidth/2)*subsection/subsections;
			floating cost = fabs(std::arg(evaluateFreqResp(w))-pi/2);
			if (cost > 0.9) return 10 + (subsections-subsection); // Early out
			if (cost > maxcost) maxcost = cost;
			w = bandbottom + bandwidth/2 - (bandwidth/2)*subsection/subsections;
			cost = fabs(std::arg(evaluateFreqResp(w))-pi/2);
			if (cost > 0.9) return 10 + (subsections-subsection); // Early out
			if (cost > maxcost) maxcost = cost;
		}
		return maxcost;
	}

	// Generate IIR allpass pair with 90 degree phase difference
	//
	// bandbottom = lower frequency limit of band (angular freq) 
	// bandtop    = lower frequency limit of band (angular freq)
	// allpasses1 = first filter (0deg) 1-pole-1-zero allpass pair count
	// allpasses2 = second filter (+90deg) 1-pole-1-zero allpass pair count
	// delay      = delay for second filter
	void generate(floating bandbottom, floating bandtop, int allpasses1, int allpasses2, int delay) {
		this->bandbottom = bandbottom;
		this->bandwidth = bandtop-bandbottom;
		this->allpasses1 = allpasses1;
		this->allpasses2 = allpasses2;
		this->delay = delay;
		
		floating *min = new floating[(allpasses1+allpasses2)*2];
		floating *max = new floating[(allpasses1+allpasses2)*2];
		for (int allpass = 0; (allpass < (allpasses1+allpasses2)); allpass++) {
			min[allpass*2+0] = 0; // Angle
			max[allpass*2+0] = pi;
			min[allpass*2+1] = 0; // Radius
			max[allpass*2+1] = 1;
		}
		DEEvolution evolution(60, (allpasses1+allpasses2)*2, min, max);
		double previouscost = 0;
		int birthtime = 0;
		for(int t = 0;; t++) {		
			floating cost = evolution.evolve(this, 0, 0.7, 1, 1);
			if (cost != previouscost) {
				birthtime = t;
				previouscost = cost;
			};
			printf("\nGeneration:%6d  Birth:%6d  Cost: %5.22f", t, birthtime, cost);
			if (kbhit()) {
				getch();
				this->c = evolution.getBest();
				PhaseRespPlot phaseplot("d:/deip/phase.eps", 20, 15, pi/20, pi/2/10, 0, 20, 20, 20);
				phaseplot.plot(this);
				print();
				printf("\nPress ESC to exit, R to restart, any other key to continue evolution\n");
				char key = getch();
				if (key == 27) break;
				if ((key == 82) || (key == 114)) {
					evolution.randomPopulation(&min[0], &max[0]);
					t = -1;
				}
			}
		}
		delete[] min;
		delete[] max;
	}
};

const float FrequencyResponsePlot::leftmargin = 21;
const float FrequencyResponsePlot::rightmargin = 6;
const float FrequencyResponsePlot::bottommargin = 12;
const float FrequencyResponsePlot::topmargin = 3;

void main() {
		MDEEvolution::randomize();
		IIRHilbertPair hilbert;

//> > Anybody know where to find a cookbook or code for an IIR Hilbert
//> > transform (-pair) for telephonic bandwidth (300-3400 Hz)?
//> 
//> What's your sampling frequency? How accurate need the phase difference be?
//> I'm playing with genetic algo -designed filters and that would be
//> interesting to try to design.
//
//Either 8K or 16K (depends what part of the system), +- 1 degree.  Need
//it for single-sideband modulation.  

		const floating bottomhz   = 50;
		const floating tophz      = 3400;
		const floating samplerate = 8000;
		hilbert.generate(2*pi*bottomhz/samplerate, 2*pi*tophz/samplerate, 3, 6, 2);

		//Good ones for 300, 3400, 16000:
		//7, 8, -6 (zero to zero)
		//6, 7, -2 (zero to zero)
		//5, 6,  0 (zero to -2pi) 

	/*{
		PolynomialInterpolator *ip = &optimal2xp2o3;
		const int oversample = 1;
		const int polyterms = 5;

		const int subsamples = 128;
		const int polyexp[] = {2, 4, 6, 8, 10, 12, 14, 16};
		const floating min[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		const floating max[] = {+1, +1, +1, +1, +1, +1, +1, +1, +1, +1, +1};
		PassBandPolyCost passbandpolycost(ip, polyexp, polyterms, oversample, subsamples);
		DEvolution::randomize();
		DEvolution evolution(80, polyterms, min, max);
		double previouscost = 0;
		int birthtime = 0;
		for(int t = 0;; t++) {		
			floating cost = evolution.evolve(&passbandpolycost, 0, 0.5, 1.0, 1);
			if (cost != previouscost) {
				birthtime = t;
				previouscost = cost;
			};
			printf("\nGeneration:%6d  Birth:%6d  Cost(dB): %5.22f", t, birthtime, cost);
			if (kbhit()) {
				getch();
				char buffer[100];
				ip->getFullName(buffer);
				printf("\nInterpolator: %s, Oversampling: %d.", buffer, oversample);
				printf("\n1 + ");
				for(int t = 0; (t < polyterms); t ++) {
					printf("x^%d*%.17f", polyexp[t], evolution.getBest()[t]);
					if (t < polyterms-1) printf(" + ");
				}
				printf("\nPress ESC to exit, R to restart, any other key to continue evolution\n");
				char key = getch();
				if (key == 27) break;
				if ((key == 82) || (key == 114)) {
					evolution.randomPopulation(&min[0], &max[0]);
					t = -1;
				}
			}
		}
	}// */

	/*{
		// Test polynomial evaluator
		const floating c[] = {2, 3, 1};
		const int e[]      = {1, 2, 0};
		printf("polynomial f(x) test (should return 17) returns: %5.5f\n", polyeval.y(c, e, 3, 2.0));
		printf("polynomial f'(x) test (should return 14) returns: %5.5f\n", polyeval.dn(c, e, 3, 2.0, 1));
		printf("polynomial f''(x) test (should return 6) returns: %5.5f\n", polyeval.dn(c, e, 3, 2.0, 2));
		printf("polynomial integrator test (should return 500) returns: %5.5f\n", polyeval.integrate(c, e, 3, 4.0, 8.0));
		getch();
	}// */

	/*{
		PolynomialInterpolatorRoutinePrint::zForm(&optimal32xp6o5);
	}// */
	
	/*{
		{
			PolynomialInterpolator *irip = &lagrangep6o5;
			const int differential = 4;

			PolynomialInterpolatorImpulseResponsePlot irplot("d:/deip/ir.eps", (20*irip->pxsections > 20)? 20*irip->pxsections : 40, 40, 20, 80 >> differential);
			//yunity = 40 for 2x oversampling, others 40 or 20 >> diff.

			irplot.plot(irip, differential);
		}
	}// */
/*	{
		PolynomialInterpolator *plotip1 = &optimal32xp6o4;
		PolynomialInterpolator *plotip2 = &optimal4xp6o5;
		PolynomialInterpolator *plotip3 = &optimal8xp6o5;
		PolynomialInterpolator *plotip4 = &optimal16xp6o5;
		PolynomialInterpolator *plotip5 = &optimal32xp6o5;

		{
			PolynomialInterpolatorFrequencyResponsePlot freqrespplot("d:/deip/fr.eps", 25.0f, 12.0f, 16, 0, 9);
			//plotip1->modify(1);
			//freqrespplot.eps->setDash(1, 1);
			freqrespplot.plot(plotip1);
			freqrespplot.eps->setNoDash();
			plotip1->modify(2);
			freqrespplot.plot(plotip1);
			freqrespplot.eps->setRGBColor(0, 0, 0.75);
			freqrespplot.eps->setDash(6, 0.5);
			plotip2->modify(4);
			freqrespplot.plot(plotip2);
			freqrespplot.eps->setRGBColor(0, 0.75, 0);
			freqrespplot.eps->setDash(4, 0.5);
			plotip3->modify(8);
			freqrespplot.plot(plotip3);
			freqrespplot.eps->setRGBColor(0.75, 0.75, 0);
			freqrespplot.eps->setDash(2, 0.5);
			plotip4->modify(16);
			freqrespplot.plot(plotip4);
			freqrespplot.eps->setRGBColor(0.75, 0, 0.75);
			freqrespplot.eps->setDash(1, 0.5);
			plotip5->modify(32);
			freqrespplot.plot(plotip5);
		}

	}// */

	/*{
		optiip.terms = optiipterms;
		optiip.pxsections = optiipsections;
		optiip.name = optiipname;
		OptiIpCost costfunc;
		const floating min[] = {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10};
		const floating max[] = {+10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10, +10};
		DEEvolution::randomize();
		DEEvolution evolution(200, optiipterms*optiipsections, min, max);
		double previouscost = 0;
		int costage = -1;
		for(int t = 0;; t++) {		
			floating cost = evolution.evolve(&costfunc, 0, 0.7, 1, 1);
			if (cost != previouscost) {
				costage = 0;
				previouscost = cost;
			} else costage++;
			printf("\nGeneration:%6d  Age:%6d  Cost(dB): %5.22f", t, costage, 20*log10(cost));
			if (kbhit()) {
				getch();
				optiip.c = evolution.getBest();
				{
					optiip.modify(optiipoversampling);
					FrequencyResponsePlot freqrespplot("d:/deip/fr.eps", 30.0f, 7.5f, 13, 1, 30);
					freqrespplot.plot(&optiip);
					printf("\nModified SNR: %.1f", freqrespplot.lastmodsnr);

					optiip.unModify();
					FrequencyResponsePlot freqrespplot2("d:/deip/fr2.eps", 30.0f, 7.5f, 13, 1, 30);
					freqrespplot2.plot(&optiip);
				}
				optiip.modify(optiipoversampling);
				char buffer[200];
				optiip.getFullName(buffer);
				printf("\n%s",buffer);
				optiip.printFunction();
				printf("\nPress ESC to exit, R to restart, any other key to continue evolution\n");
				char key = getch();
				if (key == 27) break;
				if ((key == 82) || (key == 114)) {
					evolution.randomPopulation(&min[0], &max[0]);
					t = -1;
				}
			}
		}
	}// */

	/*{	
		LowpassZeroFIR costfunc;
		floating *min = new floating[optifirzeros*2];
		floating *max = new floating[optifirzeros*2];
		for (int zero = 0; (zero < optifirzeros); zero++) {
			min[zero*2+0] = -pi;
			max[zero*2+0] = pi;
			min[zero*2+1] = 0;
			max[zero*2+1] = 1;
		}
		MDEEvolution::randomize();	
		MDEEvolution evolution(200, optifirzeros*2, min, max);
		double previouscost = 0;
		int costage = -1;
		for(int t = 0;; t++) {		
			floating cost = evolution.evolve(&costfunc, 0.7, 1);
			if (cost != previouscost) {
				costage = 0;
				previouscost = cost;
			} else costage++;
			printf("\nGeneration:%6d  Age:%6d  Cost(dB): %5.22f", t, costage, cost);
			if (kbhit()) {
				getch();
				ZeroFIRFilter bestfir(evolution.getBest(), optifirzeros);
				FrequencyResponsePlot freqrespplot("d:/deip/fr.eps", 300.0f, 10.0f, 2, 1, 40);
				freqrespplot.plot(&bestfir);
				bestfir.printZeros();
				printf("\nPress ESC to exit, R to restart, any other key to continue evolution\n");
				char key = getch();
				if (key == 27) break;
				if ((key == 82) || (key == 114)) {
					evolution.randomPopulation(&min[0], &max[0]);
					t = -1;
				}
			}
		}
		delete[] min;
		delete[] max;
	}// */
}