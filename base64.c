#include "base64.h"

static const char *codes = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char map[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,
	255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255 };

int base64_encode(const unsigned char *in,  unsigned long len, 
	unsigned char *out)
{
	unsigned long i, leven;
	unsigned char *p;

	p = out;
	leven = 3*(len / 3);
	for (i = 0; i < leven; i += 3) 
	{
		*p++ = codes[in[0] >> 2];
		*p++ = codes[((in[0] & 3) << 4) + (in[1] >> 4)];
		*p++ = codes[((in[1] & 0xf) << 2) + (in[2] >> 6)];
		*p++ = codes[in[2] & 0x3f];
		in += 3;
	}
	/* Pad it if necessary...  */
	if (i < len) 
	{
		unsigned a = in[0];
		unsigned b = (i+1 < len) ? in[1] : 0;
		unsigned c = 0;

		*p++ = codes[a >> 2];
		*p++ = codes[((a & 3) << 4) + (b >> 4)];
		*p++ = (i+1 < len) ? codes[((b & 0xf) << 2) + (c >> 6)] : '=';
		*p++ = '=';
	}

	/* append a NULL byte */
	*p = '\0';

	return p - out;
}

int base64_decode(const unsigned char *in, unsigned char *out)
{
	unsigned long t, x, y, z;
	unsigned char c;
	int	g = 3;

	for (x = y = z = t = 0; in[x]!=0;) 
	{
		c = map[in[x++]];
		if (c == 255) return -1;
		if (c == 253) continue;
		if (c == 254) { c = 0; g--; }
		t = (t<<6)|c;
		if (++y == 4) 
		{
			//			if (z + g > *outlen) { return CRYPT_BUFFER_OVERFLOW; }
			out[z++] = (unsigned char)((t>>16)&255);
			if (g > 1) out[z++] = (unsigned char)((t>>8)&255);
			if (g > 2) out[z++] = (unsigned char)(t&255);
			y = t = 0;
		}
	}
	//	if (y != 0) {
	//		return -1;
	//	}
	return z;
}

int NumOfHex(unsigned char b)
{
	if(b>='0' && b<='9') 
		return b-'0';
	else if(b>='A' && b<='F')
		return b-'A'+10;
	else
		return -1;
}

int hex_decode(unsigned char *in, unsigned char *out)
{
	unsigned char *p = out;
	while (1)
	{
		int b = *in++;
		int b1 = *in++;
		if (b == 0) return p - out;
		if (b1 == 0) return -2;
		b = NumOfHex(b);
		if (b < 0) return -1;
		b1 = NumOfHex(b1);
		if (b1 < 0) return -1;
		*p++ = b * 16 + b1;
	}
	return 0;
}


BOOL isbase64str(unsigned char* str, unsigned int len)
{
	unsigned int i = 0;
	if (len % 4 != 0)
	{
		return FALSE;
	}
	for (i=0;i<len;i++)
	{
		if (!is_base64(str[i]) && str[i] != '=')
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL is_base64(unsigned char c) 
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}