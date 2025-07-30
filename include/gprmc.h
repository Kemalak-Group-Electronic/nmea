#ifndef INC_NMEA_GPRMC_H
#define INC_NMEA_GPRMC_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "nmea0183Type.h"

typedef struct {
	nmea_sentence_s sentence;
	struct tm time;				// Time in UTC (HHMMSS.SSS)
	char status; 				// Status, 'A' = active, 'V' = void
	nmea_position latitude; 	// Latitude & cardinal direction 
	nmea_position longitude; 	// Longitude & cardinal direction
	double speed_knots; 		// Speed over ground in knots
	double heading_deg; 		// Track angle in degrees
	struct tm date; 			// Date (DDMMYY)
	nmea_magvar magvar; 		// Magnetic variation & cardinal direction
	char positioning_mode; // Positioning mode, 'A' = autonomous, 'D' = differential, 'E' = estimated
	
	bool isUpdated;		// Flag to indicate if the data is updated
} nmea_gprmc_s;

// Indexes for GPRMC fields NOT for nmea_gprmc_s
#define GPRMC_SENTENCE 				0
#define GPRMC_TIME 					1
#define GPRMC_STATUS 				2
#define GPRMC_LATITUDE 				3
#define GPRMC_LATITUDE_CARDINAL 	4
#define GPRMC_LONGITUDE				5
#define GPRMC_LONGITUDE_CARDINAL	6
#define GPRMC_SPEED_KNOTS 			7
#define GPRMC_HEADING_DEG 			8
#define GPRMC_DATE 					9
#define GPRMC_MAGVAR_DEG 			10
#define GPRMC_MAGVAR_CARDINAL 		11
#define GPRMC_POSITIONING_MODE 		12

#endif  /* INC_NMEA_GPRMC_H */