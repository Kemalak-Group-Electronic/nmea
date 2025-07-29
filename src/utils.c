#include "utils.h"

// ----- GENERAL UTILITY FUNCTIONS -----

uint16_t
utils_splitStrByComma (char *string, char **values, int max_values)
{
    uint16_t i = 0;

    values[i++] = string;
    while (i < max_values && NULL != (string = strchr(string, ',')))
    {
        *string     = '\0';
        values[i++] = ++string;
    }

    return i;
}

bool
utils_isCharExist (const char *string, const char character)
{
    char *pchar = NULL;
    pchar       = strchr(string, character);

    if (pchar == NULL)
    {
        return false;
    }

    if (*pchar != character)
    {
        return false;
    }

    return true;
}

char *
utils_crop_string (char *string, size_t offset, size_t last)
{
    static char sentence[NMEA_MAX_LENGTH];
    /* Null terminate before end of line/sentence, 2 characters to Remove
     * checksum */

    strncpy(sentence, string + offset, last);
    sentence[last + 1] = '\0';

    return sentence;
}

// --- END GENERAL UTILITY FUNCTIONS ----

// --------- BEARING FUNCTIONS ----------
void
utils_brg_charToPosition (const char *nmea_pos, int *degrees, float *minutes)
{
    float value = atof(nmea_pos);
    *degrees    = (int)(value / 100);
    *minutes    = value - (*degrees * 100);
}

// ------- END BEARING FUNCTIONS --------

// --------- CHECKSUM FUNCTIONS ---------

uint8_t
utils_nmea_chksum_getFromStr (const char *string)
{
    char checksum[3];
    int  length;

    length      = strlen(string);
    checksum[0] = string[length - 4];
    checksum[1] = string[length - 3];
    checksum[2] = '\0';

    return (uint8_t)strtol(checksum, NULL, 16);
}

uint8_t
utils_nmea_chksum_calculate (const char *string)
{
    const char *n   = string + 1;
    uint8_t     chk = 0;

    /* While current char isn't '*' or string ending (newline) */
    while ('*' != *n && NMEA_END_CHAR_1 != *n && '\0' != *n)
    {
        chk ^= (uint8_t)*n;
        n++;
    }

    return chk;
}

//--------- END CHECKSUM FUNCTION --------