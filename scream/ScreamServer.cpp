#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <poll.h>

#include "socket.hh"
#include "timestamp.hh" 
#include "util.hh"

#include "ScreamRx.h"

using namespace std;

void recvRtp(ScreamRx *screamRx, UDPSocket &socket) 
{
}

int main(int argc, char *argv[]) 
{
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " PORT" << endl; 
    return EXIT_FAILURE;
  }

  UDPSocket socket;
  socket.bind(Address("0", argv[1]));
  cerr << "Listening on " << socket.local_address().to_string() << endl;

  ScreamRx *screamRx = new ScreamRx();

  struct pollfd fds[1];
  fds[0].fd = socket.fd_num();

  while (true) {
    SystemCall("poll", poll(fds, 1, -1));

    /* Incoming RTP packet */
    if (fds[0].revents | POLLIN) {
      recvRtp(screamRx, socket);
    }
  }

  delete screamRx;

  return EXIT_SUCCESS;
}
