/* HW2 Template - Fall 2024 */
#include "hw2.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


#define ITEMS_TO_READ (9)
#define MIN_VAL (0)
#define MIN_DATE (1)
#define MAX_MONTH (12)
#define MAX_DAY (30)
#define WATER_MULTIPLIER (1.065)
#define ELECTRIC_MULTIPLIER (0.0984)
#define STEAM_MULTIPLIER  (0.06)
#define TRASH_MULTIPLIER  (0.04)



/*
 *  This function takes an input file as the first argument
 *  and a dorm char array as the second. From that, it
 *  traverses the input file, reading the data, and
 *  returns the calculated amount for the total utility cost
 *  of the dorm specified in the input dorm char array.
 */

double calculate_bill(char *infile, char *dorm) {

  //Open the input file for reading with error checking

  FILE *read_in = 0;
  read_in = fopen(infile, "r");
  if (read_in == NULL) {
    return FILE_READ_ERR;
  }


  //define variables that will be used throughout this function

  char dorm_name[MAX_NAME_LENGTH] = {};
  float water = 0;
  int year = 0;
  int month = 0;
  int day = 0;
  int residents = 0;
  int electric = 0;
  int steam = 0;
  int trash = 0;
  double tot_bill = 0;
  bool found = false;

  //loop through each line of the input file, reading the data using
  //the format specified in fscanf. Performs thorough error checking
  //and calculates from the scanned strings, the total utilily cost
  //of the specified dorm

  while (true) {

    //Read line in the file according to format specified in HW2
    //49 is specified as it is MAX_NAME_LENGTH - 1.

    int items = fscanf(read_in, "%d-%d-%d,\"%49[^\"]\",%d\\%f,%d,%d,%d",
      &year, &month, &day, dorm_name, &residents, &water, &electric,
      &steam, &trash);

    //Ensure that the file has Data

    if (items == EOF) {
      break;
    }

    //During the loop, ensure all data is formatted correctly

    if ( (items != ITEMS_TO_READ) && (found == false) ) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensures all values scanned are within acceptable ranges

    if ((residents < MIN_VAL) || (water < MIN_VAL) ||
        (electric < MIN_VAL) || (steam < MIN_VAL) || (trash < MIN_VAL)) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensures the dates are within valid date ranges

    if ((year < MIN_VAL) || (month < MIN_DATE) ||
        (month > MAX_MONTH) || (day < MIN_DATE) ||
        (day > MAX_DAY)) {
      fclose(read_in);
      return BAD_DATE;
    }

    //Ensures there is at least 1 resident

    if (residents == MIN_VAL) {
      fclose(read_in);
      return NO_RESIDENTS;
    }

    //if the dorm names match, add the calulated bill to the total

    if (strcmp(dorm, dorm_name) == 0) {
      found = true;
      tot_bill += (WATER_MULTIPLIER * water) +
                  (ELECTRIC_MULTIPLIER * electric) +
                  (STEAM_MULTIPLIER * steam) +
                  (TRASH_MULTIPLIER * trash);
    }

  }
  //if no applicable data was found, return appropriate error

  if (found == false) {
    fclose(read_in);
    return NO_DATA_POINTS;
  }

  fclose(read_in);
  return tot_bill;

} /* calculate_bill() */


/*
 *  This function generates a monthly water report for a
 *  specified dorm in a specific year. It reads the contents
 *  of the input file and prints out the resulting report
 *  to the output file. The water report consists of a cost
 *  breakdown of each month for the specified dorm and year.
 */

int generate_monthly_water_report(char *in_file, char *dorm,
                                  int year, char *out_file) {

  //Open the input file for reading, and output file for writing

  FILE *read_in = 0;
  FILE *water_rep = 0;
  read_in = fopen(in_file, "r");
  water_rep = fopen(out_file, "w");

  //Perform error checking on both opened files

  if (read_in == NULL) {
    fclose(water_rep);
    return FILE_READ_ERR;
  }
  if (water_rep == NULL) {
    fclose(read_in);
    return FILE_WRITE_ERR;
  }

  //Define variables which will be used throughout this function

  double water_usage_monthly[MAX_MONTH] = {};
  char dorm_name[MAX_NAME_LENGTH] = {};
  float water = 0;
  int item_year = 0;
  int month = 0;
  int day = 0;
  int residents = 0;
  int electric = 0;
  int steam = 0;
  int trash = 0;
  bool found = false;

  //This loops through each input and if the dorm matches the
  //specified dorm, and the year matches the specified year, then the
  //water usage is saved to an array of the monthly usage.

  while (true) {

    //reads the input file line by line and saves the values to the specified
    //variables. 49 is used in place of MAX_NAME_LENGTH - 1.

    int items = fscanf(read_in, "%d-%d-%d,\"%49[^\"]\",%d\\%f,%d,%d,%d",
                &item_year, &month, &day, dorm_name, &residents, &water,
                &electric, &steam, &trash);

    //Ensure that the file is not empty

    if (items == EOF) {
      break;
    }

    //Ensure that the current line being read is not malformed

    if ((items != ITEMS_TO_READ) && (found == false)) {
      fclose(read_in);
      fclose(water_rep);
      return BAD_RECORD;
    }

    //Ensure all variables fall within their respective ranges

    if ((residents < MIN_VAL) || (water < MIN_VAL) || (electric < MIN_VAL) ||
        (steam < MIN_VAL) || (trash < MIN_VAL)) {
      fclose(read_in);
      fclose(water_rep);
      return BAD_RECORD;
    }

    //Ensure all dates fall within valid date ranges

    if ((item_year < MIN_VAL) || (year < MIN_VAL) || (month < MIN_DATE) ||
        (month > MAX_MONTH) || (day < MIN_DATE) || (day > MAX_DAY)) {
      fclose(read_in);
      fclose(water_rep);
      return BAD_DATE;
    }

    //Ensure there is at least one resident

    if (residents == MIN_VAL) {
      fclose(read_in);
      fclose(water_rep);
      return NO_RESIDENTS;
    }

    //add wataer usage of spefied month to the array if dorm name and year match

    if ((strcmp(dorm, dorm_name) == 0) && (year == item_year)) {
      found = true;
      water_usage_monthly[month - 1] += water;
    }

  }

  //If not data was found, return applicable error

  if (found == false) {
    fclose(read_in);
    fclose(water_rep);
    return NO_DATA_POINTS;
  }

  fclose(read_in);

  //Loop through the newly created water usage array and print
  //out the contents in a specified format to the output file

  fprintf(water_rep, "%s\n", dorm);
  for (int i = (MIN_DATE - 1); i < MAX_MONTH; i++) {
    fprintf(water_rep, "%02d:%.3lf\n", i + 1, water_usage_monthly[i]);
  }
  fprintf(water_rep, "%d", year);

  fclose(water_rep);

  return SUCCESS;

} /* generate_monthly_water_report() */


/*
 *  This function calculates the electricity usage of a dorm
 *  throughout the course of a year by reading the contents of
 *  the provided input file.
 */

int get_electricity_usage(char *in_file, char *dorm, int year) {


  //Open the input file for reading and perform error checking
  FILE *read_in = 0;
  read_in = fopen(in_file, "r");
  if (read_in == NULL) {
    return FILE_READ_ERR;
  }


  //Define variables which will be used throughout this function
  int electricity_usage = 0;
  char dorm_name[MAX_NAME_LENGTH] = {};
  float water = 0;
  int item_year = 0;
  int month = 0;
  int day = 0;
  int residents = 0;
  int electric = 0;
  int steam = 0;
  int trash = 0;
  bool found = false;


  //This while loop traverses the input file and saves the contents to
  //specific variables which will be checked to ensure the data was input
  //correctly. It loops through each input and if the dorm matches the
  //specified dorm, and the year matches the specified year, then the
  //electicity usage is added to the overall tally.

  while (true) {

    //reads the input file line by line and saves the values to the specified
    //variables. 49 is used in place of MAX_NAME_LENGTH - 1.

    int items = fscanf(read_in, "%d-%d-%d,\"%49[^\"]\",%d\\%f,%d,%d,%d",
                &item_year, &month, &day, dorm_name, &residents, &water,
                &electric, &steam, &trash);

    //Ensure that the input file is not empty

    if (items == EOF) {
      break;
    }

    //Ensure that the input string is not malformed

    if ((items != ITEMS_TO_READ) && (found == false)) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensure that the variables fall within their respective ranges

    if ((residents < MIN_VAL) || (water < MIN_VAL) || (electric < MIN_VAL) ||
        (steam < MIN_VAL) || (trash < MIN_VAL)) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensure the dates are within the appropriate date ranges

    if ((item_year < MIN_VAL) || (year < MIN_VAL) || (month < MIN_DATE) ||
        (month > MAX_MONTH) || (day < MIN_DATE) || (day > MAX_DAY)) {
      fclose(read_in);
      return BAD_DATE;
    }

    //Ensure there is at least one resident

    if (residents == MIN_VAL) {
      fclose(read_in);
      return NO_RESIDENTS;
    }

    //compare the dorm names and year, and if they match, add the
    //electric usage to the overall tally

    if ((strcmp(dorm, dorm_name) == 0) && (year == item_year)) {
      found = true;
      electricity_usage += electric;
    }

  }

  //if no results are found, return appropriate error code

  if (found == false) {
    fclose(read_in);
    return NO_DATA_POINTS;
  }

  fclose(read_in);

  //return total electricity usage of a specified dorm during a specific year

  return electricity_usage;

} /* get_electricity_usage() */



/*
 *  This function takes the sum of the averages of all water usage per resident
 *  and divides it by the number of relevant entries to calulate the average
 *  resident water usage in a specific month of a spefified year.
 */

double get_average_resident_water_usage(char *in_file, int month, int year) {

  //open input file for reading and perform error check

  FILE *read_in = 0;
  read_in = fopen(in_file, "r");
  if (read_in == NULL) {
    return FILE_READ_ERR;
  }

  //define constants which will be used throughout the function

  double average_wpr = 0;
  int num_entries = 0;
  double avg_res_wtr_usg = 0;
  char dorm_name[MAX_NAME_LENGTH] = {};
  float water = 0;
  int item_year = 0;
  int item_month = 0;
  int day = 0;
  int residents = 0;
  int electric = 0;
  int steam = 0;
  int trash = 0;
  bool found = false;

  //This while loop traverses the input file and saves the contents to
  //specific variables which will be checked to ensure the data was input
  //correctly. It loops through each line of input, calcuting that dorm's
  //average water usage per resident

  while (true) {

    //reads the input file line by line and saves the values to the specified
    //variables. 49 is used in place of MAX_NAME_LENGTH - 1.

    int items = fscanf(read_in, "%d-%d-%d,\"%49[^\"]\",%d\\%f,%d,%d,%d",
                &item_year, &item_month, &day, dorm_name, &residents, &water,
                &electric, &steam, &trash);

    //Ensures the file is not empty

    if (items == EOF) {
      break;
    }

    //Ensures that each line has the correct number of variables assigned

    if ((items != ITEMS_TO_READ) && (found == false)) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensures that all variables fall within their specified ranges

    if ((residents < MIN_VAL) || (water < MIN_VAL) || (electric < MIN_VAL) ||
        (steam < MIN_VAL) || (trash < MIN_VAL)) {
      fclose(read_in);
      return BAD_RECORD;
    }

    //Ensures that all dates fall within thier specified ranges

    if ((item_year < MIN_VAL) || (year < MIN_VAL) || (item_month < MIN_DATE) ||
        (item_month > MAX_MONTH) || (month < MIN_DATE) || (month > MAX_MONTH) ||
        (day < MIN_DATE) || (day > MAX_DAY)) {
      fclose(read_in);
      return BAD_DATE;
    }

    //Ensures that there is at least one resident per entry

    if (residents == MIN_VAL) {
      fclose(read_in);
      return NO_RESIDENTS;
    }

    //If the year and month match, calculate the average water usage per
    //resident and add one to the tally

    if ((year == item_year) && (month == item_month)) {
      found = true;
      average_wpr += ( (double) water / residents);
      num_entries++;
    }

  }

  //if no results are found, return error code

  if (found == false) {
    fclose(read_in);
    return NO_DATA_POINTS;
  }

  fclose(read_in);

  //calculate the average resident water usage for that month in
  //the specified year

  avg_res_wtr_usg = (average_wpr / num_entries);

  return avg_res_wtr_usg;

} /* get_average_resident_water_usage() */





