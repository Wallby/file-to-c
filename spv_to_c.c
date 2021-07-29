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
	char* str = "usage..\n \
		./spv-to-c <argument|file location>\n \
		arguments..\n \
		* help (show this text)\n";
	fputs(str, stdout);
}

int main(int argc, char** argv)
{
	if(argc == 1) //< NOTE: argv[0] == "./spv-to-c"
	{
		usage();
	    return 0;
	}

	char* ARGUMENT = argv[1]; //< don't modify

	if(strcmp(ARGUMENT, "help") == 0)
	{
		usage();
		return 0;
	}
	else
	{
		int b = length(ARGUMENT);
		char* d;
		if(ARGUMENT[b - 1] == 'h' && ARGUMENT[b - 2] == '.')
		{
			int j = b - 2;
			// filename == <ARGUMENT[0:c]>.h

			//d = new char[f + 2 + 1];
			d = malloc(j + 2 + 1);
			int h = 0;
			memcpy(d, ARGUMENT, j);
			h += j;
			d[h] = '.';
			++h;
			d[h] = 'h';
			++h;
			d[h] = '\0';
		}
		else
		{
			// filename == <ARGUMENT>.h
			//d = new char[b + 2 + 1];
			d = malloc(b + 2 + 1);
			int h = 0;
			memcpy(d, ARGUMENT, b);
			h += b;
			d[h] = '.';
			++h;
			d[h] = 'h';
			++h;
			d[h] = '\0';
		}

		FILE* c = fopen(d, "r");
		if(c != NULL)
		{
			printf("error: a file named %s already exists\n", d);
			return 1;
		}

		FILE* a = fopen(ARGUMENT, "rb");
		if(a == NULL)
		{
			printf("error: failed to open file %s\n", ARGUMENT);
			return 1;
		}

		fseek(a, 0, SEEK_END);
		int f = ftell(a);
		fseek(a, 0, SEEK_SET);

		if(f % 4 != 0)
		{
			fputs("error: invalid spirv (spirv bytecode is always 4-byte alligned)\n", stdout);
			return 1;
		}

		c = fopen(d, "w");

		//char* e = new char[f];
		char* e = malloc(f);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
		fread((void*)e, 1, f, a);
#pragma GCC diagnostic pop

		fclose(a);

		int g = length(d) - 2;

		//#ifndef <to upper ARGUMENT>_H
		fwrite("#ifndef ", 1, 8, c);
		for(int i = 0; i < g; ++i)
		{
			char h;
			if(ARGUMENT[i] == '.')
			{
				h = '_';
			}
			else
			{
				h = toupper(ARGUMENT[i]);
			}
			fwrite(&h, 1, 1, c);
		}
		fwrite("_H\n", 1, 3, c);
		//#define <to upper ARGUMENT>_H
		fwrite("#define ", 1, 8, c);
		for(int i = 0; i < g; ++i)
		{
			char h;
			if(ARGUMENT[i] == '.')
			{
				h = '_';
			}
			else
			{
				h = toupper(ARGUMENT[i]);
			}
			fwrite(&h, 1, 1, c);
		}
		fwrite("_H\n\n\n", 1, 5, c);

		// IDEA: perhaps change this tool/make a new tool bin-to-c (i.e. which outputs an array of unsigned chars)
		fwrite("unsigned int ", 1, 13, c);
		for(int i = 0; i < g; ++i)
		{
			if(d[i] == '.')
			{
				char h = '_';
				fwrite(&h, 1, 1, c);
			}
			else
			{
				fwrite(&d[i], 1, 1, c);
			}
		}
		fwrite("[] = {", 1, 6, c);

		for(int i = 0; i < f/4; ++i)
		{
			if(i > 0)
			{
				fwrite(",",1,1,c);
			}

			// NOTE: ffffffff == 255
			char h[2+8+1];
			unsigned int k = 0;
			memcpy(&k, &e[i*4], sizeof(unsigned int));
			sprintf(h, "0x%x", k);

			fwrite(h, 1, length(h), c);
		}

		fwrite("};\n\n", 1, 4, c);

		fwrite("#endif\n", 1, 7, c);

		fclose(c);

		//delete d;
		free(d);
		//delete e;
		free(e);

		return 0;
	}

	return 1;
}
