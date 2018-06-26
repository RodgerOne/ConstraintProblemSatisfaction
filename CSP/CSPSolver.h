#pragma once
#include "Variable.h"
#include <vector>

/// <summary>
/// An interface to be implemented by a actual CSP solver (eg. Bactracking) to match
/// </summary>
class CSPSolver {
	public:
		//count of found solutions
		int solutions;
		
		//length of the square matrix
		int size;
		
		//full size of the matrix (size*size)
		int full_size;
		
		//size of the varaible values domain - common for all variables in that problem.
		int domain_size;

		// count of backtracks
		long backtracks;

		//matrix of variables represented as an casual array.
		Variable * * vars;

		/// <summary>
		/// Gets the variable by an (x, y) position in a square matrix. 
		/// </summary>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position</param>
		/// <returns></returns>
		virtual Variable* getVariable(int x, int y) = 0;

		/// <summary>
		/// Initializes the constraints for problem to solve.
		/// </summary>
		/// <param name="initializer">The initializer - the function witch will initatie the constraints set - it takes as a param a reference to sovler - neccessary to access constraint set.</param>
		virtual void initConstraints(void(*initializer)(CSPSolver &solver)) = 0;

		/// <summary>
		/// Finds the first solution.
		/// </summary>
		/// <returns>True if exists any solution, otherwise false.</returns>
		virtual bool findFirst() = 0;

		/// <summary>
		/// Finds all solutions.
		/// </summary>
		/// <returns>Count of solutions</returns>
		virtual int findAll() = 0;

		/// <summary>
		/// Prints the solution.
		/// </summary>
		virtual void printSolution() = 0;
};