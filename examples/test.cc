#include <iostream>
#include <unistd.h>
#include "timerfd.hh"

using namespace std;

int main()
{
  Timerfd timer(TFD_NONBLOCK);
  timer.arm(1);
  sleep(1);
  cout << timer.expirations() << endl;
}
