# Fiber Optics Fundamentals: Complete Guide

## Table of Contents
1. [Why Optical Communication?](#why-optical-communication)
2. [Light and Electromagnetic Spectrum](#light-and-electromagnetic-spectrum)
3. [Optical Fiber Structure](#optical-fiber-structure)
4. [Light Propagation in Fiber](#light-propagation-in-fiber)
5. [Types of Optical Fiber](#types-of-optical-fiber)
6. [Wavelength Windows](#wavelength-windows)
7. [Fiber Characteristics](#fiber-characteristics)
8. [Loss and Attenuation](#loss-and-attenuation)
9. [Dispersion](#dispersion)
10. [Nonlinear Effects](#nonlinear-effects)

---

## Why Optical Communication?

### The Bandwidth Problem

```
Electrical Signals (Copper):
├── Frequency limit: ~10 GHz
├── Bandwidth: Limited by skin effect, dielectric loss
├── Distance: 100 meters typical (Cat6a at 10 Gbps)
└── Interference: EMI, crosstalk

Optical Signals (Fiber):
├── Frequency: ~200 THz (infrared light)
├── Bandwidth: 50+ THz available spectrum
├── Distance: 10-100 km without amplification
└── Immunity: No EMI, no crosstalk

Difference: 10,000× more bandwidth!
```

### Historical Context

```
1970: Corning develops low-loss fiber (<20 dB/km)
1980: Single-mode fiber deployed
1990: EDFAs (Erbium-Doped Fiber Amplifiers) enable long haul
2000: DWDM (Dense WDM) - 100+ wavelengths per fiber
2010: Coherent 100G optics
2020: 400G/800G coherent optics
2024: 1.6 Tbps optics emerging

Growth: From Mbps to Tbps in 50 years
```

---

## Light and Electromagnetic Spectrum

### Electromagnetic Spectrum

```
Radio Waves: 3 Hz - 300 GHz
    └── WiFi, cellular, broadcast

Microwave: 300 MHz - 300 GHz
    └── Satellite, radar

Infrared (IR): 300 GHz - 430 THz
    ├── Far IR: 300 GHz - 20 THz (thermal)
    ├── Mid IR: 20 THz - 100 THz
    └── Near IR: 100 THz - 430 THz ← OPTICAL COMMS HERE!

Visible Light: 430 THz - 750 THz
    └── 400-700 nm wavelength
    └── Red (700nm) to Violet (400nm)

Ultraviolet: 750 THz - 30 PHz
    └── UV-A, UV-B, UV-C

X-rays, Gamma rays: Higher frequencies
```

### Why Infrared for Communications?

```
Requirements:
├── Low loss in glass fiber
├── Can be generated efficiently (lasers, LEDs)
├── Can be detected efficiently (photodiodes)
└── Invisible (eye safety at low power)

Near-Infrared Windows:
├── 850 nm (0.85 μm): Multimode, short reach
├── 1310 nm (1.31 μm): Single-mode, metro
├── 1550 nm (1.55 μm): Single-mode, long haul
└── C-band (1530-1565 nm): DWDM systems

Physics:
Frequency (f) × Wavelength (λ) = Speed of Light (c)
c = 3 × 10⁸ m/s in vacuum
c = 2 × 10⁸ m/s in fiber (refractive index ~1.5)

Example:
1550 nm → f = c/λ = (3×10⁸)/(1550×10⁻⁹) = 193.5 THz
```

---

## Optical Fiber Structure

### Basic Fiber Anatomy

```
Cross-Section (Single-Mode Fiber):

        Outer Jacket (Plastic)
              ↓
    ┌─────────────────────┐
    │   Coating (Acrylate) │ 250 μm diameter
    │  ┌─────────────────┐ │
    │  │   Cladding      │ │ 125 μm diameter
    │  │  (Pure Silica)  │ │ Refractive index: n₂ = 1.444
    │  │  ┌───────────┐  │ │
    │  │  │   Core    │  │ │ 8-10 μm diameter
    │  │  │ (Doped    │  │ │ Refractive index: n₁ = 1.447
    │  │  │  Silica)  │  │ │
    │  │  └───────────┘  │ │
    │  └─────────────────┘ │
    └─────────────────────┘

Key: Core has slightly higher refractive index than cladding
Difference: ~0.3% (Δn ≈ 0.003)
```

### Materials

**Silica (SiO₂) - Silicon Dioxide**:

```
Core Doping:
├── GeO₂ (Germanium oxide): Increases refractive index
├── P₂O₅ (Phosphorus pentoxide): Increases index
└── Typical: 3-8% GeO₂

Cladding:
├── Pure silica (undoped)
└── Or fluorine-doped (lower index)

Properties:
├── Extremely pure (99.9999999%)
├── 1 ppb (part per billion) impurities
├── Transparent in IR (800-1700 nm)
└── Low loss (~0.2 dB/km at 1550 nm)

Manufacturing:
└── MCVD (Modified Chemical Vapor Deposition)
└── OVD (Outside Vapor Deposition)
└── VAD (Vapor Axial Deposition)
```

**Plastic Optical Fiber (POF)**:

```
Core: PMMA (Polymethyl methacrylate)
Cladding: Fluorinated polymer

Properties:
├── Large core (1 mm)
├── Flexible
├── Easy to handle
└── High loss (~150 dB/km)

Use:
├── Very short distances (<50m)
├── Home networking (obsolete)
├── Automotive
└── Consumer devices
```

---

## Light Propagation in Fiber

### Total Internal Reflection

**The Core Principle**:

```
Light travels from denser (core) to less dense (cladding):

             Cladding (n₂ = 1.444)
    ─────────────────────────────────
                    ↗ Refracted (lost)
    Light ────→  /  ← Critical angle
                ↙
    ──────────────────────────────── Reflected
             Core (n₁ = 1.447)

Snell's Law:
n₁ sin(θ₁) = n₂ sin(θ₂)

Critical Angle (θc):
sin(θc) = n₂/n₁ = 1.444/1.447 ≈ 0.998
θc ≈ 86.5°

When angle > θc:
└── Total Internal Reflection (100% reflected)
└── Light trapped in core

When angle < θc:
└── Light refracts into cladding
└── Lost (attenuation)
```

### Numerical Aperture (NA)

**Acceptance Cone**:

```
Light entering fiber:

    Air (n₀ = 1.0)
         │
    ─────┴───── Fiber end face
         │ ↘ θₐ
         │   ↘
    ─────┼────↘── Core
         │     ↘

Only light within acceptance angle θₐ is guided

Numerical Aperture:
NA = sin(θₐ) = √(n₁² - n₂²)

For typical SMF:
NA = √(1.447² - 1.444²) ≈ 0.14
θₐ = arcsin(0.14) ≈ 8°

Meaning:
├── Light must enter within ±8° to be guided
├── Easier to couple into multimode (larger NA)
└── Harder to couple into single-mode (small NA)
```

---

## Types of Optical Fiber

### Multimode Fiber (MMF)

**Structure**:

```
Core Diameter: 50 μm or 62.5 μm (large)
Cladding: 125 μm
NA: 0.2 (larger acceptance cone)

Light Propagation:
    Multiple paths (modes) simultaneously

    ─────────────────────────────────
    ↗     ↘     ↗     ↘     ↗
       ↘     ↗     ↘     ↗
    ────────────────────────────────
    
    Different modes = different path lengths
```

**Types of MMF**:

**Step-Index MMF**:
```
Refractive Index Profile:

n₁ │     ┌─────┐
   │     │     │
n₂ │─────┘     └─────
   └─────────────────
     Cladding Core Cladding

Sharp step in refractive index
High dispersion (different modes different speeds)
Rarely used today
```

**Graded-Index MMF**:
```
Refractive Index Profile:

n₁ │      ╱─╲
   │    ╱     ╲
n₂ │───╱       ╲───
   └─────────────────
     Cladding Core Cladding

Parabolic index profile
Core center: highest index
Core edge: lower index

Benefit:
├── Light near center travels faster (lower index)
├── Light at edge travels shorter path
└── Modes arrive at similar times (lower dispersion)
```

**MMF Bandwidth**:

```
OM1 (62.5/125 μm):
├── Bandwidth: 200 MHz·km @ 850 nm
├── Distance: 300m @ 1 Gbps
└── Legacy

OM2 (50/125 μm):
├── Bandwidth: 500 MHz·km @ 850 nm
├── Distance: 550m @ 1 Gbps
└── Legacy

OM3 (50/125 μm, laser-optimized):
├── Bandwidth: 2000 MHz·km @ 850 nm
├── Distance: 300m @ 10 Gbps, 100m @ 40 Gbps
└── Common

OM4 (50/125 μm, enhanced):
├── Bandwidth: 4700 MHz·km @ 850 nm
├── Distance: 400m @ 10 Gbps, 150m @ 40 Gbps
└── Current standard

OM5 (50/125 μm, wideband):
├── Optimized for 850nm AND 950nm (SWDM)
├── Distance: 150m @ 40 Gbps, 100m @ 100 Gbps
└── Newer, enables short-wave WDM
```

### Single-Mode Fiber (SMF)

**Structure**:

```
Core Diameter: 8-10 μm (very small)
Cladding: 125 μm
NA: 0.14 (small acceptance cone)

Light Propagation:
    Only one mode (fundamental mode)

    ─────────────────────────────────
         ─────────────→
    ─────────────────────────────────
    
    Single path = no modal dispersion
```

**Why "Single Mode"?**

```
Mode Condition:
V = (2πa/λ) × NA

Where:
a = core radius
λ = wavelength
NA = numerical aperture

Single mode when: V < 2.405

For 1310nm light:
Core radius a ≈ 4.5 μm
V ≈ 2.2 < 2.405 ✓ Single mode

For 850nm light (same fiber):
V ≈ 3.4 > 2.405 ✗ Multimode

Conclusion: SMF is single-mode at infrared wavelengths
```

**Types of SMF**:

**OS1 (Indoor)**:
```
Construction: Tight-buffered
Max attenuation: 1.0 dB/km @ 1310nm, 1.0 dB/km @ 1550nm
Use: Indoor, short distances
```

**OS2 (Outdoor)**:
```
Construction: Loose-tube
Max attenuation: 0.4 dB/km @ 1310nm, 0.3 dB/km @ 1550nm
Use: Outdoor, long distances
Quality: Better (lower loss)
```

**Specialized SMF**:

```
G.652 (Standard SMF):
└── Most common, optimized for 1310nm
└── Works at 1550nm too

G.653 (Dispersion-Shifted):
└── Zero dispersion at 1550nm
└── Problems with WDM (FWM)
└── Rarely used

G.654 (Cut-off Shifted):
└── Lower loss at 1550nm
└── Submarine cables

G.655 (Non-Zero Dispersion-Shifted):
└── Low but non-zero dispersion at 1550nm
└── Good for DWDM
└── Metro networks

G.656 (Wideband):
└── Low dispersion across 1460-1625nm
└── CWDM, DWDM

G.657 (Bend-Insensitive):
└── Can bend to small radius (7.5mm)
└── FTTH, data centers
└── Reduced macro-bend loss
```

---

## Wavelength Windows

### Transmission Windows

```
Wavelength Bands in Fiber:

O-Band (Original): 1260-1360 nm
├── Zero dispersion around 1310nm
├── Used for short/metro distances
└── Lower cost optics

E-Band (Extended): 1360-1460 nm
├── Historically high water peak
├── Modern fibers (G.652.D) low loss here
└── CWDM applications

S-Band (Short): 1460-1530 nm
├── Rarely used
└── Some CWDM

C-Band (Conventional): 1530-1565 nm
├── Lowest loss (~0.2 dB/km)
├── EDFA amplification
├── DWDM primary band
└── Long-haul systems

L-Band (Long): 1565-1625 nm
├── Secondary DWDM band
├── EDFA amplification possible
└── Extended capacity

U-Band (Ultra-long): 1625-1675 nm
├── Monitoring, Raman amplification
└── Rarely used for data
```

### Attenuation Spectrum

```
Fiber Loss vs Wavelength:

Loss
(dB/km)
  4 │
    │
  2 │     Water Peak (OH⁻)
    │        ↓
  1 │   ╱╲  │
    │  ╱  ╲ │╲
0.5 │ ╱    ╲│ ╲___
    │╱      ╲   ╲___1310nm
0.2 │        ╲      ╲___1550nm (minimum)
    └─────────────────────────────→
    800  1000  1310  1550  1700  Wavelength (nm)

Water Peak:
├── OH⁻ (hydroxyl) ions in glass
├── Absorption around 1380-1410nm
├── Modern "low water peak" fiber (G.652.D)
└── Reduced to <0.4 dB/km across E-band

Why 1550nm for long haul:
└── Lowest loss point (~0.16-0.20 dB/km)
```

---

## Fiber Characteristics

### Attenuation (Loss)

**Definition**: Power loss as light travels through fiber

```
Attenuation (dB) = 10 × log₁₀(Pout / Pin)

Or per distance:
α (dB/km) = (10/L) × log₁₀(Pout / Pin)

Example:
Input power: 1 mW (0 dBm)
After 20 km: 0.1 mW (-10 dBm)

α = (10/20) × log₁₀(0.1/1) = (10/20) × (-10) = -0.5 dB/km
Wait, should be positive! Use: α = -(10/20) × (-10) = 0.5 dB/km

Actually: Loss = Pin - Pout (in dB) = 0 - (-10) = 10 dB
α = 10 dB / 20 km = 0.5 dB/km ✓
```

**Causes of Attenuation**:

```
1. Absorption:
   ├── Intrinsic: Electronic transitions in silica
   ├── Extrinsic: Impurities (OH⁻, metal ions)
   └── Contribution: ~0.03 dB/km @ 1550nm

2. Rayleigh Scattering:
   ├── Light scattered by density fluctuations
   ├── Proportional to 1/λ⁴
   ├── Higher at shorter wavelengths
   └── Contribution: ~0.12-0.16 dB/km @ 1550nm

3. Waveguide imperfections:
   ├── Core-cladding interface roughness
   ├── Diameter variations
   └── Contribution: ~0.01 dB/km

4. Bending losses:
   ├── Macro-bends: Large radius curves
   ├── Micro-bends: Small local deformations
   └── Installation-dependent

Total @ 1550nm: ~0.16-0.22 dB/km (typical)
```

### Bandwidth and Dispersion

**Modal Dispersion** (Multimode only):

```
Different modes = different path lengths
→ Different arrival times
→ Pulse spreading

Example:
├── Fast mode: 5.0 μs for 1 km
├── Slow mode: 5.1 μs for 1 km
└── Spread: 100 ns/km

Limits bandwidth:
└── Pulse width must be > 100ns to avoid overlap
└── Max rate ≈ 1/(100ns) = 10 MHz per km
└── Or 10 MHz·km bandwidth

Mitigation: Graded-index fiber reduces modal dispersion
```

**Chromatic Dispersion** (All fiber):

```
Different wavelengths travel at different speeds

Material Dispersion:
└── Refractive index varies with wavelength
└── n(λ) → different velocities

Waveguide Dispersion:
└── Fraction of power in core vs cladding varies
└── Changes effective index

Total Chromatic Dispersion:
D (ps/nm·km) - dispersion parameter

Typical values:
├── 1310nm (zero dispersion): D ≈ 0 ps/nm·km
├── 1550nm (G.652): D ≈ 17 ps/nm·km
└── Sign indicates slope

Impact:
Pulse spreading (ps) = D × L(km) × Δλ(nm)

Example:
├── D = 17 ps/nm·km
├── L = 80 km
├── Δλ = 0.1 nm (laser linewidth)
└── Spread = 17 × 80 × 0.1 = 136 ps

For 10 Gbps (100ps bit period):
└── Spread comparable to bit period → problem!
```

**Polarization Mode Dispersion (PMD)**:

```
Fiber is not perfectly circular:
├── Slight asymmetry (stress, bends)
├── Two polarization modes
└── Different propagation speeds

PMD (ps) = DPMD × √L

Where DPMD ≈ 0.1-1 ps/√km (fiber quality)

Example:
├── DPMD = 0.5 ps/√km
├── L = 100 km
└── PMD = 0.5 × 10 = 5 ps

Impact:
├── Random (varies with temperature, stress)
├── Major issue for 40G+ long-haul
└── Requires compensation or DSP
```

---

## Loss and Attenuation

### Loss Budget Calculation

```
Total Link Loss = Fiber Loss + Connector Loss + Splice Loss + Margin

Example 10km link:

Fiber Loss:
├── 10 km × 0.3 dB/km = 3.0 dB

Connector Loss:
├── 2 connectors × 0.5 dB = 1.0 dB

Splice Loss:
├── 4 splices × 0.1 dB = 0.4 dB

System Margin:
└── 3.0 dB (safety factor)

Total Loss Budget: 7.4 dB

Transmitter: +0 dBm (1 mW)
Receiver sensitivity: -20 dBm
Available budget: 20 dB
Used budget: 7.4 dB
Margin: 12.6 dB ✓ (plenty!)
```

### Connector and Splice Loss

**Connector Types**:

```
SC (Subscriber Connector):
├── Push-pull
├── Loss: 0.3-0.5 dB
└── Common

LC (Lucent Connector):
├── Small form factor
├── Loss: 0.3 dB
└── Most common today

MPO/MTP (Multi-fiber Push-On):
├── 12 or 24 fibers
├── Loss: 0.5-1.0 dB per fiber
└── High-density

FC (Ferrule Connector):
├── Screw-on
├── Loss: 0.3 dB
└── Precision applications

Loss Causes:
├── Core misalignment
├── Air gap
├── End-face angle mismatch
├── Dirt/contamination
└── Fresnel reflection
```

**Splicing**:

```
Fusion Splice:
├── Fibers melted together
├── Loss: 0.02-0.1 dB
├── Permanent
└── Lowest loss

Mechanical Splice:
├── Fibers held by fixture + gel
├── Loss: 0.1-0.3 dB
├── Semi-permanent
└── Easier, lower cost

Quality depends on:
├── Cleave quality
├── Alignment precision
├── Arc power/duration (fusion)
└── Fiber matching
```

---

## Dispersion

### Dispersion Compensation

**Problem**: Pulse spreading limits distance and bit rate

**Compensation Techniques**:

**1. Dispersion Compensating Fiber (DCF)**:

```
Standard Fiber: +17 ps/nm·km @ 1550nm
DCF: -100 ps/nm·km @ 1550nm

For 80km standard fiber:
Dispersion = 80 × 17 = 1360 ps/nm

Compensation needed:
DCF length = 1360 / 100 = 13.6 km

Drawback:
├── DCF has higher loss (~0.5 dB/km)
├── 13.6 km × 0.5 = 6.8 dB additional loss
└── Requires amplification
```

**2. Dispersion Shifted Fiber**:

```
G.653: Zero dispersion at 1550nm
└── Problem: Four-wave mixing in WDM
└── Rarely used

G.655: Non-zero dispersion-shifted
├── D ≈ 2-6 ps/nm·km @ 1550nm
├── Low enough for long distances
├── Non-zero prevents FWM
└── Good for DWDM metro
```

**3. Digital Signal Processing (DSP)**:

```
Modern approach (coherent optics):
├── Measure dispersion in receiver
├── Compensate digitally (equalization)
├── No physical DCF needed
├── Adaptive
└── Standard in 100G+ systems

Benefit:
├── No loss penalty
├── Handles chromatic + PMD
└── Flexible
```

---

## Nonlinear Effects

**At high optical powers, fiber becomes nonlinear**

### Kerr Effects (Intensity-Dependent Refractive Index)

**Self-Phase Modulation (SPM)**:

```
Single channel:
├── Signal intensity varies over pulse
├── Changes refractive index
├── Different parts travel at different speeds
└── Chirps the pulse (frequency shift)

Result: Pulse distortion, interacts with dispersion
```

**Cross-Phase Modulation (XPM)**:

```
Multiple channels (WDM):
├── Channel A intensity affects Channel B
├── Cross-talk between channels
└── Phase/frequency shifts

Mitigation: Proper spacing, dispersion management
```

**Four-Wave Mixing (FWM)**:

```
Three wavelengths create fourth:

λ₁, λ₂, λ₃ → λ₄ = λ₁ + λ₂ - λ₃

Example DWDM:
Channel 1: 193.1 THz
Channel 2: 193.2 THz
Channel 3: 193.3 THz

FWM product: 193.1 + 193.2 - 193.3 = 193.0 THz
└── May interfere with another channel!

Worse when:
├── High power
├── Low dispersion (channels walk together)
├── Equal channel spacing
└── Long fiber spans

Mitigation:
├── Unequal channel spacing
├── Non-zero dispersion
└── Lower launch power
```

### Scattering Effects

**Stimulated Raman Scattering (SRS)**:

```
High-frequency photon → Lower-frequency photon + phonon

In WDM:
├── Shorter wavelengths transfer power to longer
├── Blue channels lose power
├── Red channels gain power
└── Tilt in spectrum

Use: Raman amplification (intentional SRS)
```

**Stimulated Brillouin Scattering (SBS)**:

```
Creates backward-traveling wave
├── Power threshold: ~1-10 mW (single channel)
├── Limits launch power
└── Narrow linewidth sources most affected

Mitigation:
├── Phase/frequency modulation (broaden linewidth)
├── Keep below threshold
└── Use in distributed sensing (OTDR)
```

---

## Summary

**Fiber optics fundamentals** enable modern high-speed networking:

**Key Principles**:
- Light guided by total internal reflection
- Core has higher refractive index than cladding
- Single-mode for long distance, multimode for short

**Fiber Types**:
- **Multimode** (50/62.5 μm core): 300m @ 10G, easy coupling
- **Single-mode** (9 μm core): 10-80 km @ 10G+, lowest loss

**Wavelengths**:
- **850nm**: Multimode, short reach, cheap
- **1310nm**: Single-mode, metro, zero dispersion
- **1550nm**: Single-mode, long haul, lowest loss, DWDM

**Loss**:
- Typical: 0.2-0.3 dB/km @ 1550nm
- Rayleigh scattering (fundamental limit)
- Connectors: 0.3-0.5 dB, Splices: 0.02-0.1 dB

**Dispersion**:
- Modal dispersion (multimode only)
- Chromatic dispersion (17 ps/nm·km @ 1550nm)
- PMD (random, temperature-dependent)

**Nonlinear effects**:
- SPM, XPM, FWM (Kerr effects)
- SRS, SBS (scattering)
- Limit power in DWDM systems

Understanding these fundamentals is essential for designing optical networks, selecting appropriate fiber types, and troubleshooting transmission issues.

---

# Optical Transceivers: Complete Guide

## Table of Contents
1. [What is an Optical Transceiver?](#what-is-an-optical-transceiver)
2. [Transceiver Form Factors](#transceiver-form-factors)
3. [Internal Architecture](#internal-architecture)
4. [Laser Types and Modulation](#laser-types-and-modulation)
5. [Photodetectors](#photodetectors)
6. [Transceiver Specifications](#transceiver-specifications)
7. [Digital Diagnostics (DDM/DOM)](#digital-diagnostics-ddmdom)
8. [Pluggable Standards Evolution](#pluggable-standards-evolution)
9. [Selection Guide](#selection-guide)
10. [Troubleshooting](#troubleshooting)

---

## What is an Optical Transceiver?

### Purpose

```
Transceiver = Transmitter + Receiver

Function:
├── Electrical → Optical (TX)
└── Optical → Electrical (RX)

In Network Equipment:
┌─────────────────────────────┐
│  Switch/Router ASIC         │
│  (Electrical signals)       │
└────────────┬────────────────┘
             │
             │ Electrical (SerDes)
             ↓
┌────────────────────────────┐
│     Optical Transceiver    │
│  ┌──────────┐ ┌──────────┐ │
│  │   Laser  │ │ Photodet │ │
│  │   (TX)   │ │   (RX)   │ │
│  └────┬─────┘ └─────┬────┘ │
└───────┼─────────────┼───────┘
        │             │
     Fiber (Optical signals)
```

### Hot-Pluggable

```
Key Feature: Can be inserted/removed without powering down

Benefits:
├── Easy upgrades (1G → 10G → 25G)
├── Quick replacement (failed module)
├── Flexible deployment (copper vs fiber)
└── Different reach (SR, LR, ER)

Standards:
└── MSA (Multi-Source Agreement)
└── Ensures interoperability
```

---

## Transceiver Form Factors

### GBIC (Gigabit Interface Converter)

```
Year: 1995-2005
Speed: 1 Gbps
Size: Large (bulky)
Interface: SC connector

Status: Obsolete
Replaced by: SFP (mini-GBIC)
```

### SFP (Small Form-Factor Pluggable)

```
Year: 2001-present
Speed: Up to 4.25 Gbps
Size: 56.5mm × 13.4mm × 8.5mm
Connector: LC duplex

Variants:
├── 100BASE-FX (100 Mbps)
├── 1000BASE-SX/LX (1 Gbps)
└── 4G Fibre Channel

Applications:
├── Gigabit Ethernet
├── SONET/SDH
└── Still very common

Port Density:
└── 48 ports per 1U switch

┌────┐ ┌────┐ ┌────┐
│SFP │ │SFP │ │SFP │  ← Individual modules
└┬──┬┘ └┬──┬┘ └┬──┬┘
 └──┘   └──┘   └──┘
  LC     LC     LC
```

### SFP+ (Enhanced SFP)

```
Year: 2006-present
Speed: Up to 16 Gbps
Size: Same as SFP
Connector: LC duplex

Speeds:
├── 10GBASE-SR/LR/ER (10 Gbps Ethernet)
├── 8G/16G FC (Fibre Channel)
└── 10G SONET/SDH

Backward Compatible:
└── SFP+ port can accept SFP (at lower speed)

Power: ~1-1.5W per module

Key Improvement over SFP:
└── Higher speed (10G vs 1G)
└── Same physical size
```

### SFP28

```
Year: 2014-present
Speed: 25 Gbps (25.78125 Gbps with FEC)
Size: Same as SFP/SFP+
Connector: LC duplex

Use:
├── 25G Ethernet (server NICs)
├── 100G (4×25G breakout)
└── Common in modern data centers

Variants:
├── 25GBASE-SR (100m on OM4)
├── 25GBASE-LR (10km SMF)
└── 25GBASE-ER (30-40km SMF)

Power: ~1.5-3.5W
```

### SFP56

```
Year: 2019-present
Speed: 50 Gbps (PAM4 modulation)
Size: Same as SFP/SFP+/SFP28
Connector: LC duplex

Use:
├── 50G Ethernet
├── 200G (4×50G breakout)
└── Emerging standard

Modulation: PAM4 (4-level)
└── 2 bits per symbol @ 25 GBaud
```

### QSFP (Quad SFP)

```
Year: 2006
Speed: 4 Gbps (4×1G)
Size: Larger than SFP
Connector: MPO-12 or LC quad

Use: 4G aggregation
Status: Largely obsolete
```

### QSFP+ (Quad SFP+)

```
Year: 2010-present
Speed: 40 Gbps (4×10G lanes)
Size: 72mm × 18.4mm × 8.5mm
Connector: MPO-12 (12-fiber) or LC quad

Applications:
├── 40G Ethernet (40GBASE-SR4/LR4)
├── 4×10G breakout
└── InfiniBand QDR/FDR

Variants:
├── QSFP+ SR4: 100m on OM3/OM4 (MPO)
├── QSFP+ LR4: 10km SMF, 4×10G CWDM
├── QSFP+ BiDi: 40G on duplex SMF
└── QSFP+ DAC: Direct Attach Copper (1-5m)

Power: ~3.5-5W
```

### QSFP28

```
Year: 2014-present
Speed: 100 Gbps (4×25G lanes)
Size: Same as QSFP+
Connector: MPO-12 (12-fiber) or MPO-24 (24-fiber)

Most Common 100G:
├── 100GBASE-SR4: 100m on OM4 (MPO-12)
├── 100GBASE-LR4: 10km SMF, 4×25G CWDM (duplex)
├── 100GBASE-CWDM4: 2km SMF (duplex LC)
├── 100GBASE-PSM4: 500m SMF (MPO-12)
└── 100GBASE-ER4: 40km SMF (duplex LC)

Breakout:
└── 4×25G (QSFP28 → 4× SFP28)

Power: ~3.5-6W

Backward Compatible:
└── QSFP28 port can accept QSFP+ (40G)
```

### QSFP56

```
Year: 2018-present
Speed: 200 Gbps (4×50G PAM4)
Size: Same as QSFP+/QSFP28
Connector: MPO-12 or duplex LC

Variants:
├── 200GBASE-SR4: 100m on OM4 (MPO)
├── 200GBASE-FR4/LR4: 2km/10km SMF
└── 200GBASE-DR4: 500m SMF

Breakout:
└── 4×50G (QSFP56 → 4× SFP56)

Power: ~8-10W
```

### QSFP-DD (Double Density)

```
Year: 2017-present
Speed: 400 Gbps (8×50G PAM4)
Size: 18.4mm × 89.4mm × 8.5mm (taller than QSFP)
Lanes: 8 electrical lanes
Connector: MPO-16 or MPO-24

Key Feature: Backward compatible
├── QSFP-DD port accepts QSFP28 (in lower portion)
└── Uses 8 lanes vs 4 in QSFP28

Variants:
├── 400GBASE-SR8: 100m on OM4 (2×MPO-12)
├── 400GBASE-DR4: 500m SMF (MPO-12)
├── 400GBASE-FR4/LR4: 2km/10km SMF (duplex LC)
└── 400GBASE-ER8: 40km SMF (2×duplex LC)

Breakout:
├── 2×200G (QSFP-DD → 2× QSFP56)
└── 8×50G (QSFP-DD → 8× SFP56)

Power: ~10-15W
```

### OSFP (Octal SFP)

```
Year: 2016-present
Speed: 400 Gbps / 800 Gbps
Size: 22.58mm × 107.8mm × 9.27mm (larger than QSFP-DD)
Lanes: 8 electrical lanes
Power: Up to 15W (better cooling)

Advantages over QSFP-DD:
├── More space for heat dissipation
├── Higher power budget
└── Better for 800G coherent optics

Not backward compatible with QSFP

Status: Competing with QSFP-DD for 400G/800G
```

### QSFP112

```
Year: 2023-present
Speed: 800 Gbps (8×100G PAM4)
Size: Same as QSFP-DD
Lanes: 8 lanes @ 100 Gbps each

Modulation: 100G PAM4 (2 bits/symbol @ 50 GBaud)

Breakout:
└── 8×100G (QSFP112 → 8× future 100G modules)

Status: Emerging, standardization ongoing
```

### CFP/CFP2/CFP4 (100G+)

```
CFP (C Form-factor Pluggable):
├── Speed: 100G
├── Size: Very large (144mm × 82mm)
├── Use: Long-haul coherent 100G
└── Status: Obsolete (replaced by QSFP28/CFP2)

CFP2:
├── Half the size of CFP
├── 100G/200G coherent
└── Still used in some systems

CFP4:
├── Quarter size of CFP
├── Similar to QSFP form factor
└── Superseded by QSFP28

Trend: Moving to QSFP-DD for coherent optics
```

---

## Internal Architecture

### Basic Transceiver Block Diagram

```
┌─────────────────────────────────────────┐
│          Optical Transceiver            │
│                                         │
│  Transmit Path (TX):                    │
│  ┌──────────┐   ┌──────────┐   ┌─────┐ │
│  │ TX Data  │→  │ Laser    │→  │Fiber│ │
│  │ (Elec.)  │   │ Driver   │   │Optic│ │
│  └──────────┘   └────┬─────┘   └─────┘ │
│                      │                  │
│                 ┌────┴─────┐            │
│                 │  Laser   │            │
│                 │  Diode   │            │
│                 │(VCSEL/DFB)│           │
│                 └──────────┘            │
│                                         │
│  Receive Path (RX):                     │
│  ┌──────────┐  ┌───────────┐  ┌──────┐ │
│  │ RX Data  │← │ TIA +     │← │Fiber │ │
│  │ (Elec.)  │  │ Limiting  │  │Optic │ │
│  └──────────┘  │ Amp       │  └──────┘ │
│                └─────▲─────┘            │
│                      │                  │
│                ┌─────┴──────┐           │
│                │Photodiode  │           │
│                │  (PIN/APD) │           │
│                └────────────┘           │
│                                         │
│  Control & Monitoring:                  │
│  ┌──────────────────────────────────┐  │
│  │ Microcontroller                  │  │
│  │ - Temperature monitoring         │  │
│  │ - Bias current control           │  │
│  │ - Power monitoring               │  │
│  │ - Digital diagnostics (I²C)      │  │
│  └──────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

### Transmitter Components

**Laser Driver**:

```
Function: Convert electrical data to laser modulation

Types:
├── Direct Modulation: Vary laser current
│   └── Simple, cheap, ~10-25 Gbps max
│
└── External Modulation: Laser always on, modulator varies light
    └── Complex, expensive, 40 Gbps+

Driver Specifications:
├── Rise/fall time: <20 ps (for 25G)
├── Output swing: 0.4-1.2V
├── Differential output
└── Pre-emphasis (compensate for frequency response)
```

**Laser Diode Types**:

```
VCSEL (Vertical Cavity Surface Emitting Laser):
├── Emits perpendicular to chip surface
├── Wavelength: 850nm (multimode)
├── Power: 0-5 dBm
├── Cost: Low ($5-20)
├── Speed: Up to 25 Gbps/lane
├── Temperature: Uncooled
└── Use: Short reach (SR), data centers

FP (Fabry-Perot):
├── Multi-longitudinal mode
├── Wavelength: 1310nm (SMF)
├── Cost: Low
├── Speed: Up to 10 Gbps
├── Temperature: Uncooled
└── Use: Short/medium reach, legacy

DFB (Distributed Feedback):
├── Single longitudinal mode (narrow linewidth)
├── Wavelength: 1310nm or 1550nm
├── Power: 0-10 dBm
├── Cost: Medium ($50-200)
├── Speed: Up to 25 Gbps
├── Temperature: Cooled or uncooled
└── Use: Long reach (LR), DWDM

EML (Electro-absorption Modulated Laser):
├── Integrated DFB + modulator
├── Wavelength: 1310nm or 1550nm
├── Speed: 25-100 Gbps
├── Cost: Medium-high
└── Use: 100G LR4, metro

Coherent (IQ Modulator + DFB/ECL):
├── Complex modulation (QPSK, 16-QAM)
├── Wavelength: 1550nm C-band
├── Speed: 100-800 Gbps
├── Cost: Very high ($1000-5000+)
└── Use: Long-haul DWDM
```

### Receiver Components

**Photodiode Types**:

```
PIN (Positive-Intrinsic-Negative):
├── Simple structure
├── Sensitivity: -15 to -18 dBm
├── Speed: Up to 25 Gbps
├── Cost: Low
├── No gain
└── Use: Most transceivers

APD (Avalanche Photodiode):
├── Internal gain (avalanche effect)
├── Sensitivity: -25 to -30 dBm (10 dB better!)
├── Speed: Up to 10-25 Gbps
├── Cost: Higher
├── Requires high voltage (30-90V)
└── Use: Long reach, high sensitivity

Coherent Receiver:
├── 90° hybrid + balanced detectors
├── Sensitivity: -35 dBm or better
├── Speed: 100-800 Gbps
├── Cost: Very high
└── Use: Long-haul coherent
```

**TIA (Trans-Impedance Amplifier)**:

```
Function: Convert photodiode current to voltage

Characteristics:
├── High gain (1-10 kΩ)
├── Wide bandwidth (10-30 GHz)
├── Low noise
└── Converts μA to mV/V

Integrated with:
└── Limiting amplifier (LA)
└── Clock/Data Recovery (CDR)
```

---

## Laser Types and Modulation

### Direct Modulation

```
Principle: Vary laser drive current

    Current
      ↑
   I_th│     ┌─On─┐
      │     │     │
      │─────┘     └───Off
      └─────────────→ Time

Above threshold (I_th): Lasing
Below threshold: No lasing

Advantages:
├── Simple
├── Low cost
└── Small size

Disadvantages:
├── Chirp (frequency modulation during intensity change)
├── Limited bandwidth
└── Speed limit: ~25 Gbps

Use: Short reach (SR) transceivers
```

### External Modulation

```
Principle: Continuous laser + external modulator

┌────────┐    ┌──────────────┐    ┌──────┐
│ Laser  │ ─→ │  Modulator   │ ─→ │ Fiber│
│ (CW)   │    │(Mach-Zehnder)│    │      │
└────────┘    └──────▲───────┘    └──────┘
                     │
                  Data signal

Mach-Zehnder Modulator (MZM):
├── Splits light into two arms
├── Apply voltage to one arm (phase shift)
├── Recombine (constructive/destructive interference)
└── On/off keying (OOK)

Advantages:
├── Low chirp
├── High speed (100+ Gbps)
├── Better for long distance
└── Enables advanced modulation

Disadvantages:
├── Complex
├── Expensive
└── Higher power

Use: Long reach (LR), coherent optics
```

### Modulation Formats

**OOK (On-Off Keying)**:

```
Simplest: Light on = 1, light off = 0

    Power
      ↑
      │ ┌─┐   ┌─┐ ┌─┐
      │ │ │   │ │ │ │
      └─┘ └───┘ └─┘ └─→ Time
        1  0  1  0  1

Use: Up to 25 Gbps/lane
```

**PAM4 (4-level Pulse Amplitude Modulation)**:

```
Four levels: 00, 01, 10, 11 (2 bits/symbol)

    Power
      ↑
   L3 │     ┌─┐         (11)
   L2 │   ┌─┘ └─┐       (10)
   L1 │ ┌─┘     └─┐     (01)
   L0 │─┘         └─    (00)
      └─────────────→ Time
        01 11 10 01

Advantages:
├── 2× bits at same baud rate
├── 50G at 25 GBaud
└── 100G at 50 GBaud

Disadvantages:
├── Less SNR margin (smaller eye)
├── Requires DSP
└── More complex

Use: 50G/100G/200G/400G (QSFP56/QSFP-DD)
```

**Coherent Modulation (QPSK, 16-QAM, etc.)**:

```
Modulate amplitude + phase

QPSK (Quadrature Phase Shift Keying):
├── 4 phase states (0°, 90°, 180°, 270°)
├── 2 bits/symbol
└── Used in 100G coherent

16-QAM (Quadrature Amplitude Modulation):
├── 16 states (4 amplitudes × 4 phases)
├── 4 bits/symbol
└── Used in 200G/400G coherent

Requires:
├── IQ modulator
├── Coherent receiver
├── DSP
└── Very complex

Use: Long-haul 100G+
```

---

## Photodetectors

### PIN Photodiode

**Structure**:

```
Layers:
┌─────────────┐ P+ (heavily doped)
│   P-type    │
├─────────────┤
│ Intrinsic   │ ← Depletion region (wide)
│  (undoped)  │
├─────────────┤
│   N-type    │
└─────────────┘ N+ (heavily doped)

Light absorbed in intrinsic region
→ Electron-hole pairs created
→ Drift to P and N regions
→ Photocurrent
```

**Characteristics**:

```
Responsivity: 0.6-0.9 A/W @ 1550nm
├── 1W optical → 0.6-0.9A current

Quantum Efficiency: 70-95%
├── % of photons converted to electrons

Bandwidth: 10-50 GHz
Dark Current: 0.1-10 nA
```

### APD (Avalanche Photodiode)

**Principle**: Internal gain via avalanche multiplication

```
High reverse bias (30-90V)
→ Strong electric field
→ Accelerated electrons
→ Impact ionization (secondary electrons)
→ Avalanche effect
→ Current gain (10-100×)

Gain (M): 10-100 typical

Effective responsivity = R × M
Example: 0.9 A/W × 20 = 18 A/W
```

**Advantages**:
- Higher sensitivity (~10 dB better than PIN)
- Good for long reach

**Disadvantages**:
- Higher noise (multiplication noise)
- Requires high voltage
- Temperature sensitive
- More expensive

---

## Transceiver Specifications

### Key Parameters

**Optical Power**:

```
Transmit Power:
├── Typical: -5 to +5 dBm (0.3-3 mW)
├── High power: +10 dBm (10 mW) for ER

Receive Sensitivity:
├── Good: -20 to -24 dBm (10-4 μW)
├── APD: -28 to -32 dBm (1.5-0.6 μW)

Receive Overload:
├── Maximum RX power before damage
├── Typical: 0 to +2 dBm

Power Budget:
└── TX power - RX sensitivity
└── Must exceed link loss
```

**Example: 10GBASE-LR**:

```
TX Power: -8 to +5 dBm
RX Sensitivity: -14.4 dBm
RX Overload: +0.5 dBm

Power Budget: 5 - (-14.4) = 19.4 dB

For 10km link:
├── Fiber loss: 10km × 0.3 dB/km = 3 dB
├── Connector loss: 2 × 0.5 dB = 1 dB
├── Margin: 3 dB
└── Total: 7 dB < 19.4 dB ✓
```

**Wavelength**:

```
Typical wavelengths:
├── 850 ± 20 nm (VCSEL, multimode)
├── 1310 ± 20 nm (FP, DFB)
├── 1550 ± 20 nm (DFB)
└── ITU grid (DWDM): 1530-1565 nm, 50/100 GHz spacing

Tolerance: ±20 nm typical
```

**Extinction Ratio**:

```
ER (dB) = 10 log₁₀(P₁ / P₀)

Where:
P₁ = Power for '1' bit
P₀ = Power for '0' bit

Good ER: >8.2 dB
Better ER: >10 dB

Low ER → Poor signal quality
```

**Rise/Fall Time**:

```
10-90% transition time

For 10 Gbps:
├── Bit period: 100 ps
├── Rise/fall time: <35 ps
└── Limits bandwidth

For 25 Gbps:
└── Rise/fall time: <14 ps
```

---

## Digital Diagnostics (DDM/DOM)

### SFF-8472 (SFP DDM)

**Monitored Parameters**:

```
Real-time monitoring via I²C:

1. Temperature:
   ├── Range: -40 to +125°C
   ├── Resolution: 1/256°C
   └── Alarm thresholds

2. Supply Voltage (Vcc):
   ├── Nominal: 3.3V
   ├── Range: 3.0-3.6V
   └── Alarm if outside

3. TX Bias Current:
   ├── Laser drive current (mA)
   ├── Typical: 10-80 mA
   └── Indicates laser health

4. TX Power:
   ├── Optical output power
   ├── dBm or mW
   └── Monitors laser degradation

5. RX Power:
   ├── Received optical power
   ├── dBm or mW
   └── Link budget monitoring

Alarm/Warning Thresholds:
├── High alarm
├── High warning
├── Low warning
└── Low alarm
```

**Reading DDM Data**:

```bash
# Linux with ethtool
ethtool -m eth0

Output:
        Identifier: SFP
        Module temperature: 45.00 C
        Module voltage: 3.29 V
        Laser bias current: 35.2 mA
        Laser output power: 0.50 mW / -3.01 dBm
        Receiver signal average optical power: 0.31 mW / -5.08 dBm
        Laser bias current high alarm threshold: 80.00 mA
        ...

# Or via vendor CLI (Cisco example)
show interface transceiver detail
```

**Use Cases**:

```
Monitoring:
├── Detect failing transceivers (low TX power)
├── Verify link budget (RX power)
├── Temperature issues
└── Predictive maintenance

Troubleshooting:
├── No link: Check RX power
├── High errors: Check RX power vs sensitivity
├── Intermittent: Check temperature
└── Link flapping: Check bias current drift
```

---

## Pluggable Standards Evolution

### Timeline

```
1995: GBIC (1G, large)
2001: SFP (1G, small)
2006: SFP+ (10G, same size as SFP)
2010: QSFP+ (40G, quad 10G)
2014: SFP28 (25G), QSFP28 (100G)
2016: OSFP (400G, large)
2017: QSFP-DD (400G, backward compatible)
2018: QSFP56 (200G)
2020: QSFP-DD 400G widely deployed
2023: QSFP112 (800G), 400G coherent in QSFP-DD

Trend:
└── More speed in same or slightly larger form factor
└── Backward compatibility critical
```

### Comparison Table

| Form Factor | Lanes | Speed/Lane | Total | Year | Size (L×W×H mm) |
|-------------|-------|------------|-------|------|-----------------|
| SFP         | 1     | 1G         | 1G    | 2001 | 56.5×13.4×8.5   |
| SFP+        | 1     | 10G        | 10G   | 2006 | 56.5×13.4×8.5   |
| SFP28       | 1     | 25G        | 25G   | 2014 | 56.5×13.4×8.5   |
| SFP56       | 1     | 50G PAM4   | 50G   | 2019 | 56.5×13.4×8.5   |
| QSFP+       | 4     | 10G        | 40G   | 2010 | 72×18.4×8.5     |
| QSFP28      | 4     | 25G        | 100G  | 2014 | 72×18.4×8.5     |
| QSFP56      | 4     | 50G PAM4   | 200G  | 2018 | 72×18.4×8.5     |
| QSFP-DD     | 8     | 50G PAM4   | 400G  | 2017 | 89.4×18.4×8.5   |
| QSFP112     | 8     | 100G PAM4  | 800G  | 2023 | 89.4×18.4×8.5   |
| OSFP        | 8     | 50-100G    | 400-800G | 2016 | 107.8×22.6×9.3 |

---

## Selection Guide

### By Distance

**0-100m (Short Reach)**:

```
Multimode Fiber:
├── 10GBASE-SR (SFP+): 300m on OM3, 850nm
├── 25GBASE-SR (SFP28): 100m on OM4, 850nm
├── 40GBASE-SR4 (QSFP+): 100m on OM4, 850nm, MPO
├── 100GBASE-SR4 (QSFP28): 100m on OM4, 850nm, MPO
└── 400GBASE-SR8 (QSFP-DD): 100m on OM4, 850nm, 2×MPO

Advantages:
├── Lowest cost ($20-200)
├── Easy to terminate
└── Good for data centers

Single-Mode (DR - Data Rate):
├── 100GBASE-DR (QSFP28): 500m SMF
└── 400GBASE-DR4 (QSFP-DD): 500m SMF
```

**2-10km (Long Reach)**:

```
Single-Mode Fiber:
├── 10GBASE-LR (SFP+): 10km, 1310nm
├── 25GBASE-LR (SFP28): 10km, 1310nm
├── 40GBASE-LR4 (QSFP+): 10km, 1310nm CWDM4
├── 100GBASE-LR4 (QSFP28): 10km, 1310nm CWDM4
└── 400GBASE-LR8 (QSFP-DD): 10km, 1310nm

Cost: $100-800

Use: Campus, metro access
```

**40-80km (Extended Reach)**:

```
Single-Mode Fiber:
├── 10GBASE-ER (SFP+): 40km, 1550nm
├── 100GBASE-ER4 (QSFP28): 40km, 1310nm CWDM4
└── 400GBASE-ER8 (QSFP-DD): 40km

Cost: $500-2000

Use: Metro, regional networks
```

**80km+ (Zero Reach / Coherent)**:

```
Coherent Optics:
├── 100G coherent: 80-120km (unamplified)
├── 200G coherent: 80-120km
├── 400G coherent: 80-120km
└── With amplifiers: 1000s of km

Cost: $2000-8000+

Use: Long-haul, subsea
```

### By Application

**Data Center Leaf-Spine**:

```
ToR to Server: SFP28 (25G) or QSFP28 breakout
Leaf to Spine: QSFP28 (100G) or QSFP-DD (400G)

Requirements:
├── Short reach (SR)
├── Low cost
├── High port density
└── Low power

Choice: SR optics (850nm, multimode)
```

**Campus/Enterprise**:

```
Building to Building: 10GBASE-LR (SFP+)
Core: 40G/100G LR

Requirements:
├── Medium reach (2-10km)
├── Single-mode fiber
└── Reliability

Choice: LR optics (1310nm)
```

**Service Provider Metro**:

```
Access/Aggregation: 10G/100G LR/ER
Core: 100G/400G coherent

Requirements:
├── Long reach (10-80km)
├── DWDM capable
└── High capacity

Choice: CWDM/DWDM optics, coherent
```

---

## Troubleshooting

### Common Issues

**No Link**:

```
Check List:
1. RX power:
   └── ethtool -m ethX
   └── Should be above sensitivity (-14 dBm typical)
   
2. TX power:
   └── Should be within spec (-5 to +5 dBm typical)
   
3. Fiber:
   └── Check for damage, bends
   └── Clean connectors
   
4. Wavelength mismatch:
   └── LX (1310nm) vs LH (1550nm)
   
5. Fiber type mismatch:
   └── SR on SMF (won't work)
   └── LR on MMF (may work short distance)

6. Duplex mismatch:
   └── TX on one end → RX on other end
```

**High Error Rate**:

```
Causes:
├── RX power too low (near sensitivity)
├── RX power too high (overload)
├── Dispersion (chromatic or PMD)
├── Dirty connectors
└── Bad transceiver

Debug:
└── Check RX power
└── Measure BER (Bit Error Rate)
└── Check interface errors:
    show interface ethX
    └── CRC errors, alignment errors
```

**Intermittent Link**:

```
Possible Causes:
1. Temperature:
   └── Transceiver overheating
   └── Check DDM temperature

2. Loose connector:
   └── Reseat fiber

3. Laser bias drift:
   └── Aging transceiver
   └── Replace

4. Marginal RX power:
   └── Near sensitivity threshold
   └── Small variations cause loss
```

### DDM Interpretation

```
Normal Values:
Temperature: 20-60°C
Voltage: 3.2-3.4V
TX Bias: 20-60 mA (varies by transceiver)
TX Power: 0 ± 3 dBm
RX Power: -10 to -3 dBm (for short links)

Warning Signs:
TX Power dropping → Laser aging
TX Bias increasing → Laser aging (compensating)
RX Power very low → Fiber issue or far-end TX problem
Temperature high → Cooling issue
Voltage out of range → Power supply problem
```

---

## Summary

**Optical transceivers** are the critical electro-optical interface:

**Form Factors**:
- **SFP/SFP+/SFP28/SFP56**: 1G to 50G, LC connector, ubiquitous
- **QSFP+/QSFP28/QSFP56**: 40G to 200G, MPO or LC, data center
- **QSFP-DD**: 400G, backward compatible, current standard
- **QSFP112/OSFP**: 800G, emerging

**Key Components**:
- **Lasers**: VCSEL (850nm, cheap), DFB (1310/1550nm), EML/coherent (100G+)
- **Photodetectors**: PIN (standard), APD (high sensitivity)
- **Modulation**: OOK (simple), PAM4 (50/100G), coherent (long-haul)

**Selection Criteria**:
- **Distance**: SR (<100m), LR (10km), ER (40km), coherent (80km+)
- **Fiber type**: MMF (short), SMF (long)
- **Cost vs performance**: SR cheapest, coherent most expensive
- **Power budget**: TX power - RX sensitivity must exceed losses

**Digital Diagnostics**:
- Monitor temperature, voltage, bias current, optical power
- Critical for troubleshooting and preventive maintenance
- Standards: SFF-8472 (SFP), SFF-8636 (QSFP)

Understanding transceivers is essential for network design, troubleshooting, and optimization.

---

# TDM and WDM Multiplexing: Complete Guide

## Table of Contents
1. [Multiplexing Fundamentals](#multiplexing-fundamentals)
2. [Time Division Multiplexing (TDM)](#time-division-multiplexing-tdm)
3. [SONET/SDH](#sonetsdh)
4. [Optical Transport Network (OTN)](#optical-transport-network-otn)
5. [Wavelength Division Multiplexing (WDM)](#wavelength-division-multiplexing-wdm)
6. [CWDM vs DWDM](#cwdm-vs-dwdm)
7. [ITU Grid and Channel Plans](#itu-grid-and-channel-plans)
8. [WDM Components](#wdm-components)
9. [Advanced WDM Techniques](#advanced-wdm-techniques)
10. [Comparison and Use Cases](#comparison-and-use-cases)

---

## Multiplexing Fundamentals

### Why Multiplexing?

```
Problem:
├── Single fiber pair
├── Multiple users/services need connectivity
└── How to share the fiber?

Solutions (Domains):
├── Time: Different users at different times (TDM)
├── Wavelength: Different users on different colors (WDM)
├── Space: Different fibers (SDM - future)
└── Code: Different spreading codes (CDM - wireless)
```

### Historical Context

```
1980s: TDM (SONET/SDH)
├── 155 Mbps - 10 Gbps
├── Dominated telecom
└── Circuit-switched, rigid

1990s: WDM emerges
├── 2-8 wavelengths initially
├── Erbium amplifiers enable DWDM
└── Exponential capacity growth

2000s: DWDM mature
├── 40-80 channels standard
├── 100+ channels possible
└── Terabit capacity per fiber

2010s: Flexible grid, coherent
├── Software-defined WDM
├── 100G/200G/400G per wavelength
└── Petabit capacity

2020s: Spatial multiplexing research
├── Multi-core fiber
├── Multi-mode fiber (mode multiplexing)
└── Next frontier
```

---

## Time Division Multiplexing (TDM)

### Basic TDM Concept

```
Principle: Different users transmit in different time slots

User A: ████░░░░████░░░░████░░░░
User B: ░░░░████░░░░████░░░░████
User C: ████░░░░████░░░░████░░░░
User D: ░░░░████░░░░████░░░░████

Timeline: →→→→→→→→→→→→→→→→→→→→→→

Frame structure:
┌──┬──┬──┬──┐┌──┬──┬──┬──┐┌──┬──┬──┬──┐
│A │B │C │D ││A │B │C │D ││A │B │C │D │
└──┴──┴──┴──┘└──┴──┴──┴──┘└──┴──┴──┴──┘
  Frame 1      Frame 2      Frame 3

Each user gets fixed time slot
Regular intervals (synchronous)
```

### TDM Hierarchy

**Electrical TDM (Pre-SONET)**:

```
DS0 (Digital Signal 0):
├── 64 kbps (one voice channel)
├── 8-bit sample at 8 kHz
└── Basic building block

DS1 (T1):
├── 24 × DS0 = 1.544 Mbps
├── North America/Japan
└── 24 voice channels

E1:
├── 32 × DS0 = 2.048 Mbps
├── Europe/Rest of world
├── 30 voice + 2 control
└── More efficient

DS3 (T3):
├── 28 × DS1 = 44.736 Mbps
└── Common in 1990s

E3:
├── 16 × E1 = 34.368 Mbps
└── European equivalent
```

### Limitations of Electrical TDM

```
Problems:
├── Multiple incompatible standards (T-carrier vs E-carrier)
├── Inefficient multiplexing (bit-stuffing)
├── Difficult add/drop (must demux entire hierarchy)
├── No standardized optical interface
└── Limited scalability

Solution: SONET/SDH (optical TDM standard)
```

---

## SONET/SDH

**SONET (Synchronous Optical Network)** - North America
**SDH (Synchronous Digital Hierarchy)** - International

### Basic Frame Structure

**STS-1 / OC-1 (51.84 Mbps)**:

```
Frame: 810 bytes (90 columns × 9 rows)
Duration: 125 μs (8000 frames/second)

┌────────────────────────────────────┐
│  Overhead  │      Payload         │
│  (27 bytes)│    (783 bytes)       │
│            │                      │
│  Section   │                      │
│  Line      │    SPE               │
│  Path      │    (Synchronous      │
│  Overhead  │     Payload          │
│            │     Envelope)        │
│            │                      │
└────────────────────────────────────┘
 3 columns   87 columns

Overhead:
├── Section: Framing, error monitoring
├── Line: Multiplexing, protection switching
└── Path: End-to-end payload monitoring

Bandwidth: 810 bytes / 125 μs = 51.84 Mbps
```

### SONET/SDH Hierarchy

```
┌──────────┬─────────────┬─────────────┬──────────┐
│ SONET    │ SDH         │ Line Rate   │ Payload  │
├──────────┼─────────────┼─────────────┼──────────┤
│ STS-1    │ -           │ 51.84 Mbps  │ 50.11 Mbps│
│ STS-3    │ STM-1       │ 155.52 Mbps │ 150.34 Mbps│
│ STS-12   │ STM-4       │ 622.08 Mbps │ 601.34 Mbps│
│ STS-48   │ STM-16      │ 2.488 Gbps  │ 2.405 Gbps│
│ STS-192  │ STM-64      │ 9.953 Gbps  │ 9.621 Gbps│
│ STS-768  │ STM-256     │ 39.813 Gbps │ 38.486 Gbps│
└──────────┴─────────────┴─────────────┴──────────┘

Naming:
STS: Synchronous Transport Signal (electrical)
OC: Optical Carrier (optical equivalent)
STM: Synchronous Transport Module

Examples:
OC-3 = 155 Mbps optical
OC-48 = 2.5 Gbps optical
OC-192 = 10 Gbps optical
```

### Virtual Tributaries (VT)

```
Mapping sub-STS-1 signals into SONET:

VT1.5: 1.728 Mbps (carries T1 - 1.544 Mbps)
VT2: 2.304 Mbps (carries E1 - 2.048 Mbps)
VT3: 3.456 Mbps
VT6: 6.912 Mbps

One STS-1 can carry:
├── 28 × VT1.5 (T1s), or
├── 21 × VT2 (E1s), or
└── Various combinations

Allows efficient transport of legacy services
```

### SONET/SDH Add/Drop Multiplexing

**ADM (Add/Drop Multiplexer)**:

```
Traditional:
           ┌──────────────┐
  OC-48 ──→│     ADM      │──→ OC-48
           │  Drop: OC-3  │
           └──────┬───────┘
                  │
                OC-3 (local traffic)

Drops specific tributary without demuxing entire signal

Advantages:
├── Flexible
├── Efficient
└── Enables ring topologies

Protection:
├── BLSR (Bidirectional Line Switched Ring)
├── 50ms failover
└── 1+1 or 1:1 protection
```

### SONET/SDH Overhead

**Section Overhead (SOH)**:

```
A1, A2: Framing bytes (0xF628)
├── Synchronization
└── Start of frame marker

C1: STS-1 ID
├── Identifies position in STS-N
└── For concatenation

B1: BIP-8 (Bit Interleaved Parity)
├── Error detection
└── Section level

E1: Orderwire
├── Voice communication between technicians
└── 64 kbps channel

F1: Section user channel
D1-D3: Section data channels
```

**Line Overhead (LOH)**:

```
B2: BIP-8 for line
├── Error monitoring
└── More detailed than B1

K1, K2: APS (Automatic Protection Switching)
├── Coordination for protection switching
└── 50ms switchover

D4-D12: Line data channels
└── DCC (Data Communications Channel)
└── Management, signaling

S1: Synchronization status
Z1, Z2: Growth bytes
```

**Path Overhead (POH)**:

```
J1: Path trace
├── 64-byte string identifying path
└── End-to-end verification

B3: BIP-8 for path
├── Error monitoring
└── End-to-end quality

C2: Signal label
├── Identifies payload type
└── 0x13 = ATM, 0x1C = GbE

G1: Path status
├── Remote error indication
└── Remote defect indication

F2: Path user channel
H4: Multiframe indicator
Z3-Z5: Growth bytes
```

---

## Optical Transport Network (OTN)

**OTN (G.709)** - Modern digital wrapper for optical transport

### Why OTN?

```
SONET/SDH Limitations:
├── Designed for voice (TDM oriented)
├── Inefficient for packet data
├── Limited error correction
├── No wavelength management
└── Fixed granularity

OTN Benefits:
├── Strong FEC (Forward Error Correction)
├── Efficient data transport
├── Multi-protocol support
├── Wavelength-level management
├── Flexible hierarchy
└── Better OAM (Operations, Administration, Maintenance)
```

### OTN Hierarchy

```
┌──────────┬─────────────┬──────────────┬──────────┐
│ Signal   │ Line Rate   │ Payload      │ Client   │
├──────────┼─────────────┼──────────────┼──────────┤
│ OTU1     │ 2.666 Gbps  │ 2.488 Gbps   │ OC-48    │
│ OTU2     │ 10.709 Gbps │ 10.037 Gbps  │ 10GbE    │
│ OTU2e    │ 11.095 Gbps │ 10.355 Gbps  │ 10GbE LAN│
│ OTU3     │ 43.018 Gbps │ 40.319 Gbps  │ 40GbE    │
│ OTU4     │ 111.809 Gbps│ 104.794 Gbps │ 100GbE   │
│ OTUCn    │ n×100 Gbps  │ Flexible     │ FlexE    │
└──────────┴─────────────┴──────────────┴──────────┘

OTU: Optical Channel Transport Unit (full frame with FEC)
ODU: Optical Channel Data Unit (payload + overhead)
OPU: Optical Channel Payload Unit (client signal)

OTU = ODU + FEC
ODU = OPU + ODU Overhead
OPU = Client signal + stuff
```

### OTN Frame Structure

```
OTU Frame (4 rows × 4080 columns):

┌────┬──────────────────────────────────┬─────┐
│ OA │         OPU                      │ FEC │
│ M  │         (Payload)                │     │
├────┼──────────────────────────────────┼─────┤
│ OA │         ODU Overhead             │ FEC │
│ M  │         + OPU Overhead           │     │
├────┼──────────────────────────────────┼─────┤
│    │                                  │     │
│    │         Client Data              │ FEC │
│    │                                  │     │
└────┴──────────────────────────────────┴─────┘
  14    3808 columns                     256

Total: 4080 columns × 4 rows = 16,320 bytes
Duration: Varies by rate (48.97 μs for OTU2)

FEC: Reed-Solomon RS(255, 239)
└── Can correct up to 8 symbol errors per codeword
└── ~6 dB coding gain
```

### OTN Multiplexing

**ODUflex**:

```
Flexible rate containers

Example: Transport 10GbE WAN (9.95328 Gbps)
├── Create ODUflex of exactly this rate
├── More efficient than fixed OTU2
└── Bandwidth on demand

ODU0: 1.25 Gbps (GbE)
ODU1: 2.5 Gbps
ODU2: 10 Gbps
ODU2e: 10.3 Gbps
ODU3: 40 Gbps
ODU4: 100 Gbps
ODUflex: Variable
```

**OTN Multiplexing Example**:

```
ODU4 (100G) can carry:
├── 10 × ODU0 (GbE), or
├── 4 × ODU2 (10GbE), or
├── 2 × ODU3 (40GbE), or
├── 1 × ODU4 (100GbE), or
└── Mix of above

Hierarchical:
ODU4 → ODU3 → ODU2 → ODU1 → ODU0
        ↓       ↓
      40GbE   10GbE

Efficient, flexible transport
```

### OTN Tandem Connection Monitoring (TCM)

```
Up to 6 TCM levels:

End-to-end path:
A ─────────────────────────────→ D
    ↑           ↑           ↑
    B           C           

TCM1: A → D (end-to-end)
TCM2: A → B (operator 1)
TCM3: B → C (operator 2)
TCM4: C → D (operator 3)

Each level independent OAM:
├── Error monitoring
├── Performance monitoring
└── Defect reporting

Enables multi-operator networks
```

---

## Wavelength Division Multiplexing (WDM)

### Basic WDM Concept

```
Principle: Different wavelengths (colors) simultaneously on one fiber

     λ1 (Red)    ──────────────→
     λ2 (Orange) ──────────────→
     λ3 (Yellow) ──────────────→  All on same fiber!
     λ4 (Green)  ──────────────→
     λ5 (Blue)   ──────────────→

Each wavelength = Independent channel
Each can carry 10G/40G/100G/400G

Example:
├── 80 wavelengths
├── Each at 100 Gbps
└── Total: 8 Tbps on one fiber pair!
```

### WDM Components

**Multiplexer (Mux)**:

```
Combines multiple wavelengths:

λ1 ──┐
λ2 ──┤
λ3 ──┼──→ Fiber (λ1+λ2+λ3+λ4)
λ4 ──┘

Technology:
├── Thin-Film Filters (TFF)
├── Arrayed Waveguide Grating (AWG)
└── Diffraction grating
```

**Demultiplexer (Demux)**:

```
Separates wavelengths:

Fiber (λ1+λ2+λ3+λ4) ──┬──→ λ1
                       ├──→ λ2
                       ├──→ λ3
                       └──→ λ4

Same technology as Mux
Often bidirectional (Mux/Demux)
```

**Optical Add/Drop Multiplexer (OADM)**:

```
Add/drop specific wavelengths:

           ┌──────────────┐
λ1,λ2,λ3,λ4│     OADM     │λ1,λ2,λ5,λ4
    ───────→│   Drop: λ3   │───────→
           │   Add: λ5    │
           └──────┬───┬───┘
                  │   │
                 λ3  λ5
              (drop) (add)

Passive OADM:
└── Fixed wavelengths

Reconfigurable OADM (ROADM):
└── Software-selectable wavelengths
└── Enables flexible networks
```

---

## CWDM vs DWDM

### CWDM (Coarse WDM)

```
Channel Spacing: 20 nm

Wavelength Range: 1270-1610 nm

Channels:
1270, 1290, 1310, 1330, 1350, 1370,
1390, 1410, 1430, 1450, 1470, 1490,
1510, 1530, 1550, 1570, 1590, 1610 nm

Total: 18 channels (typically use 4-8)

Characteristics:
├── Wide spacing (20 nm)
├── Uncooled lasers (±3nm tolerance OK)
├── Low cost ($200-500 per transceiver)
├── No amplification (EDFA doesn't work across range)
├── Distance: 40-80 km max
└── No temperature control needed

Use Cases:
├── Metro access
├── Campus networks
├── Data center interconnect
└── Cost-sensitive applications

Advantages:
├── Simple, low cost
├── Low power
└── Easy to deploy

Disadvantages:
├── Limited channels
├── No amplification
└── Limited distance
```

**CWDM Transceiver Example**:

```
CWDM4 (100GBASE-CWDM4):

4 wavelengths:
├── λ1: 1271 nm
├── λ2: 1291 nm
├── λ3: 1311 nm
└── λ4: 1331 nm

Each: 25 Gbps (NRZ or PAM4)
Total: 100 Gbps

Distance: 2 km on SMF
Connector: Duplex LC
Cost: ~$300-600
```

### DWDM (Dense WDM)

```
Channel Spacing: 50 GHz or 100 GHz (0.4 nm or 0.8 nm)

Wavelength Range: 1530-1565 nm (C-band)
                  1565-1625 nm (L-band)

Channels:
├── 100 GHz grid: 40 channels (C-band)
├── 50 GHz grid: 80 channels (C-band)
├── 25 GHz grid: 160 channels (possible)
└── Flexible grid: Variable spacing

Characteristics:
├── Narrow spacing (0.4-0.8 nm)
├── Cooled lasers (±0.01nm stability required)
├── High cost ($2000-8000 per transceiver)
├── Amplification (EDFA)
├── Distance: 100s to 1000s of km
└── Precise temperature control (TEC)

Use Cases:
├── Long-haul transport
├── Submarine cables
├── Metro core
└── High-capacity needs

Advantages:
├── Many channels (40-80+)
├── Long distance (amplifiers)
├── High total capacity (Tbps)
└── Scalable

Disadvantages:
├── Expensive
├── Complex
├── Higher power
└── Requires tight wavelength control
```

---

## ITU Grid and Channel Plans

### ITU-T G.694.1 (DWDM)

**C-Band Grid**:

```
Reference Frequency: 193.1 THz (1552.52 nm)

100 GHz spacing:
Channel 20: 192.1 THz (1561.42 nm)
Channel 21: 192.2 THz (1560.61 nm)
Channel 22: 192.3 THz (1559.79 nm)
...
Channel 60: 196.1 THz (1529.16 nm)
Channel 61: 196.2 THz (1528.38 nm)

Formula:
Frequency (THz) = 193.1 + (n × 0.1)
Where n is channel number offset from reference

50 GHz spacing:
Interleaves 100 GHz grid
Channels: ..., 20.5, 21, 21.5, 22, 22.5, ...
Doubles capacity
```

**Wavelength to Frequency Conversion**:

```
c = λ × f

Where:
c = speed of light (3 × 10⁸ m/s)
λ = wavelength (meters)
f = frequency (Hz)

Example:
1550 nm = 1550 × 10⁻⁹ m

f = (3 × 10⁸) / (1550 × 10⁻⁹)
  = 193.548 THz

Channel spacing:
100 GHz = 0.1 THz
≈ 0.8 nm @ 1550 nm

50 GHz = 0.05 THz
≈ 0.4 nm @ 1550 nm
```

### Flexible Grid (Flex Grid)

```
ITU-T G.694.1 Amendment:

Traditional:
├── Fixed 50/100 GHz slots
├── Wastes spectrum
└── All channels same size

Flexible Grid:
├── Variable slot widths (12.5 GHz granularity)
├── Spectrum efficiency
└── Match bandwidth to need

Example:
┌────┬────┬────────┬────┬────┬────┐
│10G │10G │ 100G   │10G │10G │10G │
└────┴────┴────────┴────┴────┴────┘
 50G  50G   150 GHz 50G  50G  50G

100G channel uses 3× 50GHz slots
10G channels use 1× 50GHz slot

Benefits:
├── Efficient spectrum use
├── Flexible allocation
└── Supports super-channels (400G+)
```

---

## WDM Components

### Optical Amplifiers

**EDFA (Erbium-Doped Fiber Amplifier)**:

```
Structure:
Input ──→ [Er-doped fiber] ──→ Output
            ↑
         980/1480nm pump laser

Principle:
├── Erbium ions in fiber
├── Pumped to excited state
├── Stimulated emission amplifies signal
└── Amplifies all wavelengths simultaneously

Characteristics:
├── Gain: 15-30 dB
├── Wavelength: C-band (1530-1565 nm)
├── Noise Figure: 4-6 dB
├── Pump: 980 nm (low noise) or 1480 nm (high power)
└── Gain flatness: ±0.5 dB (with GFF)

Advantages:
├── Amplifies all DWDM channels at once
├── No electrical conversion
├── Low noise
└── Mature technology

Disadvantages:
├── C-band only
├── ASE noise accumulation
└── Non-flat gain (needs equalization)
```

**Raman Amplification**:

```
Distributed amplification in fiber itself

Principle:
├── Pump light (1450-1480 nm)
├── Stimulated Raman Scattering
├── Transfer energy to signal (1530-1565 nm)
└── Amplification distributed along fiber

Advantages:
├── Lower noise figure (distributed gain)
├── Wider bandwidth (can cover C+L bands)
├── Reduces EDFA spacing
└── Better OSNR

Disadvantages:
├── Requires high pump power
├── More complex
└── Expensive

Use: Long-haul submarine, ultra-long-haul
```

### Optical Filters

**Thin-Film Filter (TFF)**:

```
Multiple dielectric layers:

Glass ─┬─ Layer 1 (n₁)
       ├─ Layer 2 (n₂)
       ├─ Layer 3 (n₁)
       ├─ ...
       └─ Layer N

Each layer: λ/4 optical thickness
Interference creates:
├── Passband (transmitted)
└── Stopband (reflected)

Characteristics:
├── Narrow passband (0.2-0.8 nm)
├── Low insertion loss (<0.5 dB)
├── High isolation (>25 dB)
└── Temperature stable

Use: Mux/Demux, OADM
```

**Arrayed Waveguide Grating (AWG)**:

```
Planar lightwave circuit (PLC):

Input ──→ [Free prop region] ──→ [Waveguide array] ──→ [Free prop region] ──→ Outputs
                                    (different lengths)

Principle:
├── Different wavelengths diffract at different angles
├── Waveguide array creates phase delays
├── Constructive interference at specific outputs
└── Wavelength-dependent routing

Characteristics:
├── Multiple channels simultaneously
├── Low loss (2-4 dB)
├── Flat-top passband
└── Temperature sensitive (needs heater)

Use: 40-channel DWDM Mux/Demux
```

### ROADM (Reconfigurable OADM)

```
Software-configurable wavelength add/drop

Architecture:
          ┌───────────────┐
    In ──→│  WSS (West)   │──→ West
          │               │
          │  WSS (East)   │──→ East
    Out ←─│               │
          │  WSS (Add)    │←── Add
          │  WSS (Drop)   │──→ Drop
          └───────────────┘

WSS = Wavelength Selective Switch

Features:
├── Remotely configurable
├── Per-wavelength control
├── Built-in monitoring
├── Colorless/directionless/contentionless
└── Mesh network support

ROADM Degrees:
├── Degree 2: Linear (West-East)
├── Degree 4: Four directions
├── Degree 9+: Hub sites
└── More degrees = more flexibility

CDC (Colorless, Directionless, Contentionless):
├── Colorless: Any transceiver, any wavelength
├── Directionless: Any port, any direction
├── Contentionless: Same wavelength, multiple directions
└── Maximum flexibility
```

---

## Advanced WDM Techniques

### Super-Channels

```
Multiple closely-spaced carriers acting as one channel

Example: 400G super-channel
├── 4 × 100G subcarriers
├── Each on separate wavelength
├── Spaced 50 GHz apart
├── Total: 150 GHz width
└── Managed as single entity

Benefits:
├── Higher spectral efficiency
├── Better OSNR
├── Coordinated modulation
└── Nyquist shaping reduces guard bands

Challenges:
├── Requires flexible grid
├── More complex transceivers
└── Tight wavelength control
```

### Alien Wavelengths

```
Third-party wavelengths on DWDM system

Traditional:
└── Transponders from same vendor as DWDM

Alien Wavelength:
└── Transponders from different vendor

Requirements:
├── Proper power levels
├── Correct wavelength (ITU grid)
├── Chromatic dispersion compensation
└── OSNR budget

Benefits:
├── Cost reduction
├── Vendor diversity
└── Flexibility

Challenges:
├── Interoperability testing
├── Support complexity
└── Performance guarantees
```

### Optical Grooming

```
Efficient packing of sub-wavelength services

Example:
10 × 10GbE clients → 1 × 100G wavelength

Traditional:
├── 10 wavelengths (one per 10GbE)
├── Expensive, wasteful

Grooming:
├── Multiplex 10GbE into OTU4 (100G)
├── One wavelength carries all
├── 10× spectrum efficiency
└── Lower cost

Technologies:
├── OTN (ODU multiplexing)
├── FlexE (Ethernet grooming)
└── Packet-optical integration
```

---

## Comparison and Use Cases

### TDM vs WDM

```
┌──────────────┬─────────────┬─────────────┐
│ Aspect       │ TDM         │ WDM         │
├──────────────┼─────────────┼─────────────┤
│ Domain       │ Time        │ Wavelength  │
│ Granularity  │ Fixed (64k) │ Wavelength  │
│ Capacity/    │ 10 Gbps     │ 100s Gbps - │
│ fiber        │             │ 10+ Tbps    │
│ Flexibility  │ Low         │ High        │
│ Add/drop     │ Any rate    │ Per-λ only  │
│ Distance     │ Limited     │ Very long   │
│ Cost         │ Moderate    │ High (DWDM) │
│ Complexity   │ Moderate    │ High        │
│ Use case     │ Legacy      │ Modern      │
└──────────────┴─────────────┴─────────────┘
```

### CWDM vs DWDM

```
┌──────────────┬─────────────┬─────────────┐
│ Aspect       │ CWDM        │ DWDM        │
├──────────────┼─────────────┼─────────────┤
│ Channels     │ 8-18        │ 40-80+      │
│ Spacing      │ 20 nm       │ 0.4-0.8 nm  │
│ Distance     │ 40-80 km    │ 1000+ km    │
│ Amplify      │ No          │ Yes (EDFA)  │
│ Cost/channel │ $200-500    │ $2000-8000  │
│ Laser        │ Uncooled    │ Cooled      │
│ Complexity   │ Low         │ High        │
│ Use case     │ Metro/access│ Long-haul   │
└──────────────┴─────────────┴─────────────┘
```

### Use Case Selection

**Metro Access (5-20 km)**:
```
Choice: CWDM or DWDM
├── CWDM if <16 channels needed
├── DWDM if >16 channels
├── Passive mux/demux
└── No amplifiers
```

**Metro Core (20-80 km)**:
```
Choice: DWDM
├── 40-80 channels
├── 100 GHz spacing
├── Passive or ROADM
└── May need one amplifier
```

**Long-Haul (80-600 km)**:
```
Choice: DWDM
├── 80+ channels
├── 50 GHz spacing
├── ROADM networks
├── EDFAs every 80 km
└── Coherent optics
```

**Ultra-Long-Haul (600+ km)**:
```
Choice: DWDM
├── Coherent optics (100-400G per λ)
├── Raman + EDFA
├── 50 GHz or flex grid
├── Advanced FEC
└── Dispersion compensation
```

---

## Summary

**Multiplexing** enables sharing expensive optical fiber:

**TDM (Time Division)**:
- Different users at different times
- SONET/SDH: Legacy, circuit-switched
- OTN: Modern, packet-friendly, strong FEC
- Limited to single wavelength capacity

**WDM (Wavelength Division)**:
- Different users on different colors
- **CWDM**: 18 channels, 20nm spacing, cheap, metro
- **DWDM**: 80+ channels, 0.4nm spacing, expensive, long-haul
- Massive capacity scaling (Tbps per fiber)

**Key Technologies**:
- **EDFA**: Optical amplifier (C-band)
- **ROADM**: Software-reconfigurable add/drop
- **ITU Grid**: Standardized channel plan
- **Coherent**: Advanced modulation (100G-800G per wavelength)

**Evolution**:
- 1980s: TDM only (10 Gbps per fiber)
- 1990s: WDM emerges (100 Gbps per fiber)
- 2000s: DWDM mature (1 Tbps per fiber)
- 2010s: Coherent optics (10 Tbps per fiber)
- 2020s: Flexible grid, super-channels (50+ Tbps per fiber)

Understanding these multiplexing techniques is fundamental for designing and operating modern optical networks.

---

