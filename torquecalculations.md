<!--
=============================================================
 DESIGN ANALYSIS — EDITING LEGEND
 - [FILL IN] marks a value you supply from CAD / datasheet.
 - <!-- comments --> are guidance and do NOT render on GitHub.
 - Math uses ```math fenced blocks (display) and $...$ (inline).
   The fenced block avoids the $8...$12 dollar-sign collision.
 - PREVIEW ON github.com, not VS Code — VS Code won't render $$.
 - Keep units inside \text{...} in math, or in prose.
=============================================================
-->

# Design Analysis — Robotic Arm

Torque sizing and actuator selection for the 5-DOF arm, worked from first principles.

## Requirement

Lift **0.5 kg** at **30 cm** maximum extension.

Configuration: 2-axis shoulder (pitch + yaw), single-axis elbow, single-axis wrist, gripper.

## Method
Total Torque:
```math
\tau_{\text{tot}} = \tau_s + \tau_I
```
Static torque about a joint:
```math
\tau = m \cdot L \cdot \cos\theta
```

Torque is expressed in **kgf·cm**. Because kilogram-force already carries gravitational acceleration, using mass in kg and length in cm yields torque directly — there is no separate $g = 9.81$ term. Worst case is $\theta = 0$ (arm horizontal), where the moment arm is longest.

For a multi-joint system, a joint carries the payload **and** every link and servo outboard of it, each at its own moment arm:

```math
\tau_{\text{joint}} = \sum m \cdot L \cdot \cos\theta
```

Inertial Torque about a joint:

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

![Free-body diagram — shoulder at θ = 0](img/fbd-shoulder.png)
<!-- Drop your FBD sketch at docs/img/fbd-shoulder.png. Every symbol below should trace back to this figure. -->

---

## Shoulder Pitch Torque

This joint carries the entire arm, so it is the sizing case.
Masses are estimated as point masses, lengths as distance from center of mass to shoulder pitch axis.

| Symbol | Quantity | Value |
|---|---|---|
| $m_p$ | Payload mass | 0.5 kg |
| $L_p$ | Extension to payload | 30 cm |
| $m_{\text{se}}$ | Shoulder-elbow link | 0.05 kg |
| $L_{\text{se}}$ | Sholder-elbow link | 7.5 cm |
| $m_{\text{es}}$ | Elbow servo | 0.055 kg |
| $L_{\text{es}}$ | Elbow servo | 13 cm |
| $m_{\text{ew}}$ | Elbow-wrist link | 0.04 kg |
| $L_{\text{ew}}$ | Elbow-wrist link | [20.5] cm |
| $m_{\text{ws}}$ | Wrist servo | 0.055 kg |
| $L_{\text{ws}}$ | Wrist servo | 26 cm |
| $m_{\text{f}}$ | Fork | 0.03 kg |
| $L_{\text{f}}$ | Fork | 28 cm |

Static Torque
Evaluated at $\theta = 0$, where the arm will experience the maximum load.
**Payload contribution:**

```math
\tau_p = m_p \cdot L_p \cdot \cos 0^\circ = 0.5 \times 30 \times 1 = 15\ \text{kgf·cm}
```

**Link + servo contributions:**

```math
\tau_{\text{unloaded arm}} = ((m_{\text{se}} \cdot L_{\text{se}}) + (m_{\text{es}} \cdot L_{\text{es}}) + (m_{\text{ew}} \cdot L_{\text{ew}}) + (m_{\text{ws}} \cdot L_{\text{ws}}) + (m_{\text{f}} \cdot L_{\text{f}})) \cdot \cos 0^\circ = \text{}\ \text{kgf·cm}
```

**Total static torque at the shoulder:**

```math
\tau_{\text{shoulder}} = \tau_p + \tau_{\text{unloaded arm}} = \text{19.18}\ \text{kgf·cm}
```

Inertial Torque
Because the acceleration of the arm does not have a requirement, we will set the time to accelerate the arm $90^\circ$ or 1.57 rad to 2 seconds.
```math
\alpha = \frac{4(1.57)}{2^2} = 1.57 \text{rad/s}^2
```
We then calculate the inertia using the data table above.
```math
I = (m_{\text{se}} \cdot (L_{\text{se}})^2) + (m_{\text{es}} \cdot (L_{\text{es}})^2) + (m_{\text{ew}} \cdot (L_{\text{ew}})^2) + (m_{\text{ws}} \cdot (L_{\text{ws}})^2) + (m_{\text{f}} \cdot (L_{\text{f}})^2) = \text{}
```
Then we use the formula for inertial torque
```math
\tau_I = I \cdot \alpha = (1.57) \cdot () =
```
### Safety factor

The stack is shown as reasoning, not a single opaque multiplier:

| Factor | Value | Rationale |
|---|---|---|
| Continuous vs. stall | 2.0 | Rated stall torque is not usable continuous torque |
| Estimation error | 1.2 | Uncertainty in masses and arm lengths |
| Friction | 1.1 | Joint friction|
| **Stacked** | **≈ 2.6×** | product of the above |

**Required stall torque:**

```math
\tau_{\text{req}} = \tau_{\text{shoulder}} \times 2.6 = [\text{FILL IN}]\ \text{kgf·cm}
```

### Selection

| Spec | Value |
|---|---|
| Required stall torque | [FILL IN] kgf·cm |
| Servo selected | ST3250 |
| Rated stall torque | [FILL IN] kgf·cm |
| Margin | [FILL IN]× |

<!-- ST3250 stall torque is on the Feetech datasheet. Margin = rated / required. State whether the margin confirms the choice or flags it as tight. -->

---

## Other joints

<!--
Duplicate the block below for shoulder yaw, elbow, and wrist.
Each joint carries only the masses OUTBOARD of it, so torque drops as you move toward the gripper — which is why the ST3215 (lower torque) covers yaw/elbow/wrist while the shoulder pitch needs the ST3250.
-->

### [Joint name] — worst-case static torque

| Symbol | Quantity | Value |
|---|---|---|
| ... | ... | [FILL IN] |

```math
\tau_{\text{[joint]}} = \sum_i m_i \cdot L_i \cdot \cos 0^\circ = [\text{FILL IN}]\ \text{kgf·cm}
```

Required stall torque (× 2.6): **[FILL IN] kgf·cm** → **ST3215** selected, rated [FILL IN] kgf·cm.

---

## Counterweight — tip-over moment

The 2.5 lb plate mounted opposite the arm addresses **static stability** (rover tip-over moment about the front edge of the base). This is a **separate problem** from shoulder-servo torque — the counterweight does **not** reduce the torque the shoulder servo must supply.

<!-- Keep this distinction explicit. Conflating tip-over moment with shoulder torque reduction is the single most common misread of this system. -->

The restoring moment from the counterweight varies with arm angle as the arm's CG swings through its arc:

```math
M_{\text{tip}}(\theta) = m_{\text{arm}} \cdot d_{\text{arm}} \cdot \cos\theta
```

```math
M_{\text{restore}} = m_{\text{cw}} \cdot d_{\text{cw}}
```

Stability requires $M_{\text{restore}} \geq M_{\text{tip}}(\theta)$ across the full range. Because the arm's tipping moment scales with $\cos\theta$, it is worst at full horizontal extension and vanishes when vertical:

| $\theta$ | $\cos\theta$ | Tipping moment $M_{\text{tip}}$ |
|---|---|---|
| 0° (horizontal) | 1.00 | [FILL IN] — worst case |
| 30° | 0.87 | [FILL IN] |
| 60° | 0.50 | [FILL IN] |
| 90° (vertical) | 0.00 | 0 |

| Symbol | Quantity | Value |
|---|---|---|
| $m_{\text{cw}}$ | Counterweight mass | 2.5 lb ([FILL IN] kg) |
| $d_{\text{cw}}$ | Counterweight moment arm | [FILL IN] cm |
| $m_{\text{arm}}$ | Total arm mass | [FILL IN] kg |
| $d_{\text{arm}}$ | Arm CG moment arm at θ = 0 | [FILL IN] cm |

<!-- Optional but strong: replace or supplement the table with a τ-vs-θ plot at docs/img/counterweight-arc.png -->

---

## Validation

<!-- Link the measured result here so the analysis connects to real hardware. Keep the number on the page. -->

Demonstrated lift: **800 g** at [FILL IN] cm extension — exceeds the 0.5 kg requirement. Full test conditions and video: see [`engineering-log.md`](engineering-log.md).
