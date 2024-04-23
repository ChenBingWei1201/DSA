#ifndef TM_USAGE_H
#define TM_USAGE_H

#include <stdbool.h>

// Define a data structure to store time and memory usage
typedef struct {
  long vmSize; // in kilobytes
  long vmPeak; // in kilobytes
  long vmDiff; // in kilobytes
  long rTime;  // real time in microseconds
  long uTime;  // user time in microseconds
  long sTime;  // system time in microseconds
} TmStat;

// Forward declaration of the struct
typedef struct TmUsage TmUsage;

struct TmUsage {
  TmStat tStart_;                     // store start times and memory usage
  TmStat pStart_;                     // store periodic times and memory usage
  bool (*totalStart)(TmUsage *self);  // start the total timer
  bool (*periodStart)(TmUsage *self); // start the period timer
  bool (*getTotalUsage)(const TmUsage *self,
                        TmStat *st); // get the total time and memory usage
  bool (*getPeriodUsage)(const TmUsage *self,
                         TmStat *st); // get the period time and memory usage
  bool (*checkUsage)(const TmUsage *self,
                     TmStat *st); // internal check to update the usage stats
};

// Function prototypes
TmUsage *createTmUsage();              // Create a TmUsage instance
void destroyTmUsage(TmUsage *tmUsage); // Destroy a TmUsage instance

// Function declarations (to be implemented in tm_usage.c)
bool totalStart(TmUsage *self);
bool periodStart(TmUsage *self);
bool getTotalUsage(const TmUsage *self, TmStat *st);
bool getPeriodUsage(const TmUsage *self, TmStat *st);
bool checkUsage(const TmUsage *self, TmStat *st);

#endif /* TM_USAGE_H */
