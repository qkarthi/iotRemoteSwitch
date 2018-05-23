void prepareIds() {
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
            (uint16_t) ((chipId >> 16) & 0xff),
            (uint16_t) ((chipId >>  8) & 0xff),
            (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistent_uuid = "Socket-1_0-" + serial;
}

void startHttpServer1()
{

  HTTP1.on("/upnp/control/basicevent1", HTTP_POST,  []() {
    String request = HTTP1.arg(0);
    if (request.indexOf("SetBinaryState") > 0 ) {
      if (request.indexOf("<BinaryState>1</BinaryState>") > 0)
      {
        // turn on relay with voltage HIGH
        d1_state  = HIGH;
        d1_update_func(0, 0);
        Blynk.virtualWrite(V_D1ManButPin, d1_state);
      }

      if (request.indexOf("<BinaryState>0</BinaryState>") > 0) {

        // turn on relay with voltage LOW
        d1_state  = LOW;
        d1_update_func(0, 0);
        Blynk.virtualWrite(V_D1ManButPin, d1_state);
      }
    }
    HTTP1.send(200, "text/plain", "");
  });


  HTTP1.on("/eventservice.xml", HTTP_GET, []() {
    String eventservice_xml = "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
                              "<actionList>"
                              "<action>"
                              "<name>SetBinaryState</name>"
                              "<argumentList>"
                              "<argument>"
                              "<retval/>"
                              "<name>BinaryState</name>"
                              "<relatedStateVariable>BinaryState</relatedStateVariable>"
                              "<direction>in</direction>"
                              "</argument>"
                              "</argumentList>"
                              "<serviceStateTable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>BinaryState</name>"
                              "<dataType>Boolean</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>level</name>"
                              "<dataType>string</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "</serviceStateTable>"
                              "</action>"
                              "</scpd>\r\n"
                              "\r\n";
    HTTP1.send(200, "text/plain", eventservice_xml.c_str());
  });

  HTTP1.on("/setup.xml", HTTP_GET, []() {
    String setup_xml = "<?xml version=\"1.0\"?>"
                       "<root>"
                       "<device>"
                       "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                       "<friendlyName>" +  String(configStore.device_1_name) + "</friendlyName>"
                       "<manufacturer>Belkin International Inc.</manufacturer>"
                       "<modelName>Socket</modelName>"
                       "<modelNumber>3.1415</modelNumber>"
                       "<UDN>uuid:" + persistent_uuid + "1" + "</UDN>"
                       "<serialNumber>221517K0101769</serialNumber>"
                       "<binaryState>0</binaryState>"
                       "<serviceList>"
                       "<service>"
                       "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                       "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                       "<controlURL>/upnp/control/basicevent1</controlURL>"
                       "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                       "<SCPDURL>/eventservice.xml</SCPDURL>"
                       "</service>"
                       "<service>"
                       "<serviceType>urn:Belkin:service:metainfo:1</serviceType>"
                       "<serviceId>urn:Belkin:serviceId:metainfo1</serviceId>"
                       "<controlURL>/upnp/control/metainfo1</controlURL>"
                       "<eventSubURL>/upnp/event/metainfo1</eventSubURL>"
                       "<SCPDURL>/metainfoservice.xml</SCPDURL>"
                       "</service>"
                       "</serviceList>"
                       "</device>"
                       "</root>\r\n"
                       "\r\n";
    HTTP1.send(200, "text/xml", setup_xml.c_str());
  });
  HTTP1.begin();
}
void startHttpServer2()
{

  HTTP2.on("/upnp/control/basicevent1", HTTP_POST,  []() {
    String request = HTTP2.arg(0);
    if (request.indexOf("SetBinaryState") > 0 ) {
      if (request.indexOf("<BinaryState>1</BinaryState>") > 0)
      {
        // turn on relay with voltage HIGH
        d2_state  = HIGH;
        d2_update_func(0, 0);
        Blynk.virtualWrite(V_D2ManButPin, d2_state);
      }

      if (request.indexOf("<BinaryState>0</BinaryState>") > 0) {

        // turn on relay with voltage LOW
        d2_state  = LOW;
        d2_update_func(0, 0);
        Blynk.virtualWrite(V_D2ManButPin, d2_state);
      }
    }
    HTTP2.send(200, "text/plain", "");
  });


  HTTP2.on("/eventservice.xml", HTTP_GET, []() {

    String eventservice_xml = "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
                              "<actionList>"
                              "<action>"
                              "<name>SetBinaryState</name>"
                              "<argumentList>"
                              "<argument>"
                              "<retval/>"
                              "<name>BinaryState</name>"
                              "<relatedStateVariable>BinaryState</relatedStateVariable>"
                              "<direction>in</direction>"
                              "</argument>"
                              "</argumentList>"
                              "<serviceStateTable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>BinaryState</name>"
                              "<dataType>Boolean</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>level</name>"
                              "<dataType>string</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "</serviceStateTable>"
                              "</action>"
                              "</scpd>\r\n"
                              "\r\n";
    HTTP2.send(200, "text/plain", eventservice_xml.c_str());
  });

  HTTP2.on("/setup.xml", HTTP_GET, []() {

    String setup_xml = "<?xml version=\"1.0\"?>"
                       "<root>"
                       "<device>"
                       "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                       "<friendlyName>" +  String(configStore.device_2_name) + "</friendlyName>"
                       "<manufacturer>Belkin International Inc.</manufacturer>"
                       "<modelName>Socket</modelName>"
                       "<modelNumber>3.1415</modelNumber>"
                       "<UDN>uuid:" + persistent_uuid + "2" + "</UDN>"
                       "<serialNumber>221517K0101769</serialNumber>"
                       "<binaryState>0</binaryState>"
                       "<serviceList>"
                       "<service>"
                       "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                       "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                       "<controlURL>/upnp/control/basicevent1</controlURL>"
                       "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                       "<SCPDURL>/eventservice.xml</SCPDURL>"
                       "</service>"
                       "<service>"
                       "<serviceType>urn:Belkin:service:metainfo:1</serviceType>"
                       "<serviceId>urn:Belkin:serviceId:metainfo1</serviceId>"
                       "<controlURL>/upnp/control/metainfo1</controlURL>"
                       "<eventSubURL>/upnp/event/metainfo1</eventSubURL>"
                       "<SCPDURL>/metainfoservice.xml</SCPDURL>"
                       "</service>"
                       "</serviceList>"
                       "</device>"
                       "</root>\r\n"
                       "\r\n";
    HTTP2.send(200, "text/xml", setup_xml.c_str());
  });

  HTTP2.begin();
}
void respondToSearch1() {
  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  String response =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Tue, 14 Dec 2016 02:30:00 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":1001/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persistent_uuid + "1" + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(response.c_str());
  UDP.endPacket();
}
void respondToSearch2() {
  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  String response =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Tue, 14 Dec 2016 02:30:00 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":1002/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persistent_uuid + "2" + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(response.c_str());
  UDP.endPacket();
}

/***********************************************
    Connect to WiFi and UDP
 **********************************************/

void start_alexa()
{
  bool udpConnected = connectUDP();   // Connect to UDP:
  if (udpConnected) {
    startHttpServer1(); // Start the HTTP Server
    startHttpServer2(); // Start the HTTP Server
  }
}

void parsePackets()
{
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    int len = UDP.read(packetBuffer, 255);

    if (len > 0) {
      packetBuffer[len] = 0;
    }

    String request = packetBuffer;
    if (request.indexOf('M-SEARCH') > 0) {
      if (request.indexOf("urn:Belkin:device:**") > 0) {
        respondToSearch1();
        respondToSearch2();
      }
    }
  }
}

/***********************************************
    Connect UDP
 **********************************************/
bool connectUDP()
{
  boolean state = false;
  if (UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti1))
  {
    state = true;
  }
  else
  {
    Serial.println("Connection failed");
  }
  return state;
}
