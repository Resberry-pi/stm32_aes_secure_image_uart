<p align="center">
  <img src="https://img.shields.io/badge/MCU-STM32F407VETx-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white" alt="MCU"/>
  <img src="https://img.shields.io/badge/AES-128--CBC-success?style=for-the-badge" alt="AES"/>
  <img src="https://img.shields.io/badge/RNG-Hardware%20On--Chip-blue?style=for-the-badge" alt="RNG"/>
  <img src="https://img.shields.io/badge/Language-Embedded%20C%20%2B%20Python-yellow?style=for-the-badge" alt="Lang"/>
  <img src="https://img.shields.io/badge/AES%20Lib-Public%20Domain-lightgrey?style=for-the-badge" alt="License"/>
</p>

<h1 align="center">AES-128 Secure Image Transmission</h1>
<h3 align="center">STM32F407VETx — Bare-Metal Embedded Cryptography</h3>

<p align="center">
  <em>A two-stage progression from basic ECB block cipher learning to a complete hardware-RNG-keyed AES-CBC secure image transmission system — all running on bare-metal ARM Cortex-M4, no OS, no cloud.</em>
</p>

---

## Overview

| | Stage 1 | Stage 2 |
|---|---|---|
| **Name** | AES-ECB Demo | Secure Image Sender |
| **Mode** | AES-128 ECB | AES-128 CBC |
| **Key** | Hardcoded (learning) | Hardware RNG — unique per boot |
| **Data** | 16-byte plaintext | 320×240 monochrome image (9600 bytes) |
| **Output** | UART terminal | Encrypted stream → Python receiver → PNG |
| **Purpose** | Understand AES block cipher | Full secure embedded-to-host data link |

---

## Hardware

| Component | Details |
|---|---|
| **MCU** | STM32F407VETx LQFP100 — ARM Cortex-M4 @ 168 MHz |
| **SRAM** | 192 kB |
| **Flash** | 512 kB — 5WS latency, instruction + data cache enabled |
| **Clock** | HSE Crystal → PLL → 168 MHz SYSCLK |
| **RNG** | On-chip hardware RNG — analog noise entropy source |
| **UART** | USART2 — PA2 (TX), PA3 (RX) — 115200 baud, 8N1 |
| **GPIO** | PA6, PA7 — status indicators |
| **Debug** | SWD — PA13 (SWDIO), PA14 (SWCLK) |
| **Firmware** | STM32Cube FW_F4 V1.28.3 |

<!--
  TIP: Add a photo of your hardware setup here.
  Place the image in a /docs/images/ folder in your repo, then replace the line below:

  ![Hardware Setup](docs/images/hardware_setup.jpg)
-->

---

## Repository Structure

```
stm32_aes_secure_image_uart/
│
├── 01_AES_ECB_Demo/               ← Stage 1: AES-128 ECB baseline
│   ├── Src/
│   │   ├── main.c                 — encrypt/decrypt + UART output
│   │   └── aes.c                  — tiny-AES-c library
│   └── Inc/
│       ├── main.h
│       └── aes.h
│
├── 02_Secure_Image_Sender/        ← Stage 2: HW-RNG + CBC image TX
│   ├── Src/
│   │   ├── main.c                 — RNG keygen + CBC encrypt + UART stream
│   │   ├── aes.c
│   │   └── image_data.c           — 320×240 bitmap stored in Flash
│   ├── Inc/
│   │   ├── main.h
│   │   ├── aes.h
│   │   └── image_data.h
│   └── Python/
│       └── receiver.py            — decrypt + reconstruct image on host PC
│
└── docs/
    └── AES_RND_image_cubemx.pdf   — CubeMX hardware configuration report
```

---

## Stage 1 — AES-128 ECB Demo

> Minimal proof-of-concept to establish baseline understanding of the AES block cipher on embedded hardware.

### Pipeline

```
 ┌─────────────────────────────────────────┐
 │         STM32F407 @ 168 MHz             │
 │                                         │
 │  Plaintext: "HELLO STM32!!!"            │
 │         │                               │
 │         ▼                               │
 │   AES-128 ECB Encrypt                   │
 │   key: "STM32CryptoKey!!"               │
 │         │                               │
 │         ▼                               │
 │   Ciphertext (16 bytes raw binary)      │
 │         │                               │
 │         ▼                               │
 │   AES-128 ECB Decrypt                   │
 │         │                               │
 │         ▼                               │
 │   Recovered Plaintext                  │
 │         │                               │
 └─────────┼───────────────────────────────┘
           │ USART2 @ 115200 baud
           ▼
      PC Terminal
  [Original] → [Encrypted] → [Decrypted]
```

<!--
  TIP: Add a UART terminal screenshot here, e.g.:
  ![ECB UART Output](docs/images/ecb_uart_output.png)
-->

### Parameters

| Parameter | Value |
|---|---|
| AES Mode | ECB (Electronic Codebook) |
| Key | 128-bit hardcoded |
| Block size | 16 bytes |
| UART | 115200 baud |

> ️ **ECB is intentionally used here for learning only.** ECB encrypts each block independently — identical plaintext always produces identical ciphertext, leaking data patterns. Stage 2 fixes this with CBC.

---

## Stage 2 — Secure Image Sender

> Complete embedded-to-host secure data link with hardware-generated keys, AES-CBC encryption, and Python-based image reconstruction.

### Pipeline

```
 ┌─────────────────────────────────────────────────┐
 │              STM32F407 @ 168 MHz                │
 │                                                 │
 │  Power-On                                       │
 │      │                                          │
 │      ▼                                          │
 │  Hardware RNG ──► 128-bit AES Key  (random)     │
 │  Hardware RNG ──► 128-bit IV       (random)     │
 │                   (unique every boot)           │
 │      │                                          │
 │      ▼                                          │
 │  image_data[] in Flash                          │
 │  320×240 monochrome bitmap = 9600 bytes         │
 │      │                                          │
 │      ▼                                          │
 │  memcpy ──► RAM tx_buffer                       │
 │      │                                          │
 │      ▼                                          │
 │  AES-128 CBC Encrypt (key + IV)                 │
 │      │                                          │
 └──────┼──────────────────────────────────────────┘
        │ USART2 @ 115200 baud
        ▼
  ┌──────────┬──────────┬──────────────────────┐
  │ 16 bytes │ 16 bytes │    9600 bytes        │
  │   KEY    │    IV    │    CIPHERTEXT        │
  └──────────┴──────────┴──────────────────────┘
        │
        ▼
 ┌─────────────────────────────────────────────────┐
 │              Python Receiver (PC)               │
 │                                                 │
 │  Read KEY (16 bytes) + IV (16 bytes)            │
 │  Read CIPHERTEXT (9600 bytes)                   │
 │      │                                          │
 │      ▼                                          │
 │  AES-128 CBC Decrypt                            │
 │      │                                          │
 │      ▼                                          │
 │  numpy.unpackbits ──► 76800 pixels              │
 │  Reshape ──► 320×240 ──► PNG saved             │
 └─────────────────────────────────────────────────┘
```

<!--
  TIP: Add the reconstructed output image here, e.g.:
  ![Decrypted Image Output](docs/images/decrypted_output.png)
-->

### Parameters

| Parameter | Value |
|---|---|
| AES Mode | CBC (Cipher Block Chaining) |
| Key | 128-bit — on-chip hardware RNG |
| IV | 128-bit — on-chip hardware RNG |
| Image | 320×240 monochrome, 9600 bytes in Flash |
| Stream format | KEY → IV → CIPHERTEXT (raw binary) |
| UART | 115200 baud |

### Why Hardware RNG?

The STM32F407 has a dedicated **on-chip hardware RNG peripheral** sourcing entropy from analog noise in the silicon:

-  No hardcoded keys anywhere in firmware Flash
-  No software PRNG vulnerabilities
-  Fresh unique key + IV on every power-on
-  No two transmissions are ever identical

### Why CBC over ECB?

CBC XORs each plaintext block with the previous ciphertext block before encrypting — identical plaintext blocks produce **completely different ciphertext**. The image structure is fully hidden in the encrypted stream.

---

## Python Receiver

Included at `02_Secure_Image_Sender/Python/receiver.py`

### Install

```bash
pip install pyserial pycryptodome pillow numpy
```

### Run

```bash
python receiver.py --port COM3                               # Windows
python receiver.py --port /dev/ttyUSB0                       # Linux
python receiver.py --port COM3 --output decrypted_image.png  # custom name
```

---

## Third Party

| Library | Source | License |
|---|---|---|
| tiny-AES-c | https://github.com/kokke/tiny-AES-c | The Unlicense (Public Domain) |

---

## Author

**Rohan Kumar Singh**
B.Tech ECE — Babasaheb Bhimrao Ambedkar University, Lucknow
Embedded Systems Intern — BIT Mesra (2025)

<p>
  <a href="https://linkedin.com/in/YOUR_LINKEDIN_ID">
    <img src="https://img.shields.io/badge/LinkedIn-Connect-0A66C2?style=for-the-badge&logo=linkedin" alt="LinkedIn"/>
  </a>
  &nbsp;
  <a href="mailto:rohankumar17362@gmail.com">
    <img src="https://img.shields.io/badge/Email-rohankumar17362@gmail.com-D14836?style=for-the-badge&logo=gmail&logoColor=white" alt="Email"/>
  </a>
</p>
