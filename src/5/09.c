#include <stdio.h>

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

#define DAYS_IN_YEAR 365
#define MONTH_IN_YEAR 12

int day_of_year(int year, int month, int day)
{
	if(month < 1 || month > MONTH_IN_YEAR) return -1;
	int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
	if(day < 1 || day > daytab[leap][month]) return -1;

	char *months = daytab[leap];
	while(--month)
	{
		day += *++months;
	}
	
	return day;
}

int month_day(int year, int yearday, int *pmonth, int *pday)
{
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    if(yearday < 1 || yearday > DAYS_IN_YEAR + leap) return -1;

	char *months = daytab[leap];
	while(yearday > *++months)
	{
		yearday -= *months;
	}

    *pmonth = (int) (months - daytab[leap]);
    *pday = yearday;
    return 0;
}


int main(void)
{
	int year = 2003;
	int month = 7;
	int day = 29;

	int yearday;	
	printf("%02d.%02d.%d: %03d\n", day, month, year, (yearday = day_of_year(year, month, day)));

	int calc_year = 2003;
	int calc_month;
	int calc_day;
	month_day(calc_year, yearday, &calc_month, &calc_day);
	printf("%02d.%02d.%d: %03d\n", calc_day, calc_month, calc_year, yearday);

	return 0;
}
