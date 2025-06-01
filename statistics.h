#ifndef STATISTICS_H
#define STATISTICS_H

int showUserStatus(int problemNum, char ID[]);
void showTotalStatus(int problemNum);
void calculateAverage(int problemNum);
void append_log(int problemNum, const char* ID, int score, const char* submissionTime);

#endif
