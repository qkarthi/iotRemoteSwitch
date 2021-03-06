/**************************************************************
   This is a DEMO. You can use it only for development and testing.

   If you would like to add these features to your product,
   please contact Blynk for Business:

                    http://www.blynk.io/

 **************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>

ESP8266WebServer server(WIFI_AP_CONFIG_PORT);
ESP8266HTTPUpdateServer httpUpdater;

unsigned long fall_to_error_mode = 0;
unsigned long wifi_fail_to_connect = 0;
DNSServer dnsServer;
const byte DNS_PORT = 53;
unsigned long ap_timeout_millis = 0;
bool DFConfig = false;
const char* config_form = R"html(
<!DOCTYPE HTML><html>
<form method='get' action='config'>
  <label>WiFi SSID: </label><input type="text" name="ssid" length=32 required="required"><br/>
  <label>Password:  </label><input type="text" name="pass" length=32><br/>
  <label>Auth token:</label><input type="text" name="blynk" placeholder="a0b1c2d..." pattern="[a-zA-Z0-9]{32}" maxlength="32" required="required"><br/>
  <label>Host: </label><input type="text" name="host" length=32><br/>
  <label>Port: </label><input type="number" name="port" value="80" min="1" max="65535"><br/>
  <input type='submit' value="Apply">
</form>
</html>
)html";

void restartMCU() {
  
        DEBUG_PRINT("INFO - esp restart via sw");
  ESP.restart();
}

void enterConfigMode()
{
  WiFi.mode(WIFI_OFF);
  DEBUG_PRINT("INFO - WIFI OFF");
  delay(100);
  WiFi.mode(WIFI_AP);
  DEBUG_PRINT("INFO - WIFI AP");
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  String HOTSPOT_SSID_STRING=String(PRODUCT_WIFI_SSID)+String("_")+String(WiFi.macAddress());
  char SSID_CHAR[HOTSPOT_SSID_STRING.length()];
  HOTSPOT_SSID_STRING.toCharArray(SSID_CHAR, HOTSPOT_SSID_STRING.length()+1);
  WiFi.softAP(SSID_CHAR);
  delay(500);
  IPAddress myIP = WiFi.softAPIP();
  DEBUG_PRINT(String("AP SSID: ") + SSID_CHAR);
  DEBUG_PRINT(String("AP IP:   ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);

  if (myIP == (uint32_t)0)
  {
    BlynkState::set(MODE_ERROR);
    
    DEBUG_PRINT("INFO - ip not generated");
    return;
  }

  // Set up DNS Server
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
#ifdef WIFI_CAPTIVE_PORTAL_ENABLE
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // Point all to our IP
  server.onNotFound(handleRoot);
#else
  dnsServer.start(DNS_PORT, BOARD_CONFIG_AP_URL, WiFi.softAPIP());
  DEBUG_PRINT(String("AP URL:  ") + BOARD_CONFIG_AP_URL);
#endif

  httpUpdater.setup(&server);

  server.on("/", []() {
    server.send(200, "text/html", config_form);
  });
  server.on("/config", []() {
    String ssid = server.arg("ssid");
    String ssidManual = server.arg("ssidManual");
    String pass = server.arg("pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = server.arg("blynk");
    String host  = server.arg("host");
    String port  = server.arg("port");

    String content;
    unsigned statusCode;

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("Blynk cloud: ") + token + " @ " + host + ":" + port);

    if (token.length() == 32 && ssid.length() > 0) {
      configStore.flagConfig = false;
      CopyString(ssid, configStore.wifiSSID);
      CopyString(pass, configStore.wifiPass);
      CopyString(token, configStore.cloudToken);
      if (host.length()) {
        CopyString(host,  configStore.cloudHost);
      }
      if (port.length()) {
        configStore.cloudPort = port.toInt();
      }

      DFConfig=true;
      configStore.Dev_1_state = false;
      
      configStore.Dev_2_state = false;
      
      content = R"json({"status":"ok","msg":"Configuration saved"})json";
      statusCode = 200;
      server.send(statusCode, "application/json", content);

      BlynkState::set(MODE_SWITCH_TO_STA);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
      statusCode = 404;
      server.send(statusCode, "application/json", content);
    }
  });
  server.on("/board_info.json", []() {
    char buff[256];
    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","vendor":"%s","tmpl_id":"%s","fw_ver":"%s","hw_ver":"%s"})json",
      BOARD_NAME,
      BOARD_VENDOR,
      BOARD_TEMPLATE_ID,
      BOARD_FIRMWARE_VERSION,
      BOARD_HARDWARE_VERSION
    );
    server.send(200, "application/json", buff);
  });
  server.on("/reset", []() {
    BlynkState::set(MODE_RESET_CONFIG);
    server.send(200, "application/json", R"json({"status":"ok","msg":"Configuration reset"})json");
  });
  server.on("/reboot", []() {
    restartMCU();
  });

  server.begin();
  ap_timeout_millis=millis();
  while (BlynkState::is(MODE_WAIT_CONFIG) || BlynkState::is(MODE_CONFIGURING)) {
    
    if(millis()-ap_timeout_millis>180000){
       WiFi.mode(WIFI_OFF);
        delay(1000);
      WiFi.mode(WIFI_STA);
       restartMCU();
    }
    
    dnsServer.processNextRequest();
    server.handleClient();
    if (BlynkState::is(MODE_WAIT_CONFIG) && WiFi.softAPgetStationNum() > 0) {
      BlynkState::set(MODE_CONFIGURING);
    } else if (BlynkState::is(MODE_CONFIGURING) && WiFi.softAPgetStationNum() == 0) {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  server.stop();
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);
  
  WiFi.mode(WIFI_STA);
   
  DEBUG_PRINT("INFO : wifi station mode initiated");
  if (!WiFi.begin(configStore.wifiSSID, configStore.wifiPass))
    return;
  
  unsigned long timeoutMs = millis() + WIFI_NET_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) && (WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    if (!BlynkState::is(MODE_CONNECTING_NET)) {
      WiFi.disconnect();
      
       DEBUG_PRINT("INFO : disconnecting wifi");
      return;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    
  DEBUG_PRINT("INFO : wifi connection success");
  
  DEBUG_PRINT("INFO : MAC ADDRESS");
  DEBUG_PRINT(String(WiFi.macAddress()));
  
  DEBUG_PRINT("INFO : LAN IP ADDRESS");
  DEBUG_PRINT(WiFi.localIP());
    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else {
    BlynkState::set(MODE_ERROR);
    
       DEBUG_PRINT("ERROR : wifi not connected");
  }
}

void enterConnectCloud() {
  BlynkState::set(MODE_CONNECTING_CLOUD);
 DEBUG_PRINT("INFO : disconnecting blynk - enter connect cloud region");

  Blynk.disconnect();
  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  
  DEBUG_PRINT(String("cloudToken: ") + configStore.cloudToken);
  DEBUG_PRINT(String("cloudHost: ") + configStore.cloudHost);
  DEBUG_PRINT(String("cloudPort: ") + configStore.cloudPort);
  
  Blynk.connect(0);
 DEBUG_PRINT("INFO : blynk config and connect initiated ");

  unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) &&
        (Blynk.connected() == false))
  {
    Blynk.run();
    if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
      Blynk.disconnect();
      return;
    }
  }
  
  if (Blynk.connected()) {
    
DEBUG_PRINT("INFO : blynk connected");
    BlynkState::set(MODE_RUNNING);

    if (!configStore.flagConfig) {
      configStore.flagConfig = true;
      config_save();
      DEBUG_PRINT("Configuration stored to flash");
    }
  } else {
    
    DEBUG_PRINT("ERROR : blynk not connected");
    BlynkState::set(MODE_ERROR);
  }
}

void enterSwitchToSTA() {
  BlynkState::set(MODE_SWITCH_TO_STA);

  DEBUG_PRINT("Switching to STA...");

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  BlynkState::set(MODE_CONNECTING_NET);
}

void enterError() {
    BlynkState::set(MODE_ERROR);
    fall_to_error_mode++;

    DEBUG_PRINT("fall_to_error");
    DEBUG_PRINT(fall_to_error_mode);
    
  if (configStore.flagConfig) {
     BlynkState::set(MODE_DUMB);

  int n = WiFi.scanNetworks();

  if (n > 0)
  {
    DEBUG_PRINT("No.of - WIFI - found = "+ String(n));
    for (int i = 0; i < n; ++i)
    { // check weather the ssid match with saved ssid
      
      DEBUG_PRINT(WiFi.SSID(i) +"-" +String(WiFi.RSSI(i)));
      if (WiFi.SSID(i) == configStore.wifiSSID )
      {
        if(WiFi.status() == WL_CONNECTED){
        DEBUG_PRINT("ERROR - INFO - ROUTER-SSID  found");
        wifi_fail_to_connect++;
        DEBUG_PRINT("wifi_fail_to_connect");
        DEBUG_PRINT(wifi_fail_to_connect);
        
        }else{
          DEBUG_PRINT("INFO - connected to wifi router");
          
        }
       
       
       //WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
       // wifi_auth_error_count++;
       //Serial.println(wifi_auth_error_count);
      }
    }
  }
  }else{
  
  unsigned long timeoutMs = millis() + 10000;
  while (timeoutMs > millis())
  {
    delay(10);
    if (!BlynkState::is(MODE_ERROR)) {
      return;
    }
  }
  DEBUG_PRINT("Restarting after error.");
  delay(10);

  restartMCU();
}
}

