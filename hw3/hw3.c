/* HW3 - Fall 2024 */
#include "hw3.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DAYS_IN_YEAR (360)
#define DAYS_IN_MONTH (30)
#define HOURS_IN_DAY (24)
#define MIN_DATE (1)
#define MAX_MONTH (12)
#define MAX_DAY (30)
#define ITEMS_READ (11)
#define STAGE1_SUCCESS_FACTOR (0.5)
#define STAGE2_SUCCESS_FACTOR (1.0)
#define STAGE3_SUCCESS_FACTOR (1.5)
#define STAGE4_SUCCESS_FACTOR (2.0)
#define LOW_EXP_RATIO (0.2)
#define MID_EXP_RATIO (0.5)
#define HIGH_EXP_RATIO (0.8)


int g_mission_count = 0;
char g_astronaut_and_mission[MAX_MISSIONS][NUM_MISSION_COLS][MAX_NAME_LEN] = {};
char g_equipment[MAX_MISSIONS][NUM_COLS_IN_PAIR][MAX_NAME_LEN] = {};
float g_op_hours[MAX_MISSIONS][NUM_COLS_IN_PAIR] = {};
char g_mission_dates[MAX_MISSIONS][NUM_COLS_IN_PAIR][MAX_DATE_LEN] = {};
int g_experiments[MAX_MISSIONS][NUM_COLS_IN_PAIR] = {};


/*
 * This function takes 3 inputs, a year int, a month int, and a day int
 * and converts that to hours, and returns that value.
 */

int calculate_hours(int year, int month, int day) {

  //returns the total hours by first converting to days, then
  //by multiplying by 24
  return (((year * DAYS_IN_YEAR) + (month * DAYS_IN_MONTH) +
           (day)) * HOURS_IN_DAY);

} /* calculate_hours() */

/*
 * This function takes an input file as its only argument
 * and traverses the file, reading the data, and storing
 * the data in its respective global variable. Also performs
 * thorough error checking on the data and input file
 * to ensure data is in correct format.
 */

int read_tables(char *file_name) {

  //Open the input file for reading with error checking
  FILE *my_file = fopen(file_name, "r");
  if (my_file == NULL) {
    return FILE_READ_ERR;
  }
  //define constants to be used in the while loop
  int start_year = 0;
  int start_month = 0;
  int start_day = 0;
  int end_year = 0;
  int end_month = 0;
  int end_day = 0;

  //loop through each line of the input file, reading the data
  //using the format specified in fscanf. Perform error checking
  //and save the scanned strings to their appropriate gloabl variables
  while (true) {

    //Check if the mission count is more than the maximum missions
    if (g_mission_count > MAX_MISSIONS) {
      fclose(my_file);
      return OUT_OF_BOUNDS;
    }
    //Read line in the file according to the format specified in HW2
    //99 specifies MAX_NAME_LEN - 1 for an object and must be changed
    //if MAX_NAME_LEN is changed. 10 specifies MAX_DATE_LEN - 1 and
    //must be changed if MAX_DATE_LEN is changed.
    int items = fscanf(my_file,
                       "%99[^|]|%99[^|]|%99[^*]*%f|%99[^*]*%f|"
                       "%10[^|]|%10[^|]|%d|%d|%99[^|]|\n",
                       g_astronaut_and_mission[g_mission_count][0],
                       g_astronaut_and_mission[g_mission_count][1],
                       g_equipment[g_mission_count][0],
                       &g_op_hours[g_mission_count][0],
                       g_equipment[g_mission_count][1],
                       &g_op_hours[g_mission_count][1],
                       g_mission_dates[g_mission_count][0],
                       g_mission_dates[g_mission_count][1],
                       &g_experiments[g_mission_count][0],
                       &g_experiments[g_mission_count][1],
                       g_astronaut_and_mission[g_mission_count][2]);

    //If EOF has been reached and >= 1 mission has been recorded, break loop
    if ((items == EOF) && (g_mission_count > 0)) {
      fclose(my_file);
      break;
    }
    //If EOF has been reached and no missions have been recorded,
    //return the appropriate error
    if ((items == EOF) && (g_mission_count == 0)) {
      fclose(my_file);
      return NO_DATA_POINTS;
    }
    //If the line in the file that has been read does not have exactly 11
    //items, return the appropriate error
    if (items != ITEMS_READ) {
      fclose(my_file);
      return BAD_DATA;
    }
    //Check to esnure that all of the op hours are positive and that
    //all experiments are also positive, otherwise, return appropriate error
    if ((g_op_hours[g_mission_count][0] < 0) ||
        (g_op_hours[g_mission_count][1] < 0) ||
        (g_experiments[g_mission_count][0] < 0) ||
        (g_experiments[g_mission_count][1] < 0)) {
      fclose(my_file);
      return BAD_DATA;
    }
    //set all date values to 0 before scanning for them
    start_year = 0;
    start_month = 0;
    start_day = 0;
    end_year = 0;
    end_month = 0;
    end_day = 0;

    //Scan the mission dates global variable for the start and end date
    //data, and save the values to the start and end variables, respectively
    sscanf(g_mission_dates[g_mission_count][0], "%d-%d-%d", &start_year,
                                  &start_month, &start_day);
    sscanf(g_mission_dates[g_mission_count][1], "%d-%d-%d", &end_year,
                                &end_month, &end_day);

    //Check to ensure the dates are within valid date ranges, otherwise,
    //throw appropriate error
    if ((start_year < MIN_YEAR) || (start_year > MAX_YEAR) ||
        (start_month < MIN_DATE) || (start_month > MAX_MONTH) ||
        (start_day < MIN_DATE) || (start_day > MAX_DAY) ||
        (end_year < MIN_YEAR) || (end_year > MAX_YEAR) ||
        (end_month < MIN_DATE) || (end_month > MAX_MONTH) ||
        (end_day < MIN_DATE) || (end_day > MAX_DAY)) {
      fclose(my_file);
      return BAD_DATA;
    }
    //Check and make sure that the start date is before the end date,
    //otherwise, throw the appropriate error
    if ((calculate_hours(start_year, start_month, start_day)) >
        (calculate_hours(end_year, end_month, end_day))) {
      return BAD_DATA;
    }
    //increment global mission count after scanning the specified line
    g_mission_count += 1;
  }
  //return the total mission count of all lines in file
  return g_mission_count;

} /* read_tables() */



/*
 * This function takes the input of an astronauts name
 * and calculates the total time that this astronaut has
 * spend on a mission. The function returns an integer
 * of the number of hours the astronaut has spend on
 * missions.
 */

int get_total_time(char *astronaut_name) {

  //Define constants that will be referenced in this function
  int start_year = 0;
  int start_month = 0;
  int start_day = 0;
  int end_year = 0;
  int end_month = 0;
  int end_day = 0;
  int total_time = 0;
  int current_mission = 0;
  bool located = false;

  //Ensure that there is at least one mission
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }

  //loop through all missions, to sum up the total hours spent
  //on mission for the specified astronaut
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //ensure that the astronaut name matches the current item
    //in global array
    if (strcmp(astronaut_name,
               g_astronaut_and_mission[current_mission][0]) == 0) {

      //reset constants
      start_year = 0;
      start_month = 0;
      start_day = 0;
      end_year = 0;
      end_month = 0;
      end_day = 0;

      //pull out the date information such as month, day, and year
      //from the inputted string
      sscanf(g_mission_dates[current_mission][0], "%d-%d-%d",
             &start_year, &start_month, &start_day);
      sscanf(g_mission_dates[current_mission][1], "%d-%d-%d",
             &end_year, &end_month, &end_day);

      //calculate the difference between them to find the current misison
      //time and add it to the total time spend on missions
      total_time += (calculate_hours(end_year, end_month, end_day) -
                     calculate_hours(start_year, start_month, start_day));

      //denote that we have found at least one mission with the specified
      //astronauts name
      located = true;
    }
  }
  //if no matching astronauts were located, return appropriate error
  if (!located) {
    return NO_SUCH_ASTRONAUT;
  }
  //return the total calculated time on missions of the specified astronaut
  return total_time;
} /* get_total_time() */



/*
 * This function takes no input, and calculated the most popular used
 * equipment pair in the array of equipment pairs. This is calculated
 * not by the number of hours spent, but the number of instances of
 * these pairs that occur in the global variables.
 */

int popular_equipment_pair() {

  //Define constants to be used in this function
  int highest_equipment_usage = 0;
  int highest_equipment_index = 0;
  int current_mission = 0;
  int current_pair = 0;
  int total_pairs = 0;
  bool located = false;

  //Define a struct of the equipment pair, the number of occurences of
  //that pair in the global array, and the first index that the pair occurs
  typedef struct {
    char equipment1[MAX_NAME_LEN];
    char equipment2[MAX_NAME_LEN];
    int num_occurences;
    int first_index;
  } equipment_pairs;

  //if there are no missions in the file, return appropriate error
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }
  //initialize the struct
  equipment_pairs pairs[MAX_MISSIONS] = {0};

  //loop through all missions in the global mission counter
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //reset discovered variable every time a new mission is checked
    located = false;

    //loop through all pairs in the struct. This starts at 0, and increases
    //in size as values are added to the struct
    for (current_pair = 0; current_pair < total_pairs; current_pair++) {

      //Check if the current global pair equals the current struct pair
      //must check both ways, eg. rover and machine = machine and rover
      if (((strcmp(g_equipment[current_mission][0],
                   pairs[current_pair].equipment1) == 0) &&
           (strcmp(g_equipment[current_mission][1],
                   pairs[current_pair].equipment2) == 0)) ||
           ((strcmp(g_equipment[current_mission][0],
                    pairs[current_pair].equipment2) == 0) &&
           (strcmp(g_equipment[current_mission][1],
                   pairs[current_pair].equipment1) == 0))) {

        //set discovered variable to true after pair has been identified
        located = true;

        //add 1 to number of total occurences of current pair in struct array
        pairs[current_pair].num_occurences += 1;

        //check if the current pairs number of occurences is the highest
        //if so, set the highest index to the first occurence index of the
        //current pair and set the highest occurences to the current pair
        //occurences
        if (pairs[current_pair].num_occurences > highest_equipment_usage) {
          highest_equipment_usage = pairs[current_pair].num_occurences;
          highest_equipment_index = pairs[current_pair].first_index;
        }
      }
    }
    //if the pair was not located in the struct array
    //add it to the struct array, save its index as the first
    //occurence of the index, and add 1 to its occurence
    //finally, increment the total pairs variable as 1 has just
    //been added to the struct array
    if (!located) {
      strcpy(pairs[total_pairs].equipment1, g_equipment[current_mission][0]);
      strcpy(pairs[total_pairs].equipment2, g_equipment[current_mission][1]);
      pairs[total_pairs].first_index = current_mission;
      pairs[total_pairs].num_occurences += 1;
      total_pairs++;
    }
  }
  //initialize counter for how many times multiple pairs have the same
  //number of occurences
  int num_occur = 0;

  //loop through struct array and determine the number of pairs that
  //have the highest equipment usage.
  for (current_pair = 0; current_pair < total_pairs; current_pair++) {
    if (pairs[current_pair].num_occurences == highest_equipment_usage) {
      num_occur++;
    }

  }
  //if more than one pair has the the highest usage, return appropriate error
  if (num_occur > 1) {
    return MULTIPLE_PAIRS;
  }
  //finally, return the index of the highest occuring pair in the global array
  return highest_equipment_index;

} /* popular_equipment_pair() */



/*
 * This function loops through the array of missions, and calculates
 * the current experiment ratio of that experiment. It then compares
 * this value to the highest ratio, and replaces it if the value is
 * larger. After the loop, it returns the index of the mission with
 * the highest success ratio.
 */

int highest_success_mission() {

  //define constants to be used in this function
  double best_experiment_ratio = 0;
  double current_experiment_ratio = 0;
  int best_experiment_index = 0;
  int current_mission = 0;

  //if no missions are in global array, return appropriate value
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }
  //loop through each experiment in the global experiment array and
  //calculate the experiment ratio for the current mission.
  //Then compare this to the largest experiment ratio, and if it is
  //greater, replace it
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //calculate experiment ratio for the current mission experiment
    current_experiment_ratio = ((double) g_experiments[current_mission][1]) /
                               ((double) g_experiments[current_mission][0]);

    //check if the current ratio is larger that the previous best ratio
    //if it is larger, set the best ratio to the current ratio, and
    //set the index to the current index.
    if (current_experiment_ratio > best_experiment_ratio) {
      best_experiment_ratio = current_experiment_ratio;
      best_experiment_index = current_mission;
    }
  }
  //return the index of the best experiment ratio throughout all missions
  return best_experiment_index;

} /* highest_success_mission() */



/*
 * This function takes a mission type char parameter
 * and calculates the average mission duration over all
 * missions listed in the global array that match this mission type.
 * It returns a float of the average mission duration for the
 * entire mission array.
 */

float avg_mission_duration(char *mission_type) {

  //define constants to be used
  int start_year = 0;
  int start_month = 0;
  int start_day = 0;
  int end_year = 0;
  int end_month = 0;
  int end_day = 0;
  int current_mission = 0;
  int total_matching_missions = 0;
  int total_time = 0;
  bool located = false;
  double average_mission_time = 0;

  //if global mission count is empty, return appropriate error
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }
  //Loop through the global mission array and calculate the total
  //time spend on mission, and also get the total matching missions
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //ensure that the mission type input matches the mission type of the
    //current mission.
    if ((strcmp(mission_type,
                g_astronaut_and_mission[current_mission][2]) == 0)) {

      //tally 1 to number of missions that match the mission type
      total_matching_missions += 1;

      //reset constants before calculations
      start_year = 0;
      start_month = 0;
      start_day = 0;
      end_year = 0;
      end_month = 0;
      end_day = 0;

      //scan the date string and get the values for the starting
      //and ending dates respectively. Save the values for year, month
      //and day to their respective variables.
      sscanf(g_mission_dates[current_mission][0], "%d-%d-%d",
             &start_year, &start_month, &start_day);
      sscanf(g_mission_dates[current_mission][1], "%d-%d-%d",
             &end_year, &end_month, &end_day);

      //add the calculated time to the total mission time
      total_time += (calculate_hours(end_year, end_month, end_day) -
                     calculate_hours(start_year, start_month, start_day));

      //denote this variable as showcasing we have found at least one match
      located = true;

    }
  }
  //if a mission matching the mission type was not found
  //return the appropriate error
  if (!located) {
    return NO_SUCH_TYPE;
  }
  //calculate the average mission time by dividing the total
  //time by the total missions. Cast to double to ensure
  //integer division does not occur
  average_mission_time = ((double) total_time) /
                         ((double) total_matching_missions);

  //return the average mission time of the matching mission types
  //as a float
  return (float) average_mission_time;

} /* avg_mission_duration() */



/*
 * This function uses takes no parameters and uses the get_total_time
 * function to calculate which astronaut spend the longest amount of
 * time on missions.
 */

char *longest_mission_astronaut() {
  int current_mission_time = 0;
  int longest_mission_index = 0;
  int longest_mission_time = 0;
  int current_mission = 0;

  //ensure that there the mission count is not empty before continuing
  if (g_mission_count == 0) {
    return NULL;
  }
  //loop through the missions in the global array, get the total time
  //each astronaut has spend on missions, and find the index of that
  //astronaut
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //get the total time the current astronaut has spend on missions
    current_mission_time =
      get_total_time(g_astronaut_and_mission[current_mission][0]);

    //if the time returned by the get_total_time function is greater than
    //the current longest time, set the longest time to the current time
    //and set the saved index to the current index.
    if (current_mission_time > longest_mission_time) {
      longest_mission_time = current_mission_time;
      longest_mission_index = current_mission;
    }
  }
  //return the name of the astronaut with the most time
  //spend on missions
  return g_astronaut_and_mission[longest_mission_index][0];

} /* longest_mission_astronaut() */



/*
 * This function takes the parameter of an astronaut name and calculates
 * their efficiency score based on their number of successful experiments,
 * their total operational times for their equipment, and the success factor
 * which is based on the ratio of successful experiments conducted.
 */

float calc_efficiency_score(char *astronaut_name) {

  //define variables which will be used throughout the function
  int total_successful_experiments = 0;
  int total_attempt_experiments = 0;
  float experiment_ratio = 0;
  float success_factor = 0;
  float total_op_time1 = 0;
  float total_op_time2 = 0;
  int current_mission = 0;
  float efficiency_score = 0;
  bool located = false;

  //ensure that the mission count is not empty
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }
  //loop through the missions in the global array, comparing the
  //astronaut name to the current name in the loop. If they are the same
  //add the data from experiments and operational hours to the totals for
  //those variables
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //if the astronaut name matches the current name, add experiment and
    //operational hours data to their respective variables. Then set the
    //located variable to true to denote at least one instance of the
    //astronaut has been located
    if ((strcmp(astronaut_name,
                g_astronaut_and_mission[current_mission][0]) == 0)) {
      total_successful_experiments += g_experiments[current_mission][1];
      total_attempt_experiments += g_experiments[current_mission][0];
      total_op_time1 += g_op_hours[current_mission][0];
      total_op_time2 += g_op_hours[current_mission][1];
      located = true;
    }
  }
  //if no astronaut was located in the global array, return appropriate error
  if (!located) {
    return NO_SUCH_ASTRONAUT;
  }
  //calcalate the experiment ratio from the total and successful
  //experiment variables
  experiment_ratio = ((float) total_successful_experiments) /
                     ((float) total_attempt_experiments);

  //These if statements act as essentially a switch case for determining
  //the success factor to use, depending on the experiment ratio calculated
  if (experiment_ratio < LOW_EXP_RATIO) {
    success_factor = STAGE1_SUCCESS_FACTOR;
  }
  else if (experiment_ratio < MID_EXP_RATIO) {
    success_factor = STAGE2_SUCCESS_FACTOR;
  }
  else if (experiment_ratio < HIGH_EXP_RATIO) {
    success_factor = STAGE3_SUCCESS_FACTOR;
  }
  else if (experiment_ratio >= HIGH_EXP_RATIO) {
    success_factor = STAGE4_SUCCESS_FACTOR;
  }
  //This calculates the efficiency score using the formula listed in
  //the HW3 manual
  efficiency_score = ((total_successful_experiments) /
                      (total_op_time1 + total_op_time2)) * success_factor;

  //return the calculated efficiency score
  return efficiency_score;

} /* calc_efficiency_score() */


/*
 * This function takes an astronaut name as an input, and calculates
 * the most popular used equipment pair in the array of equipment pairs.
 * It only considers equipment that have been used by the specific astronaut
 * specified in the parameter. The most popular equipment pair is calculated
 * not by the number of hours spent, but the number of instances of
 * these pairs that occur in the global variables.
 */

int popular_equipment_pair_astronaut(char *astronaut_name) {

  //Define constants to be used in this function
  int highest_equipment_usage = 0;
  int highest_equipment_index = 0;
  int current_mission = 0;
  int current_pair = 0;
  int total_pairs = 0;
  bool located = false;

  //Define a struct of the equipment pair, the number of occurences of
  //that pair in the global array, and the first index that the pair occurs
  typedef struct {
    char equipment1[MAX_NAME_LEN];
    char equipment2[MAX_NAME_LEN];
    int num_occurences;
    int first_index;
  } equipment_pairs;

  //if there are no missions in the file, return appropriate error
  if (g_mission_count == 0) {
    return NO_DATA_POINTS;
  }
  //initialize the struct
  equipment_pairs pairs[MAX_MISSIONS] = {0};

  //loop through all missions in the global mission counter
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //reset check variable
    located = false;
    for (current_pair = 0; current_pair < total_pairs; current_pair++) {

      //Check if the current global pair equals the current struct pair
      //must check both ways, eg. rover and machine = machine and rover
      if (((strcmp(g_equipment[current_mission][0],
                   pairs[current_pair].equipment1) == 0) &&
           (strcmp(g_equipment[current_mission][1],
                   pairs[current_pair].equipment2) == 0)) ||
          ((strcmp(g_equipment[current_mission][0],
                   pairs[current_pair].equipment2) == 0) &&
           (strcmp(g_equipment[current_mission][1],
                   pairs[current_pair].equipment1) == 0))) {

        //ensure that the current astronaut name matches the specified
        //astronaut name before continuing
        if (strcmp(astronaut_name,
                   g_astronaut_and_mission[current_mission][0]) == 0) {

          //set discovered variable to true after pair has been identified
          located = true;

          //add 1 to number of total occurences of current pair in struct array
          pairs[current_pair].num_occurences += 1;
        }
      }
    }
    //if the pair was not located in the struct array and the
    //specified astronaut name equals the current astronaut name
    //add it to the struct array, save its index as the first
    //occurence of the index, and add 1 to its occurence
    //finally, increment the total pairs variable as 1 has just
    //been added to the struct array
    if ((!located) &&
        (strcmp(astronaut_name,
                g_astronaut_and_mission[current_mission][0]) == 0)) {
      strcpy(pairs[total_pairs].equipment1,
             g_equipment[current_mission][0]);
      strcpy(pairs[total_pairs].equipment2,
             g_equipment[current_mission][1]);
      pairs[total_pairs].first_index = current_mission;
      pairs[total_pairs].num_occurences += 1;
      total_pairs++;
    }
  }
  //loop through struct array and find the highest usage, and the
  //specified index
  for (current_pair = 0; current_pair < total_pairs; current_pair++) {
    if (pairs[current_pair].num_occurences > highest_equipment_usage) {
      highest_equipment_usage = pairs[current_pair].num_occurences;
      highest_equipment_index = pairs[current_pair].first_index;
    }
  }
  //initialize counter for how many times multiple pairs have the same
  //number of occurences
  int num_occur = 0;

  //loop through struct array and determine the number of pairs that
  //have the highest equipment usage.
  for (current_pair = 0; current_pair < total_pairs; current_pair++) {
    if (pairs[current_pair].num_occurences == highest_equipment_usage) {
      num_occur++;
    }
  }
  //if more than one pair has the the highest usage, return appropriate error
  if (num_occur > 1) {
    return MULTIPLE_PAIRS;
  }
  //finally, return the index of the highest occuring pair in the global array
  return highest_equipment_index;

}/* popular_equipment_pair_astronaut() */



/*
 * This function takes an outfile and an astronaut name as parameters
 * and generates a report on that astronaut, outputting it to the
 * specified out file.
 */

int generate_report(char *out_file, char *astronaut_name) {

  //Open output file and check for errors
  FILE *output = fopen(out_file, "w");
  if (output == NULL) {
    return FILE_WRITE_ERR;
  }
  //If there are no missions, return appropriate error
  if (g_mission_count == 0) {
    fclose(output);
    return NO_DATA_POINTS;
  }
  //define variables to be used throughout this function
  int current_mission = 0;
  int start_year = 0;
  int start_month = 0;
  int start_day = 0;
  int end_year = 0;
  int end_month = 0;
  int end_day = 0;
  int mission_time = 0;
  bool located = false;
  int total_missions_led = 0;
  int total_mission_time = 0;
  int highest_equipment_index = 0;

  //loop through each mission in the mission count, and print out
  //applicable information for summary specified in HW3 manual
  for (current_mission = 0; current_mission < g_mission_count;
       current_mission++) {

    //Compare the current name to the specified name, and if it matches
    //denote the located boolean as true, and add 1 to the missions led count.
    if (strcmp(astronaut_name,
               g_astronaut_and_mission[current_mission][0]) == 0) {

      located = true;
      total_missions_led++;

      //print the mission name
      fprintf(output, "%s:\n", g_astronaut_and_mission[current_mission][1]);

      //print the op hours for the first equipment of this mission
      fprintf(output, "Equipment 1 and operational hours -> %s | %f\n",
              g_equipment[current_mission][0], g_op_hours[current_mission][0]);

      //print the op hours for the second equipment of this mission
      fprintf(output, "Equipment 2 and operational hours -> %s | %f\n",
              g_equipment[current_mission][1], g_op_hours[current_mission][1]);

      //print out the mission type and start date for the current mission
      fprintf(output, "Mission Type -> %s\n",
              g_astronaut_and_mission[current_mission][2]);
      fprintf(output, "Mission Start Date -> %s\n",
              g_mission_dates[current_mission][0]);

      //reset date variables
      start_year = 0;
      start_month = 0;
      start_day = 0;
      end_year = 0;
      end_month = 0;
      end_day = 0;
      mission_time = 0;

      //calculate year, month, and day for the start and end dates
      sscanf(g_mission_dates[current_mission][0], "%d-%d-%d",
             &start_year, &start_month, &start_day);
      sscanf(g_mission_dates[current_mission][1], "%d-%d-%d",
             &end_year, &end_month, &end_day);

      //calculate the mission time using helper function calculate_hours
      mission_time = (calculate_hours(end_year, end_month, end_day) -
                      calculate_hours(start_year, start_month, start_day));

      //add mission time to total mission time, then print duration of mission
      total_mission_time += mission_time;
      fprintf(output, "Duration of the mission in hours -> %d\n", mission_time);
    }
  }
  //if no mission with the specified astronaut name was found, return error
  if (!located) {
    fclose(output);
    return NO_SUCH_ASTRONAUT;
  }
  //print out the summary title for the missions
  fprintf(output, "Overall Summary:\n");

  //print the total missions led, mission time, and maximum efficiency score
  //of the specified astronaut
  fprintf(output, "Total missions led by %s -> %d\n",
          astronaut_name, total_missions_led);
  fprintf(output, "Total mission time of %s -> %d\n",
          astronaut_name, total_mission_time);
  fprintf(output, "Maximum efficiency score of %s -> %f\n",
          astronaut_name, calc_efficiency_score(astronaut_name));

  //save index of the astronauts most popular equipment pair using the
  //helper function popular_equipment_pair_astronaut)
  highest_equipment_index = popular_equipment_pair_astronaut(astronaut_name);

  //if there are multiple pairs, print that, otherwise, print the most popular
  //equipment pair used by the specified astronaut
  if (highest_equipment_index < 0) {
    fprintf(output,
            "Most popular equipment pair used by %s -> multiple pairs\n",
            astronaut_name);
  }
  else {
    fprintf(output, "Most popular equipment pair used by %s -> %s,%s\n",
            astronaut_name, g_equipment[highest_equipment_index][0],
            g_equipment[highest_equipment_index][1]);
  }
  //close the output and return Success code
  fclose(output);
  return OK;

} /* generate_report() */
