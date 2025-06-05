import eventlet
eventlet.monkey_patch()  # ⚠️ Gọi sớm nhất

import os
import json
import numpy as np
import joblib
import traceback
from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit
from tensorflow.keras.models import load_model
from collections import deque

sensor_buffer = deque(maxlen=6)

# --- Cấu hình đường dẫn ---
MODEL_PATH = './lstm_model.h5'
SCALER_PATH = './input_scaler.pkl'
OUTPUT_SCALER_PATH = './output_scaler.pkl'

# --- Khởi tạo Flask App ---
app = Flask(__name__, template_folder='templates')
app.config['SECRET_KEY'] = 'secret_key'

# --- Cấu hình SocketIO ---
socketio = SocketIO(
    app,
    cors_allowed_origins="*",
    async_mode='eventlet',
    logger=True,
    engineio_logger=True,
    ping_timeout=60,
    ping_interval=25,
    allow_upgrades=True,
    websocket=True
)

# --- Load mô hình và scaler ---
print("🔄 Đang load mô hình và scaler...")
try:
    model = load_model(MODEL_PATH, compile=False)
    scaler = joblib.load(SCALER_PATH)
    output_scaler = joblib.load(OUTPUT_SCALER_PATH)
    print("✅ Đã load thành công.")
except Exception as e:
    print("❌ Lỗi khi load mô hình hoặc scaler:", e)
    traceback.print_exc()
    exit()

# --- Route trả về giao diện chính ---
@app.route('/')
def index():
    return render_template('index.html')

# --- Sự kiện khi client kết nối ---
@socketio.on('connect')
def handle_connect():
    sid = request.sid
    print(f"📡 Client connected! SID: {sid}")
    emit('connect_response', {'status': 'connected'})

@socketio.on('disconnect')
def handle_disconnect():
    sid = request.sid
    print(f"🔌 Client disconnected! SID: {sid}")

# --- Nhận dữ liệu cảm biến từ ESP32 ---
@socketio.on('sensor_data')
def handle_sensor_data(data):
    global sensor_buffer
    try:
        print("📥 Dữ liệu nhận được:", data)

        if isinstance(data, str):
            sensor_values = json.loads(data)
        else:
            sensor_values = data

        if not isinstance(sensor_values, list) or len(sensor_values) < 3:
            emit('error', {'message': 'Dữ liệu phải là 1 mảng chứa ít nhất 3 giá trị.'})
            return

        # ✅ Giữ lại 3 đặc trưng đầu tiên (pH, DO, Temp)
        trimmed_values = sensor_values[:3]

        # Thêm vào buffer
        sensor_buffer.append(trimmed_values)

        # Phát lại dữ liệu để hiển thị dashboard
        socketio.emit('sensor_data_received', list(sensor_buffer))

        if len(sensor_buffer) < 6:
            print(f"🕐 Đã nhận {len(sensor_buffer)}/6 dòng. Chưa dự đoán.")
            return

        recent_data = np.array(sensor_buffer)
        print(f"📊 Hình dạng dữ liệu gom: {recent_data.shape}")

        scaled_input = scaler.transform(recent_data)
        X_input = np.expand_dims(scaled_input, axis=0)
        y_scaled_pred = model.predict(X_input)
        y_pred = output_scaler.inverse_transform(y_scaled_pred)

        response = {
            'pH': round(float(y_pred[0][0]), 2),
            'DO': round(float(y_pred[0][1]), 2),
            'Temperature': round(float(y_pred[0][2]), 2)
        }

        print("📤 Gửi dự đoán:", response)
        socketio.emit('prediction', response)

    except Exception as e:
        print("❌ Lỗi xử lý dữ liệu:", e)
        traceback.print_exc()
        emit('error', {'message': str(e)})

# --- Khởi động server ---
if __name__ == '__main__':
    print("=" * 50)
    print("--- 🚀 Server Flask-SocketIO đang khởi động (eventlet) ---")
    print(f"🔧 Chế độ async: {socketio.async_mode}")
    print("=" * 50)
    socketio.run(app, host='0.0.0.0', port=5000, debug=True, use_reloader=False, allow_unsafe_werkzeug=True)
