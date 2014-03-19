
#include <stdio.h>
#include <stdint.h>

#define SECS_DAY    (60*60*24)
#define SECS_WEEK   (60*60*24*7)

#define isleap(x) ((((x)%400)==0) || (!(((x)%100)==0) && (((x)%4)==0)))

const int8_t DAYS_MONTH[12] =
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


void itow2time (uint16_t  gps_week,
                uint32_t  gps_itow,
                uint8_t*  gps_second,
                uint8_t*  gps_minute,
                uint8_t*  gps_hour,
                uint8_t*  gps_day,
                uint8_t*  gps_month,
                uint16_t* gps_year)
{
    uint32_t total_seconds, total_days, month_days, years=0, months=0;
    uint32_t gps_tow = gps_itow / 1000;
    
    /* sanity check */
    if (gps_tow >= SECS_WEEK) return;

    /* seconds since gps start 6-JAN-1980 00:00:00 */
    total_seconds = (uint32_t)(gps_tow + gps_week*SECS_WEEK);

    /* days since 1-JAN-1980 */
    total_days = (total_seconds / (SECS_DAY)) + 5;

    /* years since 1980 */
    while (1)
    {
        if (total_days < (365 + isleap(1980 + years))) break;
        total_days -= (365 + isleap(1980 + years));
        years++;
    }

    /* months since start of year */
    while (1)
    {
        if ((isleap(years)) && (months == 1))
            month_days = 29;
        else
            month_days = DAYS_MONTH[months];

        if (total_days < month_days) break;

        total_days -= month_days;
        months++;
    }

    /* convert */
    *gps_hour   = (uint8_t) ((total_seconds % SECS_DAY) / 3600);
    *gps_minute = (uint8_t) ((total_seconds % 3600) / 60);
    *gps_second = (uint8_t) ((total_seconds % 60));
    *gps_month  = (uint8_t) (months + 1);
    *gps_day    = (uint8_t) (total_days + 1);
    *gps_year   = (uint16_t)(1980 + years);
}

int main()
{
    uint16_t  gps_week;
    uint32_t  gps_itow;
    uint8_t   gps_second, gps_minute, gps_hour;
    uint8_t   gps_day, gps_month;
    uint16_t  gps_year;
    
    gps_week = 1554;
    gps_itow = 39692499;
    
    itow2time (gps_week, gps_itow, &gps_second, &gps_minute,
                        &gps_hour, &gps_day, &gps_month, &gps_year);
                        
    printf("week %d, itow %d\n", gps_week, gps_itow);
    printf("18.10.2009  11:00:50\n");
    printf("%02d.%02d.%04d  %02d:%02d:%02d\n", gps_day, gps_month, gps_year, gps_hour, gps_minute, gps_second);

    gps_week = 1553;
    gps_itow = 211594250;
    
    itow2time (gps_week, gps_itow, &gps_second, &gps_minute,
                        &gps_hour, &gps_day, &gps_month, &gps_year);
                        
    printf("week %d, itow %d\n", gps_week, gps_itow);
    printf("13.10.2009  10:46:19\n");
    printf("%02d.%02d.%04d  %02d:%02d:%02d\n", gps_day, gps_month, gps_year, gps_hour, gps_minute, gps_second);
    
    gps_week = 1514;
    gps_itow = 211594250;
    
    itow2time (gps_week, gps_itow, &gps_second, &gps_minute,
               &gps_hour, &gps_day, &gps_month, &gps_year);
                        
    printf("week %d, itow %d\n", gps_week, gps_itow);
    printf("13.01.2009  10:46:19\n");
    printf("%02d.%02d.%04d  %02d:%02d:%02d\n", gps_day, gps_month, gps_year, gps_hour, gps_minute, gps_second);    
    
    gps_week = 1511;
    gps_itow = 211594250;
    
    itow2time (gps_week, gps_itow, &gps_second, &gps_minute,
               &gps_hour, &gps_day, &gps_month, &gps_year);
                        
    printf("week %d, itow %d\n", gps_week, gps_itow);
    printf("23.12.2008  10:46:19\n");
    printf("%02d.%02d.%04d  %02d:%02d:%02d\n", gps_day, gps_month, gps_year, gps_hour, gps_minute, gps_second);    
    
    gps_week = 0;
    gps_itow = 1000;
    
    itow2time (gps_week, gps_itow, &gps_second, &gps_minute,
               &gps_hour, &gps_day, &gps_month, &gps_year);
                        
    printf("week %d, itow %d\n", gps_week, gps_itow);
    printf("06.01.1980  00:00:01\n");
    printf("%02d.%02d.%04d  %02d:%02d:%02d\n", gps_day, gps_month, gps_year, gps_hour, gps_minute, gps_second);    
    
    return 0;
}
