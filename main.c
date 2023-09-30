#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

const int NF_ACCEPT;

static int Callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
    struct nfq_data *nfa, void *data)
{
  struct nfqnl_msg_packet_hdr *ph;
  unsigned char *packetData;
  int id = 0;

  ph = nfq_get_msg_packet_hdr(nfa);
  if (ph) {
    id = ntohl(ph->packet_id);
  }

  int ret = nfq_get_payload(nfa, &packetData);
  if (ret >= 0) {
    printf("Packet ID: %d, Payload Length: %d\n", id, ret);
  }

  return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

int main(){

  struct nfq_handle *h;
  struct nfq_q_handle *qh;
  struct nfnl_handle *nh;
  int fd;
  int rv;
  char buf[4096] __attribute__((aligned));

  printf("Starting netfilterqueue...\n");

  // Initialize nfq
  h = nfq_open();
  if (!h) {
    fprintf(stderr, "Error opening nfq!\n");
    return 1;
  }

  // Unbind existing nf_queue handler (if any)
  if (nfq_unbind_pf(h, AF_INET) < 0) {
    fprintf(stderr, "Error unbinding existing nf_queue handler!\n");
    return 1;
  }

  // Bind nf_queue handler
  if (nfq_bind_pf(h, AF_INET) < 0) {
    fprintf(stderr, "Error binding nf_queue handler!\n");
    return 1;
  }

  // Create a queue
  qh = nfq_create_queue(h,  0, &Callback, NULL);
  if (!qh) {
    fprintf(stderr, "Error creating nf_queue!\n");
    return 1;
  }

  // Set the copy mode to copy the whole packet
  if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
    fprintf(stderr, "Could not set packet copy mode!\n");
    return 1;
  }

  // Get the file descriptor associated with the queue
  fd = nfq_fd(h);

  // Main processing loop
  while ((rv = recv(fd, buf, sizeof(buf), 0))) {
    nfq_handle_packet(h, buf, rv);
  }

  // Cleanup
  nfq_destroy_queue(qh);
  nfq_close(h);

  printf("Main 0/n");
  return 0;
}
