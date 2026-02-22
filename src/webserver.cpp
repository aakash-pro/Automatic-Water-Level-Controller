#include "webserver.h"

//////////helper functions for web server///////
String getUptime()
{
  unsigned long seconds = millis() / 1000;
  unsigned int days = seconds / 86400;
  seconds %= 86400;
  unsigned int hours = seconds / 3600;
  seconds %= 3600;
  unsigned int minutes = seconds / 60;
  seconds %= 60;

  char buffer[32];
  sprintf(buffer, "%ud %02uh %02um %02lus", days, hours, minutes, seconds);
  return String(buffer);
}

int getSignalQuality(int rssi)
{
  if (rssi <= -100)
    return 0;
  if (rssi >= -50)
    return 100;
  return 2 * (rssi + 100);
}
//////////////////////////////////////////////

void initWebServer()
{
  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    return;
  }

  // Serve index.html
  server.on("/", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/index.html")) {
      File file = LittleFS.open("/index.html", "r");
      server.streamFile(file, "text/html");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });

  // Serve CSS
  server.on("/style.css", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/style.css")) {
      File file = LittleFS.open("/style.css", "r");
      server.streamFile(file, "text/css");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });

  // Serve JS
  server.on("/app.js", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/app.js")) {
      File file = LittleFS.open("/app.js", "r");
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });

  ///////////bootstrap.min.css///////
  server.on("/bootstrap.min.css", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/bootstrap.min.css")) {
      File file = LittleFS.open("/bootstrap.min.css", "r");
      server.streamFile(file, "text/css");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });
  ///////////bootstrap.min.js///////
  server.on("/bootstrap.min.js", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/bootstrap.min.js")) {
      File file = LittleFS.open("/bootstrap.min.js", "r");
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });

  ///////////////gauge.min.js//////////
  server.on("/gauge.min.js", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/gauge.min.js")) {
      File file = LittleFS.open("/gauge.min.js", "r");
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });
  /////////////jquery.min.js//////////
  server.on("/jquery.min.js", HTTP_GET, [](void)
            {
    if (LittleFS.exists("/jquery.min.js")) {
      File file = LittleFS.open("/jquery.min.js", "r");
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    } });

  // API: GET device info
  server.on("/api/device", HTTP_GET, []()
            {
  DynamicJsonDocument doc(512);
  doc["device_name"] = "Water Controller";
  doc["chip_id"] = ESP.getChipId();
  doc["flash_size"] = ESP.getFlashChipSize();
  doc["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  doc["free_heap"] = ESP.getFreeHeap();
  doc["sdk_version"] = ESP.getSdkVersion();
  doc["uptime"] = getUptime();
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json); });

  // API: GET network info
  server.on("/api/network", HTTP_GET, []()
            {
  DynamicJsonDocument doc(512);

  doc["ssid"] = WiFi.SSID();
  doc["ip"] = WiFi.localIP().toString();
  doc["gateway"] = WiFi.gatewayIP().toString();
  doc["subnet"] = WiFi.subnetMask().toString();
  doc["mac"] = WiFi.macAddress();
  doc["rssi"] = WiFi.RSSI();
  doc["signal_quality"] = getSignalQuality(WiFi.RSSI());

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json); });

  // API: GET sensor data
  server.on("/api/sensors", HTTP_GET, []()
            {
  DynamicJsonDocument doc(1024);
  doc["waterLevel"] = waterLevel;
  doc["tank_full"] = tank_full;
  doc["pump_running"] = pump_running;
  doc["currentLPM"] = currentLPM;
  doc["voltage"] = voltage;
  doc["current"] = current;
  doc["power"] = power;
  doc["ApparentPower"] = ApparentPower;
  doc["ReactivePower"] = ReactivePower;
  doc["PowerFactor"] = PowerFactor;
  doc["TotalEnergy"] = TotalEnergy;
  doc["todayEnergy"] = todayEnergy;
  doc["yesterdayEnergy"] = yesterdayEnergy;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json); });

  // API: GET configurations
  server.on("/api/config", HTTP_GET, []()
            {
  DynamicJsonDocument doc(512);

  doc["dry_run_cutoff_protection"] = dry_run_cutoff_protection;
  doc["dry_run_cutoff_power_1"] = dry_run_cutoff_power_1;
  doc["dry_run_cutoff_power_2"] = dry_run_cutoff_power_2;
  doc["dry_run_cutoff_power_3"] = dry_run_cutoff_power_3;
  doc["dry_run_cutoff_power_4"] = dry_run_cutoff_power_4;
  doc["dry_run_cutoff_delay"] = dry_run_cutoff_delay;
  doc["tank_full_cutoff_protection"] = tank_full_cutoff_protection;
  doc["tank_full_cutoff_level"] = tank_full_cutoff_level;
  doc["tank_full_cutoff_delay"] = tank_full_cutoff_delay;
  doc["dashboard_style"] = dashboard_style;
  doc["led_strip_style"] = led_strip_style;
  doc["overload_cutoff_protection"] = overload_cutoff_protection;
  doc["overload_cutoff_power_1"] = overload_cutoff_power_1;
  doc["overload_cutoff_power_2"] = overload_cutoff_power_2;
  doc["overload_cutoff_power_3"] = overload_cutoff_power_3;
  doc["overload_cutoff_power_4"] = overload_cutoff_power_4;
  doc["overload_cutoff_delay"] = overload_cutoff_delay;
  
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json); });
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // API POst to Set Configurations
  server.on("/api/config", HTTP_POST, []()
            {
 
  if (!server.hasArg("plain"))
    {
      server.send(400, "application/json", "{\"error\":\"No body\"}");
      return;
    }

    DynamicJsonDocument doc(256);
    DeserializationError err = deserializeJson(doc, server.arg("plain"));

    if (err)
    {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

  if (doc.containsKey("dry_run_cutoff_protection"))
    {
    uint8_t val = doc["dry_run_cutoff_protection"];
    if (val == 0 || val == 1)
      dry_run_cutoff_protection = val;
    }
  if (doc.containsKey("dry_run_cutoff_power_1"))
    {
    uint8_t val = doc["dry_run_cutoff_power_1"];
    if (val == 0 || val == 1)
      dry_run_cutoff_power_1 = val;
    }
  if (doc.containsKey("dry_run_cutoff_power_2"))
    {
    uint8_t val = doc["dry_run_cutoff_power_2"];
    if (val <= 9)
       dry_run_cutoff_power_2 = val;
    }
  if (doc.containsKey("dry_run_cutoff_power_3"))
    {
    uint8_t val = doc["dry_run_cutoff_power_3"];
    if (val <= 9)
      dry_run_cutoff_power_3 = val;
    }
  if (doc.containsKey("dry_run_cutoff_power_4"))
    {
    uint8_t val = doc["dry_run_cutoff_power_4"];
    if (val <= 9)
      dry_run_cutoff_power_4 = val;
    }
  if (doc.containsKey("dry_run_cutoff_delay"))
    {
    uint8_t val = doc["dry_run_cutoff_delay"];
    if (val <= 60)
      dry_run_cutoff_delay = val;
    }

  if (doc.containsKey("tank_full_cutoff_level"))
    {
    uint8_t val = doc["tank_full_cutoff_level"];
    if (val >= 1 && val <= 8)
      tank_full_cutoff_level = val;
    }
  if (doc.containsKey("tank_full_cutoff_protection"))
    {
    uint8_t val = doc["tank_full_cutoff_protection"];
    if (val == 0 || val == 1)
      tank_full_cutoff_protection = val;
    }
  if (doc.containsKey("tank_full_cutoff_delay"))    
    {
    uint8_t val = doc["tank_full_cutoff_delay"];
    if (val <= 200)
      tank_full_cutoff_delay = val;
    }

    if (doc.containsKey("overload_cutoff_protection"))
    {
    uint8_t val = doc["overload_cutoff_protection"];
    if (val == 0 || val == 1)
      overload_cutoff_protection = val;
    }
  if (doc.containsKey("overload_cutoff_power_1"))
    {
    uint8_t val = doc["overload_cutoff_power_1"];
    if (val == 0 || val == 1)
      overload_cutoff_power_1 = val;
    }
  if (doc.containsKey("overload_cutoff_power_2"))
    {
    uint8_t val = doc["overload_cutoff_power_2"];
    if (val <= 9)
      overload_cutoff_power_2 = val;
    }
  if (doc.containsKey("overload_cutoff_power_3"))
    {
    uint8_t val = doc["overload_cutoff_power_3"];
    if (val <= 9)
      overload_cutoff_power_3 = val;
    }
  if (doc.containsKey("overload_cutoff_power_4"))
    {
    uint8_t val = doc["overload_cutoff_power_4"];
    if (val <= 9)
      overload_cutoff_power_4 = val;
    }
  if (doc.containsKey("overload_cutoff_delay"))
    {
    uint8_t val = doc["overload_cutoff_delay"];
    if (val <= 60)
      overload_cutoff_delay = val;
    }

  if (doc.containsKey("dashboard_style"))
    {
    uint8_t val = doc["dashboard_style"];
    if (val >= 1 && val <= 4)
      dashboard_style = val;
    }
  if (doc.containsKey("led_strip_style"))
    {
    uint8_t val = doc["led_strip_style"];
    if (val >= 1 && val <= 4)
      led_strip_style = val;
    }

  saveValuesToEEPROM();
  server.send(200, "application/json", "{\"status\":\"ok\"}"); });

  //////////////////////////////////////////////////////////////////////////////////////////////
  // API: POST to start/stop pump
  server.on("/api/pump", HTTP_POST, []()
            {
              if (!server.hasArg("plain"))
              {
                server.send(400, "application/json", "{\"error\":\"No body\"}");
                return;
              }

              DynamicJsonDocument doc(256);
              DeserializationError err = deserializeJson(doc, server.arg("plain"));

              if (err)
              {
                server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
                return;
              }

              if (!doc.containsKey("state"))
              {
                server.send(400, "application/json", "{\"error\":\"Missing state\"}");
                return;
              }

              bool requestedState = doc["state"];

              if (requestedState == true)
              {
                turnOnPumpAsync();
              }
              if (requestedState == false)
              { 
                turnOffPumpAsync();
              }
              
              server.send(200, "application/json", "{\"status\":\"ok\"}");
            });



  // API: POST to call specific fucntions & device Actions
  server.on("/api/system", HTTP_POST, []()
            {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  DynamicJsonDocument doc(256);
  deserializeJson(doc, server.arg("plain"));

  String cmd = doc["command"];

  if (cmd == "reset") {
    server.send(200, "application/json", "{\"status\":\"resetting\"}");
    delay(1000);
    reset();
  } });

  // Handle 404
  server.onNotFound([](void)
{ server.send(404, "application/json", "{\"error\":\"Not found\"}"); });

  server.begin();
}

void handleWebServer()
{
  server.handleClient();
}
