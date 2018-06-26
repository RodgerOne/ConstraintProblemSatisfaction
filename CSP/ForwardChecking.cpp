#include "stdafx.h"
#include "ForwardChecking.h"
#include <iostream>

ForwardChecking::ForwardChecking(int size, int domain) {
	domain_size = domain;
	solutions = 0;
	this->size = size;
	full_size = size * size;
	vars = new Variable*[full_size];
	for (int i = 0; i < full_size; i++)	vars[i] = new Variable(-1, domain, i);
}

ForwardChecking::~ForwardChecking() {
	for (int i = 0; i < full_size; i++)	delete vars[i];
	delete vars;
}


bool ForwardChecking::findFirst() {
	backtracks = 0;
	//init backups
	bool *** domains_backup = new bool**[full_size];
	for (int i = 0; i < full_size; i++) {
		int tmp = vars[i]->constraint_size;
		domains_backup[i] = new bool*[tmp];
		for (int j = 0; j < tmp; j++) {
			domains_backup[i][j] = new bool[domain_size];
			for (int k = 0; k < domain_size; k++) domains_backup[i][j][k] = true;
		}
	}
	int i = 0;
	//start
	while (i < full_size) {
		Variable &var = *vars[i];	
		while (getNextValue(var)) {
			backup_domains(var, domains_backup[i]);
			if (eliminate_from_domain(var)) {
				var.assigned = true;
				break;
			}
			else restore_domains(var, domains_backup[i]);
		}
		if (!var.assigned) {
			backtracks++;
			if (i == 0) return false;
			ResetValue(var);
			--i;
			vars[i]->assigned = false;
			restore_domains(*vars[i], domains_backup[i]);
		}
		else i++;
	}
	//end
	// delete backups at the end
	for (int i = 0; i < full_size; i++) {
		for (int j = 0; j < vars[i]->constraint_size; j++) {
			delete domains_backup[i][j];
		}
		delete domains_backup[i];
	}
	return true;
}

int ForwardChecking::findAll() {
	backtracks = 0;
	//init backups
	bool *** domains_backup = new bool**[full_size];
	for (int i = 0; i < full_size; i++) {
		int tmp = vars[i]->constraint_size;
		domains_backup[i] = new bool*[tmp];
		for (int j = 0; j < tmp; j++) {
			domains_backup[i][j] = new bool[domain_size];
			for (int k = 0; k < domain_size; k++) domains_backup[i][j][k] = true;
		}
	}
	//start
	int i = 0;
	while (i < full_size) {
		Variable &var = *vars[i];
		while (getNextValue(var)) {
			backup_domains(var, domains_backup[i]);
			if (eliminate_from_domain(var)) {
				var.assigned = true;
				break;
			}
			else  restore_domains(var, domains_backup[i]);
		}
		if (var.assigned && i == full_size - 1) { solutions++;  /*cout << "solution: " << solutions << endl;*/ var.assigned = false; }
		if (!var.assigned) {
			backtracks++;
			if (i == 0) return solutions;
			ResetValue(var);
			--i;
			vars[i]->assigned = false;
			restore_domains(*vars[i], domains_backup[i]);
		}
		else i++;
	}
	//end
	//delete backups at the end
	for (int i = 0; i < full_size; i++) {
		for (int j = 0; j < vars[i]->constraint_size; j++) {
			delete domains_backup[i][j];
		}
		delete domains_backup[i];
	}
	return solutions;
}

bool ForwardChecking::getNextValue(Variable &var) {
	var.value_position++;
	while (var.value_position < var.domain_size && !var.usable[var.value_position]) var.value_position++;
	if (var.value_position < var.domain_size) {  return true; }
	else return false;
}

void ForwardChecking::ResetValue(Variable &var) { var.value_position = -1; var.assigned = false; }

void ForwardChecking::backup_domains(Variable &var, bool ** backup) {
	for (int i = 0; i < var.constraint_size; i++) {
		Constraint &cs = *var.constraints[i];
		if (!cs.var2->assigned) {
			Variable &var2 = *cs.var2;
			for (int j = 0, z = var2.domain_size; j < z; j++) backup[i][j] = var2.usable[j];
		}
	}
}

void ForwardChecking::restore_domains(Variable &var, bool ** backup) {
	for (int i = 0; i < var.constraint_size; i++) {
		Constraint &cs = *var.constraints[i];
		if (!cs.var2->assigned)	for (int j = 0; j < domain_size; j++)	cs.var2->usable[j] = backup[i][j];
	}
}

bool ForwardChecking::eliminate_from_domain(Variable &var) {
	for (int i = 0; i < var.constraint_size; i++) {
		Constraint &cs = *var.constraints[i];
		if (!cs.var2->assigned) {
			Variable &var2 = *cs.var2;
			bool isEmpty = true;

			for (int j = 0; j < var2.domain_size; j++) {
				if (var2.usable[j]) {
					var2.usable[j] = cs.func(var2.domain[j], var.getValue());
					if (isEmpty) isEmpty = !var2.usable[j];
				}
			}
			if (isEmpty)  return false; 
		}
	}
	return true;
}

void ForwardChecking::printSolution() {
	cout << "\n\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Variable & var = *getVariable(i, j);
			if (var.value_position != -1 && var.value_position != var.domain_size)cout << "  " << var.domain[var.value_position]; else cout << " -1";;
		}
		cout << "\n";
	}
}