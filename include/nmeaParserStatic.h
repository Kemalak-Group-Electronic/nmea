#ifndef NMEAPARSERSTATIC_H
#define NMEAPARSERSTATIC_H

#include "nmea0183Type.h"
#include "gpgga.h"
#include "gprmc.h"

nmea_sentence_t nmea_get_type(const char *string);

void nmea_init(nmea_gpgga_s *pgpgga, nmea_gprmc_s *pgprmc);
void nmea_gpgga_init(nmea_gpgga_s *pgpgga);
void nmea_gprmc_init(nmea_gprmc_s *pgprmc);

err_code_t nmea_validate(const char *string, size_t length, bool chksum_enable);
void nmea_allocater(char **values, nmea_sentence_t type);

err_code_t nmea_check_gpgga(nmea_gpgga_s *pgpgga);
void nmea_get_gpgga(nmea_gpgga_s *pgpgga);
void nmea_gpgga_allocater(char **values);
void nmea_gpgga_allocater_time(char **values);
void nmea_gpgga_allocater_position(char **values);

err_code_t nmea_check_gprmc(nmea_gprmc_s *pgprmc);
void nmea_get_gprmc(nmea_gprmc_s *pgprmc);
void nmea_gprmc_allocater(char **values);
void nmea_gprmc_allocater_time_date(char **values);
void nmea_gprmc_allocater_position(char **values);
void nmea_gprmc_allocater_magvar(char **values);

#endif // NMEAPARSERSTATIC_H