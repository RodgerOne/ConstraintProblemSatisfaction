#pragma once
#include "CSPSolver.h"

/// <summary>
/// An implementation of solving CSP with a bactracking algorithm
/// </summary>
/// <seealso cref="CSPSolver" />
class BackTracking : public CSPSolver {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BackTracking"/> class.
		/// </summary>
		/// <param name="size">The size of a square matrix of varaibles reprezenting problem domain.</param>
		/// <param name="domain_size">Size of the domain - actual scope of the values.</param>
		BackTracking(int size, int domain_size);
		
		/// <summary>
		/// Finalizes an instance of the <see cref="BackTracking"/> class.
		/// </summary>
		~BackTracking();
		
		/// <summary>
		/// Gets the variable by an (x, y) position in a square matrix. 
		/// </summary>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position</param>
		/// <returns></returns>
		Variable* getVariable(int x, int y) { return vars[x * size + y]; }

		/// <summary>
		/// Initializes the constraints for problem to solve.
		/// </summary>
		/// <param name="initializer">The initializer - the function witch will initatie the constraints set - it takes as a param a reference to sovler - neccessary to access constraint set.</param>
		void initConstraints(void(*initializer)(CSPSolver &solver)) { initializer(*this); };
		
		/// <summary>
		/// Finds the first solution.
		/// </summary>
		/// <returns>True if exists any solution, otherwise false.</returns>
		bool findFirst();

		/// <summary>
		/// Finds all solutions.
		/// </summary>
		/// <returns>Count of solutions</returns>
		int findAll();

		/// <summary>
		/// Prints the solution.
		/// </summary>
		void printSolution();
};
