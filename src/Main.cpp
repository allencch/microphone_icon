#include <iostream>
#include "MicrophoneIcon.hpp"

using namespace std;

int main(int argc, char** argv) {
  MicrophoneIcon microphoneIcon;
  int status = microphoneIcon.run(argc, argv);
  return status;
}
