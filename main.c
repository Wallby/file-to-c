#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// "fputs format"..
// (error:|warning:) <description>


//int length(char* a)
int length(const char* a)
{
	return strlen(a);
}

void usage()
{
	char* str = "usage.. ./file-to-c <--help/file>\n"
		"parameters..\n"
		"* --help (show this text)\n";
	fputs(str, stdout);
}

int main(int argc, char** argv)
{
	if(argc == 1) //< NOTE: argv[0] == "./file-to-c"
	{
		usage();
	    return 0;
	}

	char* ARGUMENT = argv[1]; //< don't modify

	// TODO: replace w. arguments-mini..?
	if(strcmp(ARGUMENT, "--help") == 0)
	{
		usage();
		return 0;
	}
	else
	{
		int pathToInputfileLength = length(ARGUMENT);
		
		// TODO: if any extension.. replace extension w. .h instead..?
		int pathToOutputfileLength = pathToInputfileLength + 2;
		// pathToOutputfile == <ARGUMENT>.h
		//char* pathToOutputfile = new char[pathToOutputfileLength + 1];
		char* pathToOutputfile = malloc(pathToOutputfileLength);
		int a = 0;
		memcpy(pathToOutputfile, ARGUMENT, pathToInputfileLength);
		a += pathToInputfileLength;
		pathToOutputfile[a] = '.';
		++a;
		pathToOutputfile[a] = 'h';
		++a;
		pathToOutputfile[a] = '\0';

		FILE* b = fopen(ARGUMENT, "rb");
		if(b == NULL)
		{
			printf("error: failed to open file %s\n", ARGUMENT);
			return 1;
		}

		fseek(b, 0, SEEK_END);
		int inputfileLength = ftell(b);
		fseek(b, 0, SEEK_SET);

		struct
		{
			struct
			{
				FILE* a;
			} file;
		} outputfile;
		
		outputfile.file.a = fopen(pathToOutputfile, "w");

		//char* inputfile = new char[inputfileLength];
		char* inputfile = malloc(inputfileLength);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
		fread((void*)inputfile, 1, inputfileLength, b);
#pragma GCC diagnostic pop

		fclose(b);

		//#ifndef <to upper ARGUMENT>_H
		fwrite("#ifndef ", 1, 8, outputfile.file.a);
		for(int i = 0; i < pathToInputfileLength; ++i)
		{
			char c;
			if(ARGUMENT[i] == '.')
			{
				c = '_';
			}
			else
			{
				c = toupper(ARGUMENT[i]);
			}
			fwrite(&c, 1, 1, outputfile.file.a);
		}
		fwrite("_H\n", 1, 3, outputfile.file.a);
		//#define <to upper ARGUMENT>_H
		fwrite("#define ", 1, 8, outputfile.file.a);
		for(int i = 0; i < pathToInputfileLength; ++i)
		{
			char c;
			if(ARGUMENT[i] == '.')
			{
				c = '_';
			}
			else
			{
				c = toupper(ARGUMENT[i]);
			}
			fwrite(&c, 1, 1, outputfile.file.a);
		}
		fwrite("_H\n\n\n", 1, 5, outputfile.file.a);

		fwrite("char ", 1, 5, outputfile.file.a);
		for(int i = 0; i < pathToInputfileLength; ++i)
		{
			char c;
			if(ARGUMENT[i] == '.')
			{
				c = '_';
			}
			else
			{
				c = ARGUMENT[i];
			}
			fwrite(&c, 1, 1, outputfile.file.a);
		}
		fwrite("[] = {", 1, 6, outputfile.file.a);

		for(int i = 0; i < inputfileLength; ++i)
		{
			if(i > 0)
			{
				fwrite(",",1,1,outputfile.file.a);
			}

			char c[3+1]; //< max 255 <- 3 characters
			sprintf(c, "%hhu", inputfile[i]);

			fwrite(c, 1, length(c), outputfile.file.a);
		}

		fwrite("};\n\n", 1, 4, outputfile.file.a);

		fwrite("#endif\n", 1, 7, outputfile.file.a);

		fclose(outputfile.file.a);

		//delete pathToOutputfile;
		free(pathToOutputfile);
		//delete inputfile;
		free(inputfile);

		return 0;
	}

	return 1;
}