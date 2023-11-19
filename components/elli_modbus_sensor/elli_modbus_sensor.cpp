#include "elli_modbus_sensor.h"
#include "esphome/core/log.h"


namespace esphome
{
    namespace elli_modbus_sensor
    {

        static const char *TAG = "elli_modbus_sensor.sensor";


        int ElliModbusComponent::find_pattern(const char *array, int len)
        {
            for (int i = 0; i < len - 1; i++)
                if (array[i] == 0x01 && array[i + 1] == 0x03)
                    return i;

            return -1;
        }

        int ElliModbusComponent::read_uart(int readch, char *buffer, int len)
        {
            static int pos = 0;
            static bool record = false;

            // found 0x01 this could maybe the start of a message, lets start saving
            if (readch == 0x01 && record == false)
            {
                record = true;
                pos = 0;
            }
            
            if (record == false)
                return 0;

            // save byte in buffer
            if (pos < len - 1)
            {
                buffer[pos++] = readch;
                buffer[pos] = 0;
            }
            else
            {
                record = false;
            }

            // if we fond the pattern (0x01, 0x03) adress and read register command,
            // chances are high that we found the start of a new message
            // if not abort..
            if (pos == 2)
            {
                if (find_pattern(buffer, 2) == -1)
                {
                    record = false;
                    return 0;
                }
            }
            // if we found the pattern again, we found the start of the next message
            // -> we completet reading one message
            else if (pos > 3)
            {
                if (find_pattern(&buffer[2], pos - 2) > 0)
                {
                    pos = 2;
                    return pos;
                }
            }
            return 0;
        }

        void ElliModbusComponent::compute_data(char *buffer)
        {
            static bool request_found = false;
            int addr = (buffer[2] << 8) + buffer[3];
            char msg_buffer[50];
            float pre_val = 0;
            float value = 0;

            // read out the meter reading response
            if (request_found)
            {
                request_found = false;
                value = ((uint64_t)buffer[10] + ((uint64_t)buffer[9] << 8) + ((uint64_t)buffer[8] << 16) + ((uint64_t)buffer[7] << 24) + ((uint64_t)buffer[6] << 32) + ((uint64_t)buffer[5] << 40) + ((uint64_t)buffer[4] << 48) + ((uint64_t)buffer[3] << 56)) / 100.0;
                // small plausibilty check, no more than 1kwh jump allowed
                if(pre_val < 1)
                    value = value;
                else if (value-pre_val > 1)
                {          
                    value = pre_val;               
                }
                pre_val = value;
                publish_state(value);
            }
            // if address 0x5000 is requested wallbox controller is aking energy meter for meter reading
            if (addr == 0x5000)
                request_found = true;
        }

        void ElliModbusComponent::loop()
        {
            const int max_line_length = 20;
            static char buffer[max_line_length];
            while (available())
            {
                if (read_uart(read(), buffer, max_line_length) > 0)
                {
                    compute_data(buffer);
                }
            }
        }

        void ElliModbusComponent::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Elli Modbus Sensor");
        }

    } // namespace elli_modbus_sensor
} // namespace esphome