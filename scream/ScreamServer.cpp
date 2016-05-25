#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <poll.h>

#include "socket.hh"
#include "timestamp.hh" 
#include "util.hh"
#include "timerfd.hh"

#include "ScreamRx.h"
#include "Packet.h"

using namespace std;

bool debug = false;

/* Send RTCP feedback */
void sendRtcp(ScreamRx *screamRx, UDPSocket &socket)
{
  uint32_t ssrc;
  /* recv_timestamp_ms will be filled in with recv_timestamp_us 
   * passed in screamRx->receive() divided by 1000 */
  uint32_t recv_timestamp_ms; 
  uint16_t ack_seq_num;
  uint8_t num_loss;
  if (screamRx->getFeedback(timestamp_us(), ssrc, 
      recv_timestamp_ms, ack_seq_num, num_loss)) {
    RtcpPacket rtcpPacket(ssrc, ack_seq_num, (uint16_t) num_loss, recv_timestamp_ms);
    socket.send(rtcpPacket.to_string());

    if (debug) {
      cerr << "Sent a RTCP packet acking sequence number " << ack_seq_num
        << " at time " << recv_timestamp_ms << endl;
    }
  }
}

/* Receive incoming RTP packet and generate RTCP feedback */
void recvRtp(ScreamRx *screamRx, UDPSocket &socket, Timerfd &feedbackTimer) 
{
  /* Feedback rate limits the target bitrate */
  static uint64_t feedbackInterval_us = 10000; 
  static bool knowClient = false;

  UDPSocket::received_datagram recd = socket.recv();
  uint64_t recv_timestamp_us = recd.timestamp * 1000;

  /* Connect to client after knowing its address */
  if (!knowClient) {
    knowClient = true;
    socket.connect(recd.source_address);
  }

  /* Assemble RTP packet */
  RtpPacket rtpPacket(recd.payload);

  if (debug) {
    cerr << "Received a RTP packet of size " << rtpPacket.payload.size()
      << " with sequence number " << rtpPacket.header.seq_num
      << " at time " << recd.timestamp << endl;
  }

  screamRx->receive(recv_timestamp_us, 0, rtpPacket.header.ssrc, 
                    (int) rtpPacket.payload.size(), rtpPacket.header.seq_num); 

  if (screamRx->isFeedback()) {
    uint64_t sinceLastFeedback_us = timestamp_us() - screamRx->getLastFeedbackT();
    if (sinceLastFeedback_us > feedbackInterval_us) {
      sendRtcp(screamRx, socket);
    } else {
      /* Pace the pending feedbacks */ 
      if (feedbackTimer.is_disarmed())
        feedbackTimer.arm((int) ((feedbackInterval_us - sinceLastFeedback_us) / 1000));
    }
  }
}

int main(int argc, char *argv[]) 
{
  if (argc == 3 && string(argv[2]) == "debug") {
    debug = true;
  } else if (argc != 2) {
    cerr << "Usage: " << argv[0] << " PORT [debug]" << endl; 
    return EXIT_FAILURE;
  }

  uint64_t start_time = timestamp_ms();
  cerr << "Server starts at " << start_time << endl;

  /* UDP socket for server */
  UDPSocket socket;
  socket.set_timestamps();
  socket.bind(Address("0", argv[1]));
  cerr << "Listening on " << socket.local_address().to_string() << endl;

  ScreamRx *screamRx = new ScreamRx();

  Timerfd feedbackTimer(TFD_NONBLOCK);

  /* Always read the timer that returned POLLIN before arm it again */
  struct pollfd fds[2];
  fds[0].fd = feedbackTimer.fd_num();
  fds[0].events = POLLIN;
  fds[1].fd = socket.fd_num();
  fds[1].events = POLLIN;

  while (true) {
    SystemCall("poll", poll(fds, 2, -1));

    /* Feedback timer expires */
    if (fds[0].revents & POLLIN) {
      if (feedbackTimer.expirations() > 0)
        sendRtcp(screamRx, socket);
    }

    /* Incoming RTP packet */
    if (fds[1].revents & POLLIN) {
      recvRtp(screamRx, socket, feedbackTimer);
    }
  }

  delete screamRx;

  return EXIT_SUCCESS;
}
