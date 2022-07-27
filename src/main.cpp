#include <Arduino.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // set IO2 as input
  // https://embarcados.com.br/configurando-gpio-do-esp8266-01/#GPIO0-e-GPIO2-como-Input
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
}

int currentValue;

// the loop function runs over and over again forever
void loop() {
  currentValue = digitalRead(0);
  digitalWrite(LED_BUILTIN, currentValue);
  delay(50);
}
