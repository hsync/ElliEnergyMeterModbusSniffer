#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace elli_modbus_sensor {

class ElliModbusComponent : public sensor::Sensor, public Component, public uart::UARTDevice {
 public:
  int find_pattern(const char *array, int len);
  int read_uart(int readch, char *buffer, int len);
  void compute_data(char *buffer);
  void loop() override;
  void dump_config() override;
};

}  // namespace elli_modbus_sensor
}  // namespace esphome