#include <client.h>
#include <server.h>

//#include <pthread.h>

int main() {
    //pthread_t serverThread;                                                                                    // Server thread.
    //pthread_create(&serverThread, NULL, server_listener, &(sla_t){.port = "8000", .callback_function = NULL}); // Start server thread.

    getc(stdin);
    request_get(s_url); // Send GET request to server.

    //pthread_cancel(serverThread); // Stop server thread.
    return 0;
}