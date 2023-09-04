#pragma onece

#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>

enum class FError {
  NoError = 0,
  Error = 1,
};

enum class FInstruct {
  EXIT = 0,
  MOVE = 1,
  RMOVE = 2,
  CLICK = 3,
};

class DFouse {
public:
  DFouse();
  ~DFouse();

private:
  void bind();
  void listen();
  void log(FError, std::string);

  int sockfd;
  struct sockaddr_un addr;
};
