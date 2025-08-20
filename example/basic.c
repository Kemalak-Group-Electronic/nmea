// To build: gcc examples/minimum.c -lnmea -o minimum

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nmea0183.h"

nmea_gpgga_s mygpgga;
nmea_gprmc_s mygprmc;

void
print_gpgga (void)
{
    printf("%s:\n", mygpgga.sentence.word);
    printf("  Time: %d:%d:%d\n",
           mygpgga.time.tm_hour,
           mygpgga.time.tm_min,
           mygpgga.time.tm_sec);
    printf("  Latitude: %d %f %c\n",
           mygpgga.latitude.degrees,
           mygpgga.latitude.minutes,
           mygpgga.latitude.cardinal);
    printf("  Longitude: %d %f %c\n",
           mygpgga.longitude.degrees,
           mygpgga.longitude.minutes,
           mygpgga.longitude.cardinal);
    printf("  Fix Quality: %d\n", mygpgga.fix_type);
    printf("  Number of Satellites: %d\n", mygpgga.num_sats);
    printf("  HDOP: %.1f\n", mygpgga.hdop);
    printf("  Altitude: %.1f %c\n", mygpgga.altitude, mygpgga.altitude_unit);
}

void
print_gprmc (void)
{
    printf("%s:\n", mygprmc.sentence.word);
    printf("  Time: %d:%d:%d\n",
           mygprmc.time.tm_hour,
           mygprmc.time.tm_min,
           mygprmc.time.tm_sec);
    printf("  Status: %c\n", mygprmc.status);
    printf("  Latitude: %d %f %c\n",
           mygprmc.latitude.degrees,
           mygprmc.latitude.minutes,
           mygprmc.latitude.cardinal);
    printf("  Longitude: %d %f %c\n",
           mygprmc.longitude.degrees,
           mygprmc.longitude.minutes,
           mygprmc.longitude.cardinal);
    printf("  Speed: %.2f knots\n", mygprmc.speed_knots);
    printf("  Heading: %.2f degrees\n", mygprmc.heading_deg);
}

int
main (void)
{
    // Sentence string to be parsed
    const char gpgga1[]
        = "$GPGGA,123519,4042.6142,N,07400.4168,W,1,08,0.9,10.0,M,46.9,M,,*"
          "6D\r\n";
    const char gpgga2[]
        = "$GPGGA,095601,,,,,0,00,99.9,,M,,M,,*7A\r\n"; // Invalid/no fix
    const char gpgga3[]
        = "$GPGGA,141350,5128.6200,N,00027.5800,W,2,10,0.8,50.0,M,45.0,M,2.0,"
          "0001*43\r\n";
    const char gpgga4[]
        = "$GPGGA,061235,3540.4480,N,13945.1234,E,1,07,1.2,30.0,M,40.0,M,,*"
          "76\r\n";
    const char gpgga5[]
        = "$GPGGA,083000,2742.3421,N,08519.4567,E,1,09,0.7,1300.0,M,-34.0,M,,*"
          "5D\r\n";

    const char gprmc1[]
        = "$GPRMC,123519,A,4042.6142,N,07400.4168,W,000.5,054.7,210725,,,*"
          "25\r\n"; // Valid fix in NY
    const char gprmc2[]
        = "$GPRMC,095601,V,,,,,,,210725,,,N*5B\r\n"; // Invalid/no fix
    const char gprmc3[]
        = "$GPRMC,141350,A,5128.6200,N,00027.5800,W,005.5,089.6,210725,,,*"
          "27\r\n"; // London fix
    const char gprmc4[]
        = "$GPRMC,061235,A,3540.4480,N,13945.1234,E,003.2,120.5,210725,,,*"
          "32\r\n"; // Tokyo moving
    const char gprmc5[]
        = "$GPRMC,083000,A,2742.3421,N,08519.4567,E,001.2,360.0,210725,,,*"
          "3D\r\n"; // Kathmandu fix

    char *sentence = (char *)gpgga1; // Change this to test different sentences
    // char *sentence = (char *)gprmc1; // Change this to test different
    // sentences

    nmea0183_init(&mygpgga, &mygprmc);
    printf("Parsing NMEA sentence: %s", sentence);

    // Pointer to struct containing the parsed data. Should be freed manually.
    int err;
    int len = strlen(sentence);
    err     = nmea0183_parse(sentence, len, true);

    if (err != NMEA_OK)
    {
        printf("Error parsing sentence: %d\n", err);
    }

    if (nmea0183_gpggaIsUpdated() == PARSER_UPDATED)

    {
        printf("GPGGA data is updated.\n");
        if (nmea0183_get_gpgga(&mygpgga) != NMEA_OK)
        {
            printf("Wrong Parser Type \n");
        }
        else
        {
            print_gpgga();
        }
    }
    else if (nmea0183_gprmcIsUpdated() == PARSER_UPDATED)

    {
        printf("GPRMC data is updated.\n");
        if (nmea0183_get_gprmc(&mygprmc) != NMEA_OK)
        {
            printf("Wrong Parser Type \n");
        }
        else
        {
            print_gprmc();
        }
    }
    else
    {
        printf("No data updated.\n");
    }
}