#include <iostream>
#include <cstdlib>

#include "socket.hh"
#include "util.hh"
#include "ScreamTx.h"
#include "RtpQueue.h"
#include "VideoEnc.h"

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

  ScreamTx *screamTx = new ScreamTx();
  RtpQueue *rtpQueue = new RtpQueue();

  float frameRate = 25.0f;
  VideoEnc *videoEnc = new VideoEnc(rtpQueue, frameRate, 0.3f);
  screamTx->registerNewStream(rtpQueue, 10, 1.0f, 5000.0f, 50000.0f, frameRate);

  delete screamTx;
  delete rtpQueue;
  delete videoEnc;

  return EXIT_SUCCESS;
}
