# Setup JUCE: Primer plugin (Gain + RMS)

## 🎯 Qué vamos a hacer

Compilar el primer plugin JUCE usando CMake. El plugin procesa audio en tiempo real dentro de un DAW (Reaper, Ableton, Logic, etc.) y muestra el RMS en pantalla.

---

## 🐧 Linux

### 1 · Instalar JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git ~/dev/JUCE
ls ~/dev/JUCE
```

### 2 · Clonar el proyecto del curso
```bash
git clone <repo-del-curso> ~/dev/pas2-plugin
cd ~/dev/pas2-plugin
```

### 3 · Configurar CMake
```bash
cmake -B build -DCMAKE_PREFIX_PATH=~/dev/JUCE
```
(Tarda 1–2 minutos la primera vez)

### 4 · Compilar
```bash
cmake --build build --config Release
```
(Primera compilación: 3–5 minutos)

### 5 · Localizar el plugin
```bash
find build -name "*.vst3" -type f
# típicamente: build/GainRMSPlugin_artefacts/Release/VST3/GainRMSPlugin.vst3
```

### 6 · Copiar a la carpeta de plugins
```bash
mkdir -p ~/.vst3
cp build/GainRMSPlugin_artefacts/Release/VST3/GainRMSPlugin.vst3 ~/.vst3/
```

---

## ⌘ macOS

### 1 · Instalar JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git ~/dev/JUCE
ls ~/dev/JUCE
```

### 2 · Clonar el proyecto del curso
```bash
git clone <repo-del-curso> ~/dev/pas2-plugin
cd ~/dev/pas2-plugin
```

### 3 · Configurar CMake
```bash
cmake -B build -DCMAKE_PREFIX_PATH=~/dev/JUCE
```

### 4 · Compilar
```bash
cmake --build build --config Release
```

### 5 · El plugin se copia automáticamente
JUCE + CMake copian el .vst3 a `~/Library/Audio/Plug-Ins/VST3/` automáticamente.

```bash
ls ~/Library/Audio/Plug-Ins/VST3/
```

---

## ⊞ Windows

### 1 · Instalar Visual Studio 2019+ (Community es gratis)
Si no lo tenés, descargá desde visualstudio.microsoft.com.

### 2 · Instalar JUCE
```bash
git clone https://github.com/juce-framework/JUCE.git C:\dev\JUCE
dir C:\dev\JUCE
```

### 3 · Clonar el proyecto del curso
```bash
git clone <repo-del-curso> C:\dev\pas2-plugin
cd C:\dev\pas2-plugin
```

### 4 · Configurar CMake
```bash
cmake -B build -DCMAKE_PREFIX_PATH=C:\dev\JUCE
```

### 5 · Compilar
```bash
cmake --build build --config Release
```

### 6 · Localizar el plugin
```bash
dir build\GainRMSPlugin_artefacts\Release\VST3\
```

### 7 · Copiar a la carpeta de plugins
```bash
mkdir "C:\Program Files\Common Files\VST3"
copy build\GainRMSPlugin_artefacts\Release\VST3\GainRMSPlugin.vst3 "C:\Program Files\Common Files\VST3\"
```

---

## 🎹 Cargar el plugin en tu DAW

### Reaper

1. Abrí Reaper
2. Insert → New Track
3. Haz clic en el panel FX → Add FX
4. Busca "Gain RMS"
5. Se abre una ventana con **RMS: X.XXXX** (fondo negro, texto blanco)
6. Cargá un audio y presioná Play — el número debería cambiar en tiempo real

**Si no ves el plugin:**
```
Options → Preferences → VST → Rescan Plugins
```

---

### Ableton Live (11+)

**Windows:**
1. Preferences → File Folder → Plugin Sources
2. Habilitá `C:\Program Files\Common Files\VST3\`
3. Re-scan Plugins

**macOS:**
1. Preferences → File Folder → Plugin Sources
2. Habilitá `~/Library/Audio/Plug-Ins/VST3/`
3. Re-scan Plugins

Luego:
1. Create new Audio Track
2. En el panel Device, arrastrá "Gain RMS"
3. Se abre la ventana del plugin
4. Cargá audio y presioná Play

**Si no ves el plugin:**
```
Preferences → Link/Tempo → Re-scan Plugins
```

---

### Logic Pro (macOS, 2024.1+)

Logic soporta VST3 desde la versión 2024.1.

1. System Settings → (si está disponible) VST Plugin Scanner
2. Abrí Logic Pro
3. Create new Audio Track
4. En el plugin selector, busca "Gain RMS"
5. Cargá audio y presioná Play

**Nota:** Si querés usar AU (más nativo en Logic), avisá — hay que cambiar CMakeLists.txt.

---

## 🔧 Qué hace el CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.22)
project(GainRMSPlugin VERSION 0.1)

# Incluir JUCE como dependencia
add_subdirectory(/ruta/a/JUCE ./JUCE)

# Definir el plugin
juce_add_plugin(GainRMSPlugin
    FORMATS VST3
    PLUGIN_NAME "Gain RMS Plugin"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Grm1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
)

# Qué archivos compilar
target_sources(GainRMSPlugin PRIVATE
    PluginProcessor.cpp
    PluginEditor.cpp
)

# Qué módulos de JUCE usar
target_link_libraries(GainRMSPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)
```

**Líneas importantes:**
- `FORMATS VST3` — tipo de plugin (VST3, AU, AAX, etc.)
- `add_subdirectory(...)` — ruta de JUCE (tiene que ser correcta)
- `target_sources(...)` — archivos .cpp que compilar
- `target_link_libraries(...)` — módulos de JUCE necesarios

---

## 💡 Qué está pasando en processBlock()

### Parte 1: Gain
```cpp
for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
    auto* data = buffer.getWritePointer(ch);
    for (int n = 0; n < buffer.getNumSamples(); ++n)
        data[n] *= gain;  // multiplicar por 0.5
}
```

**Dos loops anidados:**
- Loop externo: cada canal (estéreo = 2)
- Loop interno: cada sample (~512)
- Resultado: audio con mitad de volumen

Esto es **exactamente** lo que hicieron en Ejercicio 2. La única diferencia: ahora el buffer viene del DAW en vivo.

### Parte 2: RMS
```cpp
auto* data = buffer.getReadPointer(0);
float sum = 0.0f;
for (int n = 0; n < buffer.getNumSamples(); ++n)
    sum += data[n] * data[n];
rmsValue = std::sqrt(sum / buffer.getNumSamples());
```

**Tres pasos:**
1. Sumar los cuadrados
2. Dividir por N
3. Raíz cuadrada

Esto es **exactamente** lo que hicieron en Ejercicio 3. El resultado se guarda en `rmsValue` — el Editor lo lee ~30 veces por segundo y lo dibuja.

---

## ⚠️ Problemas comunes

### "Error: JUCE not found"
```bash
# Verificá que la ruta sea correcta
cmake -B build -DCMAKE_PREFIX_PATH=/ruta/correcta/a/JUCE

# Usá rutas absolutas sin espacios
# ❌ BAD: "C:\Program Files\JUCE"
# ✅ GOOD: "C:\dev\JUCE"
```

### "Errores de compilación"
```bash
# Verificá que el compilador esté instalado
# Linux: g++ --version
# macOS: clang --version
# Windows: "cl.exe /?" en Visual Studio Command Prompt

# Si falta algo, borrá y rehacé
rm -rf build
cmake -B build -DCMAKE_PREFIX_PATH=~/dev/JUCE
cmake --build build --config Release
```

### "El plugin no aparece en el DAW"
- Verificá que el .vst3 esté en la carpeta correcta (ver arriba)
- Reiniciá el DAW completamente
- Rescaneá plugins (instrucciones arriba por DAW)
- Verificá que tu DAW soporte VST3:
  - Reaper 6.82+
  - Ableton 11+
  - Logic 2024.1+

### "El RMS es 0 todo el tiempo"
- Verificá que hay audio llegando al track (mira el medidor)
- Probá con un archivo preexistente
- Verificá que el plugin esté activo en el FX chain

---

## 🎯 Flujo general

1. Usuario presiona **Play** en el DAW
2. DAW elige sample rate y tamaño de buffer
3. Plugin recibe `prepareToPlay()`
4. Cada frame, DAW llama `processBlock()` con audio real (~512 samples)
5. `processBlock()` aplica gain y calcula RMS
6. Timer dispara `timerCallback()` 30 veces/segundo
7. `paint()` lee `rmsValue` y lo dibuja
8. Vuelve a 4

**Todo sucede en paralelo:** Processor calcula, Timer pide redibujos, Editor pinta.

---

## ✅ Verificar que funciona

Una vez compilado y el plugin en el DAW:

1. Cargá audio en una pista
2. Insertá el plugin en el FX chain
3. Presioná Play
4. Debería ver **RMS: X.XXXX** en la ventana del plugin
5. El número debería cambiar en tiempo real con el audio

Si ves esto → **está funcionando**.

---

## 📚 Próximos pasos

El siguiente es **FFT + Spectrum Analyzer**. Usamos exactamente la misma estructura — solo agregamos:

```cpp
fft.performRealOnlyForwardTransform(fft_buffer.data());
```

Y dibujamos el espectro en pantalla.
