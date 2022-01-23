#include <Arduino_AVRSTL.h>
#include <utility>
#include <string>
#include <array>

int const led_pin = LED_BUILTIN;
int const adc_pin = 7;
int const digital_button1_pin = 2;
int const digital_button2_pin = 3;
int led_state = LOW;

void setup() {
  //use the debug LED to output a heartbeat
  pinMode(led_pin, OUTPUT);

  pinMode(digital_button1_pin, INPUT);
  pinMode(digital_button2_pin, INPUT);

  Serial.begin(57600);
  Serial.println("Setup finished");
}

void loop() {
  led_state = blink_led(led_state);

  log_buttons();

  int const loop_period_ms = 200;
  delay(loop_period_ms);
}

void log_buttons() {
  int adc_value = analogRead(adc_pin);
  auto button = poll_analogue_buttons(adc_value);
  if (button != "")
    Serial.println(button.c_str());

  button = poll_digital_buttons();
  if (button != "")
    Serial.println(button.c_str());
}

/// ADC value coresponds to voltage that drops to earth through buttons resistors  
std::string poll_analogue_buttons(unsigned adc_value) {
  using abutton_array = std::array<std::pair<std::string, unsigned>, 5>;
  abutton_array abuttons = {{"S1", 30}, {"S2", 150}, {"S3", 360}, {"S4", 535}, {"S5", 760}};

  for (auto const &btn : abuttons)
    if (adc_value < btn.second) {
      return btn.first + " button";
    }
  return "";
}

std::string poll_digital_buttons() {
  using dbutton_array = std::array<std::pair<std::string, int>, 2>;
  dbutton_array dbuttons = {{"S6", digital_button1_pin}, {"S7", digital_button2_pin}};
  
  for (auto const &btn : dbuttons)
    if (digitalRead(btn.second) == LOW) {
      return btn.first + " button";
    }
  return "";
}

int blink_led(int prev_state) {
  int state = (prev_state == LOW) ? HIGH : LOW;
  digitalWrite(led_pin, state);
  return state;
}
