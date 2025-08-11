#ifndef UTILS_H
#define UTILS_H

#include "nmea0183Type.h"

// ----- GENERAL UTILITY FUNCTIONS -----
#define ARRAY_LENGTH(a) (sizeof a / sizeof (a[0]))

uint16_t utils_splitStrByComma(char *string, char **values, int max_values);
bool utils_isCharExist(const char *string, const char character);
char * utils_crop_string(char *sentence,size_t offset, size_t last);

// --- END GENERAL UTILITY FUNCTIONS ----


// --------- BEARING FUNCTIONS ----------
void utils_brg_charToPosition(const char* nmea_pos, int* degrees, float* minutes);

// ------- END BEARING FUNCTIONS --------


// --------- CHECKSUM FUNCTIONS ---------
uint8_t utils_nmea_chksum_getFromStr(const char *string);
uint8_t utils_nmea_chksum_calculate(const char *string);

//--------- END CHECKSUM FUNCTION --------


#endif // UTILS_H