# 🌊 Arduino Turbidity Monitoring System

This repository contains a complete turbidity monitoring system based on **Arduino**. It includes hardware schematics, embedded firmware, and a web-based dashboard to monitor and log water turbidity in real-time. The system can also be configured to send email alerts and visualize data using charts.

## 📦 Project Structure

```

.
├── hardware/                 # Fritzing schematic & hardware notes
│   ├── draw\_bb.png           # Breadboard layout (image)
│   ├── draw\.fzz              # Fritzing project file
│   └── hardware.txt          # Notes or instructions
├── software/
│   ├── arduino/              # Arduino sketch, libraries, and documentation
│   │   ├── main.ino
│   │   ├── data.xlsx         # Sample data
│   │   ├── libraries/        # Local libraries used by the project
│   │   ├── Tutorial Arduino Turbidity.pdf / .docx
│   │   └── readme.md         # Arduino-specific README
│   └── website/              # PHP & HTML files for online data monitoring
│       ├── index.php         # Web dashboard
│       ├── save.php          # Data saving handler
│       ├── about.php         # About page
│       ├── dbconnect.php     # MySQL DB connection
│       └── css/, js/, img/   # Frontend assets (Bootstrap, Chart.js, etc.)
├── LICENSE
└── README.md                 # This file

```

## 🚀 Features

- Real-time turbidity sensing with Arduino
- LCD display for live readings
- Data logging to web dashboard via PHP + MySQL
- Visualizations using Chart.js
- Email alert integration (Gmail-compatible via `EMailSender` library)
- Clean breadboard circuit using Fritzing

## 🛠️ Hardware Requirements

- Arduino Uno or ESP32
- Turbidity sensor (e.g., SEN0189)
- I2C LCD 16x2 display
- SIM800L module (optional, for GSM email alert)
- Internet module (e.g., ESP8266/ESP32 for web dashboard)
- USB cable, resistors, jumper wires, breadboard

## 🧠 Software & Libraries

Arduino:
- `LiquidCrystal_I2C`
- `SimpleTimer`
- `EMailSender`

Web:
- PHP (7.x)
- MySQL/MariaDB
- Bootstrap 4
- Chart.js
- jQuery

## 🌐 Web Dashboard

Navigate to `/website/index.php` to view real-time turbidity data, visualized with charts. The database schema can be initialized with `create-table.txt`.

## 📖 Documentation

- `Tutorial Arduino Turbidity.pdf`: Full user manual and setup guide
- `hardware/draw_bb.png`: Circuit wiring reference

## 🔒 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## 🤝 Contributions

Contributions are welcome! Feel free to fork, submit pull requests, or suggest features via issues.