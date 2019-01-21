#ifndef __BASE64_H_
#define __BASE64_H_
#include <Windows.h>
#ifdef __cplusplus
extern "C" {
#endif

int base64_encode(const unsigned char *in, unsigned long srcSize, unsigned char *out);

int base64_decode(const unsigned char *in, unsigned char *out);

BOOL isbase64str(unsigned char* str, unsigned int len);

BOOL is_base64(unsigned char c);


#ifdef __cplusplus
}
#endif

#endif


