#include "stdafx.h"
#include "Backtracking.h"
#include "ForwardChecking.h"
#include <chrono>
#include <iostream>
using namespace std;

/// <summary>
/// Used to measure time of execution
/// clock_ encapsulated into a class
/// </summary>
class Timer {
public:
	Timer() : beg_(clock_::now()) {}

	/// <summary>
	/// Resets clock.
	/// </summary>
	void reset() { beg_ = clock_::now(); }

	/// <summary>
	/// Elapseds time since reset.
	/// </summary>
	/// <returns></returns>
	double elapsed() const {
		return chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef chrono::high_resolution_clock clock_;
	typedef chrono::duration<double, std::ratio<1> > second_;
	chrono::time_point<clock_> beg_;
};


/// <summary>
/// Example of how to create the constraint initialiser.
/// </summary>
/// <param name="solverReference">The CSPSolver reference.</param>
void example_constraint_initialiser(CSPSolver &solverReference) {
	// get the variables array
	Variable **variables = solverReference.vars;

	//prepare predicates to be applied to variables and constraints
	pred predicate_1 = [](int a, int b)->bool {return a == b; };
	//or in the more tradintional way
	bool(*predicate_2)(int,int) = [](int a, int b)->bool {return a != b; };

	//get the size of full variables set
	int variable_set_size = solverReference.full_size;
	// get size of the matrix
	int matrix_length = solverReference.size;

	// number of constraints that will be added
	int constraints_count = 4; //....
	//for each variable set neccessary constraints
	for (int i = 0; i < variable_set_size; i++) {
		
		//get varaible and begining of constraints array
		Variable *variable = variables[i];
		Constraint** &constraints = variable->constraints;
		
		//init constraints array
		variable->constraints = new Constraint*[constraints_count];
		
		//coordinates of variable if neccessary 
		//int x = i / matrix_length;
		//int y = i % matrix_length;
		
		//find variables to constraint
		Variable *var2 = solverReference.getVariable(0, 0);
		constraints[0] = new Constraint(variable, var2, predicate_1);
		//etc....
	}

}

/// <summary>
/// Initalisation of constraint for L(2, 1) graph coloring problem.
/// </summary>
/// <param name="solver">The solver.</param>
void constraintL21(CSPSolver &solver) {
	//variable set
	Variable ** vars = solver.vars;
	//access variable from set like matrix - by indices
	auto getVariable = [&solver](int x, int y)->Variable* {return solver.getVariable(x, y);};
	//constraint predicates
	pred AbsHigherOrEqual1 = [](int a, int b)->bool {return abs(a - b) > 0;};
	pred AbsHigherOrEqual2 = [](int a, int b)->bool { return abs(a - b) > 1;};
	pred areDifferent = [](int a, int b)->bool { return a != b;};
	//matrix size, set size
	int size = solver.size, full_size = solver.full_size;

	for (int i = 0; i < full_size; i++) {
		vector<Constraint*> buffer;
		Variable &var = *vars[i];
		int x = i / size, y = i % size;
		if (x > 0 && y > 0)	buffer.push_back(new Constraint(&var, getVariable(x - 1, y - 1), AbsHigherOrEqual1));
		if (x > 0 && y < size - 1) buffer.push_back(new Constraint(&var, getVariable(x - 1, y + 1), AbsHigherOrEqual1));
		if (x > 0) buffer.push_back(new Constraint(&var, getVariable(x - 1, y), AbsHigherOrEqual2));
		if (y > 0) buffer.push_back(new Constraint(&var, getVariable(x, y - 1), AbsHigherOrEqual2));
		if (x > 1) buffer.push_back(new Constraint(&var, getVariable(x - 2, y), AbsHigherOrEqual1));
		if (y > 1) buffer.push_back(new Constraint(&var, getVariable(x, y - 2), AbsHigherOrEqual1));

		if (x < size - 1 && y < size - 1)	buffer.push_back(new Constraint(&var, getVariable(x + 1, y + 1), AbsHigherOrEqual1));
		if (x < size - 1 && y > 0) buffer.push_back(new Constraint(&var, getVariable(x + 1, y - 1), AbsHigherOrEqual1));
		if (x < size - 1) buffer.push_back(new Constraint(&var, getVariable(x + 1, y), AbsHigherOrEqual2));
		if (y < size - 1) buffer.push_back(new Constraint(&var, getVariable(x, y + 1), AbsHigherOrEqual2));
		if (x < size - 2) buffer.push_back(new Constraint(&var, getVariable(x + 2, y), AbsHigherOrEqual1));
		if (y < size - 2) buffer.push_back(new Constraint(&var, getVariable(x, y + 2), AbsHigherOrEqual1));

		var.constraint_size = buffer.size();
		var.constraints = new Constraint*[buffer.size()];
		for (int i = 0; i < var.constraint_size; i++)	var.constraints[i] = buffer[i];
	}
}

/// <summary>
/// Initalisation of constraint for Latin Square solving problem.
/// </summary>
/// <param name="solver">The solver.</param>
void constraintLS(CSPSolver &solver) {
	//variable set
	Variable ** vars = solver.vars;
	//access variable from set like matrix - by indices
	auto getVariable = [&solver](int x, int y)->Variable* {return solver.getVariable(x, y);};
	//constraint predicates
	pred AbsHigherOrEqual1 = [](int a, int b)->bool {return abs(a - b) > 0;};
	pred AbsHigherOrEqual2 = [](int a, int b)->bool { return abs(a - b) > 1;};
	pred areDifferent = [](int a, int b)->bool { return a != b;};
	//matrix size, set size
	int size = solver.size, full_size = solver.full_size;

	for (int i = 0; i < full_size; i++) {
		vector<Constraint*> buffer;
		Variable &var = *vars[i];
		for (int j = 0; j < size; j++) {
			int x = i / size, y = i % size;
			if (&var != getVariable(x, j))buffer.push_back(new Constraint(&var, getVariable(x, j), areDifferent));
			if (&var != getVariable(j, y))buffer.push_back(new Constraint(&var, getVariable(j, y), areDifferent));
		}
		var.constraint_size = buffer.size();
		var.constraints = new Constraint*[buffer.size()];
		for (int i = 0; i < var.constraint_size; i++)	var.constraints[i] = buffer[i];
	}
}

template <typename Solver> void findAllLS(int N) {
	Timer t;
	double stop = 0;
	CSPSolver *solver = new Solver(N, N);
	solver->initConstraints(constraintLS);

	t.reset();
	int sol = solver->findAll();
	stop = t.elapsed();

	cout << "\n\n#############\t" << typeid(Solver).name() << "\t##############\n";
	cout << "LATIN SQUARE\tN: " << N << endl;
	cout << "stop: " << stop * 1000.0 << " ms" << endl;
	cout << "Backtracks: " << solver->backtracks << endl;
	cout << "Found solutions: " << sol << "\n";
	delete solver;
}

template <typename Solver> void findFirstLS(int N) {
	Timer t;
	double stop = 0;

	CSPSolver *solver = new Solver(N, N);
	solver->initConstraints(constraintLS);

	t.reset();
	int sol = solver->findFirst();
	stop = t.elapsed();

	cout << "\n\n#############\t" << typeid(Solver).name() << "\t##############\n";
	cout << "First Solution for LATIN SQUARE\tN: " << N << endl;
	cout << "avg Backtracks: " << solver->backtracks << endl;
	cout << "avg stop: " << stop << " ms" << endl;
	solver->printSolution();
	delete solver;
}

template <typename Solver> void findMinimalColorSetL21(int N) {
	Timer t;
	double stop = 0;
	t.reset();
	int set = 1;
	CSPSolver *solver = NULL;
	
	int i = 1;
	bool check = true;
	while (check) {
		i++;
		if(solver != NULL) delete solver;
		solver = new Solver(N, i);
		solver->initConstraints(constraintL21);
		t.reset();
		check = !solver->findFirst();	
	}
	set = i;
	stop = t.elapsed();
	
	cout << "\n\n#############\t" << typeid(Solver).name() << "\t##############\n";
	cout << "MINIMAL COLOR SET L(2, 1)\tN: " << N << endl;
	cout << "Avg Backtracks: " << solver->backtracks << endl;
	cout << "Avg time: " << stop << " ms" << endl;
	cout << "Minimal color set: " << set << endl;
	solver->printSolution();
	delete solver;
}

template <typename Solver> void findAllSolutionsL21(int N, int D) {
	Timer t;
	double stop = 0;
	CSPSolver *solver = new Solver(N, D);
	solver->initConstraints(constraintL21);

	t.reset();
	int sol = solver->findAll();
	stop = t.elapsed();

	cout << "\n\n#############\t" << typeid(Solver).name() << "\t##############\n";
	cout << "ALL SOLUTION FOR l(2, 1)\tN: " << N << "\t\tD: " << D << endl;
	cout << "Backtracks: " << solver->backtracks << endl;
	cout << "stop: " << stop * 1000 << " ms" << endl;
	cout << "solutions: " << sol << endl;
	delete solver;
}

template <typename Solver> void findFirstL21(int N, int D) {
	Timer t;
	double stop = 0;
	CSPSolver *solver = new Solver(N, D);
	solver->initConstraints(constraintL21);

	t.reset();
	bool b = solver->findFirst();
	stop = t.elapsed();
	
	cout << "\n\n#############\t" << typeid(Solver).name() << "\t##############\n";
	cout << "FIRST SOLUTION L(2, 1)\tN: " << N << "\t\tD: " << D << endl;
	cout << "Avg Backtracks: " << solver->backtracks << endl;
	cout << "Avg stop: " << stop << " ms" << endl;
	solver->printSolution();
	delete solver;
}

int main()
{
	int N = 14, D = 7;

	findMinimalColorSetL21<ForwardChecking>(N);
	findMinimalColorSetL21<BackTracking>(N);

	findFirstL21<ForwardChecking>(N, D);
	findFirstL21<BackTracking>(N, D);

	findAllSolutionsL21<ForwardChecking>(N, D);
	findAllSolutionsL21<BackTracking>(N, D);

	findFirstLS<ForwardChecking>(N);
	findFirstLS<BackTracking>(N);

	findAllLS<ForwardChecking>(N);
	findAllLS<BackTracking>(N);
	
    return 0;
}
