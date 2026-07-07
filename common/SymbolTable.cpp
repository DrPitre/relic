/**********************************************************************************
 * SymbolTable Class Implementation File
 *
 * (C) 2002 Acadian Embedded
 *
 * $Id$
 *
 * Description                                                       YYYY/MM/DD WHO
 * ----------------------------------------------------------------  ---------- ---
 * Created                                                           2002/09/17 BGP
 */

#include <iostream>
#include <iomanip>
#include "SymbolTable.h"


// Constructor
SymbolTable::SymbolTable(void)
{
	typestr[0] = "UNKNOWN";
	typestr[1] = "BOOLEAN";
	typestr[2] = "BYTE";
	typestr[3] = "REAL";
	typestr[4] = "INTEGER";
	typestr[5] = "STRING";
	typestr[6] = "USER";
	typestr[7] = "PROCEDURE";
	typestr[8] = "LABEL";

	typelen[0] = 0;
	typelen[1] = 1;
	typelen[2] = 1;
	typelen[3] = 5;
	typelen[4] = 2;
	typelen[5] = 32;
	typelen[6] = 0;
	typelen[7] = 0;
	typelen[8] = 0;

	return;
}


// Destructor
SymbolTable::~SymbolTable(void)
{
}


// Add a symbol
void SymbolTable::AddSymbol(Symbol *symbol)
{
	symbols.push_back(*symbol);

	return;
}


// Report symbols
void SymbolTable::ReportSymbols(void)
{
	int count = 0;
	vector<Symbol>::iterator i;

	for (i = symbols.begin(); i < symbols.end(); i++)
	{
		count++;

		cout << setfill(' ');
		cout << setw(12 - i->GetName().size()) << i->GetName() << " ";
		cout << setw(4 - i->GetValue().size()) << i->GetValue() << " ";

		if (count % 3 == 0)
		{
			cout << endl;
		}
	}

	cout << endl << count << " symbol";
	if (count != 1)
	{
		cout << "s";
	}
	cout << endl;

	return;
}


// Determines if a particular symbol is defined
// Returns 1 if symbol is defined in table, else 0 if not
Symbol *SymbolTable::GetSymbol(string symbolName)
{
	vector<Symbol>::iterator i;

	for (i = symbols.begin(); i < symbols.end(); i++)
	{
		if (i->GetName() == symbolName)
		{
			return(&*i);
		}
	}

	return(NULL);
}


// Determines if a particular symbol is defined
// Returns 1 if symbol is defined in table, else 0 if not
Symbol *SymbolTable::GetSymbolOfType(int type)
{
	vector<Symbol>::iterator i;

	for (i = symbols.begin(); i < symbols.end(); i++)
	{
		if (i->GetType() == type)
		{
			return(&*i);
		}
	}

	return(NULL);
}
