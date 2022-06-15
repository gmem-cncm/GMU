#ifndef DE_H
#define DE_H

// Put here the floating point type of your choice, double preferred
typedef double floating;

// Differential Evolution and optimization routines v2001.07.04 by Olli Niemitalo
// ------------------------------------------------------------------------------
//
// Differential Evolution algoritm by Kenneth Price and Rainer Storn

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

class CostEvaluator {
public:
	// Return function value for an argument vector
	virtual floating evaluateCost(floating *) = 0;
};

// Differential Evolution class
// ----------------------------
//
// Tries to search for the global minimum of a cost function which gets a 
// parameter vector as an argument
class DEEvolution {
private:
	floating *costs;             // Costs of parameter vectors in population
	int np;                      // Number of population members
	floating *population;        // Parameter vectors in population, one-by-one
	floating *nextgenpopulation; // Next generation that is being produced
	floating *best;        // Pointer to best parameter vector in population
	floating bestcost;     // Cost of the above (invalid if best == NULL)

public:

	int d;                 // Number of parameters

	// Get latest best parameter vector in population
	floating *getBest() {
		return best;
	}

	// Find best parameter vector in population
	//
	// z  = Pointer to cost function being minimized (argument: parameter vector) 
	//
	// Returns: Cost of best. Updates member variables best, bestcost and costs
	floating findBest(CostEvaluator *z) {
		best = &population[0];
		bestcost = z->evaluateCost(best);
		for (int member = 1; (member < np); member++) {
			costs[member] = z->evaluateCost(&population[member*d]);
			if (costs[member] < bestcost) {
				bestcost = costs[member];
				best = &population[member*d];
			}
		}
		return bestcost;
	}

	// Random functions required by the Differential Evolution class
	// -------------------------------------------------------------

	// Randomize by system clock. Please call me!
	static void randomize() {
		srand((unsigned)time(0));
	}

	// Uniform floating-point random
	//
	// Returns: 0..1
	static floating floatingRandom() {
		return rand()/(RAND_MAX+1.0f);
	}

	// Uniform integer random
	//
	// Returns: 0..maxplus1-1
	static int intRandom(int maxplus1) {
		return rand()%maxplus1; //Hope RAND_MAX is not too small!
	}

	// Non-constant member functions
	// -----------------------------

	// Fill parameters in all population members with random values (restarts evolution)
	//
	// minx[] = parameter vector with all parameters set to minimum possible values
	// maxx[] = parameter vector with all parameters set to maximum possible values
	void randomPopulation(const floating *minx, const floating *maxx) {
		for (int member = 0; (member < np); member++) {
			for (int param = 0; (param < d); param++) {
				population[member*d+param] = floatingRandom()*(maxx[param]-minx[param])+minx[param];
			}
		}
		best = NULL;
	}

	// Evolve into next generation - try evolve(&z, 0, 0.7, 1.0, 1);
	//
	// z            = Pointer to cost function to minimize (argument: parameter vector) 
	// gainbest     = Coefficient for best population member (try 1.2)
	// gainr3       = Coefficient for 3rd random population member (try 1)
	// (Note: Coefficient for original population member = 1 - sum of the above)
	// gaindiffr1r2 = Coefficient for difference of 1st and 2nd random population member (try 0.5)
	// cr       = Crossing-over amount, 0..1
	//
	// Returns: Cost of best parameter vector in population. 
	// Member variable best becomes a pointer to the best parameter vector in population.

	floating evolve(CostEvaluator *z, floating gainbest, floating gaindiffr1r2, floating gainr3, floating cr) {
		if (!best) findBest(z);
		floating *newbest = best;
		floating gainorig = 1.0-gainbest-gainr3;	
		for (int member = 0; (member < np); member++) {			

			// Choose some parameter vectors randomly
			int r1, r2, r3;
			do { 
				r1 = intRandom(np);
			} while ((r1 == member) || (&population[r1*d] == best));
			do {
				r2 = intRandom(np);			
			} while ((r2 == member) || (&population[r2*d] == best) || (r2 == r1));
			do {
				r3 = intRandom(np);			
			} while ((r3 == member) || (&population[r3*d] == best) || (r3 == r1) || (r3 == r2));

			// Generate trial vector with crossing-over
			int actualparam = intRandom(d);
			int param = 0;
			do { // Take at least one new parameter before crossing-over

				nextgenpopulation[member*d+actualparam] =
					population[member*d+actualparam]*gainorig + best[actualparam]*gainbest +
					(population[r1*d+actualparam]-population[r2*d+actualparam])*gaindiffr1r2 +
					population[r3*d+actualparam]*gainr3;
					
				actualparam = (actualparam+1)%d;
				param++;
			} while ((param < d) && (floatingRandom()<cr)); // Binomial crossing-over
			for (; (param < d); param++) {
				nextgenpopulation[member*d+actualparam] = population[member*d+actualparam];
				actualparam = (actualparam+1)%d;
			}

			// Is new one better?
			floating cost = z->evaluateCost(&nextgenpopulation[member*d]);
			if (cost < costs[member]) {
				// Yes, keep it
				costs[member] = cost;
				if (cost < bestcost) { // Check if new best
					bestcost = cost;
					newbest = &nextgenpopulation[member*d];
				}
			} else {
				// No, use old
				memcpy(&nextgenpopulation[member*d], &population[member*d], sizeof(floating)*d);
			}
		}

		// New population becomes current population
		std::swap(nextgenpopulation, population);

		best = newbest;
		return bestcost;
	}

	// Constructor
	// 
	// np     = Number of population members, 5 or more (30 is nice)
	// d      = Number of parameters in parameter vector, 1 or more
	// minx[] = Parameter vector with all parameters set to minimum possible values
	// maxx[] = Parameter vector with all parameters set to maximum possible values
	DEEvolution(int np, int d, const floating *minx, const floating *maxx) {
		this->np = np;
		this->d = d; 
		population = new floating[np*d]; 
		nextgenpopulation = new floating[np*d]; 
		costs = new floating[np]; 
		randomPopulation(minx, maxx); // Initialize population
	}

	// Destructor
	~DEEvolution() {
		delete[] population;
		delete[] nextgenpopulation;
		delete[] costs;
	}
};

class OneArgumentFunction {
public:
	// Return function value at a point
	virtual floating evaluate(floating) = 0;
};

class ExtremeFinder {
public:
	// Find a local maximum of a function in a range.
	//
	// y          = Single-argument function to analyze (derived from class OneArgumentFunction)
	// x1..x2     = Range to analysis
	// iterations = Number of iterations (approx. number of bits solved)
	//
	// Returns f(x) at maximum
	static floating localMax(OneArgumentFunction &y, floating x1, floating x2, int iterations) {
		floating shift = 0.5*(x2-x1);
		floating xmid = x1+shift;
		floating ymid = y.evaluate(xmid);
		for(int round = 0; (round < iterations); round++) {
			shift *= 0.5;
			floating ytrial = y.evaluate(xmid-shift);
			if (ytrial > ymid) { // Pick left
				xmid = xmid-shift;       
				ymid = ytrial;
			} else { // Only need one minimum
				ytrial = y.evaluate(xmid+shift);
				if (ytrial > ymid) { // Pick right
					xmid = xmid+shift; 
					ymid = ytrial;
				} 
			} // else keep mid
		}
		floating y1 = y.evaluate(x1);
		floating y2 = y.evaluate(x2);
		if (y1 > ymid) return y1; else return (y2 > ymid)? y2 : ymid;
	}

	// Find a local minimum of a function in a range.
	//
	// y          = Single-argument function to analyze (derived from class OneArgumentFunction)
	// x1..x2     = Range to analysis
	// iterations = Number of iterations (approx. number of bits solved)
	//
	// Returns f(x) at minimum
	static floating localMin(OneArgumentFunction &y, floating x1, floating x2, int iterations) {
		floating shift = 0.5*(x2-x1);
		floating xmid = x1+shift;
		floating ymid = y.evaluate(xmid);
		for(int round = 0; (round < iterations); round++) {
			shift *= 0.5;
			floating ytrial = y.evaluate(xmid-shift);
			if (ytrial < ymid) { // Pick left
				xmid = xmid-shift;       
				ymid = ytrial;
			} else { // Only need one minimum
				ytrial = y.evaluate(xmid+shift);
				if (ytrial < ymid) { // Pick right
					xmid = xmid+shift; 
					ymid = ytrial;
				} 
			} // else keep mid
		}
		floating y1 = y.evaluate(x1);
		floating y2 = y.evaluate(x2);
		if (y1 < ymid) return y1; else return (y2 < ymid)? y2 : ymid;
	}

private:
	LocalMaxFinder();
};

// Modified Differential Evolution class
// -------------------------------------
//
// Tries to search for the global minimum of a cost function which gets a 
// parameter vector as an argument
class MDEEvolution {
private:
	floating *costs;             // Costs of parameter vectors in population
	int np;                      // Number of population members
	floating *population;        // Parameter vectors in population, one-by-one
	floating *nextgenpopulation; // Next generation that is being produced
	floating *best;        // Pointer to best parameter vector in population
	floating bestcost;     // Cost of the above (invalid if best == NULL)

public:

	int d;                 // Number of parameters

	// Get latest best parameter vector in population
	floating *getBest() {
		return best;
	}

	// Find best parameter vector in population
	//
	// z  = Pointer to cost function being minimized (argument: parameter vector) 
	//
	// Returns: Cost of best. Updates member variables best, bestcost and costs
	floating findBest(CostEvaluator *z) {
		best = &population[0];
		bestcost = z->evaluateCost(best);
		for (int member = 1; (member < np); member++) {
			costs[member] = z->evaluateCost(&population[member*d]);
			if (costs[member] < bestcost) {
				bestcost = costs[member];
				best = &population[member*d];
			}
		}
		return bestcost;
	}

	// Random functions required by the Differential Evolution class
	// -------------------------------------------------------------

	// Randomize by system clock. Please call me!
	static void randomize() {
		srand((unsigned)time(0));
	}

	// Uniform floating-point random
	//
	// Returns: 0..1
	static floating floatingRandom() {
		return rand()/(RAND_MAX+1.0f);
	}

	// Uniform integer random
	//
	// Returns: 0..maxplus1-1
	static int intRandom(int maxplus1) {
		return rand()%maxplus1; //Hope RAND_MAX is not too small!
	}

	// Non-constant member functions
	// -----------------------------

	// Fill parameters in all population members with random values (restarts evolution)
	//
	// minx[] = parameter vector with all parameters set to minimum possible values
	// maxx[] = parameter vector with all parameters set to maximum possible values
	void randomPopulation(const floating *minx, const floating *maxx) {
		for (int member = 0; (member < np); member++) {
			for (int param = 0; (param < d); param++) {
				population[member*d+param] = floatingRandom()*(maxx[param]-minx[param])+minx[param];
			}
		}
		best = NULL;
	}

	// Evolve into next generation - try evolve(&z, 0, 0.7, 1.0, 1);
	//
	// z            = Pointer to cost function to minimize (argument: parameter vector) 
	// gaindiffr1r2 = Coefficient for difference of 1st and 2nd random population member (try 0.7)
	// gainr3       = Coefficient for 3rd random population member (try 1)
	//
	// Returns: Cost of best parameter vector in population. 
	// Member variable best becomes a pointer to the best parameter vector in population.

	floating evolve(CostEvaluator *z, floating gaindiffr1r2, floating gainr3) {
		if (!best) findBest(z);
		floating *newbest = best;
		for (int member = 0; (member < np); member++) {			

			memcpy(&nextgenpopulation[member*d], &population[member*d], sizeof(floating)*d);

			{
				int param = intRandom(d);
				// Choose some parameter vectors randomly for this parameter
				int r1 = intRandom(np);
				int r2 = intRandom(np);
				int r3 = intRandom(np);
				nextgenpopulation[member*d+param] = 
					(population[r1*d+param]-population[r2*d+param])*gaindiffr1r2 +
					population[r3*d+param]*gainr3;
			}
			{
				int param = intRandom(d);
				// Choose some parameter vectors randomly for this parameter
				int r1 = intRandom(np);
				int r2 = intRandom(np);
				int r3 = intRandom(np);
				nextgenpopulation[member*d+param] += floatingRandom()*0.001-0.0005;
			}
			
			// Is new one better?
			floating cost = z->evaluateCost(&nextgenpopulation[member*d]);
			if (cost < costs[member]) {
				// Yes, keep it
				costs[member] = cost;
				if (cost < bestcost) { // Check if new best
					bestcost = cost;
					newbest = &nextgenpopulation[member*d];
				}
			} else {
				// No, use old
				memcpy(&nextgenpopulation[member*d], &population[member*d], sizeof(floating)*d);
			}
		}

		// New population becomes current population
		std::swap(nextgenpopulation, population);

		best = newbest;
		return bestcost;
	}

	// Constructor
	// 
	// np     = Number of population members, 5 or more (30 is nice)
	// d      = Number of parameters in parameter vector, 1 or more
	// minx[] = Parameter vector with all parameters set to minimum possible values
	// maxx[] = Parameter vector with all parameters set to maximum possible values
	MDEEvolution(int np, int d, const floating *minx, const floating *maxx) {
		this->np = np;
		this->d = d; 
		population = new floating[np*d]; 
		nextgenpopulation = new floating[np*d]; 
		costs = new floating[np]; 
		randomPopulation(minx, maxx); // Initialize population
	}

	// Destructor
	~MDEEvolution() {
		delete[] population;
		delete[] nextgenpopulation;
		delete[] costs;
	}
};

// Unique Mother Differential Evolution class
// ------------------------------------------
//
// Tries to search for the global minimum of a cost function which gets a 
// parameter vector as an argument
class UMDEEvolution {
private:
	floating *costs;             // Costs of parameter vectors in population
	int np;                      // Number of population members
	floating *population;        // Parameter vectors in population, one-by-one
	floating *nextgenpopulation; // Next generation that is being produced
	floating *best;        // Pointer to best parameter vector in population
	floating bestcost;     // Cost of the above (invalid if best == NULL)
	int *r1table;  // Shuffled mother table

public:

	int d;                 // Number of parameters

	// Get latest best parameter vector in population
	floating *getBest() {
		return best;
	}

	// Find best parameter vector in population
	//
	// z  = Pointer to cost function being minimized (argument: parameter vector) 
	//
	// Returns: Cost of best. Updates member variables best, bestcost and costs
	floating findBest(CostEvaluator *z) {
		best = &population[0];
		bestcost = z->evaluateCost(best);
		for (int member = 1; (member < np); member++) {
			costs[member] = z->evaluateCost(&population[member*d]);
			if (costs[member] < bestcost) {
				bestcost = costs[member];
				best = &population[member*d];
			}
		}
		return bestcost;
	}

	// Random functions required by the Differential Evolution class
	// -------------------------------------------------------------

	// Randomize by system clock. Please call me!
	static void randomize() {
		srand((unsigned)time(0));
	}

	// Uniform floating-point random
	//
	// Returns: 0..1
	static floating floatingRandom() {
		return rand()/(RAND_MAX+1.0f);
	}

	// Uniform integer random
	//
	// Returns: 0..maxplus1-1
	static int intRandom(int maxplus1) {
		return rand()%maxplus1; //Hope RAND_MAX is not too small!
	}

	// Non-constant member functions
	// -----------------------------

	// Fill parameters in all population members with random values (restarts evolution)
	//
	// minx[] = parameter vector with all parameters set to minimum possible values
	// maxx[] = parameter vector with all parameters set to maximum possible values
	void randomPopulation(const floating *minx, const floating *maxx) {
		for (int member = 0; (member < np); member++) {
			for (int param = 0; (param < d); param++) {
				population[member*d+param] = floatingRandom()*(maxx[param]-minx[param])+minx[param];
			}
		}
		best = NULL;
	}

	// Evolve into next generation - try evolve(&z, 0, 0.7, 1.0, 1);
	//
	// z            = Pointer to cost function to minimize (argument: parameter vector) 
	// gainbest     = Coefficient for best population member (try 1.2)
	// gainr3       = Coefficient for 3rd random population member (try 1)
	// (Note: Coefficient for original population member = 1 - sum of the above)
	// gaindiffr1r2 = Coefficient for difference of 1st and 2nd random population member (try 0.5)
	// cr       = Crossing-over amount, 0..1
	//
	// Returns: Cost of best parameter vector in population. 
	// Member variable best becomes a pointer to the best parameter vector in population.

	floating evolve(CostEvaluator *z, floating gainbest, floating gaindiffr1r2, floating gainr3, floating cr) {
		if (!best) findBest(z);
		floating *newbest = best;
		floating gainorig = 1.0-gainbest-gainr3;
		int member;
		for (member = 0; (member < np); member++) {
			int r = intRandom(np);
			int temp = r1table[r];
			r1table[r] = r1table[member];
			r1table[member] = temp;
		}
		for (member = 0; (member < np); member++) {			

			// Choose some parameter vectors randomly
			int r1, r2, r3;
			r1 = r1table[member];
			do {
				r2 = intRandom(np);			
			} while ((r2 == member) || (&population[r2*d] == best) || (r2 == r1));
			do {
				r3 = intRandom(np);			
			} while ((r3 == member) || (&population[r3*d] == best) || (r3 == r1) || (r3 == r2));

			// Generate trial vector with crossing-over
			int actualparam = intRandom(d);
			int param = 0;
			do { // Take at least one new parameter before crossing-over

				nextgenpopulation[member*d+actualparam] =
					population[member*d+actualparam]*gainorig + best[actualparam]*gainbest +
					(population[r1*d+actualparam]-population[r2*d+actualparam])*gaindiffr1r2 +
					population[r3*d+actualparam]*gainr3;
					
				actualparam = (actualparam+1)%d;
				param++;
			} while ((param < d) && (floatingRandom()<cr)); // Binomial crossing-over
			for (; (param < d); param++) {
				nextgenpopulation[member*d+actualparam] = population[member*d+actualparam];
				actualparam = (actualparam+1)%d;
			}

			// Is new one better?
			floating cost = z->evaluateCost(&nextgenpopulation[member*d]);
			if (cost < costs[member]) {
				// Yes, keep it
				costs[member] = cost;
				if (cost < bestcost) { // Check if new best
					bestcost = cost;
					newbest = &nextgenpopulation[member*d];
				}
			} else {
				// No, use old
				memcpy(&nextgenpopulation[member*d], &population[member*d], sizeof(floating)*d);
			}
		}

		// New population becomes current population
		std::swap(nextgenpopulation, population);

		best = newbest;
		return bestcost;
	}

	// Constructor
	// 
	// np     = Number of population members, 5 or more (30 is nice)
	// d      = Number of parameters in parameter vector, 1 or more
	// minx[] = Parameter vector with all parameters set to minimum possible values
	// maxx[] = Parameter vector with all parameters set to maximum possible values
	UMDEEvolution(int np, int d, const floating *minx, const floating *maxx) {
		this->np = np;
		this->d = d; 
		population = new floating[np*d]; 
		nextgenpopulation = new floating[np*d]; 
		costs = new floating[np]; 
		r1table = new int[np];
		for (int member = 0; (member < np); member++) {
			r1table[member] = member;
		}
		randomPopulation(minx, maxx); // Initialize population
	}

	// Destructor
	~UMDEEvolution() {
		delete[] population;
		delete[] nextgenpopulation;
		delete[] costs;
		delete[] r1table;
	}
};
/*
// Population Average Unique Mother Differential Evolution class
// -------------------------------------------------------------
//
// Tries to search for the global minimum of a cost function which gets a 
// parameter vector as an argument
class UMDEEvolution {
private:
	floating *costs;             // Costs of parameter vectors in population
	int np;                      // Number of population members
	floating *population;        // Parameter vectors in population, one-by-one
	floating *nextgenpopulation; // Next generation that is being produced
	floating *best;        // Pointer to best parameter vector in population
	floating bestcost;     // Cost of the above (invalid if best == NULL)
	int *r1table;  // Shuffled mother table
	int d;                 // Number of parameters
	CostEvaluator *z;

public:
	// Get latest best parameter vector in population
	floating *getBest() {
		if (!best) findBestAndAverage(z);
	}

	// Find best parameter vector in population
	//
	// Returns: Cost of best. Updates member variables best, bestcost, averagecost and costs
	floating findBestAndAverage() {
		best = &population[0];
		bestcost = z->evaluateCost(best);
		for (int member = 1; (member < np); member++) {
			costs[member] = z->evaluateCost(&population[member*d]);
			if (costs[member] < bestcost) {
				bestcost = costs[member];
				best = &population[member*d];
			}
		}
		return bestcost;
	}

	floating getAverage(CostEvaluator *z) {
		for (int member = 1; (member < np); member ++) {
			costs[member] = z->evaluateCost(&population[member*d);
		}
	}

	// Random functions required by the Differential Evolution class
	// -------------------------------------------------------------

	// Randomize by system clock. Please call me!
	static void randomize() {
		srand((unsigned)time(0));
	}

	// Uniform floating-point random
	//
	// Returns: 0..1
	static floating floatingRandom() {
		return rand()/(RAND_MAX+1.0f);
	}

	// Uniform integer random
	//
	// Returns: 0..maxplus1-1
	static int intRandom(int maxplus1) {
		return rand()%maxplus1; //Hope RAND_MAX is not too small!
	}

	// Non-constant member functions
	// -----------------------------

	// Fill parameters in all population members with random values (restarts evolution)
	//
	// minx[] = parameter vector with all parameters set to minimum possible values
	// maxx[] = parameter vector with all parameters set to maximum possible values
	void randomPopulation(const floating *minx, const floating *maxx) {
		for (int member = 0; (member < np); member++) {
			for (int param = 0; (param < d); param++) {
				population[member*d+param] = floatingRandom()*(maxx[param]-minx[param])+minx[param];
			}
		}
		best = NULL;
	}

	// Evolve into next generation - try evolve(&z, 0, 0.7, 1.0, 1);
	//
	// gainbest     = Coefficient for best population member (try 1.2)
	// gainr3       = Coefficient for 3rd random population member (try 1)
	// (Note: Coefficient for original population member = 1 - sum of the above)
	// gaindiffr1r2 = Coefficient for difference of 1st and 2nd random population member (try 0.5)
	// cr       = Crossing-over amount, 0..1
	//
	// Returns: Cost of best parameter vector in population. 
	// Member variable best becomes a pointer to the best parameter vector in population.

	floating evolve(floating gainbest, floating gaindiffr1r2, floating gainr3, floating cr) {
		if (!best) findBest(z);
		floating *newbest = best;
		floating gainorig = 1.0-gainbest-gainr3;
		int member;
		for (member = 0; (member < np); member++) {
			int r = intRandom(np);
			int temp = r1table[r];
			r1table[r] = r1table[member];
			r1table[member] = temp;
		}
		for (member = 0; (member < np); member++) {			

			// Choose some parameter vectors randomly
			int r1, r2, r3;
			r1 = r1table[member];
			do {
				r2 = intRandom(np);			
			} while ((r2 == member) || (&population[r2*d] == best) || (r2 == r1));
			do {
				r3 = intRandom(np);			
			} while ((r3 == member) || (&population[r3*d] == best) || (r3 == r1) || (r3 == r2));

			// Generate trial vector with crossing-over
			int actualparam = intRandom(d);
			int param = 0;
			do { // Take at least one new parameter before crossing-over

				nextgenpopulation[member*d+actualparam] =
					population[member*d+actualparam]*gainorig + best[actualparam]*gainbest +
					(population[r1*d+actualparam]-population[r2*d+actualparam])*gaindiffr1r2 +
					population[r3*d+actualparam]*gainr3;
					
				actualparam = (actualparam+1)%d;
				param++;
			} while ((param < d) && (floatingRandom()<cr)); // Binomial crossing-over
			for (; (param < d); param++) {
				nextgenpopulation[member*d+actualparam] = population[member*d+actualparam];
				actualparam = (actualparam+1)%d;
			}

			// Is new one better?
			floating cost = z->evaluateCost(&nextgenpopulation[member*d]);
			if (cost < costs[member]) {
				// Yes, keep it
				costs[member] = cost;
				if (cost < bestcost) { // Check if new best
					bestcost = cost;
					newbest = &nextgenpopulation[member*d];
				}
			} else {
				// No, use old
				memcpy(&nextgenpopulation[member*d], &population[member*d], sizeof(floating)*d);
			}
		}

		// New population becomes current population
		std::swap(nextgenpopulation, population);

		best = newbest;
		return bestcost;
	}

	// Constructor
	// 
	// z      = Pointer to cost function evaluator (argument: parameter vector)
	// np     = Number of population members, 5 or more (30 is nice)
	// d      = Number of parameters in parameter vector, 1 or more
	// minx[] = Parameter vector with all parameters set to minimum possible values
	// maxx[] = Parameter vector with all parameters set to maximum possible values
	PAUMDEEvolution(CostEvaluator *z, int np, int d, const floating *minx, const floating *maxx) {
		this->z = z;
		this->np = np;
		this->d = d; 
		population = new floating[np*d]; 
		nextgenpopulation = new floating[np*d]; 
		costs = new floating[np]; 
		r1table = new int[np];
		for (int member = 0; (member < np); member++) {
			r1table[member] = member;
		}
		randomPopulation(minx, maxx); // Initialize population
	}

	// Destructor
	~PAUMDEEvolution() {
		delete[] population;
		delete[] nextgenpopulation;
		delete[] costs;
		delete[] r1table;
	}
};
*/
#endif