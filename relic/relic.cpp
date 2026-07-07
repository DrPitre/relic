/*
 * relic executive
 *
 * The RELIC Project
 *
 * (C) 2007 Boisy G. Pitre
 */

#define	VERMAJOR	0
#define	VERMINOR	1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <codegen.h>

int back_end(struct module_params *p);

#define	FE		"rbfe"
#define	RO		"rxopt"
#define BE		"be6809"
#define	AS		"lwasm"
#define	LN		"lwlink"

char *filenameFromPathlist(char *file)
{
	char *p = strrchr(file, '/');

	if (p == NULL)
	{
		p = file;
	}
	else
	{
		p++;
	}

	return p;
}


char *filenameWithoutExtension(char *file)
{
	static char filenameOnly[256];
	char *p = filenameOnly;
	char *e;

	strcpy(filenameOnly, filenameFromPathlist(file));
	
	e = strrchr(p, '.');
	if (e != NULL)
	{
		*e = '\0';
	}
	return p;
}


char *extensionFromFilename(char *file)
{
	char *f = filenameFromPathlist(file);
	
	char *p = strrchr(f, '.');

	if (p != NULL)
	{
		p++;
	}

	return p;
}


struct module_params
{
	char exec[256];
	char filename[256];
	char infile[256];
	char outfile[256];
	char libdir[256];
	char outputdir[256];
	int skip_rxopt;
	int quiet;
	int dryrun;
	int keeptemporaryfiles;
};


typedef int (*extfuncptr)(struct module_params *);

int exec(struct module_params *p)
{
	int status = 0;
	
	if (p->quiet == 0)
	{
		printf("%s\n", p->exec);
	}
	
	if (p->dryrun == 0)
	{
		status = system(p->exec);
	}
	
	return status;
}


void delete_temporary_files(struct module_params *p)
{
	if (p->keeptemporaryfiles == 0)
	{
		char rm[256];
				
		sprintf(rm, "rm %s", p->infile);
		system(rm);
	}
}			


/* Compiler phase functions */

int front_end(struct module_params *p)
{
	sprintf(p->outfile, "%s/%s.rx", p->outputdir, filenameWithoutExtension(p->infile));
	sprintf(p->exec, "%s %s %s", FE, p->infile, p->outfile);

	return exec(p);
}



int rxopt(struct module_params *p)
{
	if (p->skip_rxopt == 1)
	{
		return back_end(p);
	}
	
	sprintf(p->outfile, "%s/%s.ro", p->outputdir, filenameWithoutExtension(p->infile));
	sprintf(p->exec, "%s %s %s", RO, p->infile, p->outfile);

	int status = exec(p);

	if (status == 0)
	{
		delete_temporary_files(p);
	}
	
	return status;
}


int back_end(struct module_params *p)
{
	sprintf(p->outfile, "%s/%s.a", p->outputdir, filenameWithoutExtension(p->infile));
	sprintf(p->exec, "%s %s %s", BE, p->infile, p->outfile);

	int status = exec(p);

	if (status == 0)
	{
		delete_temporary_files(p);
	}
	
	return status;
}


int assembler(struct module_params *p)
{
	sprintf(p->outfile, "%s/%s.r", p->outputdir, filenameWithoutExtension(p->infile));
	sprintf(p->exec, "%s --format=obj --output=%s %s", AS, p->outfile, p->infile);

	int status = exec(p);

	if (status == 0)
	{
		delete_temporary_files(p);
	}

	return status;
}


int linker(struct module_params *p)
{
	sprintf(p->outfile, "%s/%s", p->outputdir, filenameWithoutExtension(p->infile));
	sprintf(p->exec, "%s --format=os9 --output=%s --entry=_start %s/rbstart.o %s -lrblib -lalib -lsys -L%s",
		LN, p->outfile, p->libdir, p->infile, p->libdir);

	int status = exec(p);

	if (status == 0)
	{
		delete_temporary_files(p);
	}

	return status;
}


struct extfuncmap
{
	char *extension;
	extfuncptr func;
};

struct extfuncmap extfuncs[] =
{
	{"rb", front_end},
	{"rx", rxopt},
	{"ro", back_end},
	{"a", assembler},
	{"r", linker},
	{NULL, NULL}
};


struct extmap
{
	char *extension;
	char *frontend;
};

struct  extmap exts[] =
{
	{"rb",	"rbfe"},
	{NULL,	NULL}
};


char verstr[256];


extfuncptr whichfunc(char *file)
{
	struct extfuncmap *i = extfuncs;
	extfuncptr retfunc = NULL;
	char *extension = extensionFromFilename(file);
	
	while (extension != NULL && i->extension != NULL)
	{
		if (strcmp(extension, i->extension) == 0)
		{
			retfunc = i->func;
			break;
		}
		i++;
	}
	
	return retfunc;
}



char *help[] =
{
		"RELIC - REtargetable Language Independent Compiler\n",
 		"Usage: relic {<opts>} <sourcefile>\n",
 		"Options:\n",
		"    -d         dry run (don't actually execute phases)\n",
		"    -xr        skip RELAX code optimization\n",
 		"    -q         quiet mode (don't show phases as they execute)\n",
 		"    -L=<path>  path to libraries\n",
 		"    -O=<path>  output directory\n",
		NULL
};



void showHelp(void)
{
	char **p = help;
	
	while (*p != NULL)
	{
		fprintf(stderr, *(p++));
	}
}


int main(int argc, char **argv)
{
	int i, filecount = 0;
	char *filearray[256];
	struct module_params p;

	p.skip_rxopt = 0;
	p.quiet = 0;
	p.dryrun = 0;
	p.keeptemporaryfiles = 1;
	strcpy(p.libdir, ".");
	strcpy(p.outputdir, ".");
	
 	sprintf(verstr, "v%d.%d", VERMAJOR, VERMINOR);
 	
 	if (argc == 1)
 	{
		showHelp();
 		exit(1);
 	}
 	
	/* process command line options */
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'x':
					if (argv[i][2] == 'r')
					{
						p.skip_rxopt = 1;
					}
					break;

				case 'q':
					p.quiet = 1;
					break;

				case 'd':
					p.dryrun = 1;
					break;

				case 'L':
					if (argv[i][2] == '=')
					{
						strcpy(p.libdir, &argv[i][3]);
					}
					break;

				case 'O':
					if (argv[i][2] == '=')
					{
						strcpy(p.outputdir, &argv[i][3]);
					}
					break;
			}
		}
		else
		{
			filearray[filecount++] = argv[i];
		}
	}
	
	
	/* execute phases */
	{
		strcpy(p.filename, filenameFromPathlist(filearray[0]));
		strcpy(p.infile, filearray[0]);

		extfuncptr f = whichfunc(filearray[0]);
		
		while (f != NULL)
		{
			int status;
			
			
			status = f(&p);
			if (status != 0)
			{
				exit(status);
			}

			f = whichfunc(p.outfile);
			
			strcpy(p.infile, p.outfile);
		}
	}

	return 0;
}
 

