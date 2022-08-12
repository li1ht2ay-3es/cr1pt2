int mp4_ftyp, mp4_moov, mp4_mdat, mp4_free;


int detect_mp4(int decrypt)
{
	int ptr, blocksize;


	memcache(0, 0x10);
	ptr = MSB32(0);
	if (strncmp(memblock+4, "ftyp", 4) != 0) {
		return 0;
	}
	mp4_ftyp = 0;


	while (mp4_moov == -1 || mp4_mdat == -1)
	{
		memcache(ptr, 0x10);
		blocksize = MSB32(0);

		if (blocksize == 1) {
			printf("[mp4] int64 size unhandled\n");
			_sleep(10000);

			exit(1);
		}

		else
		if (strncmp(memblock+4, "moov", 4) == 0) {
			mp4_moov = ptr;
		}

		else
		if (strncmp(memblock+4, "mdat", 4) == 0) {
			mp4_mdat = ptr;
		}

		else
		if (strncmp(memblock+4, "free", 4) == 0) {
			mp4_free = ptr;
		}

		else
		if (decrypt == 1) {
			return 0;
		}

		else {
			printf("[mp4] unknown block type\n");
			_sleep(10000);

			exit(1);
		}

		ptr += blocksize;
	}


	return 1;
}


int encrypt_mp4()
{
	mp4_ftyp = mp4_moov = mp4_mdat = mp4_free = -1;

	if (detect_mp4(0) == 0) return 0;


	memcrypt(mp4_ftyp, mp4_cryptsize, mp4_cryptkey1);

	if (mp4_free >= mp4_cryptsize) {
		memcrypt(mp4_free, mp4_cryptsize, mp4_cryptkey2);
	}

	else
	if (mp4_moov >= mp4_cryptsize) {
		memcrypt(mp4_moov, mp4_cryptsize, mp4_cryptkey2);
	}

	else
	if (mp4_mdat >= mp4_cryptsize) {
		memcrypt(mp4_mdat, mp4_cryptsize, mp4_cryptkey2);
	}


	printf("mp4 encrypted = %X %X\n", mp4_moov, mp4_mdat);
	return 1;
}


int decrypt_mp4()
{
	int ptr;
	mp4_ftyp = mp4_moov = mp4_mdat = mp4_free = -1;


	if (detect_mp4(1) == 1) return 1;


	memcache(0, 0x10);
	memtest(0, 0x10, mp4_cryptkey1);

	if (strncmp(memblock+4, "ftyp", 4) != 0) {
		return 0;
	}


	memcrypt(0, mp4_cryptsize, mp4_cryptkey1);
	memcache(0, mp4_cryptsize);
	ptr = MSB32(0);


	if (strncmp(memblock+ptr+4, "mdat", 4) != 0 &&
		strncmp(memblock+ptr+4, "moov", 4) != 0) {
		return 0;
	}


	ptr += MSB32(ptr);
	if (ptr >= mp4_cryptsize) {
		memcrypt(ptr, mp4_cryptsize, mp4_cryptkey2);
	}


	if (detect_mp4(1) == 0) {
		printf("mp4 corrupt = %X %X\n", mp4_moov, mp4_mdat);
		exit(-1);
	}


	printf("mp4 decrypted = %X %X\n", mp4_moov, mp4_mdat);
	return 1;
}
