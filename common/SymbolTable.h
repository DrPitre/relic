#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
/**********************************************************************************
 * SymbolTable Class Header File
 *
 * (C) 2002 Acadian Embedded
 *
 * $Id$
 *
 * Description                                                       YYYY/MM/DD WHO
 * ----------------------------------------------------------------  ---------- ---
 * Created                                                           2002/09/17 BGP
 */


#include "NSymbol.h"
#include <vector>


class SymbolTable
{
	private:
		vector<Symbol> symbols;
		string typestr[9];
		int typelen[9];

	public:
		SymbolTable(void);
		~SymbolTable(void);
		void AddSymbol(Symbol *symbol);
		void RemoveSymbol(Symbol *symbol);
		Symbol *GetSymbol(string symbolName);
		void ReportSymbols(void);
};
#endif	// _SYMBOLTABLE_H
