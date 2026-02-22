#pragma once
#include "libraries.h"
#include "data.h"

void saveValuesToEEPROM();
void loadValuesFromEEPROM();
void clearEEPROM();
void reset();

constexpr uint8_t EEPROM_dry_run_cutoff_protection_ADDR = 0;
constexpr uint8_t EEPROM_dry_run_cutoff_power_1_ADDR = 1;
constexpr uint8_t EEPROM_dry_run_cutoff_power_2_ADDR = 2;
constexpr uint8_t EEPROM_dry_run_cutoff_power_3_ADDR = 3;
constexpr uint8_t EEPROM_dry_run_cutoff_power_4_ADDR = 4;
constexpr uint8_t EEPROM_dry_run_cutoff_delay_ADDR = 5;
constexpr uint8_t EEPROM_tank_full_cutoff_protection_ADDR = 6;
constexpr uint8_t EEPROM_tank_full_cutoff_level_ADDR = 7;
constexpr uint8_t EEPROM_tank_full_cutoff_delay_ADDR = 8;
constexpr uint8_t EEPROM_dashboard_style_ADDR = 9;
constexpr uint8_t EEPROM_led_strip_style_ADDR = 10;
constexpr uint8_t EEPROM_overload_cutoff_protection_ADDR = 11;
constexpr uint8_t EEPROM_overload_cutoff_power_1_ADDR = 12;
constexpr uint8_t EEPROM_overload_cutoff_power_2_ADDR = 13;
constexpr uint8_t EEPROM_overload_cutoff_power_3_ADDR = 14;
constexpr uint8_t EEPROM_overload_cutoff_power_4_ADDR = 15;
constexpr uint8_t EEPROM_overload_cutoff_delay_ADDR = 16;
