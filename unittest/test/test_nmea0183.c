#include "unity.h"
#include "nmea0183.h"
#include "gpgga.h"
#include "gprmc.h"
#include "nmeaParserStatic.h"
#include "utils.h"

void
setUp (void)
{
}

void
tearDown (void)
{
}

void test_nmea0183_init (void)
{
    nmea_gpgga_s gpgga;
    nmea_gprmc_s gprmc;

    nmea0183_init(&gpgga, &gprmc);

    //TEST_ASSERT_NOT_NULL(gpgga);
    //TEST_ASSERT_NOT_NULL(gprmc);
}