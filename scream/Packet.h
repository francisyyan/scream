#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <stdint.h>

struct RtpPacket
{
  struct Header {
    uint32_t ssrc;
    uint16_t seq_num;
  } header;

  std::string payload; 

  /* Create RTP packet with dummy payload */
  RtpPacket(uint32_t ssrc, uint32_t size, uint16_t seq_num); 

  /* Parse incoming RTP packet */
  RtpPacket(std::string &str);

  /* Make wire representation of packet */
  std::string to_string();
};

struct RtcpPacket
{
  struct Header {
    uint32_t ssrc;
    uint16_t ack_seq_num;
    uint16_t num_loss; 
    uint32_t recv_timestamp; 
  } header;

  /* Create RTCP packet */
  RtcpPacket(uint32_t ssrc, uint16_t ack_seq_num,
             uint16_t num_loss, uint32_t recv_timestamp); 

  /* Parse incoming RTCP packet */
  RtcpPacket(std::string &str);

  /* Fill in the recv_timestamp field */
  void set_recv_timestamp(uint32_t recv_timestamp);

  /* Make wire representation of packet */
  std::string to_string();
};

#endif /* PACKET_H */
