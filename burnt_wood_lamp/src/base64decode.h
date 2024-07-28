#ifndef _BASE64_H_
#define _BASE64_H_

typedef unsigned char BYTE;

size_t base64_decode(String const&, uint8_t* output_buffer);

#endif
