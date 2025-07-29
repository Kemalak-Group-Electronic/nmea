#include "unity.h"
#include "nmeaParserStatic.h"
#include "utils.h"

void
unittest_convert_char_position (const char *nmea_pos,
                                int        *degrees,
                                float      *minutes)
{
    float value = atof(nmea_pos);
    *degrees    = (int)(value / 100);
    *minutes    = value - (*degrees * 100);
}

void
setUp (void)
{
}

void
tearDown (void)
{
}

void
test_nmea_get_type (void)
{
    nmea_gpgga_s gpgga;
    nmea_gprmc_s gprmc;

    nmea_init(&gpgga, &gprmc);

    const char *gpgga_sentence
        = "$GPGGA,123456.789,1234.5678,N,12345.6789,E,1,08,0.9,100.0,M,0.0,M,,*"
          "47\r\n";
    const char *gprmc_sentence
        = "$GPRMC,123456,A,1234.5678,N,12345.6789,E,0.5,180.0,010120,,*1C\r\n";

    TEST_ASSERT_EQUAL(NMEA_GPGGA, nmea_get_type(gpgga_sentence));
    TEST_ASSERT_EQUAL(NMEA_GPRMC, nmea_get_type(gprmc_sentence));
    TEST_ASSERT_EQUAL(NMEA_UNKNOWN, nmea_get_type("INVALID,SENTENCE"));
}

void
test_nmea_init (void)
{
    nmea_gpgga_s gpgga;
    nmea_gprmc_s gprmc;

    nmea_init(&gpgga, &gprmc);

    nmea_get_gpgga(&gpgga);
    nmea_get_gprmc(&gprmc);

    TEST_ASSERT_EQUAL(NMEA_GPGGA, gpgga.sentence.type);
    TEST_ASSERT_EQUAL_STRING("GPGGA", gpgga.sentence.word);

    TEST_ASSERT_EQUAL(NMEA_GPRMC, gprmc.sentence.type);
    TEST_ASSERT_EQUAL_STRING("GPRMC", gprmc.sentence.word);
}

void
test_nmea_validate_length (void)
{
    const char eightChar[] = "12345678";
    size_t     len8        = strlen(eightChar);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_LENGTH,
                      nmea_validate(eightChar, len8, true));

    const char eightTwoChar[]
        = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do "
          "eiusmod tempor incididunt ut";
    size_t len82 = strlen(eightTwoChar);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_LENGTH,
                      nmea_validate(eightTwoChar, len82, true));
}

void
test_nmea_validate_sentence_check (void)
{
    const char noDollarSign[] = "GPGGA,12345678\r\n";
    size_t     len            = strlen(noDollarSign);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_SENTENCE,
                      nmea_validate(noDollarSign, len, true));

    const char nonCapSentence[] = "$GPgga,12345678\r\n";
    len                         = strlen(nonCapSentence);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_SENTENCE,
                      nmea_validate(nonCapSentence, len, true));

    const char noSentenceComma[] = "$GPGGA123456,78,\r\n";
    len                          = strlen(noSentenceComma);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_SENTENCE,
                      nmea_validate(noSentenceComma, len, true));
}

void
test_nmea_validate_check_end_termination (void)
{
    const char noCRLF[] = "$GPGGA,12345678";
    size_t     len      = strlen(noCRLF);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_CR_LF, nmea_validate(noCRLF, len, true));

    const char wrongCRLF[] = "$GPGGA,12345678\r";
    len                    = strlen(wrongCRLF);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_CR_LF,
                      nmea_validate(wrongCRLF, len, true));

    const char swapCRLF[] = "$GPGGA,12345678\n\r";
    len                   = strlen(swapCRLF);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_CR_LF,
                      nmea_validate(swapCRLF, len, true));
}

void
test_nmea_validate_checksum (void)
{
    const char noChecksum[] = "$GPGGA,12345678\r\n";
    size_t     len          = strlen(noChecksum);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_CHECKSUM,
                      nmea_validate(noChecksum, len, true));

    const char wrongChecksum[] = "$GPGGA,12345678*00\r\n";
    len                        = strlen(wrongChecksum);
    TEST_ASSERT_EQUAL(NMEA_ERR_INVALID_CHECKSUM,
                      nmea_validate(wrongChecksum, len, true));

    const char correctChecksum[] = "$GPGGA,12345678*72\r\n";
    len                          = strlen(correctChecksum);
    TEST_ASSERT_EQUAL(NMEA_OK, nmea_validate(correctChecksum, len, true));
}

void
test_nmea_gpgga_allocater (void)
{
    nmea_gpgga_s  gpgga;
    nmea_gprmc_s  gprmc;
    nmea_position position;

    gpgga.sentence.type = NMEA_GPGGA;
    strcpy(gpgga.sentence.word, "GPGGA");

    nmea_init(&gpgga, &gprmc);
    //"$GPGGA,110332.000,3150.7822,N,11711.9278,E,2,14,0.75,46.0,M,0.0,M,0000,0000*63\r\n"
    char **fields = (char *[]) {
        "GPGGA",
        "110332.000", // 0 Time in UTC (HHMMSS.SSS)
        "3150.7822",  // 1 Latitude in DDDMM.MMMM format
        "N",          // 2 Latitude cardinal direction
        "11711.9278", // 3 Longitude in DDDMM.MMMM format
        "E",          // 4 Longitude cardinal direction
        "2",          // 5 Fix type (1 = GPS fix)
        "14",         // 6 Number of satellites in view
        "0.75",       // 7 Horizontal Dilution of Precision
        "46.0",       // 8 Altitude above mean sea level
        "M",          // 9 Altitude unit (M for meters)
        "0.0",        // 10 Geoidal separation
        "M",          // 11 Geoidal separation unit (M for meters)
        "0000",       // 12 Age of differential GPS data (not used here)
        "0000" // 13 ID of the differential reference station (not used here)
    };

    nmea_allocater(fields, NMEA_GPGGA);
    nmea_get_gpgga(&gpgga);

    TEST_ASSERT_EQUAL_STRING("GPGGA", gpgga.sentence.word);

    int actual_time = gpgga.time.tm_hour * 10000 + gpgga.time.tm_min * 100
                      + gpgga.time.tm_sec;
    TEST_ASSERT_EQUAL(atoi(fields[GPGGA_TIME]), actual_time);

    unittest_convert_char_position(
        fields[GPGGA_LONGITUDE], &position.degrees, &position.minutes);
    TEST_ASSERT_EQUAL_INT(position.degrees, gpgga.latitude.degrees);
    TEST_ASSERT_EQUAL_FLOAT(position.minutes, gpgga.latitude.minutes);
    TEST_ASSERT_EQUAL_STRING(fields[GPGGA_LONGITUDE_CARDINAL],
                             &gpgga.latitude.cardinal);

    unittest_convert_char_position(
        fields[GPGGA_LATITUDE], &position.degrees, &position.minutes);
    TEST_ASSERT_EQUAL_INT(position.degrees, gpgga.longitude.degrees);
    TEST_ASSERT_EQUAL_FLOAT(position.minutes, gpgga.longitude.minutes);
    TEST_ASSERT_EQUAL_STRING(fields[GPGGA_LATITUDE_CARDINAL],
                             &gpgga.longitude.cardinal);

    TEST_ASSERT_EQUAL_INT(atoi(fields[GPGGA_FIX_TYPE]), gpgga.fix_type);
    TEST_ASSERT_EQUAL_INT(atoi(fields[GPGGA_NUM_SATS]), gpgga.num_sats);

    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPGGA_HDOP]), gpgga.hdop);
    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPGGA_ALTITUDE]), gpgga.altitude);
    TEST_ASSERT_EQUAL_CHAR(fields[GPGGA_ALTITUDE_UNIT][0], gpgga.altitude_unit);
    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPGGA_UNDULATION]), gpgga.undulation);
    TEST_ASSERT_EQUAL_CHAR(fields[GPGGA_UNDULATION_UNIT][0],
                           gpgga.undulation_unit);

    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPGGA_DIFF_AGE]), gpgga.diff_age);
    TEST_ASSERT_EQUAL_STRING(fields[GPGGA_DIFF_STATION_ID],
                             gpgga.diff_station_id);
}

void
test_nmea_gprmc_allocater (void)
{
    nmea_gpgga_s  gpgga;
    nmea_gprmc_s  gprmc;
    nmea_position position;

    gprmc.sentence.type = NMEA_GPRMC;
    strcpy(gprmc.sentence.word, "GPRMC");

    nmea_init(&gpgga, &gprmc);
    //"$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A*6A\r\n"
    char **fields = (char *[]) {
        "GPRMC",
        "123519",    // 0 Time in UTC (HHMMSS.SSS)
        "A",         // 1 Status (A = active)
        "4807.038",  // 2 Latitude in DDDMM.MMMM format
        "N",         // 3 Latitude cardinal direction
        "01131.000", // 4 Longitude in DDDMM.MMMM format
        "E",         // 5 Longitude cardinal direction
        "022.4",     // 6 Speed over ground in knots
        "084.4",     // 7 Course over ground in degrees
        "230394",    // 8 Date (DDMMYY)
        "003.1",     // 9 Magnetic variation
        "W",         // 10 Variation direction
        "A" // 11 Positioning mode (A = autonomous, D = differential, E =
            // estimated)
    };

    nmea_allocater(fields, NMEA_GPRMC);
    nmea_get_gprmc(&gprmc);

    TEST_ASSERT_EQUAL_STRING("GPRMC", gprmc.sentence.word);

    int actual_time = gprmc.time.tm_hour * 10000 + gprmc.time.tm_min * 100
                      + gprmc.time.tm_sec;
    TEST_ASSERT_EQUAL(atoi(fields[GPRMC_TIME]), actual_time);

    TEST_ASSERT_EQUAL_CHAR(fields[GPRMC_STATUS][0], gprmc.status);

    unittest_convert_char_position(
        fields[GPRMC_LATITUDE], &position.degrees, &position.minutes);

    TEST_ASSERT_EQUAL_INT(position.degrees, gprmc.latitude.degrees);
    TEST_ASSERT_EQUAL_FLOAT(position.minutes, gprmc.latitude.minutes);
    TEST_ASSERT_EQUAL_CHAR(fields[GPRMC_LATITUDE_CARDINAL][0],
                           gprmc.latitude.cardinal);

    unittest_convert_char_position(
        fields[GPRMC_LONGITUDE], &position.degrees, &position.minutes);
    TEST_ASSERT_EQUAL_FLOAT(position.degrees, gprmc.longitude.degrees);
    TEST_ASSERT_EQUAL_FLOAT(position.minutes, gprmc.longitude.minutes);
    TEST_ASSERT_EQUAL_CHAR(fields[GPRMC_LONGITUDE_CARDINAL][0],
                           gprmc.longitude.cardinal);

    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPRMC_SPEED_KNOTS]), gprmc.speed_knots);
    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPRMC_HEADING_DEG]), gprmc.heading_deg);

    int actual_date = gprmc.date.tm_year + gprmc.date.tm_mon * 100
                      + gprmc.date.tm_mday * 10000; // DDMMYY
    TEST_ASSERT_EQUAL_INT(atoi(fields[GPRMC_DATE]), actual_date);

    TEST_ASSERT_EQUAL_FLOAT(atof(fields[GPRMC_MAGVAR_DEG]),
                            gprmc.magvar.degrees);
    TEST_ASSERT_EQUAL_CHAR(fields[GPRMC_MAGVAR_CARDINAL][0],
                           gprmc.magvar.cardinal);
    TEST_ASSERT_EQUAL_CHAR(fields[GPRMC_POSITIONING_MODE][0],
                           gprmc.positioning_mode);
}