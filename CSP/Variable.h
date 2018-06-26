#pragma once
#include "Constraint.h"
using namespace std;
//Predeclaration to know about class representing constraint
class Constraint;

/// <summary>
/// Class representing a variable to be used in a CSP
/// </summary>
class Variable {
public:
	// An array of canstraints to a current variable
	Constraint** constraints;
	
	// size of an array
	int constraint_size;
	
	// domain array (is not destructed in destructor - its a pointer to a domain ina solver)
	int *domain;
	
	// size of domain
	int domain_size;

	// array bound to domain, whish value is currently available
	bool *usable;

	// index of value in domain
	int value_position;		

	// is value assigned to variable
	bool assigned;

	Variable(int value_position, int domain_size, int debug_id);
	Variable(Variable &other);
	~Variable();


	/// <summary>
	/// Gets the current value of the variable basing of the domain value position.
	/// </summary>
	/// <returns></returns>
	int getValue() { return domain[value_position]; }
	
	/// <summary>
	/// Checks the constraints.
	/// </summary>
	/// <returns>true if all constraints satisfied, otherwise false</returns>
	bool checkConstraints();
};