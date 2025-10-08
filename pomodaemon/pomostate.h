#ifndef __POMOSTATE_H__
#define __POMOSTATE_H__

enum Phase { SHORT_REST, LONG_REST, WORK };

struct PomoState {
  enum Phase phase;
  int stopped;
  long remaining_seconds;
  int session_count;
  int sessions_since_lr;
};

#endif
