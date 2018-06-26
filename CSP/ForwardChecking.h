#pragma once
#include "CSPSolver.h"


/// <summary>
/// An implementation of solving CSP with a forward checking algorithm
/// </summary>
/// <seealso cref="CSPSolver" />
class ForwardChecking : public CSPSolver {
	private:		
		/// <summary>
		/// Moves value domain position to next.
		/// </summary>
		/// <param name="var">The variable.</param>
		/// <returns>true if value exists(domain is not out of scope), otherwise false</returns>
		bool getNextValue(Variable &var);
		
		/// <summary>
		/// Resets the variable domain position to default (begining).
		/// </summary>
		/// <param name="var">The variable.</param>
		void ResetValue(Variable &var);

		/// <summary>
		/// Backups the domains to the backup.
		/// </summary>
		/// <param name="var">The variable.</param>
		/// <param name="backup">The backup (bool array of available positions).</param>
		void backup_domains(Variable &var, bool ** backup);
		
		/// <summary>
		/// Restores the domains from backup.
		/// </summary>
		/// <param name="var">The variable.</param>
		/// <param name="backup">The backup to be restored. (bool array of available positions)</param>
		void restore_domains(Variable &var, bool ** backup);

		/// <summary>
		/// Eliminates from domain (set as unavailable) values  which do not satisfy constraints.
		/// </summary>
		/// <param name="var">The variable.</param>
		/// <returns>true if the domain is not empty, otherwise false</returns>
		bool eliminate_from_domain(Variable &var);

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BackTracking"/> class.
		/// </summary>
		/// <param name="size">The size of a square matrix of varaibles reprezenting problem domain.</param>
		/// <param name="domain_size">Size of the domain - actual scope of the values.</param>
		ForwardChecking(int size, int domain);

		/// <summary>
		/// Finalizes an instance of the <see cref="ForwardChecking"/> class.
		/// </summary>
		~ForwardChecking();
		
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
