/**********************************************************************************
 * Symbol Class Implementation File
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


// Constructor -- Builds a symbol based on name, value and line no.
Symbol::Symbol(string symbolName, string symbolValue, int isOverridable)
{
	name = symbolName;
	value = symbolValue;
	overridable = isOverridable;

	return;
}


// Destructor -- Destroys a symbol
Symbol::~Symbol(void)
{
	return;
}


string Symbol::GetName(void)
{
	return(name);
}


string Symbol::GetValue(void)
{
	return(value);
}


string Symbol::SetValue(string newValue)
{
	if (overridable == false)
	{
		return("Symbol not overridable");
	}

	value = newValue;

	return("OK");
}
