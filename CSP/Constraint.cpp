#include "stdafx.h"
#include "Constraint.h"

Constraint::Constraint(Variable *v1, Variable *v2, pred f) {
	var1 = v1; var2 = v2;
	func = f;
}

Constraint::Constraint(Constraint &other) {
	func = other.func;
	var1 = other.var1;
	var2 = other.var2;
}

Constraint::~Constraint() {}

bool Constraint::eval() {
	return !var2->assigned || func(var1->getValue(), var2->getValue());
}
