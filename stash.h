#ifndef STASH_H
#define STASH_H
void stashCreator(char* message);
void stashList();
void stashShow(char* stashIndex);
void stashPop(char* stashIndex, int state);
void stashClear();
#endif