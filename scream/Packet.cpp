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
