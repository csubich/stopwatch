#ifndef STOPWATCH_STOPWATCH_H
#define STOPWATCH_STOPWATCH_H

// Function status constants
#define STOPWATCH_OK 0
#define STOPWATCH_ERR (-1)

#define STOPWATCH_MAX_EVENTS 10
#define NULL_TERM_MAX_ROUTINE_NAME_LEN 16

// =====================================================================================================================
// Valid Events
// =====================================================================================================================
enum StopwatchEvents {
  L1_CACHE_MISS = 1,
  L2_CACHE_MISS,
  L3_CACHE_MISS,
  BRANCH_MISPREDICT,
  BRANCH_PREDICT,
  CYCLES_STALLED_RESOURCE,
  TOTAL_CYCLES,
  SP_FLOAT_OPS,
  DP_FLOAT_OPS
};

// =====================================================================================================================
// Structure holding results for a specific entry
// =====================================================================================================================
struct StopwatchMeasurementResult {
  long long total_real_cyc;
  long long total_real_usec;
  long long total_event_values[STOPWATCH_MAX_EVENTS];
  long long total_times_called;
  char routine_name[NULL_TERM_MAX_ROUTINE_NAME_LEN];
  unsigned int stack_depth;
  unsigned int num_of_events;
  int event_names[STOPWATCH_MAX_EVENTS];
};

// =====================================================================================================================
// Monotonic clock initialization and destruction
// =====================================================================================================================
// TODO: Change all unsigned to size_t
// Initializes the event timers. Currently the events that are measured are hard coded. This will also start the
// monotonic measurement clock as currently it is assumed that consumers would immediately start the clock after
// initializing the stopwatch structure.
int stopwatch_init(const enum StopwatchEvents *events_to_add, unsigned int num_of_events);

// Stops the monotonic event timers and cleans up resources used by the timer. Interestingly valgrind still reports a
// memory leak with the PAPI specific resources
void stopwatch_destroy();

// =====================================================================================================================
// Operations
// =====================================================================================================================

// Records the current values on the monotonic event timers
int stopwatch_record_start_measurements(int routine_call_num, const char *function_name, unsigned int stack_depth);

// Records the current values on the monotonic event timers. Will also perform a delta between the values recorded from
// `stopwatch_record_start_measurements` and the current values to give a measurement on the profile on the procedure executing
// between the calls of `stopwatch_record_start_measurements` and `stopwatch_record_end_measurements`
int stopwatch_record_end_measurements(int routine_call_num);

int stopwatch_get_measurement_results(unsigned int routine_call_num, struct StopwatchMeasurementResult *result);

// Prints out the results
void stopwatch_print_measurement_results(struct StopwatchMeasurementResult *result);

// Generates a table as a pretty printed string
void stopwatch_print_result_table();

#endif //STOPWATCH_STOPWATCH_H
