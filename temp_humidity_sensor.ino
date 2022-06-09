#include <DHT.h>
#include <DHT_U.h>
#include <arduino_homekit_server.h>
#include <WiFiManager.h>
#include <BlockNot.h>

namespace 
{
  DHT dht(D4, DHT11);

  BlockNot notifyTimer(10, SECONDS);

  extern "C" homekit_characteristic_t cha_temperature;
  extern "C" homekit_characteristic_t cha_humidity;
  extern "C" homekit_server_config_t config;

  auto report_temperature() -> void
  {
    auto temperature = dht.readTemperature();
    Serial.printf("Temperature: %f\t", temperature);

    cha_temperature.value.float_value = temperature;
    homekit_characteristic_notify(&cha_temperature, cha_temperature.value);
  }

  auto report_humidity() -> void
  {
    auto humidity = dht.readHumidity();
    Serial.printf("Humidity: %f\t", humidity);

    cha_humidity.value.float_value = humidity;
    homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
  }

  auto notify_homekit() -> void
  {
    report_temperature();
    report_humidity();

    Serial.printf("\n");
  }
}

auto setup() -> void
{
  Serial.begin(9600);

  WiFiManager wifi;
  wifi.autoConnect("AutoConnectAP");
  Serial.printf("WiFi connected.\n");
  
  arduino_homekit_setup(&config);
  Serial.printf("Homekit setup complete.\n");

  dht.begin();
}

auto loop() -> void
{
  arduino_homekit_loop();
  if (notifyTimer.triggered()) notify_homekit();
}
