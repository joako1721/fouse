#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <string>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

class Fouse {
public:
  void parse_command(int argc, char **argv) {
    if (argc < 0) {
      std::cerr << "No arguments provided try with -h or --help" << std::endl;
      exit(1);
    }

    // ./fouse -m|--mode <move|click>
    // fouse  move <x> <y>
    // fouse click <button>
    // fouse move_relative <x> <y>

    for (int x = 0; x < argc; x++) {
      if (std::string(argv[x]) == "-h" || std::string(argv[x]) == "--help") {
        std::cout << "-h|--help" << std::endl
                  << "-m|--mode <move|click>" << std::endl
                  << "----------------------" << std::endl
                  << "move <x> <y>" << std::endl
                  << "click <button>" << std::endl
                  << "rmove <x> <y>" << std::endl;
        exit(0);
      }

      if (std::string(argv[x]) == "-m" || std::string(argv[x]) == "--mode") {
        std::string mode = std::string(argv[x + 1]);
        if (mode == "move") {
          int _x = std::stoi(argv[x + 2]);
          int _y = std::stoi(argv[x + 3]);
          this->move(_x, _y);
          exit(0);
        } else if (mode == "click") {
          int button = std::stoi(argv[x + 2]);
          this->click(button);
          exit(0);
        } else if (mode == "rmove") {
          int _x = std::stoi(argv[x + 2]);
          int _y = std::stoi(argv[x + 3]);
          this->rmove(_x, _y);
          exit(0);
        }
      }
    }
  }

  Fouse() { this->display = XOpenDisplay(NULL); }

  ~Fouse() { XCloseDisplay(this->display); }

public:
  Display *display;

  void click(int button, int time_pressed = 50) {
    if (this->display == NULL) {
      std::cerr << "Can't open display" << std::endl;
      exit(1);
    }

    XTestFakeButtonEvent(this->display, button, 1, CurrentTime);
    XFlush(this->display);
    this->sleep(time_pressed);
    XTestFakeButtonEvent(this->display, button, 0, CurrentTime);
    XFlush(this->display);
  }

  void move(int x, int y) {
    if (this->display == NULL) {
      std::cerr << "Can't open display" << std::endl;
      exit(1);
    }

    int cur_x, cur_y;
    this->coords(&cur_x, &cur_y);
    XWarpPointer(this->display, None, None, 0, 0, 0, 0, -cur_x, -cur_y);
    XWarpPointer(this->display, None, None, 0, 0, 0, 0, x, y);
    XFlush(this->display);
  }

  void coords(int *x, int *y) {
    if (this->display == NULL) {
      std::cerr << "Can't open display" << std::endl;
      exit(1);
    }
    XEvent event;
    XQueryPointer(this->display, DefaultRootWindow(this->display),
                  &event.xbutton.root, &event.xbutton.window,
                  &event.xbutton.x_root, &event.xbutton.y_root,
                  &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    XFlush(this->display);
    *x = event.xbutton.x;
    *y = event.xbutton.y;
  }

  void rmove(int x, int y) {
    if (this->display == NULL) {
      std::cerr << "Can't open display" << std::endl;
      exit(1);
    }
    XWarpPointer(this->display, None, None, 0, 0, 0, 0, x, y);
    XFlush(this->display);
  }

  void sleep(int amount) {
    std::this_thread::sleep_for(std::chrono::milliseconds(amount));
  }
};

int main(int argc, char **argv) {
  Fouse fouse;
  fouse.parse_command(argc, argv);
  return 0;
}
