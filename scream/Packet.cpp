#include "Packet.h"
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <endian.h>

using namespace std;

RtpPacket::RtpPacket(uint32_t ssrc, uint32_t size, uint16_t seq_num)
{
  header.ssrc = ssrc;
  header.seq_num = seq_num;
  payload = move(string(size, 'x'));
}

RtpPacket::RtpPacket(std::string &str)
{
  if (str.size() < sizeof(header)) {
    cerr << "RTP packet too small to contain header" << endl; 
    exit(EXIT_FAILURE);
  }

  const char *header_ptr = reinterpret_cast<const char *>(str.data()); 
  header.ssrc = ntohl(*(uint32_t *) header_ptr);
  header.seq_num = ntohs(*(uint16_t *) (header_ptr + 4));
  payload = move(string(str.begin() + sizeof(header), str.end()));
}

string RtpPacket::to_string()
{
  uint32_t net_ssrc = htonl(header.ssrc);
  string ssrc_str(reinterpret_cast<const char *>(&net_ssrc),
                  sizeof(net_ssrc));
  uint16_t net_seq_num = htons(header.seq_num);
  string seq_num_str(reinterpret_cast<const char *>(&net_seq_num), 
                     sizeof(net_seq_num));

  return ssrc_str + seq_num_str + payload;
}

RtcpPacket::RtcpPacket(uint32_t ssrc, uint16_t ack_seq_num,
                       uint16_t num_loss, uint64_t recv_timestamp)
{
  header.ssrc = ssrc;
  header.ack_seq_num = ack_seq_num;
  header.num_loss = num_loss;
  header.recv_timestamp = recv_timestamp;
}

RtcpPacket::RtcpPacket(string &str)
{
  if (str.size() < sizeof(header)) {
    cerr << "RTCP packet too small to contain header" << endl; 
    exit(EXIT_FAILURE);
  }

  const char *header_ptr = reinterpret_cast<const char *>(str.data()); 
  header.ssrc = ntohl(*(uint32_t *) header_ptr);
  header.ack_seq_num = ntohs(*(uint16_t *) (header_ptr + 4));
  header.num_loss = ntohs(*(uint16_t *) (header_ptr + 6));
  header.recv_timestamp = be64toh(*(uint64_t *) (header_ptr + 8));
}

void RtcpPacket::set_recv_timestamp(uint64_t recv_timestamp)
{
  header.recv_timestamp = recv_timestamp;
}

string RtcpPacket::to_string()
{
  uint32_t net_ssrc = htonl(header.ssrc);
  string ssrc_str(reinterpret_cast<const char *>
      (&net_ssrc), sizeof(net_ssrc));

  uint16_t net_ack_seq_num = htons(header.ack_seq_num);
  string ack_seq_num_str(reinterpret_cast<const char *>
      (&net_ack_seq_num), sizeof(net_ack_seq_num));

  uint16_t net_num_loss = htons(header.num_loss);
  string num_loss_str(reinterpret_cast<const char *>
      (&net_num_loss), sizeof(net_num_loss));

  uint64_t net_recv_timestamp = htobe64(header.recv_timestamp);  
  string recv_timestamp_str(reinterpret_cast<const char *>
      (&net_recv_timestamp), sizeof(net_recv_timestamp));

  return ssrc_str + ack_seq_num_str + num_loss_str + recv_timestamp_str;
}
