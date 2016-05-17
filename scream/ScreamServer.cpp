#include <iostream>
#include <cstdlib>

#include "socket.hh"
#include "util.hh"
#include "ScreamRx.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " PORT" << endl; 
    return EXIT_FAILURE;
  }

  UDPSocket socket;
  socket.bind(Address("0", argv[1]));

  cerr << "Listening on " << socket.local_address().to_string() << endl;

  UDPSocket::received_datagram recd = socket.recv();
  cout << recd.payload << endl;

  ScreamRx *screamRx = new ScreamRx();

  delete screamRx;

  return EXIT_SUCCESS;
}
