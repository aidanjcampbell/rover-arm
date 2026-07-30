# Wiring & Control Architecture
 
The control electronics were migrated from an initial **Arduino Uno** prototype to the **Waveshare General Driver for Robots 
(ESP32-WROOM-32)**. This document shows both wiring architectures and explains the reasoning behind the switch.

## Why the architecture changed

The Uno prototype hit three hard limits, each tied directly to a requirement of this build. 
The decisive factor was the **serial bus servos**: the Feetech actuators run on a half-duplex serial bus at 1,000,000 baud, 
which needs a *dedicated* hardware UART. The Uno has a single UART shared with USB programming/debug, 
so the servo bus and the debug link contend for the same peripheral. Layer on the need for a wireless control
interface and onboard motor driving, and the Uno would have required a motor shield plus a Wi-Fi shield plus 
level-shifting — three boards doing what the Waveshare does on one.

| Requirement | Arduino Uno | Waveshare (ESP32) |
|---|---|---|
| Serial bus servos @ 1 Mbaud | Single UART, shared with USB — no dedicated bus | Multiple hardware UARTs; dedicated `Serial1` on GPIO18/19 |
| Wireless control | None, requires HC-05 bluetooth module | Onboard Wi-Fi; hosts the `RoverArm` AP + web UI at 192.168.4.1 |
| Motor driving | Requires separate motor driver | Integrated TB6612FNG driver |
| Compute headroom | 8-bit, 16 MHz, 2 KB SRAM | 32-bit dual-core, 240 MHz, ~520 KB SRAM |

The ESP32's headroom also matters: hosting a web server *and* coordinating multi-servo control concurrently is comfortable on the dual-core ESP32 and marginal-to-infeasible on the Uno. The switch consolidated the electronics onto a single robotics-purpose board and freed a dedicated UART for the servo bus — which later proved important when diagnosing a servo bus power issue (see [`engineering-log.md`](engineering-log.md)).

## Diagram 1 — Original architecture (Arduino Uno)

<img src="media/wiring-diagram-v.1.svg" width="700" alt="Original wiring: Arduino Uno prototype architecture">

<sub>[Full-resolution PDF](pdf/wiring-arduino-uno.pdf)</sub>

The initial prototype: [FILL IN — briefly, what this diagram shows, e.g. Uno driving the servos via software serial, external motor driver, tethered/USB control]. This layout exposed the UART contention and the lack of onboard wireless that drove the migration.

## Diagram 2 — Current architecture (Waveshare ESP32)

<img src="media/wiring-diagram-v.2.svg" width="700" alt="Current wiring: Waveshare General Driver (ESP32) architecture">

<sub>[Full-resolution PDF](pdf/wiring-waveshare.pdf)</sub>

The current system: power distribution from the 3S LiPo (via XT60), the ESP32 hosting the Wi-Fi AP and web interface, the integrated TB6612FNG driving the drive motors, and the Feetech servo bus running on `Serial1` (GPIO18 `S_RXD` / GPIO19 `S_TXD`), half-duplex at 1,000,000 baud. Wire gauges are sized to current load per the table in [`electrical.md`](electrical.md).
