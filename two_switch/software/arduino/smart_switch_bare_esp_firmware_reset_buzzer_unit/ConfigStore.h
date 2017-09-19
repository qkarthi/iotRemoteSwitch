struct ConfigStore {
  uint32_t  magic;
  char      version[9];
  uint8_t   flagConfig:1;
  uint8_t   flagApFail:1;
  uint8_t   flagSelfTest:1;
  
  char      wifiSSID[34];
  char      wifiPass[34];
  
  char      cloudToken[34];
  char      cloudHost[34];
  uint16_t  cloudPort;

  char      equip_name[34];
  
  char      mspace_1[24]; //dev1
  char      mspace_2[24]; //dev2
 
  char      mspace_3[24]; //on
  char      mspace_4[24]; //off

  char      mspace_5[24]; //on
  char      mspace_6[24]; //off

  char      mspace_7[24];
  char      mspace_8[24];
  
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
  BOARD_NAME,"no name","no name","","","","","","",0
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

