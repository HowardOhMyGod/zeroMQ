#include "zhelpers.h"
#include <unistd.h>

int main(int argc, char *argv[]){
  void *context = zmq_ctx_new();
  void *subscriber = zmq_socket(context, ZMQ_SUB);

  printf("Connect to subscriber...\n");
  int rc = zmq_connect(subscriber, "tcp://localhost:5558");
  assert(rc == 0);

  char *filter = "Howard ";
  rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE,
                      filter, strlen(filter));
  assert(rc == 0);

  int64_t start_time = s_clock();
  int64_t wait_rcv_time;
  for(int i = 0; i < 100; i++){
    char *string = s_recv(subscriber);

    if(i == 0){
      wait_rcv_time = s_clock() - start_time;
    }

    free(string);
  }

  printf("Wait %d msec for first rcv.\n", (int) wait_rcv_time);

  zmq_close (subscriber);
  zmq_ctx_destroy (context);
  return 0;
}
