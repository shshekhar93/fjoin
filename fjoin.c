#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "fjoin.h"

void help()
{
	fprintf(stdout,"Usage: fjoin [option] <filename>\n");
	fprintf(stdout,"No specific option is required for joning of file. The filename, however, should be of form *.001\n");
	fprintf(stdout,"Options can be\n");
	fprintf(stdout,"  -split <N>\t To split the file. N is size, eg 200mb\n");
	fprintf(stdout,"  --version\t Prints version of this software.\n");
	fprintf(stdout,"  --help\t Prints this help.\n");
	fprintf(stdout,"\nFor queries and bug reports contact shashi20008@gmail.com\n");
}

void version()
{
	fprintf(stdout, "fjoin version 0.0.5\n");
}

int main(int argc, char *argv[])
{
	unsigned long long ofsize, factr, spval;
	char spfactr[3];
	int i;
	if(argc<2 || strcmp(argv[1],"--help")==0)
	{
		help();
		exit(1);
	}
	else if(strcmp(argv[1],"--version")==0)
	{
		version();
		exit(0);
	}
	else if(argc==2)
	{
		join(argv[1]);
		exit(1);
	}
	else if(argc==4 && strcmp(argv[1],"-split")==0)
	{
		/* Copy the last two charaters into some other character array
		 */
		strcpy(spfactr, (argv[2]+strlen(argv[2])-2));
		
		/* Generate number from the string form in argument.
		 */
		i=0;
		spval = 0;
		while(argv[2][i])
		{
			if(argv[2][i]<'0' || argv[2][i]>'9')
				break;
			spval = (10 * spval) + (argv[2][i] - '0');
			i++;
		}
		
		if(i != strlen(argv[2])-2)
		{
			fprintf(stderr, "Invalid split size.\n");
		}
		
		/* Check the factor.
		 */	
		for(i=0; i<2; i++)
			spfactr[i] = toupper(spfactr[i]);
		
		if(strcmp(spfactr, "KB")==0)
			factr = 1024;
		else if(strcmp(spfactr, "MB")==0)
			factr = 1024 * 1024;
		else if(strcmp(spfactr, "GB")==0)
			factr = 1024 * 1024 * 1024;
		else
		{
			fprintf(stdout, "Invalid split size.\n");
		}
		
		/*calcuate the size of each file.
		 */
		ofsize=0;
		ofsize = spval * factr;
			
		/****************temp********************/
			fprintf(stdout, "factr = %lld, spval = %lld, ofsize = %lld\n",factr, spval, ofsize);
		/****************************************/
		
		/* Call the function to split the file.
		 */
		split(argv[3], ofsize);
	}
	return 0;
}
