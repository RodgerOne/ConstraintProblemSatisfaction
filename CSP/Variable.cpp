#include "stdafx.h"
#include "Variable.h"

Variable::Variable(int value_position,  int domain_size, int debug_id) {
	this->value_position = value_position;
	assigned = false;
	constraint_size = 0;
	this->domain_size = domain_size;
	domain = new int[domain_size];
	for (int i = 0; i < domain_size; i++) domain[i] = i;
	usable = new bool[domain_size];
	for (int i = 0; i < domain_size; i++) usable[i] = true;
}

Variable::Variable(Variable &other) {
	value_position = other.value_position;
	assigned = other.assigned;
	domain_size = other.domain_size;
	constraint_size = other.constraint_size;

	constraints = new Constraint*[other.constraint_size];
	for (int i = 0; i < other.constraint_size; i++)
		constraints[i] = new Constraint(*other.constraints[i]);
	
	domain = new int[domain_size];
	for (int i = 0; i < domain_size; i++) domain[i] = other.domain[i];

	usable = new bool[other.domain_size];
	for (int i = 0; i < other.domain_size; i++)
		usable[i] = (other.usable[i]);
}

Variable::~Variable() {
	for (int i = 0; i < constraint_size; i++) {
		delete constraints[i];
	}
	delete constraints;
	delete domain;
	delete usable;
}

bool Variable::checkConstraints() {
	for (int i = 0; i < constraint_size; i++)
		if(!constraints[i]->eval()) return false;
	return true;
}

