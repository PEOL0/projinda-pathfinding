#include "../../include/networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wsserver/ws.h>
#include "pathfinding.h"
#include "heightinfo.h"

/**
 * @brief This function is called whenever a new connection is opened.
 * @param client Client connection.
 */
void onopen(ws_cli_conn_t client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
}

/**
 * @brief This function is called whenever a connection is closed.
 * @param client Client connection.
 */
void onclose(ws_cli_conn_t client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
}

/**
 * @brief Message events goes here.
 * @param client Client connection.
 * @param msg    Message content.
 * @param size   Message size.
 * @param type   Message type.
 */
void onmessage(ws_cli_conn_t client,
    const unsigned char *msg, uint64_t size, int type)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("I receive a message: %s (%zu), from: %s\n", msg,
        size, cli);

    int startX, startY, targetX, targetY;
    if (sscanf((const char*)msg, "%d,%d,%d,%d", &startX, &startY, &targetX, &targetY) == 4) {
        printf("Received coordinates: start(%d,%d) target(%d,%d)\n", startX, startY, targetX, targetY);
        
        Grid* grid = createGrid(listgorareCols, listgorareRows, listgorare());
        Node** path = findPath(grid, startX, startY, targetX, targetY);
        
        if (path != NULL) {
            int pathLength = 0;
            while (path[pathLength] != NULL) {
                pathLength++;
            }

            char* response = (char*)malloc(pathLength * 20);
            response[0] = '\0';
            
            for (int i = 0; i < pathLength; i++) {
                char nodeStr[20];
                sprintf(nodeStr, "%d,%d;", path[i]->x, path[i]->y);
                strcat(response, nodeStr);
            }

            ws_sendframe_txt(client, response);
            
            free(response);
            freePath(path);
        } else {
            ws_sendframe_txt(client, "No path found");
        }

        freeGrid(grid);
    } else {
        ws_sendframe_txt(client, "Error: Message format should be 'startX,startY,targetX,targetY'");
    }
}

int serverLoop(void)
{
    /*
     * Main loop, this function never* returns.
     *
     * *If the .thread_loop is != 0, a new thread is created
     * to handle new connections and ws_socket() becomes
     * non-blocking.
     */
    ws_socket(&(struct ws_server){
        /*
         * Bind host, such as:
         * localhost -> localhost/127.0.0.1
         * 0.0.0.0   -> global IPv4
         * ::        -> global IPv4+IPv6 (Dual stack)
         */
        .host = "localhost",
        .port = 8080,
        .thread_loop   = 0,
        .timeout_ms    = 1000,
        .evs.onopen    = &onopen,
        .evs.onclose   = &onclose,
        .evs.onmessage = &onmessage
    });

    return (0);
}

int main() {
    serverLoop();
    return 0;
}