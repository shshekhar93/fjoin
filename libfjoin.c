#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define _FILE_OFFSET_BITS 64

void increment(char fname[])
{
	int i;
	i = strlen(fname)-1;
	fname[i]++;
	while(fname[i]>'9' && fname[i-1]!='.')
	{
		fname[i-1]++;
		fname[i]-=10;
		i--;
	}
	if(fname[i-1]=='.' && fname[i]>'9')
	{
		fname[0] = '\0'; //to explixitly create a file opening error.
	}
}

extern int split(char fname[], unsigned long long size)
{
	char *ofname;
	char buffer[1024];
	int rsize, wsize;
	unsigned long long totl;
	FILE *fip, *fop;
	
	/* Try to open the input file,
	   throw an error if it doesn't exist.
	 */
	fip = fopen(fname, "rb");
	if(fip == NULL)
	{
		fprintf(stderr, "The file %s was not found.\n",fname);
		return 1;
	}
	
	/* Create output filename
	 */
	ofname = (char*) malloc(sizeof(char) * (strlen(fname)+5));
	strcpy(ofname, fname);
	strcat(ofname, ".001");
	
	/* Try to open the file for output,
	   throw an error in case it couldn't be created.
	 */
	fop = fopen(ofname, "wb");
	if(fop == NULL)
	{
		fprintf(stderr, "Output file couldn\'t be created. \
					Ensure write permissions to the disk\n");
		return 3;
	}
	
	
	fseek(fip, 0, SEEK_SET);
	while(1)
	{
		totl=0;
		fseek(fop, 0, SEEK_SET);
		while(totl<size)
		{
			rsize = fread(buffer, 1, 1024, fip);
			wsize = fwrite(buffer, 1, rsize, fop);
	 		
			if(wsize!=rsize)
			{
				fprintf(stderr, "an write error occured.\n");
			}
			/* Include code later to check how many bytes
			   were actually written, just in case an error
			   occurs.
			   For now it can be ignored as write errors
			   are not that common, and are generally handled
			   by OS itself.
			 */
		
			/* Increment the totl value.
			 */ 
			totl = totl + rsize;
		
			/* If the input file (to be splitted), has reached
			   its end, break off.
			 */
			if(feof(fip))
				break;
		}
	 	
		/* If the input file has reached the end, simply break off.
		 */
		if(feof(fip))
			break;
	
		/* Generate the new filename for output.
		 */
		increment(ofname);
		
		/* Open the new output file.
	 	   Throw error if it cannot be opened for some reason.
	 	 */
	 	fop = fopen(ofname, "wb");
	 	if(fop == NULL)
	 	{
	 		fprintf(stderr, "Output file couldn\'t be created. \
	 				Ensure write permissions to the disk\n");
	 		return 3;
	 	}
	}
	
	/* Display a success message.
	 */
	fprintf(stdout, "The file %s was successfully split\n", fname);
	return 0;
}

extern int join(char fname[])
{
	int i;					// for various loops.
	int rsize, wsize;		// size read in buffer.
	char buffer[1024];		// to read data. 
	char *ofile;			// to keep output fillename temprorarily.
	FILE *fip;				// input file pointer.
	FILE *fop;				//output file pointer.
	
	/* Open input file.
	   Fail if file not present.
	 */
	fip = fopen(fname, "rb");
	if(fip == NULL)
	{
		fprintf(stderr, "The file %s was not found.\n",fname);
		return 1;
	}
	
	/* Create output filename.
	 */
	ofile = (char*) malloc(sizeof(char)*(strlen(fname)+1));
	strcpy(ofile,fname);
	for(i=strlen(ofile)-1; i>=0; i--)
	{
		if(ofile[i] == '.')
		{
			ofile[i]='\0';
			break;
		}
			
		if(ofile[i]<'0' || ofile[i]>'9')
			break;
	}
	
	/* Check if output filename was created successfully.
	 */
	if(strcmp(fname,ofile)==0)
	{
		fprintf(stderr, "Malformed filename %s.\n", fname);
		return 2;
	}
	
	/* Open output file. Exit if fail.
	 */
	fop = fopen(ofile, "wb");
	if(fop == NULL)
	{
		fprintf(stderr, "Output file could not be created. Ensure you have write permissions.\n");
		return 3;
	}
	
	/* Start joining the file.
	 */
	
	while(1)
	{
		fseek(fip, 0, SEEK_SET);
		while(!feof(fip))
		{
			rsize = fread(buffer, 1, 1023, fip);
			wsize = fwrite(buffer, 1, rsize, fop);
			if(wsize!=rsize)
			{
				fprintf(stderr, "an write error occured.\n");
			}
			/* Include code later to check how many bytes
			   were actually written, just in case an error
			   occurs.
			   For now it can be ignored as write errors
			   are not that common.
			 */
		}
		fclose(fip);
		increment(fname);
		fip = fopen(fname, "rb");
		if(fip == NULL)
		{
			fprintf(stdout, "Joining completed. Saved to %s.\n",ofile);
			fclose(fop);
			break;
		}
		
	}
	
	
	return 0;
}
