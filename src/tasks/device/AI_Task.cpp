#include "AI_Task.h"
#include <SocketIOclient.h>
#include <ArduinoJson.h>
#include <math.h>

const char *server_host = "192.168.2.4";
const uint16_t server_port = 5000;

SocketIOclient socketIO;
bool connected = false;

// Task handles để quản lý các luồng FreeRTOS
TaskHandle_t socket_init_handle = NULL;
TaskHandle_t socket_loop_handle = NULL;
TaskHandle_t sensor_data_handle = NULL;
bool ai_tasks_running = false;

int currentIndex = 0; // Gửi lần lượt từng dòng

float ph_base = 6.5, ph_slope = 0.002;
float do_base = 5.0, do_slope = -0.0015;
float temp_base = 27.0, temp_slope = 0.005;

// ======= Sinh 1 dòng dữ liệu cảm biến giả =======
void generateSingleSensorData(float *dataRow)
{
  // Tăng giảm tuyến tính có dao động nhỏ
  float ph = ph_base + ph_slope * currentIndex + sin(currentIndex * 0.1) * 0.1;
  float do_ = do_base + do_slope * currentIndex + cos(currentIndex * 0.1) * 0.15;
  float temp = temp_base + temp_slope * currentIndex + sin(currentIndex * 0.07) * 0.2;

  // Giới hạn để giữ trong khoảng hợp lý
  ph = constrain(ph, 6.0, 7.0);
  do_ = constrain(do_, 4.5, 5.5);
  temp = constrain(temp, 26.0, 28.0);

  // Gán giá trị vào mảng
  dataRow[0] = ph;
  dataRow[1] = do_;
  dataRow[2] = temp;

  // Các đặc trưng thời gian dạng sin/cos
  dataRow[3] = sin(currentIndex * 0.1);
  dataRow[4] = cos(currentIndex * 0.1);
  dataRow[5] = sin(currentIndex * 0.2);
  dataRow[6] = cos(currentIndex * 0.2);
}

// ======= Hàm mới: Gửi dữ liệu cảm biến thực =======
void AI_Task_SendSensorData(SensorData &data)
{
  if (!connected)
  {
    Serial.println("⚠️ Chưa kết nối tới server!");
    return;
  }

  // Tạo JSON mảng cho dữ liệu
  DynamicJsonDocument doc(512);
  JsonArray arr = doc.to<JsonArray>();

  // Định dạng dữ liệu theo định dạng mong muốn của server
  // Giả sử server cần mảng 7 giá trị [pH, DO, temp, sin1, cos1, sin2, cos2]
  float ph = data.get_latest_data(MEASURE_PH);
  float do_ = data.get_latest_data(MEASURE_DO);
  float temp = data.get_latest_data(MEASURE_TEMP);

  // Thêm giá trị pH, DO, nhiệt độ
  arr.add(ph);
  arr.add(do_);
  arr.add(temp);

  // Thêm các đặc trưng thời gian dạng sin/cos như trong dữ liệu giả
  int currentTime = millis() / 1000; // Thời gian hiện tại (giây)
  arr.add(sin(currentTime * 0.1));
  arr.add(cos(currentTime * 0.1));
  arr.add(sin(currentTime * 0.2));
  arr.add(cos(currentTime * 0.2));

  String payload;
  serializeJson(doc, payload);

  String message = "[\"sensor_data\"," + payload + "]";
  socketIO.sendEVENT(message);

  Serial.printf("✅ Đã gửi dữ liệu cảm biến thực: pH=%.2f, DO=%.2f, Temp=%.2f\n",
                ph, do_, temp);
}

// ======= Gửi 1 dòng dữ liệu cảm biến =======
void sendSingleSensorData()
{
  if (!connected)
  {
    Serial.println("⚠️ Chưa kết nối tới server!");
    return;
  }

  float row[7];
  generateSingleSensorData(row);

  DynamicJsonDocument doc(512);
  JsonArray arr = doc.to<JsonArray>();
  for (int j = 0; j < 7; j++)
  {
    arr.add(row[j]);
  }

  String payload;
  serializeJson(doc, payload);

  String message = "[\"sensor_data\"," + payload + "]";
  socketIO.sendEVENT(message);

  Serial.printf("✅ Đã gửi dòng %d: %s\n", currentIndex, message.c_str());

  currentIndex = (currentIndex + 1) % 6; // Quay vòng sau 6 dòng
}

// ======= Xử lý phản hồi từ server =======
void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case sIOtype_DISCONNECT:
    Serial.println("🔌 Mất kết nối server!");
    connected = false;
    break;

  case sIOtype_CONNECT:
    Serial.printf("✅ Đã kết nối tới server: %s\n", payload);
    socketIO.send(sIOtype_CONNECT, "/");
    connected = true;
    break;

  case sIOtype_EVENT:
  {
    Serial.println("📨 Nhận phản hồi:");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload, length);
    if (!error)
    {
      JsonArray arr = doc.as<JsonArray>();
      if (arr[0] == "prediction")
      {
        JsonObject pred = arr[1];
        float ph = pred["pH"];
        float do_ = pred["DO"];
        float temp = pred["Temperature"];
        Serial.printf("📡 Dự đoán: pH=%.2f, DO=%.2f, Temp=%.2f\n", ph, do_, temp);

        // Tạo JSON để gửi telemetry
        DynamicJsonDocument telemetryDoc(256);
        telemetryDoc["pH_predict"] = ph;
        telemetryDoc["DO_predict"] = do_;
        telemetryDoc["temp_predict"] = temp;

        String jsonStr;
        serializeJson(telemetryDoc, jsonStr);

        // Gửi dữ liệu lên telemetry
        sendTelemetry(jsonStr);
      }
    }
    break;
  }

  case sIOtype_ERROR:
    Serial.printf("❌ SocketIO Error [%u bytes]\n", length);
    break;

  default:
    Serial.printf("⚙️ Sự kiện khác [%u bytes]\n", length);
    break;
  }
}

// ======= Task khởi tạo socket =======
void socket_init_task(void *pvParameters)
{
  socketIO.begin(server_host, server_port, "/socket.io/?EIO=4");
  socketIO.onEvent(socketIOEvent);

  vTaskDelete(NULL);
}

// ======= Task xử lý socket =======
void socket_loop_task(void *pvParameters)
{
  while (true)
  {
    socketIO.loop();
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay nhỏ để tránh chiếm CPU
  }
}

// ======= Task gửi dữ liệu cảm biến =======
void sensor_data_task(void *pvParameters)
{
  const TickType_t xDelay = 5000 / portTICK_PERIOD_MS; // Gửi mỗi 5 giây

  while (true)
  {
    if (connected)
    {
      // sendSingleSensorData();
      AI_Task_SendSensorData(sensorData);
    }
    vTaskDelay(xDelay);
  }
}

// ======= Khởi tạo AI task =======
void AI_Task_Init()
{
  // Kiểm tra nếu các task đã được khởi tạo trước đó
  if (ai_tasks_running)
  {
    Serial.println("AI tasks đã đang chạy, không cần khởi tạo lại");
    return;
  }

  Serial.println("Khởi tạo các AI tasks...");

  // Khởi tạo task và lưu handle để có thể dừng sau này
  xTaskCreate(socket_init_task, "Socket_Init_Task", 4096, NULL, 1, &socket_init_handle);
  xTaskCreate(socket_loop_task, "Socket_Loop_Task", 8192, NULL, 2, &socket_loop_handle);
  xTaskCreate(sensor_data_task, "Sensor_Data_Task", 4096, NULL, 1, &sensor_data_handle);

  ai_tasks_running = true;
  Serial.println("Đã khởi tạo xong các AI tasks");
}

// ======= Dừng các AI task =======
void AI_Task_Stop()
{
  if (!ai_tasks_running)
  {
    Serial.println("AI tasks không chạy, không cần dừng");
    return;
  }

  Serial.println("Đang dừng các AI tasks...");

  // Dừng kết nối socket trước khi xóa task
  if (connected)
  {
    socketIO.disconnect();
    connected = false;
    // Đợi một chút để đảm bảo socket đã xử lý việc ngắt kết nối
    delay(100);
  }

  // Xóa các task nếu chúng tồn tại
  if (socket_init_handle != NULL)
  {
    vTaskDelete(socket_init_handle);
    socket_init_handle = NULL;
    Serial.println("- Đã dừng Socket_Init_Task");
  }

  if (socket_loop_handle != NULL)
  {
    vTaskDelete(socket_loop_handle);
    socket_loop_handle = NULL;
    Serial.println("- Đã dừng Socket_Loop_Task");
  }

  if (sensor_data_handle != NULL)
  {
    vTaskDelete(sensor_data_handle);
    sensor_data_handle = NULL;
    Serial.println("- Đã dừng Sensor_Data_Task");
  }

  ai_tasks_running = false;
  Serial.println("Đã dừng tất cả AI tasks");
}