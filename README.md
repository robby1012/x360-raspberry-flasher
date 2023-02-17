XBOX 360 NAND flasher firmware for Raspberry Pi Pico

Requirements :
Hardwares:
- Working Xbox 360 (all types, except Winchester Motherboard)
- Raspberry Pico (Any ROM size, firmware only 80KB, so will fit 128K ROM)
- Micro USB Cable
- PC/Laptop
- Wire
- Solder Iron
- Solder Wire
- Solder Flux
- Alcohol IPA for Cleaning

Software:
Windows 7 or Above | Linux Distro with Kernel 4+
C Compiler:
LINUX
- GCC
- CMAKE
- makesure C development packages installed
Windows
- Visual Studio
Compile using Visual Studio, GCC or your preferred compiler
or
use precompiled binary (.uf2) at BIN folder

Install Firware:
HOLD BUTTON on PICO
KEEP HOLD then connect USB cable to your Computer
If you do it right a new drive will appear/mounted
Copy .uf2 firmware to the drive
Pico will reboot for reflash
Open J-Runner to read/write nand (AutoGG not Supported)

## Wiring Installation:

### Nand Flash
| Pico | Xbox | Xbox Component Identifier
| ------------- | ------------- |
| GP16  | SPI_MISO  | FAT: J1D2 Pin 4 Slim : J2C1 Pin 4 
| GP17  | SPI_SS_N | FAT: J1D2 Pin 2 Slim: J2C1 Pin 2
| GP18  | SPI_CLK  | FAT: J1D2 Pin 3 Slim: J2C1 Pin 3
| GP19  |  SPI_MOSI | FAT: J1D2 Pin 1 Slim: J2C1 Pin 1
| GP20  |  SMC_DBG_EN | FAT: J2B1 Pin 4 Slim: J2C3 pin 4
| GP21  | SMC_RST_XDK_N  | FAT: J2B1 Pin 3 Slim: J2C3 Pin 3
| GND  |  GND | ANY GROUND
* On Xbox Motherboard there's marking for numbered pin like this
2 * * 1
  * *
  * *
  * *
10*

### ISD12xx Audible Feedback IC for Editing Xbox Chime Sound (Slim Only, Fat doesn't have speaker)
|  | Pico | Trinity | Corona |
| ------------- | ------------- | ------------- | ------------- |
SPI_RDY | GP11 | FT2V4 | J2C2-A10
SPI_MISO | GP12 | FT2R7 | J2C2-B11
SPI_SS_N | GP13 | FT2R6 | J2C2-A11
SPI_CLK | GP14 | FT2T4 | J2C2-A8
SPI_MOSI | GP15 | FT2T5 | J2C2-B8

### EMMC Flash (Xbox 360 Corona Motherboard Only)
| Pico | Xbox | Corona 4GB |
| ------------- | ------------- | ------------- |
| GP6  | FLSH_DATA<0> | U1D1 pin 16 |
| GP7  | FLSH_WP_N (CMD) | U1D1 pin 3 |
| GP8  |  FLSH_CE_N (CLK) | U1D1 pin 2 |
| GP9  |  MMC_RST_N | U1D1 pin 1 |
| GP21  | SMC_RST_XDK_N  | Same as 16MB flash |
| GND  |  GND | U1D1 PIN 4 | ANY GROUND on Xbox Motherboard

**DO NOT SOLDER ANYTHING TO THE CRYISTAL, it will shutdown SMC. Unlike 4GB reader/writer, pico need Xbox SMC to read and write NAND, 4GB reader use EMMC Controller on external board**
