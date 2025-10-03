## Dino Game on ESP LilyGO T-Display

A T-Rex game inspired by Google Chrome's offline game, implemented on the ESP LilyGO T-Display.
The dinosaur jumps over obstacles and avoids pterodactyls. This is done by pressing a button connected to GPIO 0 as an input. The game is displayed on an onboard 1.14-inch TFT screen.

### Features
- Simple gameplay: Press a button to jump over cacti and pterodactyl.
- TFT display rendering using TFT_eSPI library.
- Optimized for ESP32.

### Hardware Requirements
- LilyGO T-Display.
- USB-C cable for programming.

### Software Requirements
- Arduino IDE or PlatformIO.
- Libraries: TFT_eSPI.

### Setup
1. Install Arduino IDE and ESP32 board support
2. Configure TFT_eSPI for LilyGO T-Display
3. Copy /Dino/src/ files to your sketch
4. Upload to the ESP32 via USB-C

### Usage
- Press the onboard button (GPIO 0) to make the dinosaur jump.
- Advance and avoid obstacles to increase your score.

### File Structure
* src/main.ino - Main game logic
* src/ptero_enemy.h - Bitmap data for the pterodactyl enemies
* src/noInternet.h - Bitmap data for the "No Internet" Chrome browser screen
* src/gameover.h - Bitmap data for Game Over screen after collision with enemieses.

### Demo
