/* Copyright 2019 Neil Kettle
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "jj_ergo.h"

#ifdef RIGHT_HALF
bool i2c_initialized = 0;
i2c_status_t mcp23018_status = I2C_STATUS_ERROR;

uint8_t init_mcp23018(void)
{
  uint8_t data[3];
  mcp23018_status = I2C_STATUS_ERROR;

  // I2C subsystem
  if (i2c_initialized == 0) {
    i2c_init();  // on pins D(1,0)
    i2c_initialized = true;
    _delay_ms(1000);
  }

  // set pin direction
  // - unused  : input  : 1
  // - input   : input  : 1
  // - driving : output :
  data[0] = IODIRA;
  data[1] = 0b00000000;
  data[2] = 0b11111111;
  mcp23018_status = i2c_transmit(I2C_ADDR_WRITE, (uint8_t *)data, 3, MCP23018_I2C_TIMEOUT);
  if (mcp23018_status != I2C_STATUS_SUCCESS)
    goto out;

  // set pull-up
  // - unused  : on  : 1
  // - input   : on  : 1
  // - driving : off : 0
  data[0] = GPPUA;
  data[1] = 0b00000000;
  data[2] = 0b11111111;
  mcp23018_status = i2c_transmit(I2C_ADDR_WRITE, (uint8_t *)data, 3, MCP23018_I2C_TIMEOUT);
  if (mcp23018_status != I2C_STATUS_SUCCESS)
    goto out;

  //  set logical value (doesn't matter on inputs)
  // - unused  : hi-Z : 1
  // - input   : hi-Z : 1
  // - driving : hi-Z : 1
  data[0] = OLATA;
  data[1] = 0b11111111;
  data[2] = 0b11111111;
  mcp23018_status = i2c_transmit(I2C_ADDR_WRITE, (uint8_t *)data, 3, MCP23018_I2C_TIMEOUT);

out:
  return (mcp23018_status);
}
#endif

// Optional override functions below.
// You can leave any or all of these undefined.
// These are only required if you want to perform custom actions.

__attribute__ ((weak))
void matrix_init_user(void) {}

void matrix_init_kb(void) {
    // put your keyboard start-up code here
    // runs once when the firmware starts up

    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_user(void) {}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)

    matrix_scan_user();
}
