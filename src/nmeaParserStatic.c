#include "nmeaParserStatic.h"
#include "utils.h"

nmea_parser_module_s parsers[PARSER_MAX_LENGTH];

nmea_gprmc_s gprmc;
nmea_gpgga_s gpgga;

// ----------- Static function declarations ------------

// ---------- End Static function declarations ------------

nmea_sentence_t
nmea_get_type (const char *sentence)
{
    nmea_parser_module_s *parser;
    nmea_sentence_t       sentence_type = NMEA_UNKNOWN;

    if (NULL == sentence)
    {
        return NMEA_UNKNOWN;
    }

    for (uint8_t i = 0; i < PARSER_MAX_LENGTH; i++)
    {
        parser = &parsers[i];
        if (0
            == strncmp(sentence + 1, parser->sentence.word, NMEA_PREFIX_LENGTH))
        {
            sentence_type = parser->sentence.type;
            break;
        }
    }

    return sentence_type;
}

void
nmea_init (nmea_gpgga_s *pgpgga, nmea_gprmc_s *pgprmc)
{
    nmea_gpgga_init(&gpgga);
    nmea_gpgga_init(pgpgga);
    parsers[0].sentence.type = NMEA_GPGGA;
    strcpy(parsers[0].sentence.word, "GPGGA");

    nmea_gprmc_init(&gprmc);
    nmea_gprmc_init(pgprmc);
    parsers[1].sentence.type = NMEA_GPRMC;
    strcpy(parsers[1].sentence.word, "GPRMC");
}

void
nmea_gpgga_init (nmea_gpgga_s *pgpgga)
{
    if (pgpgga == NULL)
    {
        return;
    }

    pgpgga->sentence.type = NMEA_GPGGA;
    strcpy(pgpgga->sentence.word, "GPGGA");

    pgpgga->time.tm_hour = 0;
    pgpgga->time.tm_min  = 0;
    pgpgga->time.tm_sec  = 0;

    pgpgga->latitude.degrees  = 0;
    pgpgga->latitude.minutes  = 0.0;
    pgpgga->latitude.cardinal = NMEA_CARDINAL_DIR_UNKNOWN;

    pgpgga->longitude.degrees  = 0;
    pgpgga->longitude.minutes  = 0.0;
    pgpgga->longitude.cardinal = NMEA_CARDINAL_DIR_UNKNOWN;

    pgpgga->fix_type = 0;   // Default to invalid fix
    pgpgga->num_sats = 0;   // Default to no satellites in view
    pgpgga->hdop     = 0.0; // Default horizontal dilution of precision

    pgpgga->altitude      = 0.0; // Default altitude
    pgpgga->altitude_unit = 'M'; // Default altitude unit to meters

    pgpgga->undulation      = 0.0; // Default geoidal separation
    pgpgga->undulation_unit = 'M'; // Default undulation unit to meters

    pgpgga->diff_age = 0.0; // Default age of differential GPS

    pgpgga->diff_station_id[0]
        = '0'; // Default differential station ID to empty
    pgpgga->diff_station_id[1]
        = '0'; // Default differential station ID to empty
    pgpgga->diff_station_id[2]
        = '0'; // Default differential station ID to empty
    pgpgga->diff_station_id[3]
        = '0'; // Default differential station ID to empty
    pgpgga->diff_station_id[4]
        = '0'; // Default differential station ID to empty
    pgpgga->diff_station_id[5] = '\0'; // Ensure null termination
}

void
nmea_gprmc_init (nmea_gprmc_s *pgprmc)
{
    if (pgprmc == NULL)
    {
        return;
    }

    pgprmc->sentence.type = NMEA_GPRMC;
    strcpy(pgprmc->sentence.word, "GPRMC");

    pgprmc->time.tm_hour = 0;
    pgprmc->time.tm_min  = 0;
    pgprmc->time.tm_sec  = 0;

    pgprmc->status = 'A'; // Default status to 'A' (active)

    pgprmc->latitude.degrees  = 0;
    pgprmc->latitude.minutes  = 0.0;
    pgprmc->latitude.cardinal = NMEA_CARDINAL_DIR_UNKNOWN;

    pgprmc->longitude.degrees  = 0;
    pgprmc->longitude.minutes  = 0.0;
    pgprmc->longitude.cardinal = NMEA_CARDINAL_DIR_UNKNOWN;

    pgprmc->speed_knots = 0.0;
    pgprmc->heading_deg = 0.0;

    pgprmc->date.tm_mday = 1;  // Default to the first day of the month
    pgprmc->date.tm_mon  = 0;  // Default to January
    pgprmc->date.tm_year = 70; // Default to year 1970

    pgprmc->magvar.degrees  = 0.0;
    pgprmc->magvar.cardinal = NMEA_CARDINAL_DIR_UNKNOWN;
}

err_code_t
nmea_validate (const char *string, size_t length, bool chksum_enable)
{
    /* should have atleast 9 characters */
    if (NMEA_MIN_LENGTH > length)
    {
        return NMEA_ERR_INVALID_LENGTH;
    }

    /* should be less or equal to 82 characters */
    if (NMEA_MAX_LENGTH < length)
    {
        return NMEA_ERR_INVALID_LENGTH;
    }

    /* should start with $ */
    if ('$' != *string)
    {
        return NMEA_ERR_INVALID_SENTENCE;
    }

    /* should end with \r\n, or other... */
    if (NMEA_END_CHAR_2 != string[length - 1]
        || NMEA_END_CHAR_1 != string[length - 2])
    {
        return NMEA_ERR_INVALID_CR_LF;
    }

    /* should have a 5 letter, uppercase word */

    for (int i = 1; i < 6; i++)
    {
        if (string[i] < 'A' || string[i] > 'Z')
        {
            /* not uppercase letter */
            return NMEA_ERR_INVALID_SENTENCE;
        }
    }

    /* should have a comma after the type word */
    if (',' != string[NMEA_SENTENCEID_COMMA_INDEX])
    {
        return NMEA_ERR_INVALID_SENTENCE;
    }

    /* check for checksum */
    if (!utils_isCharExist(string, '*'))
    {
        return NMEA_ERR_INVALID_CHECKSUM;
    }

    if (chksum_enable)
    {
        uint8_t actual_chk;
        uint8_t expected_chk;

        expected_chk = utils_nmea_chksum_getFromStr(string);
        actual_chk   = utils_nmea_chksum_calculate(string);
        if (expected_chk != actual_chk)
        {
            return NMEA_ERR_INVALID_CHECKSUM;
        }
    }

    return NMEA_OK;
}

void
nmea_allocater (char **values, nmea_sentence_t type)
{
    switch (type)
    {
        case NMEA_GPGGA:
            nmea_gpgga_allocater(values);
            break;
        case NMEA_GPRMC:
            nmea_gprmc_allocater(values);
            break;
        default:
            break;
    }
}

err_code_t
nmea_check_gpgga (nmea_gpgga_s *pgpgga)
{
    if (pgpgga == NULL)
    {
        return NMEA_ERR_GPGGA_INVALID;
    }
    else if (pgpgga->sentence.type != NMEA_GPGGA
             || strcmp(pgpgga->sentence.word, "GPGGA"))
    {
        return NMEA_ERR_GPGGA_INVALID;
    }
    return NMEA_OK;
}

parser_status_t
nmea_gpggaIsUpdated (void)
{
    if (gpgga.isUpdated)
    {
        return PARSER_UPDATED;
    }
    else
    {
        return PARSER_NOT_UPDATED;
    }
}

void
nmea_get_gpgga (nmea_gpgga_s *pgpgga)
{
    if (pgpgga == NULL)
    {
        return;
    }

    if (NMEA_OK != nmea_check_gpgga(pgpgga))
    {
        return; // Handle error appropriately
    }

    // Copy the data from the static gpgga to the provided pointer
    *pgpgga         = gpgga;
    gpgga.isUpdated = false; // Reset the updated flag after getting the data

    return;
}

void
nmea_gpgga_allocater (char **values)
{
    nmea_sentence_t type = nmea_get_type(values[GPGGA_SENTENCE]);

    nmea_gpgga_allocater_time(values);
    nmea_gpgga_allocater_position(values);

    gpgga.fix_type           = atoi(values[GPGGA_FIX_TYPE]);
    gpgga.num_sats           = atoi(values[GPGGA_NUM_SATS]);
    gpgga.hdop               = atof(values[GPGGA_HDOP]);
    gpgga.altitude           = atof(values[GPGGA_ALTITUDE]);
    gpgga.altitude_unit      = values[GPGGA_ALTITUDE_UNIT][0];
    gpgga.undulation         = atof(values[GPGGA_UNDULATION]);
    gpgga.undulation_unit    = values[GPGGA_UNDULATION_UNIT][0];
    gpgga.diff_age           = atof(values[GPGGA_DIFF_AGE]);
    gpgga.diff_station_id[0] = '\0';
    if (NULL != values[GPGGA_DIFF_STATION_ID])
    {
        strncpy(gpgga.diff_station_id, values[GPGGA_DIFF_STATION_ID], 5);
        gpgga.diff_station_id[5] = '\0'; // Ensure null termination
    }

    gpgga.isUpdated = true; // Mark the data as updated
}

void
nmea_gpgga_allocater_time (char **values)
{
    gpgga.time.tm_hour = 0;
    gpgga.time.tm_min  = 0;
    gpgga.time.tm_sec  = 0;

    if (NULL == values[GPGGA_TIME])
    {
        return;
    }

    char buffer[3];

    strncpy(buffer, values[GPGGA_TIME], 2);
    buffer[2]          = '\0';
    gpgga.time.tm_hour = atoi(buffer);

    strncpy(buffer, values[GPGGA_TIME] + 2, 2);
    buffer[2]         = '\0';
    gpgga.time.tm_min = atoi(buffer);

    strncpy(buffer, values[GPGGA_TIME] + 4, 2);
    buffer[2]         = '\0';
    gpgga.time.tm_sec = atoi(buffer);
}

void
nmea_gpgga_allocater_position (char **values)
{
    if (NULL == values[GPGGA_LONGITUDE]
        || NULL == values[GPGGA_LONGITUDE_CARDINAL]
        || NULL == values[GPGGA_LATITUDE]
        || NULL == values[GPGGA_LATITUDE_CARDINAL])
    {
        return;
    }

    utils_brg_charToPosition(values[GPGGA_LONGITUDE],
                             &gpgga.latitude.degrees,
                             &gpgga.latitude.minutes);
    gpgga.latitude.cardinal = values[GPGGA_LONGITUDE_CARDINAL][0];

    utils_brg_charToPosition(values[GPGGA_LATITUDE],
                             &gpgga.longitude.degrees,
                             &gpgga.longitude.minutes);
    gpgga.longitude.cardinal = values[GPGGA_LATITUDE_CARDINAL][0];
}

err_code_t
nmea_check_gprmc (nmea_gprmc_s *pgprmc)
{
    if (pgprmc == NULL)
    {
        return NMEA_ERR_GPRMC_INVALID;
    }
    else if (pgprmc->sentence.type != NMEA_GPRMC
             || strcmp(pgprmc->sentence.word, "GPRMC"))
    {
        return NMEA_ERR_GPRMC_INVALID;
    }
    return NMEA_OK;
}

parser_status_t
nmea_gprmcIsUpdated (void)
{
    if (gprmc.isUpdated)
    {
        return PARSER_UPDATED;
    }
    else
    {
        return PARSER_NOT_UPDATED;
    }
}

void
nmea_get_gprmc (nmea_gprmc_s *pgprmc)
{
    if (pgprmc == NULL)
    {
        return;
    }

    if (NMEA_OK != nmea_check_gprmc(pgprmc))
    {
        return; // Handle error appropriately
    }

    // Copy the data from the static gprmc to the provided pointer
    *pgprmc = gprmc;

    // Reset the updated flag after getting the data
    gprmc.isUpdated = false;

    return;
}

void
nmea_gprmc_allocater (char **values)
{
    nmea_gprmc_allocater_time_date(values);
    nmea_gprmc_allocater_position(values);
    nmea_gprmc_allocater_magvar(values);

    gprmc.status           = values[GPRMC_STATUS][0];
    gprmc.speed_knots      = atof(values[GPRMC_SPEED_KNOTS]);
    gprmc.heading_deg      = atof(values[GPRMC_HEADING_DEG]);
    gprmc.positioning_mode = values[GPRMC_POSITIONING_MODE][0];

    gprmc.isUpdated = true; // Mark the data as updated
}

void
nmea_gprmc_allocater_time_date (char **values)
{
    gprmc.time.tm_hour = 0;
    gprmc.time.tm_min  = 0;
    gprmc.time.tm_sec  = 0;

    if (NULL == values[GPRMC_TIME] || NULL == values[GPRMC_DATE])
    {
        return;
    }

    char buffer[3];

    strncpy(buffer, values[GPRMC_TIME], 2);
    buffer[2]          = '\0';
    gprmc.time.tm_hour = atoi(buffer);

    strncpy(buffer, values[GPRMC_TIME] + 2, 2);
    buffer[2]         = '\0';
    gprmc.time.tm_min = atoi(buffer);

    strncpy(buffer, values[GPRMC_TIME] + 4, 2);
    buffer[2]         = '\0';
    gprmc.time.tm_sec = atoi(buffer);

    strncpy(buffer, values[GPRMC_DATE], 2);
    buffer[2]          = '\0';
    gprmc.date.tm_mday = atoi(buffer);

    strncpy(buffer, values[GPRMC_DATE] + 2, 2);
    buffer[2]         = '\0';
    gprmc.date.tm_mon = atoi(buffer); // tm_mon is 0-based

    strncpy(buffer, values[GPRMC_DATE] + 4, 2);
    buffer[2]          = '\0';
    gprmc.date.tm_year = atoi(buffer); // tm_year is years since 1900
}

void
nmea_gprmc_allocater_position (char **values)
{
    if (NULL == values[GPRMC_LATITUDE]
        || NULL == values[GPRMC_LATITUDE_CARDINAL]
        || NULL == values[GPRMC_LONGITUDE]
        || NULL == values[GPRMC_LONGITUDE_CARDINAL])
    {
        return;
    }

    utils_brg_charToPosition(values[GPRMC_LATITUDE],
                             &gprmc.latitude.degrees,
                             &gprmc.latitude.minutes);
    gprmc.latitude.cardinal = values[GPRMC_LATITUDE_CARDINAL][0];

    utils_brg_charToPosition(values[GPRMC_LONGITUDE],
                             &gprmc.longitude.degrees,
                             &gprmc.longitude.minutes);
    gprmc.longitude.cardinal = values[GPRMC_LONGITUDE_CARDINAL][0];
}

void
nmea_gprmc_allocater_magvar (char **values)
{
    if (NULL == values[GPRMC_MAGVAR_DEG]
        || NULL == values[GPRMC_MAGVAR_CARDINAL])
    {
        return;
    }

    gprmc.magvar.degrees  = atof(values[GPRMC_MAGVAR_DEG]);
    gprmc.magvar.cardinal = values[GPRMC_MAGVAR_CARDINAL][0];
}