#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

#define _sleep Sleep
#else
#include <unistd.h>

#define _sleep usleep
#endif


#include "crypt-key.h"
#include "crypt-file.h"
#include "crypt-mp4.h"


int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("encrypt <file_name> -- or drag-and-drop\n");
		_sleep(5000);

		return -1;
	}


	cryptkey_load(argv[0]);


	do
	{
		unsigned size, block, lcv;


		argc--;
		fp_data = fopen(argv[argc], "rb+");

		if (!fp_data) {
			printf("error: can't open %s\n", argv[argc]);
			_sleep(5000);

			return -1;
		}


		printf("[%s]: ", argv[argc]);


		if (encrypt_mp4()) { fclose(fp_data); continue; }


		printf("unknown format\n");
		_sleep(7000);
		exit(1);
	} while(argc > 1);


	return 0;
}
