// function from https://lxp32.github.io/docs/a-simple-example-crc32-calculation/

unsigned int crc32(unsigned char* s, size_t n) {
	unsigned int crc = 0xFFFFFFFF;

	for (size_t i = 0; i < n; i++) {
		char ch = s[i];
		for (int j = 0; j < 8; j++) {
			unsigned int b = (ch ^ crc) & 1;
			crc >>= 1;
			if (b) crc = crc ^ 0xEDB88320;
			ch >>= 1;
		}
	}
	return ~crc;
}