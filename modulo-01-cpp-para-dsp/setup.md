# Setup del entorno

## 🐧 Linux

### 1 · Terminal
Usá la terminal del sistema — viene instalada por defecto en cualquier distribución.

### 2 · VSCode
Descargá desde code.visualstudio.com e instalá el paquete `.deb` o `.rpm` según tu distribución.

Extensiones recomendadas:
- C/C++ (Microsoft)
- CMake Tools

### 3 · Compilador
```bash
# Ubuntu/Debian
sudo apt install build-essential

# Fedora
sudo dnf install gcc-c++
```

### 4 · CMake
```bash
# Ubuntu/Debian
sudo apt install cmake

# Fedora
sudo dnf install cmake

# verificar
cmake --version
```

### 5 · Dependencias JUCE
```bash
sudo apt install libasound2-dev libfreetype6-dev libx11-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev
```

### 6 · JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git ~/dev/JUCE
ls ~/dev/JUCE
```

### 7 · Estructura de carpetas
```bash
mkdir -p ~/dev/UNA-PAS2/TemplatePlugin
cd ~/dev/UNA-PAS2/TemplatePlugin
```

---

## ⌘ macOS

### 1 · iTerm2
Reemplazo de la Terminal de Mac con mejores colores, split de paneles y autocompletado.

Descarga: iterm2.com

### 2 · VSCode
Descargá desde code.visualstudio.com.

Extensiones recomendadas:
- C/C++ (Microsoft)
- CMake Tools

### 3 · Xcode Command Line Tools
```bash
xcode-select --install
# si ya están instaladas: "Command line tools are already installed"
```

### 4 · Homebrew
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# verificar
brew --version
```

### 5 · CMake
```bash
brew install cmake

# verificar
cmake --version
```

### 6 · JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git ~/dev/JUCE
ls ~/dev/JUCE
```

### 7 · Estructura de carpetas
```bash
mkdir -p ~/dev/UNA-PAS2/TemplatePlugin
cd ~/dev/UNA-PAS2/TemplatePlugin
```

### 8 · Oh My Zsh (opcional)
Personaliza el shell con colores, branch de git y prompt con símbolos.
```bash
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
```

---

## ⊞ Windows

### 1 · Windows Terminal
Descargá desde la Microsoft Store — buscá "Windows Terminal".

### 2 · VSCode
Descargá desde code.visualstudio.com.

Extensiones recomendadas:
- C/C++ (Microsoft)
- CMake Tools


### 3 · CMake
Descargá desde cmake.org/download.

Durante la instalación seleccioná: **"Add CMake to the system PATH"**

```bash
# verificar en Windows Terminal
cmake --version
```

### 4 · JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git C:\dev\JUCE
```

---

## Verificar que todo funciona

Una vez instalado todo, verificá que podés compilar un archivo C++ simple:

```cpp
// hola.cpp
#include <iostream>
int main() {
    std::cout << "Entorno funcionando" << std::endl;
    return 0;
}
```

```bash
# Mac / Linux
g++ -std=c++17 hola.cpp -o hola
./hola

# Windows
g++ -std=c++17 hola.cpp -o hola.exe
hola.exe
```

Si ves `Entorno funcionando` — está funcionando.
