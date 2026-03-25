# ATmega2560 Environmental Monitor

A fully self-contained, battery-powered temperature monitor built on the Arduino Mega 2560. Reads ambient temperature via a thermistor, converts it using the Steinhart-Hart equation with empirical calibration, and displays the result on a SparkFun SerLCD over I2C. Housed in a custom 3D printed enclosure with a hand-soldered perfboard circuit.

---

## Features

- **Steinhart-Hart thermistor conversion** with empirically derived calibration coefficients for improved accuracy
- **I2C SerLCD display** showing real-time temperature in °C or °F
- **Unit toggle button** — press to switch between Celsius and Fahrenheit instantly
- **RGB backlight cycling button** — cycles through 8 backlight colors
- **Debounced button state machine** for both inputs
- **Battery powered** — 9V via onboard VIN, slide switch for power control
- **Custom enclosure** — 3D printed in SolidWorks, ventilation holes on both sides, removable lid

---

## Hardware

| Component | Part |
|---|---|
| Microcontroller | Arduino Mega 2560 |
| Display | SparkFun SerLCD (I2C) |
| Sensor | 10kΩ NTC Thermistor |
| Circuit Board | Perfboard (hand-soldered) |
| Series Resistor | 10kΩ axial |
| Power Switch | SPDT slide switch |
| Unit Toggle | Momentary pushbutton (N.O.) |
| Color Cycle | Momentary pushbutton (N.O.) |
| Power | 9V battery |

### Wiring

```
9V Battery → Slide Switch → Arduino Mega VIN
                                    │
                     ┌──────────────┼──────────────┐
                     │              │              │
               SerLCD (I2C)    Thermistor      Buttons
               SDA → A4        A0 (with        D2 (color)
               SCL → A5        10kΩ series     D3 (unit)
               3.3V, GND       resistor)
```

---

## Firmware

### Temperature Conversion

The thermistor resistance is computed from the ADC reading via a voltage divider, then converted to temperature using the **Steinhart-Hart simplified B-coefficient equation**:

```
1/T = 1/T₀ + (1/B) * ln(R/R₀)
```

Where:
- `T₀` = 298.15 K (25°C nominal)
- `R₀` = 10,000 Ω (nominal resistance)
- `B` = 3950 (B-coefficient)

A linear calibration correction (`slope = 1.0195`, `offset = -1.2698`) is applied after conversion, derived from empirical measurements against a reference thermometer.

### Button State Machine

Both buttons use a debounced edge-detection pattern — action triggers only on `LOW && prevState == HIGH` to prevent repeated firing on a single press:

```cpp
bool currentState = digitalRead(buttonPin);
if (currentState == LOW && lastState == HIGH) {
    // handle press
    delay(50); // debounce
}
lastState = currentState;
```

### Display Update Rate

Temperature is read every ~2 seconds (200 loop iterations × 10ms delay). Pressing the unit button triggers an immediate refresh.

---

## Project Structure

```
atmega2560-env-monitor/
├── src/
│   └── main.cpp         # Full firmware
├── docs/
│   └── schematic.png    # Hardware schematic
├── enclosure/           # SolidWorks files 
└── README.md
```

---

## Build & Flash

1. Open in Arduino IDE or PlatformIO
2. Select **Arduino Mega 2560** as target board
3. Install the [SparkFun SerLCD library](https://github.com/sparkfun/SparkFun_SerLCD_Arduino_Library)
4. Flash via USB

Serial output at **9600 baud** for debugging:
```
Temperature: 24.5 *C  /  76.1 *F
Temperature: 24.6 *C  /  76.3 *F
```

---

## Photos

![](/Figures/PXL_20260311_055458537.jpg)
![](/Figures/PXL_20260311_022752965.jpg)
![](/Figures/PXL_20260310_230406552.jpg)
![](/Figures/IMG_3299.jpg)


---

## Contributors

- **Lukas Krapukaitis** — firmware, software, calibration
- **Seth Baird** — perfboard circuit, hardware assembly

---

## License

MIT
