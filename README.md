# ReverseEdge

> **Internal educational cheat for AssaultCube 1.3.0.2**  
> A small personal project to learn reverse engineering, hooking and memory manipulation — meant **only** for controlled, offline testing.

---

## 🎯 What is ReverseEdge?

ReverseEdge is a compact prototype demonstrating how simple game-assist features (ESP, a basic aimbot, bunnyhop) can be implemented for learning purposes.  

> **Do not use this against other players or on public servers.** Test only in safe, authorized environments (VMs, private servers, or with explicit permission).

---

## 🔥 Demo

```
[![Watch the demo](https://vimeo.com/1128569502)
*Check the link for the demo on vimeo*
```



---

## ✅ Features

- ESP — boxes and lines (onscreen overlay)
- Minimal aimbot with configurable FOV and smoothing
- Bunny hop automation
- Learning-oriented, clearly commented code paths

---

## 🛠 Requirements & Build

- Visual Studio (or equivalent C++ toolchain)
- Target **x86** (32-bit) — **mandatory**
- Configuration: `Release` or `Debug` (Debug produces a `.pdb` alongside the `.dll`)

### Quick start

```bash
# Clone the repository
git clone https://github.com/Bourdon94m/ReverseEdge.git

# Open the solution in Visual Studio and build (Platform: x86)
# Ensure Configuration Manager is set to x86 and your chosen build (Debug/Release)
```

⚠️ AssaultCube 1.3.0.2 is a 32-bit program — compiling as x64 will break injection and crashes will occur.

---

## 🧪 Safe Test / Installation (recommended workflow)

1. Create a clean Windows VM dedicated to testing.
2. Install AssaultCube 1.3.0.2 inside that VM.
3. Build `ReverseEdge.dll` (x86).
4. Use an injector **only** within the VM for local testing.
5. Debug and iterate while keeping the host machine isolated from any real accounts.

---

## ⚖️ Educational Purposes Only — Disclaimer

**Educational purposes only.** ReverseEdge is published solely to help learners understand reverse engineering concepts and low-level programming techniques. By using this project you agree to the following:

- You will run and test this code only in controlled environments you own or where you have explicit permission.
- You will not use this code to cheat on public servers, violate terms of service, harm others, or commit illegal acts.
- The author assumes **no responsibility** for misuse, damages, bans, legal liability, or other consequences resulting from how you use this code.

## 👤 Author

- **Bourdon94m** — https://github.com/Bourdon94m

---


