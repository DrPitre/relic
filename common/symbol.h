#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_UNKNOWN	0
#define TYPE_BOOLEAN	1
#define TYPE_BYTE	2
#define TYPE_REAL	3
#define TYPE_INTEGER	4
#define TYPE_STRING	5
#define TYPE_USER	6
#define TYPE_PROCEDURE	7
#define TYPE_LABEL	8

class SymbolTable
{
#define MAXSYMS		2000
	private:
		char *typestr[9];

		class Symbol
		{
			public:
				char	name[128];		// name of symbol
				int		type;			// data type
				int		size;			// size of type
				int		scope;			// holds offset in symbol table to symbol that this entry is scoped to
				int		init;			// initialized data flag (1 = initialized, 0 = uninitialized)
				int		param;			// parameter ordinal position (0 = not a parameter)
				int		global;			// global value flag
				char	value[128];		// value of symbol

				Symbol(void)
				{
					value[0] = name[0] = '\0';
					type = size = scope = init = param = global = 0;
				}
		};
		
		Symbol symbols[MAXSYMS];

		int numSymbols;
		int tempCounter;

	public:
		int typelen[9];
		SymbolTable(void)
		{
			numSymbols = 0;
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
		}
		
		/* removes a symbol from the symbol table
		 */
		int removeSymbol(int whichSymbol)
		{
			for (int i = whichSymbol + 1; i < numSymbols; i++)
			{
				symbols[i - 1] = symbols[i];
			}

			numSymbols--;
			
			return 0;
		}
		
		
		int count(void)
		{
			return numSymbols;
		}
		
		int mkTemp(int scope)
		{
			return mkTemp(TYPE_UNKNOWN, scope);
		}
		
		int mkTemp(int type, int scope)
		{
			char s[5];

			sprintf(s, "_%d", tempCounter++);

			return store(s, type, scope);
		}
		
		int getIndex(char *name, int scope)
		{
			int i;
			
			for (i = 0; i < numSymbols; i++)
			{
				if (strcasecmp(symbols[i].name, name) == 0
				&& (symbols[i].type == TYPE_PROCEDURE || symbols[i].scope == scope)
				)
				{
					// match
					break;
				}
			}

			if (i == numSymbols)
			{
				i = -1;
			}
			
			return i;
		}
		

		char *getLabel(int address)
		{
			int i;
			
			for (i = 0; i < numSymbols; i++)
			{
				if ((symbols[i].type == TYPE_PROCEDURE || symbols[i].type == TYPE_LABEL) && atoi(symbols[i].value) == address)
				{
					// match
					return symbols[i].name;
				}
			}
			
			return NULL;
		}
		

		int store(char *name, int type, int scope)
		{
			// First look up the name.  If it's already defined, return its location
			int location;
			
			location = getIndex(name, scope);
			
			if (location != -1)
			{
				return location;
			}
			
			// symbol doesn't exist -- add it to the end
			setType(numSymbols, type);
			symbols[numSymbols].scope = scope;
			symbols[numSymbols].init = 0;
			if (name != NULL || name[0] != '\0')
			{
				strcpy(symbols[numSymbols].name, name);
			}
			else
			{
				symbols[numSymbols].name[0] = '\0';
			}
			
			// return offset to newly created symbol and increment
			return numSymbols++;
		}


		int largerType(int type1, int type2)
		{
			if (typelen[type1] > typelen[type2])
			{
				return type1;
			}
			else
			{
				return type2;
			}
		}
	

		int setValue(int location, char *value)
		{
			strcpy(symbols[location].value, value);
			symbols[location].init = 1;

			return 0;
		}


		char *getValue(int location)
		{
			if (*symbols[location].value == '\0')
			{
				return NULL;
			}

			return symbols[location].value;
			
		}
		

		char *getTypeName(int type)
		{
			return typestr[type];
		}


		void setType(int location, int type)
		{
			symbols[location].type = type;
			symbols[location].size = typelen[type];
		}


		void setGlobal(int location, int gflag)
		{
			symbols[location].global = gflag;
		}


		int getGlobal(int location)
		{
			return symbols[location].global;
		}


		int setSize(int location, int size)
		{
			symbols[location].size = size;

			return 0;
		}


		int getSize(int location)
		{
			return symbols[location].size;
		}


		int getScope(int location)
		{
			return symbols[location].scope;
		}


		void setScope(int location, int scope)
		{
			symbols[location].scope = scope;
		}


		int getParam(int location)
		{
			return symbols[location].param;
		}


		void setParam(int location, int value)
		{
			symbols[location].param = value;
		}


		int isInitialized(int location)
		{
			return (symbols[location].init);
		}


		char *getName(int location)
		{
			return symbols[location].name;
		}


		int getParam(int scope, int param)
		{
			for (int i = 0; i < numSymbols; i++)
			{
				if (symbols[i].scope == scope &&
				symbols[i].param == param)
				{
					return i;
				}
			}

			return -1;
		}


		int getType(int location)
		{
			return symbols[location].type;
		}


		int getType(char *name, int scope)
		{
			int i;
			
			for (i = 0; i < numSymbols; i++)
			{
				if (strcasecmp(symbols[i].name, name) == 0
				&& symbols[i].scope == scope
				)
				{
					// match
					break;
				}
			}

			if (i == numSymbols)
			{
				i = -1;
			}
			
			return getType(i);
		}
		

		char *getScopedName(char *name, int location)
		{
			if (getScope(location) == -1)
			{
				sprintf(name, "%s", getName(location));
			}
			else
			{
				sprintf(name, "%s.%s", getName(getScope(location)), getName(location));
			}

			return name;
		}

 
		void dump(FILE *out)
		{
			fprintf(out, "%-10s %-16s    %-12s    %-8s    %-8s    %-8s    %-8s    %-8s    %s\n", "OFFSET", "NAME", "TYPE", "SIZE", "SCOPE", "INITFLAG", "GLOBAL", "PARAM", "VALUE");
			fprintf(out, "-----------------------------------------------------------------------------------------------------------------\n");

			for (int i = 0; i < numSymbols; i++)
			{
				char value[128];

				if (symbols[i].type == TYPE_PROCEDURE)
				{
					fprintf(out, "%-10d %-16s    ", i, getName(i));
				}
				else
				{
					fprintf(out, "%-10d %-16s    ", i, getName(i));
//					fprintf(out, "%-10d %-16s    ", i, scopedVariable(name, i));
				}
				
				if (symbols[i].value[0] == '\0')
				{
					strcpy(value, "");
				}
				else
				{
					strcpy(value, symbols[i].value);
				}

				if (symbols[i].type == TYPE_STRING)
				{
					fprintf(out, "%-12s    %-8d    %-8d    %-8d    %-8d    %-8d    \"%s\"\n", typestr[symbols[i].type], symbols[i].size, symbols[i].scope, symbols[i].init, symbols[i].global, symbols[i].param, value);
				}
				else
				{
					fprintf(out, "%-12s    %-8d    %-8d    %-8d    %-8d    %-8d    %s\n", typestr[symbols[i].type], symbols[i].size, symbols[i].scope, symbols[i].init, symbols[i].global, symbols[i].param, value);
				}

			}

			fprintf(out, "-EOF\n");
		}

	void read(FILE *in, int *dataSize)
	{
#define LINE_SIZE	256
	 	char *offset, *name, *type, *size, *scope, *initflag, *globalflag, *paramflag, *value, line[LINE_SIZE];
		char *li;
		int linenum = 0;
	
	 	*dataSize = 0;
 	
		numSymbols = 0;
	
		/* skip over two lines of header */
		fgets(line, 127, in);
		fgets(line, 127, in);

		while ((li = fgets(line, LINE_SIZE - 1, in)) && strcmp(li, "-EOF\n") != 0)
		{
			linenum++;
			offset = strtok(line, " \n");
			name = strtok(NULL, " \n");
			type = strtok(NULL, " \n");
			size = strtok(NULL, " \n");
			scope = strtok(NULL, " \n");
			initflag = strtok(NULL, " \n");
			globalflag = strtok(NULL, " \n");
			paramflag = strtok(NULL, " \n");
			value = strtok(NULL, "\n");
			if (value != NULL)
			{
				while (*value == ' ') value++;
			}
			
			if (offset != NULL && name != NULL && type != NULL && size != NULL && scope != NULL && initflag != NULL)
			{
				strcpy(symbols[numSymbols].name, name);
				symbols[numSymbols].size = atoi(size);
				symbols[numSymbols].scope = atoi(scope);
				symbols[numSymbols].global = atoi(globalflag);
				symbols[numSymbols].init = atoi(initflag);
				
				*dataSize += atoi(size);
				if (strcasecmp(type, "INTEGER") == 0)
				{
					symbols[numSymbols].type = TYPE_INTEGER;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
				else if (strcasecmp(type, "STRING") == 0)
				{
					symbols[numSymbols].type = TYPE_STRING;
					if (value != NULL)
					{
						strncpy(symbols[numSymbols].value, value + 1, strlen(value) - 2);
					}
				}
				else if (strcasecmp(type, "BOOLEAN") == 0)
				{
					symbols[numSymbols].type = TYPE_BOOLEAN;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
				else if (strcasecmp(type, "BYTE") == 0)
				{
					symbols[numSymbols].type = TYPE_BYTE;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
				else if (strcasecmp(type, "PROCEDURE") == 0)
				{
					symbols[numSymbols].type = TYPE_PROCEDURE;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
				else if (strcasecmp(type, "LABEL") == 0)
				{
					symbols[numSymbols].type = TYPE_LABEL;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
				else if (strcasecmp(type, "REAL") == 0)
				{
					// TODO : determine format for reals
					symbols[numSymbols].type = TYPE_REAL;
					if (value != NULL)
					{
						strcpy(symbols[numSymbols].value, value);
					}
				}
			}

			numSymbols++;
		}
	}
};

#endif	// _SYMBOL_H
