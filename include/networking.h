#ifndef NETWORKING_H
#define NETWORKING_H

#include <ws.h>

/**
 * @brief This function is called whenever a new connection is opened
 * @param client Client connection
 */
void onopen(ws_cli_conn_t client);

/**
 * @brief This function is called whenever a connection is closed
 * @param client Client connection
 */
void onclose(ws_cli_conn_t client);

/**
 * @brief This function is called whenever a message is received
 * @param client Client connection
 * @param msg Message content
 * @param size Message size
 * @param type Message type
 */
void onmessage(ws_cli_conn_t client, const unsigned char *msg,
    uint64_t size, int type);

/**
 * @brief Starts the WebSocket server and runs the main event loop
 * @return 0 on successful completion
 */
int serverLoop(void);

#endif /* NETWORKING_H */