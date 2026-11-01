# DeckStat

This is my Half Life Week 4 project. I made a small PC stats display using an ESP32 and an OLED screen. It shows CPU, RAM, GPU and VRAM usage, so I can see what my computer is doing without opening another window.

![DeckStat showing the usage screen](Pictures/display-running.jpg)

![ESP32 and OLED wired together](Pictures/deckstat-wiring.jpg)

## How it works

The Python script reads the usage from the computer and sends it to the ESP32 through the USB cable. It sends one line of numbers at a time, like `11,45,32,34`. The numbers mean CPU, RAM, GPU and VRAM, in that order. Both sides use 115200 baud, and the ESP32 updates the screen when it gets a new line.

The screen layout came from Lopaka. I made two screens while working on it but I kept the usage screen because it was simpler and had the information I wanted. The animation bitmap used by the firmware is included in the Firmware folder too.

here's the link to the lopaka project (if you want you can fork it and edit it ): https://lopaka.app/canvas/59863

These are the two Lopaka designs I tried. I dropped the temperature screen, so it is only here to show how the design changed. The firmware only uses the usage screen.

![Lopaka usage screen design](Pictures/usage-layout.png)

![Lopaka temperature screen design that I dropped](Pictures/discarded-temp-layout.png)

The project is made from an ESP32 and a 0.96-inch OLED screen. I wired them together and made the schematic in KiCad. The schematic is in the Hardware folder.

I did not make a custom PCB for this project. Because it's unecessary would look ugly and would overall make the project more expensive.

## How to use it

1. Install Arduino IDE, the ESP32 board package, and the U8g2 library.
2. Open `Firmware/DeckStat.ino`, choose **ESP32 Dev Module**, choose the ESP32 COM port, and upload it.
3. Install Python 3 and the Python libraries:

   ```powershell
   py -m pip install pyserial psutil nvidia-ml-py
   ```

4. Change `PORT` near the top of `Firmware/Script.py` to the ESP32 COM port you are using.
5. Close Arduino Serial Monitor then run this from the project folder:

   ```powershell
   py Firmware\Script.py
   ```

The Python script uses NVIDIA NVML for GPU and VRAM usage readings. If you don't have an NVIDIA GPU those two readings stay at zero.

## Wiring

| OLED screen | ESP32 |
| --- | --- |
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

![KiCad wiring schematic](Pictures/wiring-schematic.svg)

## Bill of materials


| Part | Quantity | Price | Link |
| --- | --- | --- | --- |
| ESP32 DevKit V1 (ESP32-WROOM-32) | 1 | Add later | Add later |
| 0.96-inch SSD1306 I2C OLED display, 128x64 | 1 | Add later | Add later |

The full parts list is also in [`BOM.csv`](BOM.csv).

## Project files

- `Firmware/DeckStat.ino` — ESP32 firmware
- `Firmware/Script.py` — Python script that reads and sends PC usage
- `Firmware/activity_32_32_28f.h` — Lopaka animation bitmap used by the firmware
- `Hardware/Display.kicad_sch` — KiCad schematic
- `Hardware/Display.kicad_pro` — KiCad project settings
- `Pictures/` — photos of the working screen and the wired build
- `BOM.csv` — parts list

I had to troubleshoot the serial connection a few times before the numbers showed up properly. I used AI for some help with the Python part because I'm still learning Python, and I am focusing more on C++ at school and at home.
