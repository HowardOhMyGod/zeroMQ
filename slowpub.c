#include "zhelpers.h"

int main(int argc, char *argv[]){
  void *context = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  int rc = zmq_bind(publisher, "tcp://*:5558");
  assert(rc == 0);

  int i = 0;
  while(1){
    s_send(publisher, "Howard ");
    i++;
  }

  zmq_close (publisher);
  zmq_ctx_destroy (context);
  return 0;
}
