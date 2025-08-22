# FunctionToWav 🎵  

This project provides a **C++ utility** for generating `.wav` audio files (or in-memory buffers) from mathematical functions.  
It uses **SFML** for playback support and allows you to synthesize tones and noise directly from user-defined wave functions.  

---

## ✨ Features  
- Generate `.wav` audio files from any mathematical function (`std::function<double(double)>`).  
- Write audio directly to memory (useful for real-time synthesis and playback).  
- Adjustable audio properties:
  - Sample rate (default: `44100 Hz`)
  - Bit depth (default: `16 bits`)
  - Channels (default: `1`, mono)
- Built-in **pitch constants** (`A3`, `A4`, `C5`, `A5`) for quick tone generation.  
- `noises` class wrapper:
  - Stores multiple functions as **sound buffers**.
  - Provides `play(index)` for quick tone playback.  

---

## 📦 Dependencies  
- **C++17 or later**  
- [SFML](https://www.sfml-dev.org/) (`Audio` module required)  

On Linux (Ubuntu/Debian-based):  
```bash
sudo apt install libsfml-dev
