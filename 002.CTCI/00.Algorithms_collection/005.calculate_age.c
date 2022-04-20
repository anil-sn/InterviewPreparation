// c program for age calculator
#include <stdio.h>
#include <stdlib.h>

// function to calculate current age
void findAge(int curr_date, int curr_month,
             int curr_year, int birth_date,
             int birth_month, int birth_year) {

    // days of every month
    int month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // if birth date is greater then current birth
    // month then do not count this month and add 30
    // to the date so as to subtract the date and
    // get the remaining days
    if (birth_date > curr_date) {
        curr_date = curr_date + month[birth_month - 1];
        curr_month = curr_month - 1;
    }

    // if birth month exceeds current month, then do
    // not count this year and add 12 to the month so
    // that we can subtract and find out the difference
    if (birth_month > curr_month) {
        curr_year = curr_year - 1;
        curr_month = curr_month + 12;
    }

    // calculate date, month, year
    int calculated_date = curr_date - birth_date;
    int calculated_month = curr_month - birth_month;
    int calculated_year = curr_year - birth_year;

    // print the present age
    printf("Present Age Years: %d  Months: %d  Days:" " %d\n",
           calculated_year,
           calculated_month,
           calculated_date);

    return;
}

// driver code to check the above function
int main() {

    // current dd// mm/yyyy
    int curr_date = 7;
    int curr_month = 12;
    int curr_year = 2017;

    // birth dd// mm// yyyy
    int birth_date = 16;
    int birth_month = 12;
    int birth_year = 2009;

    // function call to print age
    findAge(curr_date, curr_month, curr_year, birth_date, birth_month, birth_year);

    return 0;
}
