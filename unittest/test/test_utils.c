#include "unity.h"
#include "utils.h"

void
setUp (void)
{
}

void
tearDown (void)
{
}

void
test_utils_splitStrByComma (void)
{
#define ARRAY_MAX 14
    char string[]
        = "123456.00,4807.038,N,01131.000,E,1,12,0.8,545.4,M,46.9,M,,\0";
    char    *values[ARRAY_MAX];
    uint16_t valuesLength;

    valuesLength = utils_splitStrByComma(string, values, ARRAY_LENGTH(values));

    TEST_ASSERT_EQUAL_INT(ARRAY_MAX, valuesLength);
    TEST_ASSERT_EQUAL_STRING("123456.00", values[0]);
    TEST_ASSERT_EQUAL_STRING("4807.038", values[1]);
    TEST_ASSERT_EQUAL_STRING("N", values[2]);
    TEST_ASSERT_EQUAL_STRING("01131.000", values[3]);
    TEST_ASSERT_EQUAL_STRING("E", values[4]);
    TEST_ASSERT_EQUAL_STRING("1", values[5]);
    TEST_ASSERT_EQUAL_STRING("12", values[6]);
    TEST_ASSERT_EQUAL_STRING("0.8", values[7]);
    TEST_ASSERT_EQUAL_STRING("545.4", values[8]);
    TEST_ASSERT_EQUAL_STRING("M", values[9]);
    TEST_ASSERT_EQUAL_STRING("46.9", values[10]);
    TEST_ASSERT_EQUAL_STRING("M", values[11]);
    TEST_ASSERT_EQUAL_STRING("", values[12]);
    TEST_ASSERT_EQUAL_STRING("", values[13]);
}

void
test_utils_isCharExist ()
{
    const char string[]
        = "$GPGGA,123456.00,4807.038,N,01131.000,E,1,12,0.8,545.4,M,46.9,M,,*"
          "69\r\n";

    printf("Scanning String : ");
    for (size_t i = 0; i < strlen(string); i++)
    {
        bool charExist = utils_isCharExist(string, string[i]);
        printf("%c", string[i]);
        TEST_ASSERT_EQUAL_INT(true, charExist);
    }
}

void
test_utils_crop_string (void)
{
    char sentence[]
        = "$GPGGA,123456.00,4807.038,N,01131.000,E,1,12,0.8,545.4,M,46.9,M,,*"
          "69\r\n";
    size_t offset = 1; // Start after the '$'
    size_t last
        = strlen(sentence) - 6; // Exclude the checksum and CRLF at the end

    char *cropped = utils_crop_string(sentence, offset, last);

    TEST_ASSERT_EQUAL_STRING(
        "GPGGA,123456.00,4807.038,N,01131.000,E,1,12,0.8,545.4,M,46.9,M,,",
        cropped);
}

void
test_utils_brg_charToPosition (void)
{
    char  brgStr[]   = "4807.038";
    int   degreesExp = 48;
    float minutesExp = 7.038;

    int   degreesAct;
    float minutesAct;

    utils_brg_charToPosition(brgStr, &degreesAct, &minutesAct);

    TEST_ASSERT_EQUAL_INT(degreesExp, degreesAct);
    TEST_ASSERT_FLOAT_WITHIN(0.03f, minutesExp, minutesAct);
}

void
test_utils_nmea_chksum_getFromStr_calculate ()
{
    int expChkSum;
    int actChkSum;

    const char string1[]
        = "$GPGGA,123456.00,4807.038,N,01131.000,E,1,12,0.8,545.4,M,46.9,M,,*"
          "69\r\n";
    expChkSum = utils_nmea_chksum_getFromStr(string1);
    actChkSum = utils_nmea_chksum_calculate(string1);
    TEST_ASSERT_EQUAL_INT(expChkSum, actChkSum);

    const char string2[]
        = "$GPRMC,235947.00,A,5540.0152,N,01231.0000,E,0.0,0.0,060180,,,A*"
          "5C\r\n";
    expChkSum = utils_nmea_chksum_getFromStr(string2);
    actChkSum = utils_nmea_chksum_calculate(string2);
    TEST_ASSERT_EQUAL_INT(expChkSum, actChkSum);

    const char string3[] = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48\r\n";
    expChkSum            = utils_nmea_chksum_getFromStr(string3);
    actChkSum            = utils_nmea_chksum_calculate(string3);
    TEST_ASSERT_EQUAL_INT(expChkSum, actChkSum);

    const char string4[] = "$GPGLL,4916.45,N,12311.12,W,225444,A*31\r\n";
    expChkSum            = utils_nmea_chksum_getFromStr(string4);
    actChkSum            = utils_nmea_chksum_calculate(string4);
    TEST_ASSERT_EQUAL_INT(expChkSum, actChkSum);

    const char string5[]
        = "$GPGSA,A,3,04,05,09,12,24,26,29,31,32,,,1.8,1.0,1.5*13\r\n";
    expChkSum = utils_nmea_chksum_getFromStr(string5);
    actChkSum = utils_nmea_chksum_calculate(string5);
    TEST_ASSERT_EQUAL_INT(expChkSum, actChkSum);
}
