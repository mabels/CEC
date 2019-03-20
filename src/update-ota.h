// #include <ArduinoOTA.h>

void updateOTA(Print &serial, ArduinoOTAClass &aota) {
  aota.setHostname("pulsotor");
  aota.onStart([&aota, &serial]() {
    String type;
    if (aota.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    serial.println("Start updating " + type);
  });

  aota.onEnd([&serial]() {
    serial.println("\nEnd");
  });
  aota.onProgress([&serial](unsigned int progress, unsigned int total) {
    serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  aota.onError([&serial](ota_error_t error) {
    serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      serial.println("End Failed");
    }
  });
  aota.begin();
}