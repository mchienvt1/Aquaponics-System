// #include "global.h"
// #include <WiFi.h>
// #include <SocketIOclient.h>
// #include <ArduinoJson.h>
// #include <math.h>
// #include <nvs_flash.h>

// // ======= Cấu hình WiFi và Server =======
// const char *ssid = "PhuongPhuong";
// const char *password = "26011007";
// const char *server_host = "192.168.1.23";
// const uint16_t server_port = 5000;

// SocketIOclient socketIO;
// bool connected = false;
// unsigned long lastSend = 0;
// const unsigned long interval = 300000; // Gửi mỗi 5 giây

// int currentIndex = 0; // Gửi lần lượt từng dòng

// float ph_base = 6.5, ph_slope = 0.002;
// float do_base = 5.0, do_slope = -0.0015;
// float temp_base = 27.0, temp_slope = 0.005;

// // ======= Sinh 1 dòng dữ liệu cảm biến giả =======
// // ======= Biến trạng thái để tạo dao động tuyến tính =======
// void generateSingleSensorData(float *dataRow)
// {
//   // Tăng giảm tuyến tính có dao động nhỏ
//   float ph = ph_base + ph_slope * currentIndex + sin(currentIndex * 0.1) * 0.01;
//   float do_ = do_base + do_slope * currentIndex + cos(currentIndex * 0.1) * 0.015;
//   float temp = temp_base + temp_slope * currentIndex + sin(currentIndex * 0.07) * 0.02;

//   // Giới hạn để giữ trong khoảng hợp lý
//   ph = constrain(ph, 6.0, 7.0);
//   do_ = constrain(do_, 4.5, 5.5);
//   temp = constrain(temp, 26.0, 28.0);

//   // Gán giá trị vào mảng
//   dataRow[0] = ph;
//   dataRow[1] = do_;
//   dataRow[2] = temp;

//   // Các đặc trưng thời gian dạng sin/cos
//   dataRow[3] = sin(currentIndex * 0.1);
//   dataRow[4] = cos(currentIndex * 0.1);
//   dataRow[5] = sin(currentIndex * 0.2);
//   dataRow[6] = cos(currentIndex * 0.2);
// }

// // ======= Gửi 1 dòng dữ liệu cảm biến =======
// void sendSingleSensorData()
// {
//   if (!connected)
//   {
//     Serial.println("⚠️ Chưa kết nối tới server!");
//     return;
//   }

//   float row[7];
//   generateSingleSensorData(row);

//   DynamicJsonDocument doc(512);
//   JsonArray arr = doc.to<JsonArray>();
//   for (int j = 0; j < 7; j++)
//   {
//     arr.add(row[j]);
//   }

//   String payload;
//   serializeJson(doc, payload);

//   String message = "[\"sensor_data\"," + payload + "]";
//   socketIO.sendEVENT(message);

//   Serial.printf("✅ Đã gửi dòng %d: %s\n", currentIndex, message.c_str());

//   currentIndex = (currentIndex + 1) % 6; // Quay vòng sau 6 dòng
// }

// // ======= Xử lý phản hồi từ server =======
// void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
// {
//   switch (type)
//   {
//   case sIOtype_DISCONNECT:
//     Serial.println("🔌 Mất kết nối server!");
//     connected = false;
//     break;

//   case sIOtype_CONNECT:
//     Serial.printf("✅ Đã kết nối tới server: %s\n", payload);
//     socketIO.send(sIOtype_CONNECT, "/");
//     connected = true;
//     break;

//   case sIOtype_EVENT:
//   {
//     Serial.println("📨 Nhận phản hồi:");
//     DynamicJsonDocument doc(1024);
//     DeserializationError error = deserializeJson(doc, payload, length);
//     if (!error)
//     {
//       JsonArray arr = doc.as<JsonArray>();
//       if (arr[0] == "prediction")
//       {
//         JsonObject pred = arr[1];
//         float ph = pred["pH"];
//         float do_ = pred["DO"];
//         float temp = pred["Temperature"];
//         Serial.printf("📡 Dự đoán: pH=%.2f, DO=%.2f, Temp=%.2f\n", ph, do_, temp);
//       }
//     }
//     break;
//   }

//   case sIOtype_ERROR:
//     Serial.printf("❌ SocketIO Error [%u bytes]\n", length);
//     break;

//   default:
//     Serial.printf("⚙️ Sự kiện khác [%u bytes]\n", length);
//     break;
//   }
// }

// extern "C" void app_main(void)
// {
//   setup();
//   while (true)
//   {
//     loop();
//   }
// }

// // ======= Setup ESP32 =======
// void setup()
// {
//   Serial.begin(115200);
//   esp_err_t err = nvs_flash_init();
//   if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
//   {
//     ESP_ERROR_CHECK(nvs_flash_erase());
//     err = nvs_flash_init();
//   }
//   ESP_ERROR_CHECK(err);

//   WiFi.begin(ssid, password);
//   Serial.print("🔄 Đang kết nối WiFi");
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\n✅ WiFi đã kết nối.");
//   Serial.print("📡 IP: ");
//   Serial.println(WiFi.localIP());

//   socketIO.begin(server_host, server_port, "/socket.io/?EIO=4");
//   socketIO.onEvent(socketIOEvent);
// }

// // ======= Loop chính =======
// void loop()
// {
//   socketIO.loop();

//   if (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.println("🚫 WiFi ngắt! Đang thử lại...");
//     WiFi.reconnect();
//     delay(5000);
//     return;
//   }

//   if (millis() - lastSend > interval && connected)
//   {
//     lastSend = millis();
//     sendSingleSensorData();
//   }
// }

#include "global.h"

extern "C" void app_main(void)
{
  setup();
  while (true)
  {
    loop();
  }
}

void setup()
{
  Serial.begin(MAIN_BAUDRATE);
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  dispatch();
}

void loop()
{
  // Do nothing
}
