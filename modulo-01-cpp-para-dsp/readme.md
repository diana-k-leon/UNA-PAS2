# Módulo 1 · C++ para DSP

## Objetivos

Repasar C++ moderno orientado a procesamiento de señales de audio. Al terminar este módulo podés crear señales digitales, manipularlas y medir sus propiedades — las mismas operaciones que van a vivir adentro de un plugin VST.

---

## Ejercicios

### 01 · Crear una señal y explorar sus valores
`01_crear_senal.cpp`

Genera una senoidal de 440 Hz con N muestras y exporta los valores a CSV para visualizar.

**Parámetros para experimentar:**
- `N` — cantidad de muestras
- `f0` — frecuencia de la senoidal en Hz
- `sampleRate` — frecuencia de muestreo

---

### 02 · Aplicar un gain
`02_gain.cpp`

Aplica un factor de escala a cada muestra de la señal y exporta la señal original y la modificada en el mismo CSV para comparar.

**Parámetros para experimentar:**
- `gain` — probá con 0.25, 0.5, 1.0, 2.0
- ¿Qué pasa si el gain es mayor a 1.0?
- ¿Qué pasa si es negativo?

---

### 03 · Calcular RMS
`03_rms.cpp`

Calcula el Root Mean Square de la señal — la medida estándar de nivel en audio.

**Para pensar:**
- Una senoidal pura tiene RMS = 1/√2 ≈ 0.707
- ¿Qué pasa con el RMS si cambiás el gain del ejercicio anterior?
- ¿Qué pasa si sumás dos sinusoides?

---

## Compilar y correr

```bash
# compilar un ejercicio
g++ -std=c++17 01_crear_senal.cpp -o crear_senal

# correr
./crear_senal
```

## Visualizar el CSV

Abrí el archivo `.csv` generado en Excel, Google Sheets, o cualquier herramienta de graficado. En la próxima versión de estos ejercicios vamos a visualizar directo desde C++ con matplotlib-cpp.

---

## Conceptos clave

**`std::vector<float>`** — la señal es una lista de números. Cada elemento es el valor de amplitud en un instante de tiempo.

**`for (auto& sample : signal)`** — loop sobre samples para modificar. El `&` es importante — sin él trabajás sobre una copia.

**`for (const auto& sample : signal)`** — loop sobre samples para leer sin modificar.

**RMS** — Root Mean Square. Mide el nivel promedio de una señal. Es la raíz cuadrada del promedio de los cuadrados de las muestras.
