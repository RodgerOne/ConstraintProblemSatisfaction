#include "stdafx.h"
#include "constants.h"
#include "Backtracking.h"
#include <iostream>

BackTracking::BackTracking(int size, int domain_size) {
	this->size = size;
	full_size = size * size;
	solutions = 0;
	vars = new Variable*[full_size];
	int full_size = size * size;
	for (int i = 0; i < full_size; i++) {
		vars[i] = (new Variable(0, domain_size, -1));
	}
}

BackTracking::~BackTracking() {
	for (int i = 0; i < full_size; i++)
		delete vars[i];
	delete vars;
}

int BackTracking::findAll() {
	int i = 0;
	backtracks = 0;
	while (i < full_size) {
		Variable &var = *vars[i];
		while (var.value_position < var.domain_size)
			if (var.assigned = var.checkConstraints())  break; 
			else ++var.value_position;		
		
		if (var.assigned && i == full_size - 1) {
			var.assigned = false;
			solutions++;
			if (DEBUG)  cout << "Solution: " << solutions << endl; 	
		}

		if (!var.assigned) {
			backtracks++;	//backtrack
			var.value_position = 0;
			if (--i < 0) return solutions;	//domain to small or found all
			else { vars[i]->value_position++; vars[i]->assigned = false; }
		} else i++;
	}
	return solutions;
}

bool BackTracking::findFirst() {
	int i = 0;
	backtracks = 0;
	while (i < full_size) {
		Variable &var = *vars[i];
		while (var.value_position < var.domain_size)
			if (var.assigned = var.checkConstraints()) break;
			else  ++var.value_position;

			if (!var.assigned) {
				backtracks++;	//backtrack
			var.value_position = 0;
			if (--i < 0) return false;	//domain to small or found all
			else { vars[i]->value_position++; vars[i]->assigned = false; }
		}
		else i++;
		if (DEBUG) printSolution();
	}
	return true;
}

void BackTracking::printSolution() {
	cout << "\n\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Variable * var = getVariable(i, j);
			cout << "  " << (var->assigned ? var->getValue() : -1);
		}
		cout << "\n";
	}
	cout << "\n\n";
}