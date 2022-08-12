#define cryptkey_size 0x10000

unsigned char cryptkey[cryptkey_size];

int cryptkey_ptr = 0;


/*
Type
0	0x00000		mp4 block1  [ftyp]
1	0x00800		mp4 block2	[mdat or moov]
*/

#define mp4_cryptkey1	0
#define mp4_cryptkey2	1
#define mp4_cryptsize	0x800


unsigned char cryptkey_get(int type, int ptr)
{
	int base, size;

	switch (type) {
		case mp4_cryptkey1: base = 0x0; size = mp4_cryptsize; break;
		case mp4_cryptkey2: base = 0x800; size = mp4_cryptsize; break;
		default: base = 0, size = cryptkey_size; break;
	}
	
	return cryptkey[(ptr % size) + base];
}


void cryptkey_load(char *path)
{
	FILE *fp;
	static char str[512], base[512];


	strcpy(base, path);
	if (strrchr(base, '\\')) {
		*strrchr(base, '\\') = 0;
	}
	else
	if (strrchr(base, '/')) {
		*strrchr(base, '/') = 0;
	}


	sprintf(str, "%s\\private_key.bin", base);
	fp = fopen(str, "rb");
	if (!fp) {
		sprintf(str, "%s\\crypt-key.bin", base);
		fp = fopen(str, "rb");
		if (!fp) {
			printf("crypt: can't load keys\n");
			_sleep(5000);

			exit(-1);
		}
	}

	fread(cryptkey, 1, cryptkey_size, fp);

	fclose(fp);
}
