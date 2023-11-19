import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, UNIT_KILOWATT_HOURS, ICON_FLASH, STATE_CLASS_TOTAL_INCREASING, DEVICE_CLASS_ENERGY

DEPENDENCIES = ["uart"]
CONF_UART_ID = "uart_id"

elli_modbus_sensor_ns = cg.esphome_ns.namespace("elli_modbus_sensor")
ElliModbusComponent = elli_modbus_sensor_ns.class_(
    "ElliModbusComponent", sensor.Sensor, cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = sensor.sensor_schema(
    ElliModbusComponent,
    unit_of_measurement=UNIT_KILOWATT_HOURS,
    icon=ICON_FLASH,
    accuracy_decimals=2,
    state_class=STATE_CLASS_TOTAL_INCREASING,
    device_class=DEVICE_CLASS_ENERGY,
).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
