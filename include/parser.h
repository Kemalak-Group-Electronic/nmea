#ifndef INC_NMEA_PARSER_H
#define INC_NMEA_PARSER_H

#include <stdlib.h>
#include <string.h>
#include "nmea.h"
#include "parser_types.h"

typedef int (*allocate_data_f)(nmea_parser_s *);
typedef int (*set_default_f)(nmea_parser_s *);
typedef int (*free_data_f)(nmea_s *);
typedef int (*parse_f)(nmea_parser_s *, char *, int);
typedef int (*init_f)(nmea_parser_s *);

typedef struct
{
    nmea_parser_s parser;
    int           errors;
    void         *handle;

    /* Functions */
    allocate_data_f allocate_data;
    set_default_f   set_default;
    free_data_f     free_data;
    parse_f         parse;
} nmea_parser_module_s;

#endif /* INC_NMEA_PARSER_H */