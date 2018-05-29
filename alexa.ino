void prepareIds() {
  char uuid[64];
  unsigned long chip_id = ESP.getChipId();
  //  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
  //            (uint16_t) ((chipId >> 16) & 0xff),
  //            (uint16_t) ((chipId >>  8) & 0xff),
  //            (uint16_t)   chipId        & 0xff);

  snprintf_P(uuid, sizeof(uuid), PSTR("444556%06X%02X\0"), chip_id, 1); // "DEV" + CHIPID + DEV_ID
  serial = String(uuid);

  persistent_uuid = "Socket-1_0-" + serial;
  Serial.println(serial);
  char serialnum[15];
  sprintf(serialnum, "221703K0%06X\0", chip_id); // "221703K0" + CHIPID
  serialn = strdup(serialnum);
  Serial.println( serialn);
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
    HTTP1.send(200, "text/plain", xml_eventservice().c_str());
  });

  HTTP1.on("/setup.xml", HTTP_GET, []() {
    HTTP1.send(200, "text/xml", xml_setup(1, persistent_uuid, String(configStore.device_1_name).c_str() , serialn));
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
    HTTP2.send(200, "text/plain", xml_eventservice().c_str());
  });

  HTTP2.on("/setup.xml", HTTP_GET, []() {
    HTTP2.send(200, "text/xml", xml_setup(2, persistent_uuid, String(configStore.device_2_name).c_str() , serialn));
  });
  HTTP2.begin();
}
void respondToSearch1() {
  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(alexa_call_responder(1, alx_port_d[1], persistent_uuid).c_str());
  UDP.endPacket();
}
void respondToSearch2() {
  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(alexa_call_responder(2, alx_port_d[2], persistent_uuid).c_str());
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

//"<serialNumber>221517K0101769</serialNumber>"

String xml_setup(int dev_id, String persis_uuid, String dev_call_name, String serial_number) {
  String setup_xml = "<?xml version=\"1.0\"?>"
                     "<root>"
                     "<device>"
                     "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                     "<friendlyName>" +  dev_call_name + "</friendlyName>"
                     "<manufacturer>Belkin International Inc.</manufacturer>"
                     "<modelName>Socket</modelName>"
                     "<modelNumber>3.1415</modelNumber>"
                     "<UDN>uuid:" + persis_uuid + String(dev_id) + "</UDN>"
                     "<serialNumber>" + serial_number + "</serialNumber>"
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
                     "<service>"
                     "<serviceType>urn:Belkin:service:remoteaccess:1</serviceType>"
                     "<serviceId>urn:Belkin:serviceId:remoteaccess1</serviceId>"
                     "<controlURL>/upnp/control/remoteaccess1</controlURL>"
                     "<eventSubURL>/upnp/event/remoteaccess1</eventSubURL>"
                     "<SCPDURL>/remoteaccess.xml</SCPDURL>"
                     "</service>"
                     "<service>"
                     "<serviceType>urn:Belkin:service:deviceinfo:1</serviceType>"
                     "<serviceId>urn:Belkin:serviceId:deviceinfo1 </serviceId>"
                     "<controlURL>/upnp/control/deviceinfo1</controlURL>"
                     "<eventSubURL>/upnp/event/deviceinfo1</eventSubURL>"
                     "<SCPDURL>/deviceinfoservice.xml</SCPDURL>"
                     "</service>"
                     "</serviceList>"
                     "</device>"
                     "</root>\r\n"
                     "\r\n";

  return setup_xml;
}

String xml_eventservice() {
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
  return eventservice_xml;
}

String alexa_call_responder(int dev_id, int port, String persis_uuid) {
  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  String response =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Tue, 14 Dec 2016 02:30:00 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":" + String(port) + "/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persis_uuid + String(dev_id) + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";
  return response;
}
