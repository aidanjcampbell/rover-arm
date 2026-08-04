# Rover-Arm — 4-DOF Rover-Mounted Robotic Arm

![media/herogif_edit.gif](media/herogif_edit.gif)

**A 3D-printed 4-DOF robotic arm lifting 1000 g at 30 cm reach, 200% of its 0.5 kg design target. 2x 12V DC Motors drive the rover while 4x Feetech servos power the arm, all controlled wirelessly by the ESP32. Designed, built, and tested from scratch.**
---

## Contents
- [Demo](#demo)
- [Overview](#overview)
- [Specifications](#specifications)
- [Key Engineering Decisions](#key-engineering-decisions)
- [Requirements & Verification](#requirements--verification)
- [Build Gallery](#build-gallery)
- [Bill of Materials](#bill-of-materials)
- [CAD & Firmware](#cad--firmware)
- [Development Timeline](#development-timeline)

---

## Demo

| | |
|---|---|
| <video src="https://github.com/user-attachments/assets/dd992043-898a-44c7-ab88-0e8c2c1f0734" width="600" controls></video> | **Payload test** — 1000 g held at 30 cm extension |
| <video src="https://github.com/user-attachments/assets/9e35d78c-9f56-43b6-9285-de8e3515913f" width="600" controls></video> | **Range of motion** — all 4 DOF |
| <video src="https://github.com/user-attachments/assets/c3312b9b-7194-4d76-a24a-d6187b4e6c03" width="600" controls></video> | **Control** — hold-to-jog from phone over onboard WiFi |

---

## Overview

I set out to design a mobile rover-mounted manipulator that lifts 0.5 kg at 30 cm extension, with every structural component printable on a consumer FDM machine. The arm has a 2-axis shoulder (pitch + yaw), single-axis elbow and wrist, and a fork-style end effector, all driven by Feetech serial bus servos on a single half-duplex TTL line.

All structural design, torque budgeting, and component selection are my own work. Firmware was developed with AI assistance (Claude) to move quickly through boilerplate code and debugging, keeping my focus on the mechanical design and system integration. I directed the architecture, integrated it with the hardware, and validated all behavior on the physical build.

---

## Specifications

| Parameter | Value |
|---|---|
| Design payload | 0.5 kg @ 30 cm extension |
| Demonstrated payload | 1.0 kg @ 30 cm |
| Reach | 30 cm |
| Actuators | ST3250 (50 kgf·cm) · 3× ST3215 (30 kgf·cm) |
|Drive Motors| 2x 12V DC 100RPM Motors |
| Controller | ESP32-WROOM-32 (Waveshare General Driver for Robots) |
| Control | Onboard WiFi AP + web UI, hold-to-jog |
| Power | 3S LiPo (11.1 V nominal) |
---

## Key Engineering Decisions

### Switching Control Architecture

Intending to build the entire rover off of the Arduino Uno R3 as my command module, I developed the entire base of the rover using this architecture until I realized that its limited pin numbers were going to cause problems when adding the servo bus. I then made the decision to start from scratch with the ESP32 General Robotics Driver.

Result: The General Robotics Driver integrated the TB6612FNG motor driver, wireless control, and the servo control all into one chip, giving us the number of pins we needed while simplifying electronics with a 47% reduction in internal wires (34 before, 18 after). Here is the full breakdown: [docs/wiringdiagrams.md](docs/wiringdiagrams.md)

### Servo Selection & Safety Factor
As I had no prior experience with robotic arms, I needed a way to break down the problem of servo selection into first principles. To do this, I calculated the torque required for the shoulder joint to lift the arm and a 0.5 KG payload with a safety factor of 2.64 to be 52.9 kg·cm. I also did these calculations for the other joints, which you can see alongside the actual math [here](docs/torquecalculations.md).

Result: I chose the ST3250 servo with 50kg·cm of torque for the shoulder joint. Though it was 5.5% under my calculated safety factor, I reasoned it to be the ideal choice (click the link above for details), and the payload tests affirmed this decision.

### Warren-Truss Forearm Link

The elbow-to-wrist link needed high bending stiffness at a very low mass to allow the actuators to lift the target payload. Rather than a solid printed beam, I used a Warren-truss geometry, which carries load through members in tension/compression rather than bending. The Fusion 360 model is parametric with member angles and lengths driven by expressions of the link length, so the truss regenerates correctly whenever the overall geometry changes without manual rework. See the render beside the integrated truss [here](#build-gallery).

### Base Stability Under Payload

Passing the payload test exposed the next constraint: at full extension, the combined center of gravity shifts forward of the front wheel contact line and the rover tips. Because this is a *mobile* platform, any counterweight added needs to be modular and fixed opposite the arm on the rotating base to offset the tip ping moment about the wheelbase.

Result: I developed and designed a modular counterweight system adaptable to any load the rover might undertake using readily available 2.5 lb. gym plates. The counterweight system scales by roughly 1 plate for every 500g added to the arm, allowing the rover to maintain mobility no matter the load. See more [here](docs/torquecalculations.md).

---

## Requirements & Verification

| # | Requirement | Design response | Verification |
|---|---|---|---|
| R1 | Lift 0.5 kg @ 30 cm | ST3250 shoulder + optimized truss designs + modular counterweight system | ✅ **Passed** — held 0.5 kg at full extension; demonstrated to 1.0 kg |
| R2 | Mobile rover base capable of supporting arm | Differential drive DC motors + custom ball bearing + counterweight | ✅ **Passed** — Rover stays stable while executing rapid maneuvers with an arm payload of 1000g (view the mobility test [here](media/mobility-test.mp4)) |
| R3 | Untethered operation | Onboard WiFi AP + 3S LiPo | ✅ **Passed** — operated for 20 minutes with arm under variable loads |
| R4 | Consumer-FDM printable | All parts fit P1S bed | ✅ Verified — print inventory in `/cad` |

---

## Build Gallery

<table>
  <tr>
    <td><img src="media/chassis-render.png" width="400"/></td>
    <td><img src="media/internalchassis-photo.jpg" width="400"/></td>
  </tr>
  <tr>
    <td align="center"><em>Chassis render — CAD</em></td>
    <td align="center"><em>As built with internal electronics</em></td>
  </tr>
  <tr>
    <td><img src="media/elbow-wrist-link-render.PNG" width="400"/></td>
    <td><img src="media/elbow-wrist-link-photo.jpg" width="400"/></td>
  </tr>
  <tr>
    <td align="center"><em>Elbow-wrist link render — CAD</em></td>
    <td align="center"><em>As built integrated into arm </em></td>
  </tr>
  <tr>
    <td><img src="media/counterweight-assembly-render.png" width="400"/></td>
    <td><img src="media/counterweight-assembly-photo.jpg" width="400"/></td>
  </tr>
  <tr>
    <td align="center"><em>Counterweight assembly render— CAD</em></td>
    <td align="center"><em>As built with 2.5LB(1.1KG) plate </em></td>
  </tr>
  <tr>
    <td><img src="media/wheel-assembly-render.PNG" width="400"/></td>
    <td><img src="media/wheel_assembly-photo.jpg" width="400"/></td>
  </tr>
  <tr>
    <td align="center"><em>Wheel assembly — CAD</em></td>
    <td align="center"><em>As built with non-drive wheel and TPU tire </em></td>
  </tr>
</table>

---

## Bill of Materials

*Key components below. [Full BOM — all hardware, quantities, costs (XLSX)](docs/BOM.xlsx)

| Component | Qty | Spec | Source | Price |
|---|---|---|---|---|
| Feetech ST3250 | 1 | 50 kgf·cm, 12V | [Babsco Supply](https://shop.babsco.com/feetech-sts3250-50kg-heavy-duty-smart-servo) | $64.99 |
| Feetech ST3215 | 3 | 30 kgf·cm, 12V | [DFRobot](https://www.dfrobot.com/product-2962.html) | $19.99 ea |
| Waveshare General Driver for Robots | 1 | ESP32 + TB6612FNG + Servo Bus Control| [Amazon](https://www.amazon.com/Multi-Functional-ESP32-WROOM-32-Bluetooth-Communications-Compatible/dp/B0BZLHYBJL/ref=sr_1_2?crid=2X70I97140UVL&dib=eyJ2IjoiMSJ9.Qyy5JpWZVMOQAArPpvWQb1AhzzfMAPLMIgsMUYPC83mh793Pu2iuxlsDr73dC-eEvAihRaoxWlQTaKoN1ffJHItiUypsE7zhgsDQBir2OVPWklsZlQ8yNga2u20Krc-0RlrBY_QiIJeslw7W6vIPyLah9GjKpbLXJRoLWDFNdnvpTjmCQdbDVQ4sih9uB5aO_ct0vCgJj7_Nj_xZzSFNeyusjApYTRTCCBafkMetzHs.KhaRM6i3_DAIpC6gJnnSmIOD6BadsKU0M5LF1ErKx2k&dib_tag=se&keywords=waveshare+general+robotics+driver&qid=1785364271&sprefix=waveshare+general+robotics+driver%2Caps%2C199&sr=8-2) | $27.99 |
| Ovonic 3S LiPo | 1 | 11.1V 2200 mAh XT60 | [Amazon](https://www.amazon.com/2200mAh-Connector-Airplane-Quadcopter-Helicopter/dp/B08CZF373Y/ref=sr_1_11?crid=1PP7ODDTYOGMP&dib=eyJ2IjoiMSJ9.iOSWr3M_JHZ9VR8sHd9bgMSiiGApRLvMbo2dmAuFNxfPYa1OoWudkSQXx3j8jNUBqY2dwpkGCyyO8FP7hpkrX7ZnI042PlRQuCzQDilbn9TVTeT5Kbf1NsoFfEKXwrrE9zeMqbBhVeqAwM3D4Ka9YYG7Jc9s-YPQlavun6GTr6pyFDddOGYnPGrEfcAsJtA_EChIi-WRp0FS-Zt9cRvERWU5VlRl8uhEzjkiJzKjXckz6XKlOZweCqsyDzCoiH3B3mr1841YgFYd0CxHaTjQKpnEAJ8eeo_Dm-sj07VuswI.gRa2c_E-S0ACev-dciJZ67Anf647mEQ9EKgyPAOQ0BI&dib_tag=se&keywords=11.1V%2B220mAH%2Blipo%2Bovonic&qid=1785364581&sprefix=11.1v%2B220mah%2Blipo%2Caps%2C148&sr=8-11&th=1) | $16.99 |
| DC 12V 100RPM Motor | 2 | 100RPM 12V | [Amazon](https://www.amazon.com/Greartisan-Electric-Reduction-Centric-Diameter/dp/B072R57C56/ref=sr_1_5?crid=1A0FK62I470ZI&dib=eyJ2IjoiMSJ9.n8uJGkMnr4Kh8sD-n_JHp0iVcK1ON5Nc-JRtxH72hQWVOoMOjWkENwFX7h1ZLqi8toDa89avcobrDLc0GWA7_lSv4e0OYhpCC_hzYTCpIK1Uw0YJVO9OFmLWvmvEzFNg8PNBdsJGG0DJQ7qmZJ32NVx_-W2aj4pGe7wD9hqyKJC2ZlnKU1V08XDalVTrKV3LplMGrpnlkz_Z8dxxdnvuFx-WE72J_9fMzmB9tAAM48PKzJsRy6Uh79BCXsBlCShlHTEI5dAGvAtNdmQRWMigERbxi6EchFEeW8YzwWxHGKo.PdwCm_5wz4GpXb70i-l5rC5gk3XwGNLnRPAhMFnnFEg&dib_tag=se&keywords=100rpm%2B12v%2Bdc%2Bmotor&qid=1785364453&sprefix=100rpm%2B12v%2Bdc%2Bmotor%2Caps%2C147&sr=8-5&th=1) | $14.99 ea |

*Last verified: August 2026. Total: $330.15.*

---

## CAD & Firmware

**CAD** — [STEP files](cad/step/) (any CAD tool) · [STL files](cad/stl/) (print-ready; renders these in an interactive 3D viewer)

**Firmware** — PlatformIO (pioarduino ESP32 fork) + SCServo library. WiFi AP web server with hold-to-jog control, per-joint calibration arrays, build-timestamp version verification at boot.

```bash
pio run -t upload          # build & flash
# connect to "RoverArm" AP → http://192.168.4.1
```

Source and structure in [`/src`](src/).

---

## Development Timeline

- **Getting Started** - Prototyped rover base with intention to use Arduino Uno R3 & HC-05 Bluetooth module for wireless control
- **Architecture Swap** - Swapped Arduino for ESP32 General Robotics Driver after realizing pin limitations (see more [here](docs/wiringdiagrams.md))
- **Actuator sizing** - Torque & inertia calculations + safety-factor decomposition → ST3250 at shoulder pitch (see calculations [here](docs/torquecalculations.md)).
- **Servo bus bring-up** - Isolated a driver-board transceiver fault ([case study](docs/servo-issue.md)); first joint motion on replacement board.
- **Elbow-Wrist Linkage** - Designed/printed lightweight Warren-truss forearm to optimize strength within weight constraints
- **Payload test passed** - 0.5 kg at 30 cm (R1); demonstrated to 1.0 kg.
- **Base stability** — Diagnosed forward tipping resulting from heavy loads lifted by the arm and designed and printed a modular counterweight structure that could be variably loaded depending on arm load
- **Shoulder Joint Failure** - When lifting 1000g, the shoulder-elbow linkage deformed where it mounted to the shoulder servo and caused the arm to fail (view [here](media/1000g-test-failure.mp4)). Resolved by increasing the wall thickness and infill percent on the second iteration print.
- **1000g Payload Test** - Arm successfully lifted 1000g (200% of design target) and demonstrated mobile capability while under that load. Due to heat in the shoulder servo body, testing has been stopped to ensure the rover's continued function.

---

## About

Mechanical engineering student, Georgia Tech. [email](mailto:aidanjcampbell1@gmail.com)
