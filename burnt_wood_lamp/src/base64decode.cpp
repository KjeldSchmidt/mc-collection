#include "base64.h"

static const String base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(uint8_t c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

size_t base64_decode(String const& encoded_string, uint8_t* output_buffer) {
    int input_length = encoded_string.length();
    int i = 0;
    int j = 0;
    size_t output_buffer_write_index = 0;
    uint8_t encoded_bytes_buffer[4];
    uint8_t decoded_bytes_buffer[3];
    uint8_t * result;

    size_t index = 0;
    while (input_length-- && ( encoded_string[index] != '=') && is_base64(encoded_string[index])) {
        encoded_bytes_buffer[i++] = encoded_string[index];
        index++;
        if (i == 4) {
            for (i = 0; i <4; i++) {
                encoded_bytes_buffer[i] = base64_chars.indexOf(encoded_bytes_buffer[i]);
            }

            decoded_bytes_buffer[0] = (encoded_bytes_buffer[0] << 2) + ((encoded_bytes_buffer[1] & 0x30) >> 4);
            decoded_bytes_buffer[1] = ((encoded_bytes_buffer[1] & 0xf) << 4) + ((encoded_bytes_buffer[2] & 0x3c) >> 2);
            decoded_bytes_buffer[2] = ((encoded_bytes_buffer[2] & 0x3) << 6) + encoded_bytes_buffer[3];

            for (i = 0; (i < 3); i++) {
                output_buffer[output_buffer_write_index] = decoded_bytes_buffer[i];
                output_buffer_write_index++;
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j<4; j++) {
            encoded_bytes_buffer[j] = 0;
        }

        for (j = 0; j<4; j++) {
            encoded_bytes_buffer[j] = base64_chars.indexOf(encoded_bytes_buffer[j]);
        }

        decoded_bytes_buffer[0] = (encoded_bytes_buffer[0] << 2) + ((encoded_bytes_buffer[1] & 0x30) >> 4);
        decoded_bytes_buffer[1] = ((encoded_bytes_buffer[1] & 0xf) << 4) + ((encoded_bytes_buffer[2] & 0x3c) >> 2);
        decoded_bytes_buffer[2] = ((encoded_bytes_buffer[2] & 0x3) << 6) + encoded_bytes_buffer[3];

        for (j = 0; (j < i - 1); j++) {
            output_buffer[output_buffer_write_index] = decoded_bytes_buffer[i];
            output_buffer_write_index++;
        }
  }

  return output_buffer_write_index;
}