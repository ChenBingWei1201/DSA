#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "tm_usage.h"

TmUsage *createTmUsage() {
  TmUsage *tmUsage = malloc(sizeof(TmUsage));
  if (!tmUsage)
    return NULL;

  // Initialize function pointers
  tmUsage->totalStart = totalStart;
  tmUsage->periodStart = periodStart;
  tmUsage->getTotalUsage = getTotalUsage;
  tmUsage->getPeriodUsage = getPeriodUsage;
  tmUsage->checkUsage = checkUsage;

  // Initialize statistics
  tmUsage->tStart_.uTime = 0;
  tmUsage->tStart_.sTime = 0;
  tmUsage->tStart_.rTime = 0;
  tmUsage->tStart_.vmSize = 0;
  tmUsage->tStart_.vmPeak = 0;
  tmUsage->tStart_.vmDiff = 0;
  tmUsage->pStart_.uTime = 0;
  tmUsage->pStart_.sTime = 0;
  tmUsage->pStart_.rTime = 0;
  tmUsage->pStart_.vmSize = 0;
  tmUsage->pStart_.vmPeak = 0;
  tmUsage->pStart_.vmDiff = 0;

  return tmUsage;
}

void destroyTmUsage(TmUsage *tmUsage) {
  if (tmUsage) {
    free(tmUsage);
  }
}

bool totalStart(TmUsage *self) {
  return self->checkUsage(self, &self->tStart_);
}

bool periodStart(TmUsage *self) {
  return self->checkUsage(self, &self->pStart_);
}

bool getTotalUsage(const TmUsage *self, TmStat *st) {
  if (!self->checkUsage(self, st))
    return false;
  st->uTime -= self->tStart_.uTime;
  st->sTime -= self->tStart_.sTime;
  st->rTime -= self->tStart_.rTime;
  st->vmDiff = st->vmSize - self->tStart_.vmSize;
  st->vmPeak =
      st->vmPeak > self->tStart_.vmPeak ? st->vmPeak : self->tStart_.vmPeak;
  return true;
}

bool getPeriodUsage(const TmUsage *self, TmStat *st) {
  if (!self->checkUsage(self, st))
    return false;
  st->uTime -= self->pStart_.uTime;
  st->sTime -= self->pStart_.sTime;
  st->rTime -= self->pStart_.rTime;
  st->vmDiff = st->vmSize - self->pStart_.vmSize;
  st->vmPeak =
      st->vmPeak > self->pStart_.vmPeak ? st->vmPeak : self->pStart_.vmPeak;
  return true;
}

bool checkUsage(const TmUsage *self, TmStat *st) {
  struct rusage tUsg;
  struct timeval tReal;
  getrusage(RUSAGE_SELF, &tUsg);
  gettimeofday(&tReal, NULL);
  st->uTime = tUsg.ru_utime.tv_sec * 1000000 + tUsg.ru_utime.tv_usec;
  st->sTime = tUsg.ru_stime.tv_sec * 1000000 + tUsg.ru_stime.tv_usec;
  st->rTime = tReal.tv_sec * 1000000 + tReal.tv_usec;

  FILE *fmem = fopen("/proc/self/status", "r");
  if (!fmem) {
    fprintf(stderr, "**ERROR TmUsage::checkUsage(): cannot get memory usage\n");
    st->vmSize = 0;
    st->vmPeak = 0;
    return false;
  }
  char membuf[128];
  while (fgets(membuf, 128, fmem)) {
    char *ch;
    if ((ch = strstr(membuf, "VmPeak:"))) {
      st->vmPeak = atol(ch + 7);
      continue;
    }
    if ((ch = strstr(membuf, "VmSize:"))) {
      st->vmSize = atol(ch + 7);
      break;
    }
  }
  fclose(fmem);
  return true;
}
