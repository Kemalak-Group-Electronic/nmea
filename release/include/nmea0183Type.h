#ifndef NMEA0108TYPE_H
#define NMEA0108TYPE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef char nmea_cardinal_t;
#define NMEA_CARDINAL_DIR_NORTH   (nmea_cardinal_t)'N'
#define NMEA_CARDINAL_DIR_EAST    (nmea_cardinal_t)'E'
#define NMEA_CARDINAL_DIR_SOUTH   (nmea_cardinal_t)'S'
#define NMEA_CARDINAL_DIR_WEST    (nmea_cardinal_t)'W'
#define NMEA_CARDINAL_DIR_UNKNOWN (nmea_cardinal_t)'\0'

typedef enum
{
    NMEA_OK                   = 0,
    NMEA_ERR_INVALID_LENGTH   = 1,
    NMEA_ERR_INVALID_SENTENCE = 2,
    NMEA_ERR_INVALID_CR_LF    = 3,
    NMEA_ERR_INVALID_VALUE    = 4,
    NMEA_ERR_INVALID_PARSER   = 5,
    NMEA_ERR_INVALID_CHECKSUM = 6,
    NMEA_ERR_UNKNOWN_SENTENCE = 7,
    NMEA_ERR_GPGGA_INVALID    = 8,
    NMEA_ERR_GPRMC_INVALID    = 9,
    NMEA_ERR_PARSER_INVALID   = 10
} err_code_t;

typedef enum
{
    PARSER_UPDATED     = 0,
    PARSER_NOT_UPDATED = 1
} parser_status_t;

typedef enum
{
    NMEA_UNKNOWN = -1,
    NMEA_GPGGA   = 0,
    NMEA_GPRMC   = 1,
    NMEA_GNGGA   = 2,
    NMEA_GNRMC   = 3
} nmea_sentence_t;

/* NMEA cardinal direction types */

/* NMEA sentence max length, including \r\n (chars) */
#define NMEA_MAX_LENGTH 82
#define NMEA_MIN_LENGTH 9

/* NMEA sentence endings, should be \r\n according the NMEA 0183 standard */
#define NMEA_END_CHAR_1 '\r'
#define NMEA_END_CHAR_2 '\n'

/* NMEA sentence comma after the sentence ID, Ex: $GPGGA,xxxx,... */
#define NMEA_SENTENCEID_COMMA_INDEX 6

/* NMEA sentence prefix length (num chars), Ex: GPGLL */
#define NMEA_PREFIX_LENGTH 5

#define PARSER_MAX_LENGTH 4 // GPRMC, GPGGA, GNGGA, GNRMC

// Parser TYpe Start //
typedef struct
{
    nmea_sentence_t type;
    char            word[NMEA_PREFIX_LENGTH];
} nmea_sentence_s;

typedef char nmea_cardinal_t;
#define NMEA_CARDINAL_DIR_NORTH   (nmea_cardinal_t)'N'
#define NMEA_CARDINAL_DIR_EAST    (nmea_cardinal_t)'E'
#define NMEA_CARDINAL_DIR_SOUTH   (nmea_cardinal_t)'S'
#define NMEA_CARDINAL_DIR_WEST    (nmea_cardinal_t)'W'
#define NMEA_CARDINAL_DIR_UNKNOWN (nmea_cardinal_t)'\0'

typedef struct
{
    int             degrees;
    float           minutes;
    nmea_cardinal_t cardinal;
} nmea_position;

typedef struct
{
    float           degrees;
    nmea_cardinal_t cardinal;
} nmea_magvar;

typedef struct
{
    nmea_sentence_s sentence;
    int             errors;

    // int (*parse) (int);
} nmea_parser_module_s;

#endif // NMEA0108TYPE_H