#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <poll.h>

#include "socket.hh"
#include "timestamp.hh"
#include "util.hh"
#include "timerfd.hh"

#include "ScreamTx.h"
#include "RtpQueue.h"
#include "VideoEnc.h"
#include "Packet.h"

using namespace std;

const uint32_t SSRC = 10;
const uint64_t tmax_ms = 15000; // run 15s

void encodeVideoFrame(VideoEnc *videoEnc, ScreamTx *screamTx)
{
  float frameRate = videoEnc->frameRate;
  float targetBitrate = screamTx->getTargetBitrate(SSRC);
  videoEnc->setTargetBitrate(targetBitrate);

  uint64_t timestamp = timestamp_ms();
  int rtpBytes = videoEnc->encode((float) timestamp / 1000);
  screamTx->newMediaFrame(timestamp * 1000, SSRC, rtpBytes);
}

void sendRtp(ScreamTx *screamTx, RtpQueue *rtpQueue,
             Timerfd &txTimer, UDPSocket &socket)
{
  uint64_t timestamp_us = timestamp_ms() * 1000;
  uint32_t ssrc;
  float dT = screamTx->isOkToTransmit(timestamp_us, ssrc); 

  /* RTP packet with ssrc can be immediately transmitted */
  if (dT == 0.0f) {
    void *rtpPacketNull;
    int size;
    uint16_t seqNr;
    rtpQueue->sendPacket(rtpPacketNull, size, seqNr); 
    /* rtpPacketNull is still NULL because SCReAM's API doesn't send real packets */ 
  }

  /* isOkToTransmit should be called after dT seconds */
  if (dT > 0.0f) {
  }

  /* No RTP packet available to transmit */
  if (dT == -1.0f)
    return;
}

void recvRtcp(ScreamTx *screamTx, UDPSocket &socket)
{
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " HOST PORT" << endl; 
    return EXIT_FAILURE;
  }

  UDPSocket socket;
  socket.connect(Address(argv[1], argv[2]));

  float kFrameRate = 25.0f;
  ScreamTx *screamTx = new ScreamTx();
  RtpQueue *rtpQueue = new RtpQueue();
  VideoEnc *videoEnc = new VideoEnc(rtpQueue, kFrameRate, 0.1f, false, false, 5);
  screamTx->registerNewStream(rtpQueue, SSRC, 1.0f, 64000.0f, 5e6, kFrameRate);

  Timerfd txTimer(TFD_NONBLOCK);
  Timerfd videoTimer(TFD_NONBLOCK);
  videoTimer.arm(0, (int) (1e3 / kFrameRate));
  
  struct pollfd fds[3];
  fds[0].fd = txTimer.fd_num();
  fds[0].events = POLLIN;
  fds[1].fd = videoTimer.fd_num();
  fds[1].events = POLLIN;
  fds[2].fd = socket.fd_num();
  fds[2].events = POLLIN;

  while (true) {
    uint64_t timestamp = timestamp_ms();
    if (timestamp > tmax_ms)
      break;

    SystemCall("poll", poll(fds, 2, -1));

    /* Tx Timer expires */
    if (fds[0].revents | POLLIN) {
      if (txTimer.expirations() > 0)
        sendRtp(screamTx, rtpQueue, txTimer, socket);
    }

    /* Video Timer expires */
    if (fds[1].revents | POLLIN) {
      if (videoTimer.expirations() > 0) {
        encodeVideoFrame(videoEnc, screamTx);
        if (txTimer.isDisarmed())
          sendRtp(screamTx, rtpQueue, txTimer, socket);
      }
    }

    /* Incoming RTCP feedback */
    if (fds[2].revents | POLLIN) {
      recvRtcp(screamTx, socket); 
      if (txTimer.isDisarmed())
        sendRtp(screamTx, rtpQueue, txTimer, socket);
    }
  }

  delete screamTx;
  delete rtpQueue;
  delete videoEnc;

  return EXIT_SUCCESS;
}
