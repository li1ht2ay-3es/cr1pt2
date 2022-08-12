#define memblock_size 0x100000


unsigned char memblock[memblock_size];


FILE *fp_data;


#define MSB32(x) \
( \
	(memblock[x+0]<<24) | \
	(memblock[x+1]<<16) | \
	(memblock[x+2]<<8 ) | \
	(memblock[x+3]<<0 ) \
)


void memcache(int offset, int size)
{
	fseek(fp_data, offset, SEEK_SET);
	fread(memblock, 1, size, fp_data);
}


void memwrite(int offset, int size)
{
	fseek(fp_data, offset, SEEK_SET);
	fwrite(memblock, 1, size, fp_data);
}


void memcrypt(int ptr, int size, int type)
{
	int lcv;

	memcache(ptr, size);

	for (lcv = 0; lcv < size; lcv++) {
		memblock[lcv] ^= cryptkey_get(type, lcv);
	}

	memwrite(ptr, size);
}


void memtest(int ptr, int size, int type)
{
	int lcv;

	for (lcv = 0; lcv < size; lcv++) {
		memblock[lcv] ^= cryptkey_get(type, lcv);
	}
}
