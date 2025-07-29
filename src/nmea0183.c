#include "nmea0183.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nmeaParserStatic.h"
#include "utils.h"

void
nmea0183_init (nmea_gpgga_s *pgpgga, nmea_gprmc_s *pgprmc)
{
    nmea_init(pgpgga, pgprmc);
}

int
nmea0183_parse (char *sentence, size_t length, bool chksum_enable)
{
    unsigned int n_vals, val_index;
    char        *value, *val_string;
    char        *values[255];

    nmea_parser_module_s *parser;
    nmea_sentence_t       type;

    err_code_t err = nmea_validate(sentence, length, chksum_enable);

    if (err != NMEA_OK)
    {
        return err;
    }

    type = nmea_get_type(sentence);
    if (NMEA_UNKNOWN == type)
    {
        return NMEA_ERR_UNKNOWN_SENTENCE;
    }

    val_string = utils_crop_string(sentence, 1, length - 6);
    if (NULL == val_string)
    {
        return NMEA_ERR_INVALID_SENTENCE;
    }

    n_vals = utils_splitStrByComma(val_string, values, ARRAY_LENGTH(values));
    if (0 == n_vals)
    {
        return NMEA_ERR_INVALID_VALUE;
    }

    nmea_allocater(values, type);

    return NMEA_OK;
}

err_code_t
nmea0183_check_gpgga (nmea_gpgga_s *p_gpgga)
{
    nmea_check_gpgga(p_gpgga);
}

parser_status_t
nmea0183_gpggaIsUpdated (void)
{
    return nmea_gpggaIsUpdated();
}

err_code_t
nmea0183_get_gpgga (nmea_gpgga_s *p_gpgga)
{
    nmea_get_gpgga(p_gpgga);
}

err_code_t
nmea0183_check_gprmc (nmea_gprmc_s *p_gprmc)
{
    nmea_check_gprmc(p_gprmc);
}

parser_status_t
nmea0183_gprmcIsUpdated (void)
{
    return nmea_gprmcIsUpdated();
}

err_code_t
nmea0183_get_gprmc (nmea_gprmc_s *p_gprmc)
{
    nmea_get_gprmc(p_gprmc);
}