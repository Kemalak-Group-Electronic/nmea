#ifndef INC_NMEA_GPGGA_H
#define INC_NMEA_GPGGA_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "nmea0183Type.h"

typedef struct {
	nmea_sentence_s sentence;	// NEMA0183 sentence type and prefix
	struct tm time;				// 1) Time in UTC (HHMMSS.SSS)
	nmea_position longitude;	// 2,3) Longitude & cardinal direction
	nmea_position latitude;		// 4,5) Latitude & cardinal direction
	int fix_type; 				// 6) = invalid, 1 = GPS fix, 2 = DGPS fix
	int num_sats; 				// 7) Number of satellites in view
	float hdop; 				// 8) Horizontal Dilution of Precision
	float altitude; 			// 9) Altitude above mean sea level
	char altitude_unit; 		// 10) 'M' for meters
	float undulation; 			// 11) Geoidal separation
	char undulation_unit; 		// 12) 'M' for meters
	float diff_age; 			// 13) Age of differential GPS data
	char diff_station_id[6]; 	// 14) ID of the differential reference station

	bool isUpdated;		// Flag to indicate if the data is updated
} nmea_gpgga_s;


#define GPGGA_SENTENCE				0
#define GPGGA_TIME 					1
#define GPGGA_LONGITUDE 			2
#define GPGGA_LONGITUDE_CARDINAL	3
#define GPGGA_LATITUDE 				4
#define GPGGA_LATITUDE_CARDINAL 	5
#define GPGGA_FIX_TYPE 				6
#define GPGGA_NUM_SATS 				7
#define GPGGA_HDOP 					8
#define GPGGA_ALTITUDE 				9
#define GPGGA_ALTITUDE_UNIT 		10
#define GPGGA_UNDULATION 			11
#define GPGGA_UNDULATION_UNIT 		12
#define GPGGA_DIFF_AGE 				13
#define GPGGA_DIFF_STATION_ID 		14
#endif  /* INC_NMEA_GPGGA_H */