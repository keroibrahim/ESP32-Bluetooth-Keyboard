#include <WiFi.h>
#include <WebServer.h>
#include <BleCombo.h> // For keyboard emulation

WebServer server(80);

const char* ssid = "ESP32-Keyboard";
const char* password = "password123";

bool capsLock = false; // Track Caps Lock state

void setup() {
  Serial.begin(115200);
  
  // Initialize keyboard emulation
  Keyboard.begin();
  
  // Wait for Bluetooth connection
  Serial.println("Waiting for Bluetooth connection...");
  while (!Keyboard.isConnected()) {
    delay(100);
  }
  Serial.println("Bluetooth connected!");

  // Create WiFi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/key", handleKey);
  server.on("/capslock", handleCapsLock);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><head><title>ESP32 Keyboard</title>";
  html += "<style>";
  html += "button {font-size: 14px; padding: 10px; margin: 2px; width: 50px; height: 50px; border-radius: 5px; border: 1px solid #333; background-color: #f0f0f0; white-space: nowrap; overflow: hidden; text-overflow: ellipsis;}";
  html += ".wide {width: 75px;} .wider {width: 100px;} .space {width: 300px;} .caps {width: 90px;}";
  html += ".keyboard-row {display: flex; justify-content: center; margin-bottom: 5px;}";
  html += ".spacer {width: 25px; height: 50px; display: inline-block;}";
  html += ".caps-indicator {width: 20px; height: 20px; border-radius: 50%; display: inline-block; margin-left: 10px; vertical-align: middle;}";
  html += ".caps-off {background-color: #888;}";
  html += ".caps-on {background-color: #00ff00;}";
  html += "</style>";
  html += "<script>";
  html += "function sendKey(char) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/key?char=' + char, true);";
  html += "  xhr.send();";
  html += "}";
  html += "function toggleCapsLock() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/capslock', true);";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      var indicator = document.getElementById('capsIndicator');";
  html += "      indicator.className = 'caps-indicator ' + (xhr.responseText === 'ON' ? 'caps-on' : 'caps-off');";
  html += "    }";
  html += "  };";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</head><body>";
  
  // On-screen keyboard
  html += "<div style='text-align: center; margin-bottom: 10px;'>";
  html += "<span style='font-size: 14px; margin-right: 5px;'>Caps Lock</span>";
  html += "<span id='capsIndicator' class='caps-indicator " + String(capsLock ? "caps-on" : "caps-off") + "'></span>";
  html += "</div>";
  
  // Row 1: Function keys and Esc (simplified)
  html += "<div class='keyboard-row'>";
  const char* row1[] = {"ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
  for (int i = 0; i < 13; i++) {
    html += "<button onclick=\"sendKey('" + String(row1[i]) + "')\">" + String(row1[i]) + "</button>";
  }
  html += "</div>";

  // Row 2: Numbers and Symbols
  html += "<div class='keyboard-row'>";
  const char* row2[] = {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE"};
  for (int i = 0; i < 14; i++) {
    if (String(row2[i]) == "BACKSPACE") {
      html += "<button class='wider' onclick=\"sendKey('" + String(row2[i]) + "')\">Bksp</button>";
    } else {
      html += "<button onclick=\"sendKey('" + String(row2[i]) + "')\">" + String(row2[i]) + "</button>";
    }
  }
  html += "</div>";

  // Row 3: Tab and QWERTY
  html += "<div class='keyboard-row'>";
  const char* row3[] = {"TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\"};
  for (int i = 0; i < 14; i++) {
    if (String(row3[i]) == "TAB") {
      html += "<button class='wide' onclick=\"sendKey('" + String(row3[i]) + "')\">" + String(row3[i]) + "</button>";
    } else if (String(row3[i]) == "\\") {
      html += "<button class='wide' onclick=\"sendKey('" + String(row3[i]) + "')\">" + String(row3[i]) + "</button>";
    } else {
      html += "<button onclick=\"sendKey('" + String(row3[i]) + "')\">" + String(row3[i]) + "</button>";
    }
  }
  html += "</div>";

  // Row 4: Caps Lock and ASDF
  html += "<div class='keyboard-row'>";
  const char* row4[] = {"CAPS", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "ENTER"};
  for (int i = 0; i < 13; i++) {
    if (String(row4[i]) == "CAPS") {
      html += "<button class='caps' onclick=\"toggleCapsLock()\">Caps Lock</button>";
    } else if (String(row4[i]) == "ENTER") {
      html += "<button class='wider' onclick=\"sendKey('" + String(row4[i]) + "')\">" + String(row4[i]) + "</button>";
    } else {
      html += "<button onclick=\"sendKey('" + String(row4[i]) + "')\">" + String(row4[i]) + "</button>";
    }
  }
  html += "</div>";

  // Row 5: Shift and ZXCV
  html += "<div class='keyboard-row'>";
  const char* row5[] = {"SHIFT", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "SHIFT"};
  for (int i = 0; i < 12; i++) {
    if (String(row5[i]) == "SHIFT") {
      html += "<button class='wider' onclick=\"sendKey('" + String(row5[i]) + "')\">" + String(row5[i]) + "</button>";
    } else {
      html += "<button onclick=\"sendKey('" + String(row5[i]) + "')\">" + String(row5[i]) + "</button>";
    }
  }
  html += "</div>";

  // Row 6: Bottom row (Ctrl, Fn, Alt, Space, Arrows)
  html += "<div class='keyboard-row'>";
  html += "<button class='wide' onclick=\"sendKey('CTRL')\">Ctrl</button>";
  html += "<button class='wide' onclick=\"sendKey('FN')\">Fn</button>";
  html += "<button class='wide' onclick=\"sendKey('ALT')\">Alt</button>";
  html += "<button class='space' onclick=\"sendKey('SPACE')\">Space</button>";
  html += "<button class='wide' onclick=\"sendKey('ALT')\">Alt</button>";
  html += "<button onclick=\"sendKey('LEFT')\">Left</button>";
  html += "<button onclick=\"sendKey('UP')\">Up</button>";
  html += "<button onclick=\"sendKey('DOWN')\">Down</button>";
  html += "<button onclick=\"sendKey('RIGHT')\">Right</button>";
  html += "</div>";

  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleKey() {
  String key = server.arg("char");
  
  if (Keyboard.isConnected()) {
    if (key == "SPACE") {
      Serial.println("Sending SPACE");
      Keyboard.write(' ');
    } else if (key == "BACKSPACE") {
      Serial.println("Sending BACKSPACE");
      Keyboard.write(KEY_BACKSPACE);
    } else if (key == "TAB") {
      Serial.println("Sending TAB");
      Keyboard.write(KEY_TAB);
    } else if (key == "ENTER") {
      Serial.println("Sending ENTER");
      Keyboard.write(KEY_RETURN);
    } else if (key == "ESC") {
      Serial.println("Sending ESC");
      Keyboard.write(KEY_ESC);
    } else if (key == "F1") {
      Serial.println("Sending F1");
      Keyboard.write(KEY_F1);
    } else if (key == "F2") {
      Serial.println("Sending F2");
      Keyboard.write(KEY_F2);
    } else if (key == "F3") {
      Serial.println("Sending F3");
      Keyboard.write(KEY_F3);
    } else if (key == "F4") {
      Serial.println("Sending F4");
      Keyboard.write(KEY_F4);
    } else if (key == "F5") {
      Serial.println("Sending F5");
      Keyboard.write(KEY_F5);
    } else if (key == "F6") {
      Serial.println("Sending F6");
      Keyboard.write(KEY_F6);
    } else if (key == "F7") {
      Serial.println("Sending F7");
      Keyboard.write(KEY_F7);
    } else if (key == "F8") {
      Serial.println("Sending F8");
      Keyboard.write(KEY_F8);
    } else if (key == "F9") {
      Serial.println("Sending F9");
      Keyboard.write(KEY_F9);
    } else if (key == "F10") {
      Serial.println("Sending F10");
      Keyboard.write(KEY_F10);
    } else if (key == "F11") {
      Serial.println("Sending F11");
      Keyboard.write(KEY_F11);
    } else if (key == "F12") {
      Serial.println("Sending F12");
      Keyboard.write(KEY_F12);
    } else if (key == "SHIFT") {
      Serial.println("Shift key ignored (not implemented)");
    } else if (key == "CTRL") {
      Serial.println("Ctrl key ignored (not implemented)");
    } else if (key == "ALT") {
      Serial.println("Alt key ignored (not implemented)");
    } else if (key == "FN") {
      Serial.println("Fn key ignored (not implemented)");
    } else if (key == "LEFT") {
      Serial.println("Sending LEFT");
      Keyboard.write(KEY_LEFT_ARROW);
    } else if (key == "RIGHT") {
      Serial.println("Sending RIGHT");
      Keyboard.write(KEY_RIGHT_ARROW);
    } else if (key == "UP") {
      Serial.println("Sending UP");
      Keyboard.write(KEY_UP_ARROW);
    } else if (key == "DOWN") {
      Serial.println("Sending DOWN");
      Keyboard.write(KEY_DOWN_ARROW);
    } else if (key.length() == 1) {
      char charToSend = key[0];
      // If Caps Lock is ON and the character is a letter, convert to uppercase
      if (capsLock && charToSend >= 'a' && charToSend <= 'z') {
        charToSend = charToSend - 'a' + 'A';
      }
      Serial.println("Sending key: " + String(charToSend));
      Keyboard.write(charToSend);
    }
  } else {
    Serial.println("Bluetooth not connected");
  }
  
  server.send(200, "text/plain", "OK");
}

void handleCapsLock() {
  capsLock = !capsLock; // Toggle Caps Lock state
  Serial.println("Caps Lock toggled: " + String(capsLock ? "ON" : "OFF"));
  server.send(200, "text/plain", capsLock ? "ON" : "OFF");
}
