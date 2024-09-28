#include <cstdio>
#include <string>

#include "HelloWorld.h"

CHelloWorld::CHelloWorld() {
  mSecret = 42;
  std::printf("Constructor: mSecret=%d\n",mSecret);
}

CHelloWorld::~CHelloWorld() {

}

bool CHelloWorld::HelloWorld(void) {
  std::printf("HelloWorld: mSecret=%d\n",mSecret);

  std::string sentence = "Hello";
  std::printf("TEST=%s\n",sentence.c_str());

  if (mSecret == 42) {
    std::printf("CHelloWorld: HelloWorld: Hello, world!\n");
    return true;
  }
  else {
    std::printf("CHelloWorld: HelloWorld: CONSTRUCTION FAILED!\n");
    return false;
  }
}