#include <iostream>
#include <cstdlib>

#include "socket.hh"
#include "util.hh"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " HOST PORT" << endl; 
    return EXIT_FAILURE;
  }

  UDPSocket socket;
  socket.connect(Address(argv[1], argv[2]));

  cerr << "Sending to " << socket.peer_address().to_string() << endl;

  socket.send("Hello!");

  return EXIT_SUCCESS;
}
