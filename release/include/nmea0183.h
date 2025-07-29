#ifndef NMEA0108_H
#define NMEA0108_H

#include <stddef.h>
#include <stdint.h>

#include "nmea0183Type.h"
#include "gpgga.h"
#include "gprmc.h"

// Parser TYpe End //

void nmea0183_init(nmea_gpgga_s *pgpgga, nmea_gprmc_s *pgprmc);
int  nmea0183_parse(char *sentence, size_t length, bool chksum_enable);

err_code_t      nmea0183_check_gpgga(nmea_gpgga_s *parser);
parser_status_t nmea0183_gpggaIsUpdated(void);
err_code_t      nmea0183_get_gpgga(nmea_gpgga_s *parser);

err_code_t      nmea0183_check_gprmc(nmea_gprmc_s *parser);
parser_status_t nmea0183_gprmcIsUpdated(void);
err_code_t      nmea0183_get_gprmc(nmea_gprmc_s *parser);

#endif