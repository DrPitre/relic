#ifndef _SYMBOL_H
#define _SYMBOL_H
/**********************************************************************************
 * Symbol Class Header File
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
#include <string>

using namespace std;

typedef enum
{
	
class Symbol
{
	private:
		string	name;			// name of symbol
		string	value;			// value
		int		type;			// data type
		int		size;			// size of type
		int		scope;			// holds offset in symbol table to symbol that this entry is scoped to
		int		init;			// initialized data flag (1 = initialized, 0 = uninitialized)
		int		param;			// parameter ordinal position (0 = not a parameter)
		int		global;			// global value flag
		int		overridable;

	public:
		Symbol(string symbolName, string symbolValue, int isOverridable);
		~Symbol(void);
		string GetName(void);
		string GetValue(void);
		string SetValue(string newValue);
};
#endif
