#include <Pixy.h>

Pixy pixy;

void setup() {
  Serial.begin(9600);
  pixy.init();
}

void loop() {
  int blocks = pixy.getBlocks();

  if (blocks) {
    for (int i = 0; i < blocks; i++) {
      if (pixy.blocks[i].signature == 1) {
        Serial.println("Red object detected");
      } else if (pixy.blocks[i].signature == 2) {
        Serial.println("Green object detected");
      } else if (pixy.blocks[i].signature == 3) {
        Serial.println("Blue object detected");
      }
    }
  }
}
