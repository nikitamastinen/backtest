#include "TcpServerSocket.hpp"


namespace controller {

int run()
{
  TcpServerSocket server("127.0.0.1", 8000);

  server.acceptConnection();

  char message[100];

  server.receiveData(message, 100);

  printf("Client said: %s\n", message);

  strcpy(message, "Hello from server!");

  server.sendData(message, 100);

  server.closeConnection();

  return 0;
}

}  // namespace controller
