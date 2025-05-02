# 💧 Arduino Turbidity Monitoring System

An end-to-end **turbidity monitoring project** using Arduino and ESP8266 (Wemos D1 Mini), designed to read water turbidity, display the data, and log it to a web-based dashboard. It supports email notifications using Gmail and has modular firmware and server-side PHP integration.

---

## 📦 Project Structure

```

.
├── hardware/                 # Circuit design & breadboard layout
│   ├── draw\_bb.png           # Breadboard image
│   ├── draw\.fzz              # Fritzing schematic
│   └── hardware.txt          # Notes on hardware
├── software/
│   ├── arduino/              # Arduino code and libraries
│   │   ├── main.ino          # Main Arduino sketch
│   │   ├── data.xlsx         # Sample turbidity data
│   │   ├── libraries/        # Local Arduino libraries
│   └── website/              # PHP, HTML, JS for monitoring dashboard
│       ├── index.php         # Web interface
│       ├── save.php          # Data logging script
│       ├── dbconnect.php     # Database connection
│       ├── css/, js/, img/   # UI assets
│       └── create-table.txt  # SQL schema for MySQL
├── LICENSE
└── README.md

```

---

## 🚀 Features

- Real-time turbidity reading using analog sensor
- Display data on LCD (via I2C)
- Web dashboard for data logging & visualization
- PHP & MySQL backend
- Email alert support via Gmail
- Built-in chart visualization (Chart.js)
- Compatible with Wemos D1 Mini (ESP8266)

---

## 🧰 Getting Started

### 1. Install Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/main/software)

### 2. Setup Libraries

Copy the following libraries to your local Arduino folder (usually: `Documents/Arduino/libraries/`):

- `EMailSender`
- `LiquidCrystal_I2C`
- `SimpleTimer`

You can also find them inside `software/arduino/libraries/`.

### 3. Install ESP8266 Board Support

- Open **Arduino IDE**
- Go to `File → Preferences`
- In the **Additional Board Manager URLs**, add:
```

https://arduino.esp8266.com/stable/package_esp8266com_index.json

```
- Click **OK**
- Then go to `Tools → Board → Board Manager`
- Search for **esp8266** and click **Install**

### 4. Upload Code to Wemos D1 Mini

- Select board: `LOLIN(WEMOS) D1 R2 & Mini`
- Open `main.ino` from `software/arduino/`
- Connect Wemos via USB
- Click **Upload**

---

## 🌐 Web Dashboard Setup

### 1. Prepare MySQL Database

- Create a database (e.g., `turbidity_db`)
- Run the SQL schema found in `website/create-table.txt`

### 2. Configure PHP

- Copy contents of `software/website/` to your web server
- Adjust `dbconnect.php` to match your MySQL credentials

### 3. Access Dashboard

Open `index.php` in your browser to view turbidity logs, charts, and real-time status.

---

## 📧 Email Notifications

Email alerts are sent when turbidity crosses a certain threshold.

- Uses `EMailSender` library (Gmail-compatible)
- Make sure to enable **Less secure app access** in your Gmail settings
- Modify credentials in `EMailSenderKey.h` before use

---

## 🖼️ Hardware Setup

Refer to:
- `hardware/draw_bb.png` for wiring layout
- `hardware/draw.fzz` for Fritzing schematic

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for full terms.

---

## 🙌 Credits

**Author:** Ardy Seto  
Feel free to fork, modify, and contribute improvements or additional features.