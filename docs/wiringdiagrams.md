# Wiring & Control Architecture
 
The control electronics were migrated from an initial **Arduino Uno** prototype to the **Waveshare General Driver for Robots 
(ESP32-WROOM-32)**. This document shows both wiring architectures and explains the reasoning behind the switch.

## Why the architecture changed

The Uno prototype hit a hard limit with pin space, a crucial component for the platform.
The decisive factor was the **serial bus servos**: the Feetech actuators run on a half-duplex serial bus at 1,000,000 baud, 
which needs a *dedicated* hardware UART. The Uno has a single UART shared with USB programming/debug, 
so the servo bus and the debug link contend for the same peripheral, meaning a dedicated servo controller wpould have to be added into the architecture. Add in the motor driver and HC-05 Bluetooth module along with the dedicated servo controller, and the Arduino does not have enough pins to support that architecture, not to mention its complexity.

| Requirement | Arduino Uno | Waveshare (ESP32) |
|---|---|---|
| Serial bus servos @ 1 Mbaud | Single UART, shared with USB — no dedicated bus | Multiple hardware UARTs; dedicated `Serial1` on GPIO18/19 |
| Wireless control | None, requires HC-05 bluetooth module | Onboard Wi-Fi; hosts the `RoverArm` AP + web UI at 192.168.4.1 |
| Motor driving | Requires separate motor driver | Integrated TB6612FNG driver |

The switch consolidated the electronics onto a single robotics-purpose board and freed a dedicated UART for the servo bus, which later proved important when diagnosing a servo bus power issue (see [`servofaultisolation.md`](servofaultisolation.md)).

## Diagram 1 — Original architecture (Arduino Uno)

<img src="/media/wiring-diagram-v.1.svg" width="700" alt="Original wiring: Arduino Uno prototype architecture">

The initial prototype: This setup worked well in the beginning as I was familiar with using the Arduino Uno R3, but quickly reached its limits once I needed to add the servo bus and control.

## Diagram 2 — Current architecture (Waveshare ESP32)

<img src="/media/wiring-diagram-v.2.svg" width="700" alt="Current wiring: Waveshare General Driver (ESP32) architecture">

The current system: power distribution from the 3S LiPo, the ESP32 hosting the Wi-Fi AP and web interface, the integrated TB6612FNG driving the drive motors, and the Feetech servo bus running on `Serial1` (GPIO18 `S_RXD` / GPIO19 `S_TXD`), half-duplex at 1,000,000 baud.
