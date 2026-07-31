# Torque Calculations — Robotic Arm

Torque sizing and actuator selection for the 4-DOF arm, worked from first principles.

## Requirement

Lift **0.5 kg** at **30 cm** maximum extension.

Configuration: 2-axis shoulder (pitch + yaw), single-axis elbow, single-axis wrist.

## Method
**Total Torque:**
```math
\tau_{\text{tot}} = \tau_s + \tau_I
```
**Static torque about a joint:**
```math
\tau_{\text{s}} = m \cdot L \cdot \cos\theta
```

For a multi-joint system, a joint carries the payload **and** every link and servo outboard of it, each at its own moment arm:

```math
\tau_{\text{joint}} = \sum m \cdot L \cdot \cos\theta
```
Torque is expressed in **kgf·cm**. Because kilogram-force already carries gravitational acceleration, using mass in kg and length in cm yields torque directly. Worst case is $\theta = 0$ (arm horizontal), where the moment arm is longest.

**Inertial Torque about a joint:**

```math
\tau_I = I \cdot \alpha
```
Where
```math
I = \sum mr^2
```
and
```math
\alpha = \frac{4\Delta \theta}{T^2}
```
Because inertia is calculated in $\text{kg}\cdot m^2$, we convert lengths to meters to start. Once we calculate inertial torque, we will convert it from $\text{N}\cdot\text{m}$ to $\text{kgf}\cdot\text{cm}$ using 10.2 $\text{kgf}\cdot\text{cm}$ =1 $\text{N}\cdot\text{m}$ to match our static torque calculations.

---

## Shoulder Pitch Torque

This joint carries the entire arm, so it will have the most torque and will likely be the limiting factor.
Masses are estimated as point masses and lengths as the distances from the center of mass to the shoulder pitch axis.

| Symbol | Quantity | Value |
|---|---|---|
| $m_p$ | Payload mass | 0.5 kg |
| $L_p$ | Extension to payload | 30 cm |
| $m_{\text{se}}$ | Shoulder-elbow link | 0.05 kg |
| $L_{\text{se}}$ | Sholder-elbow link | 7.5 cm |
| $m_{\text{es}}$ | Elbow servo | 0.055 kg |
| $L_{\text{es}}$ | Elbow servo | 13 cm |
| $m_{\text{ew}}$ | Elbow-wrist link | 0.04 kg |
| $L_{\text{ew}}$ | Elbow-wrist link | 20.5 cm |
| $m_{\text{ws}}$ | Wrist servo | 0.055 kg |
| $L_{\text{ws}}$ | Wrist servo | 26 cm |
| $m_{\text{f}}$ | Fork | 0.03 kg |
| $L_{\text{f}}$ | Fork | 28 cm |

**Static Torque**

Evaluated at $\theta = 0$, where the arm will experience the maximum load.
Payload contribution:

```math
\tau_p = m_p \cdot L_p \cdot \cos 0^\circ = 0.5 \times 30 \times 1 = 15\ \text{kgf·cm}
```

Link + servo contributions:

```math
\tau_{\text{unloaded arm}} = ((m_{\text{se}} \cdot L_{\text{se}}) + (m_{\text{es}} \cdot L_{\text{es}}) + (m_{\text{ew}} \cdot L_{\text{ew}}) + (m_{\text{ws}} \cdot L_{\text{ws}}) + (m_{\text{f}} \cdot L_{\text{f}})) \cdot \cos 0^\circ = \text{4.18}\ \text{kgf·cm}
```

Total static torque at the shoulder:

```math
\tau_{\text{shoulder}} = \tau_p + \tau_{\text{unloaded arm}} = 19.18 \text{kgf·cm}
```

**Inertial Torque**

Because the acceleration of the arm does not have a requirement, we will set the time to accelerate the arm $90^\circ$ or 1.57 rad to 2 seconds.
```math
\alpha = \frac{4(1.57)}{2^2} = 1.57 \text{rad/s}^2
```
We then calculate the inertia using the data table above.
```math
I = (m_{\text{se}} \cdot (L_{\text{se}})^2) + (m_{\text{es}} \cdot (L_{\text{es}})^2) + (m_{\text{ew}} \cdot (L_{\text{ew}})^2) + (m_{\text{ws}} \cdot (L_{\text{ws}})^2) + (m_{\text{f}} \cdot (L_{\text{f}})^2) + (m_p \cdot (L_p)^2)= 0.0538 \text{kg}\cdot\text{m}^2
```
Then we use the formula for total inertial torque
```math
\tau_I = I \cdot \alpha = (1.57)\text{rad/s}^2 \cdot (0.0538)\text{kg}\cdot\text{m} = 0.08 \text{N}\cdot\text{m} 
```
Converting inertial torque from $\text{N}\cdot\text{m}$ to $\text{kg}\cdot\text{cm}$
```math
0.08 \text{N}\cdot\text{m} \cdot \frac{10.2 \text{kg}\cdot\text{cm}}{1 \text{N}\cdot\text{m}} = 0.86 \text{kgf}\cdot\text{cm}
```
**Total Shoulder Torque**
```math
\tau_\text{tot} = \tau_s + \tau_I = 19.18 \text{kgf·cm} + 0.86 \text{kgf·cm} = 20.04 \text{kgf·cm}
```
### Safety Factor

| Factor | Value | Rationale |
|---|---|---|
| Continuous vs. stall | 2.0 | Rated stall torque is not usable continuous torque |
| Estimation error | 1.2 | Uncertainty in masses and arm lengths |
| Friction | 1.1 | Joint friction|
| **Stacked** | **≈ 2.64×** | product of the above |

**Required stall torque:**

```math
\tau_\text{req} =  20.04 \text{kgf·cm} \cdot 2.6 = 52.9 \text{kgf·cm}
```

### Selection

| Spec | Value |
|---|---|
| Required stall torque | 52.9 kgf·cm |
| Servo selected | ST3250 |
| Rated stall torque | 50 kgf·cm |
| Margin | 5.5% under |

The ST3250 is under our calculated safety factor, giving us a new safety margin of 2.46. This is a deliberate choice, as we included a large error margin in our estimation and friction safety factor calculations. This choice eats into that safety factor by 5.5%, which is an acceptable tolerance. If the margin below the calculated safety factor was larger, we would have to select another servo.

---

## Other joints

### Elbow Torque

Because inertial torque is nominal at this joint and at the wrist joint, those calculations will be omitted as our estimation safety factor provides a significant margin for error.

| Symbol | Quantity | Value |
|---|---|---|
| $m_{\text{ew}}$ | Elbow-wrist link mass| 0.04 kg |
| $L_{\text{ew}}$ | Elbow-wrist link moment| 7.5 cm |
| $m_{\text{ws}}$ | Wrist servo mass| 0.055 kg |
| $L_{\text{ws}}$ | Wrist servo moment | 13 cm |
| $m_{\text{f}}$ | Fork mass| 0.03 kg |
| $L_{\text{f}}$ | Fork moment| 15 cm |
| $m_{\text{p}}$ | Payload  mass| 0.5 kg |
| $L_{\text{p}}$ | Payload  moment| 17 cm |


```math
\tau_{\text{elbow}} = \sum m \cdot L \cdot \cos 0^\circ = \text{9.9}\ \text{kgf·cm}
```


Required stall torque (× 2.64): **26.14 kgf·cm** → **ST3215** selected, rated 30 kgf·cm.

---

### Wrist Torque

| Symbol | Quantity | Value |
|---|---|---|
| $m_{\text{f}}$ | Fork mass| 0.03 kg |
| $L_{\text{f}}$ | Fork moment| 2 cm |
| $m_{\text{p}}$ | Payload mass| 0.5 kg |
| $L_{\text{f}}$ | Payload moment| 4 cm |



```math
\tau_{\text{wrist}} = \sum m \cdot L \cdot \cos 0^\circ = \text{2.06}\ \text{kgf·cm}
```

Required stall torque (× 2.6): **5.44 kgf·cm** → **ST3215** selected, rated 30 kgf·cm.

---
## Counterweight torque

The counterweight supplies the **restoring moment** that opposes the arm's tipping moment about the front tipping line. Because it is mounted on the shoulder opposite the arm, it swings through the same arc, so its moment arm is the horizontal distance from the tipping line to the plate's center of mass. Because the tipping line at the front wheels is the most prone to flipping, we will calculate the torque from this tipping point.

Plate spec: one 2.5 lb steel plate = **1.134 kg**.

| Symbol | Quantity | Value |
|---|---|---|
| $m_{cw}$ | Counterweight mass (1 plate) | 1.134 kg |
| $d_{cw}$ | Moment arm (tipping line → CW center of mass) | 23 cm |

### Single plate — 0.5 kg payload

```math
\tau_{cw} = m_{cw} \cdot d_{cw} = 1.134 \times 23 = 26.082\ \text{kgf·cm}
```

This 26.082 kgf·cm is the restoring torque the counterweight directly provides, sizing the arm to lift a 0.5 kg (500 g) payload at full extension. Combined with the chassis restoring moment, it yields the full stability margin recorded below.

### Two plates — 1.0 kg payload

Doubling the payload to 1.0 kg (1000 g) doubles the tipping moment the counterweight must offset. Since the 23 cm moment arm is fixed, the counterweight torque scales linearly with plate mass, so the plate count doubles. Two 2.5 lb plates give 2.267 kg:

```math
\tau_{cw} = m_{cw} \cdot d_{cw} = 2.267 \times 23 = 52.141\ \text{kgf·cm}
```

### Payload-to-counterweight scaling

| Payload | Counterweight | CW mass | Moment arm | Counterweight torque |
|---|---|---|---|---|
| 0.5 kg (500 g) | 1 × 2.5 lb plate | 1.134 kg | 23 cm | 26.082 kgf·cm |
| 1.0 kg (1000 g) | 2 × 2.5 lb plates | 2.267 kg | 23 cm | 52.141 kgf·cm |

With the moment arm held constant at 23 cm, each 2.5 lb plate contributes a fixed **26.082 kgf·cm** of restoring torque. The counterweight requirement therefore scales roughly one plate per 500 g of payload, which makes the modular system an easily extended sizing rule rather than a one-off value.

### Stability check: does the counterweight actually keep the rover upright?

Counterweight torque alone isn't the full picture — it has to be checked against everything trying to tip the rover forward: the arm's own structural weight plus the payload.

| Term | Value |
|---|---|
| $\tau_{arm}$ (arm structure) | 2.784 kgf·cm |
| $\tau_{payload}$ (0.5 kg payload) | 12.000 kgf·cm |
| **$\tau_{tip}$ (total tipping)** | **14.784 kgf·cm** |
| $\tau_{chassis}$ (chassis restoring) | 7.7722 kgf·cm |
| $\tau_{cw}$ (counterweight, 1 plate) | 26.082 kgf·cm |
| **$\tau_{restore}$ (total restoring)** | **33.854 kgf·cm** |

```math
SF = \frac{\tau_{restore}}{\tau_{tip}} = \frac{33.854}{14.784} = 2.29\times
```

Restoring torque exceeds tipping torque by **2.29×** at full extension (θ = 0), the worst-case pose — the rover is stable with meaningful margin, not just barely balanced. And because the system is scalable, we can add another 2.5 lb. plate for every 500g added to the arm load to maintain roughly the same safety factor.

### Dynamic loading margin

The stability check above assumes the arm is stationary. In motion — accelerating a joint or stopping suddenly at a limit — inertial forces briefly add to the tipping side on top of the static gravitational torque. A rigorous treatment would sum $i \alpha r^2$ across every link mass using a measured angular acceleration, but without that hardware data, I used a conservative padding factor instead.

A **1.5× dynamic multiplier** is applied to the static tipping torque to bound worst-case deceleration (e.g. a hard limit-switch stop):

```math
\tau_{tip,dynamic} = 1.5 \times \tau_{tip,static} = 1.5 \times 14.784 = 22.176\ \text{kgf·cm}
```

Checking this against the existing restoring torque:

```math
SF_{dynamic} = \frac{\tau_{restore}}{\tau_{tip,dynamic}} = \frac{33.854}{22.176} = 1.53\times
```

The single-plate counterweight still restores the rover with margin under a padded worst-case dynamic load, though the margin compresses from 2.29× to 1.53× once motion is accounted for.

**Sizing for a target dynamic margin.** Using the general sizing relationship established above, the minimum plate count for a given dynamic safety factor is:

```math
n \geq \frac{SF_{target} \cdot 1.5 \cdot \tau_{tip,static} - \tau_{chassis}}{\tau_{cw,plate}}
```

For example, requiring $SF_{dynamic} \geq 2.0\times$:

```math
n \geq \frac{2.0(22.176) - 7.7722}{26.082} = \frac{36.580}{26.082} = 1.40 \rightarrow n = 2
```

**Limitations.** The 1.5× factor is an assumed conservative bound, not a measured value — it was not derived from servo acceleration data. A more precise figure would require measuring joint angular acceleration (α) during a commanded move or hard stop, e.g. via high-speed video, and computing $\tau_{dyn} = \sum m_i \alpha r_i^2$ across the arm's link masses. This is documented as a scoped next step rather than a completed measurement.
