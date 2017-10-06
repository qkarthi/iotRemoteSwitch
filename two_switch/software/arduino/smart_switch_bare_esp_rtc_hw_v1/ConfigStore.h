struct ConfigStore {
  uint32_t  magic;
  char      version[9];
  uint8_t   flagConfig: 1;
  uint8_t   flagApFail: 1;
  uint8_t   flagSelfTest: 1;

  char      wifiSSID[34];
  char      wifiPass[34];

  char      cloudToken[34];
  char      cloudHost[34];
  uint16_t  cloudPort;

  char      equip_name[16];

  char      dev_1_name[16];
  char      dev_2_name[16];
  ////////////////////////////////////////////////////
  uint8_t scheduler_1_activation_flag: 1;

  uint8_t scheduler_1_sun_flag: 1;
  uint8_t scheduler_1_mon_flag: 1;
  uint8_t scheduler_1_tues_flag: 1;
  uint8_t scheduler_1_wednes_flag: 1;
  uint8_t scheduler_1_thurs_flag: 1;
  uint8_t scheduler_1_fri_flag: 1;
  uint8_t scheduler_1_satur_flag: 1;

  uint16_t scheduler_1_selected_device;

  uint16_t scheduler_1_on_hour;
  uint16_t scheduler_1_on_minute;
  uint16_t scheduler_1_off_hour;
  uint16_t scheduler_1_off_minute;
  ////////////////////////////////////////////////////////
  uint8_t scheduler_2_activation_flag: 1;

  uint8_t scheduler_2_sun_flag: 1;
  uint8_t scheduler_2_mon_flag: 1;
  uint8_t scheduler_2_tues_flag: 1;
  uint8_t scheduler_2_wednes_flag: 1;
  uint8_t scheduler_2_thurs_flag: 1;
  uint8_t scheduler_2_fri_flag: 1;
  uint8_t scheduler_2_satur_flag: 1;
  
  uint16_t scheduler_2_selected_device;

  uint16_t scheduler_2_on_hour;
  uint16_t scheduler_2_on_minute;
  uint16_t scheduler_2_off_hour;
  uint16_t scheduler_2_off_minute;
  ///////////////////////////////////////////////////////
  uint16_t  checksum;
} __attribute__((packed));

ConfigStore configStore;

const ConfigStore configDefault = {
  0x626C6E6B,
  BOARD_FIRMWARE_VERSION,
  0, 0, 0,
  "",
  "",
  "invalid token",
  "blynk-cloud.com", 8442,
  BOARD_NAME, "device 1", "device 2",
  0, 0, 0, 0, 0, 0, 0, 0, 1, 00, 00, 00, 00,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 00, 00, 00, 00,
  0
};

#include <EEPROM.h>
#define EEPROM_CONFIG_START 0

void config_load()
{
  EEPROM.get(EEPROM_CONFIG_START, configStore);
  if (configStore.magic != configDefault.magic) {
    DEBUG_PRINT("Using default config.");
    configStore = configDefault;
    return;
  }
}

bool config_save()
{
  EEPROM.put(EEPROM_CONFIG_START, configStore);
  EEPROM.commit();
  return true;
}

bool config_init()
{
  EEPROM.begin(sizeof(ConfigStore));
  config_load();
  return true;
}

void config_reset()
{
  DEBUG_PRINT("Resetting configuration!");
  configStore = configDefault;
  config_save();
  BlynkState::set(MODE_WAIT_CONFIG);
}

template<typename T, int size>
void CopyString(const String& s, T(&arr)[size]) {
  s.toCharArray(arr, size);
}

