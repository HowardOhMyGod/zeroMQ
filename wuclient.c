//  Weather update client
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode

#include "zhelpers.h"

int main (int argc, char *argv [])
{
    //  Socket to talk to server
    printf ("Collecting updates from weather server...\n");
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:5556");
    assert (rc == 0);

    //  Subscribe to zipcode, default is NYC, 10001
    char *filter_one = (argc > 1)? argv [1]: "10001 ";
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                         filter_one, strlen (filter_one));
    assert (rc == 0);


    char *filter_two = (argc > 2)? argv[2]: "10002 ";
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                     filter_two, strlen (filter_two));
    assert (rc == 0);

    //  Process 100 updates
    int update_one_nbr = 0;
    int update_two_nbr = 0;
    int update_nbr = 0;

    long total_one_temp = 0;
    long total_two_temp = 0;

    for (update_nbr = 0; update_nbr < 100; update_nbr++) {
        char *string = s_recv (subscriber);

        int zipcode, temperature, relhumidity;
        sscanf (string, "%d %d %d",
            &zipcode, &temperature, &relhumidity);

        // printf("zipcode: %d\n", zipcode);
        if(atoi(filter_one) == zipcode){
            total_one_temp += temperature;
            update_two_nbr ++;
          }

      	if(atoi(filter_two) == zipcode){
            total_two_temp += temperature;
            update_one_nbr ++;
          }

        free (string);
    }
    printf("filter_one: %d\n", atoi(filter_one));
    printf("filter_two: %d\n", atoi(filter_two));
    printf("update_one_nbr: %d\n", update_one_nbr);
    printf("update_two_nbr: %d\n", update_two_nbr);

    printf ("Average temperature for zipcode '%s' was %dF\n",
        filter_one, (int) (total_one_temp / update_one_nbr));

    printf ("Average temperature for zipcode '%s' was %dF\n",
        filter_two, (int) (total_two_temp / update_two_nbr));

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
