#pragma once
#include "Variable.h"
#include "cmath"
#include "constants.h"
using namespace std;
//Predeclaration to know about class representing variable
class Variable;

/// <summary>
/// A represantation of an constraint to a variable in a CSP
/// </summary>
class Constraint {
public:
	// predicate to evaluate about satisfying a constraint
	pred func;
	// variables connected by a constraint
	Variable *var1, *var2;
	Constraint(Variable *v1, Variable *v2, pred f);
	Constraint(Constraint &other);
	~Constraint();
	//evaluation of constraint - calls predicate
	bool eval();
};