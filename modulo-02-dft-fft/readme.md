# Módulo 2 · DFT y FFT

## Objetivos

Implementar la Transformada Discreta de Fourier desde cero y entender cómo pasar del dominio del tiempo al dominio de la frecuencia. Al terminar este módulo podés analizar el contenido espectral de cualquier señal de audio y entender por qué la FFT es la herramienta central del procesamiento de señales.

---

## Ejercicios

### sinusoide_real.cpp
Genera una senoidal real y exporta sus valores a CSV. Punto de partida para entender cómo se ve una señal periódica en el dominio del tiempo.

**Parámetros para experimentar:**
- `f0` — frecuencia en Hz
- `N` — cantidad de muestras
- `sampleRate` — frecuencia de muestreo

---

### sinusoide_compleja.cpp
Genera una sinusoide compleja usando la fórmula de Euler — parte real (coseno) y parte imaginaria (seno). Base matemática de la DFT.

**Para pensar:**
- ¿Qué representa la parte real? ¿Y la imaginaria?
- ¿Qué pasa cuando k = 0?

---

### dft_basica.cpp
Implementación ingenua de la DFT — O(N²). Calcula el espectro completo de una señal y exporta magnitudes a CSV.

**Parámetros para experimentar:**
- Cambiar `f0` — ¿en qué bin aparece el pico?
- Cambiar `k` — ¿cuántos ciclos en N muestras?
- Cambiar `N` — ¿qué pasa con la magnitud?

---

### dft_real.cpp
DFT optimizada para señales reales — aprovecha la simetría del espectro para calcular solo la mitad útil (hasta Nyquist).

**Para pensar:**
- ¿Por qué el espectro es simétrico para señales reales?
- ¿Cuántos bins útiles tenés con N=1024?

---

### dft_centrada.cpp
DFT centrada en cero — mueve el componente DC al centro del espectro. Es la forma de visualización más común en análisis espectral.

---

### fft_cooley_tukey.cpp
Implementación del algoritmo FFT Radix-2 de Cooley-Tukey — O(N log N). Mismo resultado que la DFT pero notablemente más rápido.

**Para pensar:**
- Medí el tiempo con N=64, 256, 1024 — ¿cuánto más rápida es la FFT?
- ¿Por qué N tiene que ser potencia de 2?

---

## Compilar y correr

```bash
# compilar
g++ -std=c++17 dft_basica.cpp -o dft_basica

# correr
./dft_basica
```

## Visualizar el CSV

Abrí el archivo `.csv` generado en Excel o Google Sheets. El eje X son los bins de frecuencia, el eje Y es la magnitud.

---

## Conceptos clave

**Bin de frecuencia** — cada elemento del espectro corresponde a una frecuencia:
```
frecuencia = k × (sampleRate / N)
```

**Nyquist** — la frecuencia máxima representable es `sampleRate / 2`. La segunda mitad del espectro es un espejo de la primera.

**N potencia de 2** — la FFT Cooley-Tukey divide el problema a la mitad en cada paso. Solo funciona limpio con N = 256, 512, 1024, 2048...

**Complejidad computacional:**

| Algoritmo | Operaciones (N=1024) |
|-----------|---------------------|
| DFT       | N² = 1.048.576      |
| FFT       | N·log₂N = 10.240    |
