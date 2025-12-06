# Rotary Encoder with Hardware Interrupts (Arduino)

This project demonstrates how to use **hardware interrupts** on the Arduino to read a **rotary encoder**, determine motor rotation **direction**, and compute **speed (ticks per second)** while controlling a DC motor through an **L298N** driver.

---

##  Features
- Read Encoder Channel A using **INT0 hardware interrupt**
- Detect rotation direction using **Channel B**
- Compute motor speed in **ticks/sec**
- Control motor speed & direction using **L298N driver**
- Automatic direction change every 5 seconds for demonstration

---

##  Hardware Connections

### **Rotary Encoder**
| Encoder Channel | Arduino Pin | Description |
|-----------------|-------------|-------------|
| Channel A       | D2 (INT0)   | Interrupt on rising edge |
| Channel B       | D4          | Used to detect direction |

### **L298N Motor Driver**
| Motor Pin | Arduino Pin | Function |
|-----------|-------------|----------|
| IN1       | D5          | Motor direction |
| IN2       | D6          | Motor direction |
| ENA       | D9 (PWM)    | Motor speed control |

---

##  Simulation (TinkerCad)

You can view and test the full circuit and code on TinkerCad:  

 *TinkerCad Project Link:*  
*https://www.tinkercad.com/things/6MDvf5pSmuo-q3ass/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard*


##  How It Works

### **1. Hardware Interrupt**
The encoder's Channel A triggers `handleEncoder()` on every **rising edge**:

```cpp
attachInterrupt(digitalPinToInterrupt(encoderA), handleEncoder, RISING);
