#include "dfouse.hpp"
#include <sys/syslog.h>
#include <sys/un.h>

DFouse::DFouse() {
  // unix socket
  this->sockfd = ::socket(AF_UNIX, SOCK_STREAM, 0);
  if (this->sockfd == -1) {
    this->log(FError::Error, "socket() failed");
    exit(1);
  }
  this->bind();
}

void DFouse::bind() {

  std::memset(&this->addr, 0x00, sizeof(this->addr));
  this->addr.sun_family = AF_UNIX;

  // sock file in /run/dfouse.sock
  std::strcpy(this->addr.sun_path, "/run/dfouse.sock");

  // bind unix socket
  if (::bind(this->sockfd, (struct sockaddr *)&this->addr, sizeof(this->addr)) <
      0) {
    this->log(FError::Error, "bind() failed");
    exit(1);
  }
}

void DFouse::listen() {}

void DFouse::log(FError error, std::string message) {
  syslog(error == FError::NoError ? LOG_INFO : LOG_ERR, "%s", message.c_str());
}
