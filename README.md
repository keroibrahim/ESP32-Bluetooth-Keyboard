# ESP32 Bluetooth Keyboard

This project turns an ESP32 into a Bluetooth keyboard with a web-based interface, mimicking a real laptop keyboard layout. It uses a WiFi access point to serve an HTML keyboard that sends keypresses over Bluetooth to a paired device.

## Features

- **Full Keyboard Layout**: Includes letters (lowercase by default), numbers, symbols, function keys (F1-F12), arrow keys, and modifiers like Caps Lock, Shift, Ctrl, Alt, and Fn.
- **Caps Lock Indicator**: A visual LED-like indicator (green when ON, gray when OFF) mimics real keyboard behavior.
- **Bluetooth HID**: Acts as a Bluetooth Human Interface Device (HID) keyboard using the `BleCombo` library.
- **Web Interface**: Accessible via a WiFi access point hosted by the ESP32, with a responsive, laptop-style keyboard layout.
- **Customizable**: Easily extendable for additional keys or modifier functionality.

## Prerequisites

- **Hardware**:
  - ESP32 board with Bluetooth support (e.g., ESP32-WROOM-32).
- **Software**:
  - Arduino IDE or compatible environment.
  - Libraries:
    - `WiFi.h` (built-in with ESP32 Arduino core).
    - `WebServer.h` (built-in with ESP32 Arduino core).
    - `BleCombo.h` (install via Arduino Library Manager or GitHub).

## Setup Instructions

1. **Install Libraries**:
   - Open Arduino IDE.
   - Go to `Sketch > Include Library > Manage Libraries`.
   - Search for and install `BleCombo` (or download from its GitHub repository).

2. **Upload the Code**:
   - Copy the code from `esp32_bluetooth_keyboard.ino` into the Arduino IDE.
   - Select your ESP32 board under `Tools > Board` (e.g., "ESP32 Dev Module").
   - Connect your ESP32 via USB and select the correct port under `Tools > Port`.
   - Click `Upload` to flash the code.
   - or via esptool : esptool.py --chip esp32 --port COMx --baud 115200 write_flash 0x0 ESP32-BT-Keyboard.bin

3. **Pair with Bluetooth**:
   - After uploading, the ESP32 will appear as a Bluetooth device on your computer or phone.
   - Pair it using your device's Bluetooth settings (no PIN required in most cases).

4. **Connect to WiFi**:
   - Open your device's WiFi settings.
   - Connect to the access point named `ESP32-Keyboard` with the password `password123`.

5. **Access the Keyboard**:
   - Open a web browser and enter the IP address printed in the Serial Monitor (e.g., `192.168.4.1`).
   - The keyboard interface will load.

## Usage

- **Typing**: Click the on-screen keys to send characters to the paired device. Letters are lowercase by default.
- **Caps Lock**: Click "Caps Lock" to toggle between uppercase and lowercase letters. The indicator turns green when ON, gray when OFF.
- **Special Keys**: Use Backspace ("Bksp"), Tab, Enter, arrow keys, and function keys (F1-F12) as needed.
- **Modifiers**: Shift, Ctrl, Alt, and Fn are included but not fully implemented (they log a message in the Serial Monitor).

## Code Structure

- **`setup()`**: Initializes Serial, Bluetooth, WiFi AP, and web server routes.
- **`loop()`**: Handles incoming web requests.
- **`handleRoot()`**: Serves the HTML keyboard interface.
- **`handleKey()`**: Processes keypresses and sends them via Bluetooth.
- **`handleCapsLock()`**: Toggles the Caps Lock state and updates the indicator.

## Troubleshooting

- **Keyboard Not Responding**:
  - Ensure the ESP32 is paired with your device via Bluetooth.
  - Check the Serial Monitor (115200 baud) for debug messages like "Bluetooth connected!".
- **Web Interface Not Loading**:
  - Verify you're connected to the `ESP32-Keyboard` WiFi AP.
  - Confirm the IP address in the Serial Monitor matches what you entered in the browser.
- **Keys Not Working**:
  - Check if the `BleCombo` library is correctly installed and compatible with your ESP32 board.

## Future Improvements

- Implement Shift, Ctrl, Alt, and Fn as functional modifiers.
- Add a Shift toggle to display alternate symbols (e.g., `!` instead of `1`).
- Improve arrow key layout to match the inverted-T style of laptops.
- Add support for multiple simultaneous keypresses (e.g., Ctrl+Alt+Del).
