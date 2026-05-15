# UNA-PAS2 · Unidad 1 — Análisis Espectral

**Programación Aplicada a Sonido II**  
Especialización en Sonido Aplicado a las Artes Digitales  
Artes Multimediales · Universidad Nacional de las Artes

---

## Sobre la materia

Programación Aplicada a Sonido II es una materia de la especialización en Sonido Aplicado a las Artes Digitales de la UNA. El enfoque es práctico — los estudiantes implementan en C++ moderno los algoritmos de procesamiento digital de señales de audio cubiertos en la parte teórica, y los integran en plugins VST3 funcionales que corren en DAWs como Ableton y Reaper.

---

## Contenido de este repositorio

Implementaciones en C++ moderno de los algoritmos de análisis espectral cubiertos en la Unidad 1.

### Módulo 1 · C++ para DSP
Repaso de C++ orientado a procesamiento de señales de audio — señales como vectores, tipos numéricos, loops sobre samples y templates.

### Módulo 2 · DFT y FFT
Implementación de la Transformada Discreta de Fourier desde cero, incluyendo las variantes básica, real, centrada y con límite de Nyquist. Algoritmo de Cooley-Tukey (FFT Radix-2) y comparación de tiempos contra la DFT ingenua.

### Módulo 3 · Ventanas y STFT
Funciones de suavizado — Hamming, Hann, Bartlett y Blackman — y análisis espectral por ventana deslizante (Short-Time Fourier Transform).

---

## Requisitos

- C++17 o superior
- CMake 3.22+

## Uso

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Cursada

**Profesoras:** Diana León · Sol Pereyra  
**Carrera:** Artes Multimediales · UNA  
**Año:** 2026
