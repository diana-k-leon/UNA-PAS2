# STFT Analyzer — Apuntes del Plugin

## ¿Qué hace este plugin?

Lo mismo que el SpectrumAnalyzer pero con dos diferencias:
1. **Aplica ventana Hann** antes de la FFT (espectro limpio)
2. **Guarda un historial** de espectros y los dibuja como espectrograma (waterfall)

En vez de barras que se mueven, ves una imagen 2D donde el eje X es frecuencia, el eje Y es tiempo, y el color es la energía.

---

## La estructura es la misma

```
STFTAnalyzer/
├── CMakeLists.txt
└── Source/
    ├── PluginProcessor.h   ← declaraciones del cerebro
    ├── PluginProcessor.cpp ← el cerebro (audio → ventana → FFT → historial)
    ├── PluginEditor.h      ← declaraciones de la cara
    └── PluginEditor.cpp    ← la cara (dibuja el espectrograma)
```

El Editor no lo tocamos — solo dibuja. Todo el DSP está en el Processor.

---

## ¿Qué cambió respecto al SpectrumAnalyzer?

| | SpectrumAnalyzer | STFT Analyzer |
|---|---|---|
| fft_size | 2048 | 1024 (más resolución temporal) |
| Ventana | ninguna | Hann |
| Resultado | 1 vector spectrum[] | historial de vectores (deque) |
| Visualización | barras que se mueven | espectrograma 2D |

---

## El Processor línea por línea

### Las constantes

```cpp
static constexpr int fft_size = 1024;
static constexpr int hop_size = 512;  // 50% overlap
```

- `fft_size = 1024`: más chico que el SpectrumAnalyzer (que usaba 2048). Menos resolución en frecuencia pero más resolución en tiempo — se actualiza más seguido.
- `hop_size = 512`: cuánto avanzamos entre un frame y el siguiente. Es la mitad de fft_size → 50% overlap. Cada sample aparece en dos frames distintos.

### El constructor — qué reservamos

```cpp
buffer_acumulador.resize(fft_size, 0.0f);
```
1024 posiciones — acá van los samples del DAW.

```cpp
fft_buffer.resize(2 * fft_size, 0.0f);
```
2048 posiciones — el doble, como necesita JUCE.

```cpp
window.resize(fft_size);
for (int n = 0; n < fft_size; ++n)
    window[n] = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * n / (fft_size - 1)));
```
Pre-calcula la ventana Hann. Se hace una sola vez en el constructor. Es la misma fórmula que vimos en los ejercicios de ventanas: `w[n] = 0.5 - 0.5 * cos(2π * n / (N-1))`.

```cpp
for (int i = 0; i < 512; ++i)
    spectrum_history.push_back(std::vector<float>(fft_size / 2 + 1, 0.0f));
```
Pre-llena el historial con 512 frames vacíos (todo negro). El historial es un `deque` — una cola donde podemos agregar al final y sacar del principio.

### processBlock — el corazón

```cpp
auto* input = buffer.getReadPointer(0);
int numSamples = buffer.getNumSamples();
```
Igual que antes: puntero al audio del DAW y cuántos samples hay.

```cpp
for (int n = 0; n < numSamples; ++n)
{
    buffer_acumulador[buffer_idx++] = input[n];
```
Igual que antes: acumular de a un sample.

```cpp
    if (buffer_idx >= fft_size)
    {
```
Cuando el acumulador tiene 1024 samples, disparamos la FFT.

#### Acá viene lo diferente:

```cpp
        for (int i = 0; i < fft_size; ++i)
        {
            float windowed = buffer_acumulador[i] * window[i];
            fft_buffer[2 * i] = windowed;
            fft_buffer[2 * i + 1] = 0.0f;
        }
```
**Dos cosas en un solo loop:**
1. Multiplica cada sample por la ventana Hann (`buffer_acumulador[i] * window[i]`)
2. Lo pone en formato intercalado para JUCE (posición par = real, impar = imaginario = 0)

**NOTA:** Acá el formato es diferente al SpectrumAnalyzer. En el SpectrumAnalyzer usamos `performRealOnlyForwardTransform` que espera la señal corrida. Acá usamos el formato intercalado. Ambos funcionan — son dos formas de usar la FFT de JUCE.

```cpp
        fft.performRealOnlyForwardTransform(fft_buffer.data());
```
FFT en una línea. Igual que antes.

```cpp
        std::vector<float> spectrum(fft_size / 2 + 1);
        for (int k = 0; k < fft_size / 2 + 1; ++k)
        {
            float re = fft_buffer[2 * k];
            float im = fft_buffer[2 * k + 1];
            spectrum[k] = std::sqrt(re * re + im * im);
        }
```
Extraer magnitudes — igual que antes. Crea un vector nuevo para este frame.

#### La parte nueva — el historial:

```cpp
        if (spectrum_history.size() >= 512)
            spectrum_history.pop_front();
        spectrum_history.push_back(spectrum);
```
- Si el historial ya tiene 512 frames, saca el más viejo (`pop_front`)
- Agrega el frame nuevo al final (`push_back`)
- Es como una cinta transportadora: lo nuevo entra por un lado, lo viejo sale por el otro

Esto es lo que el Editor lee para dibujar el espectrograma. Cada frame es una línea horizontal.

```cpp
        buffer_idx = 0;
```
Resetear y empezar de nuevo.

---

## Resumen: ¿qué hace el processBlock?

1. Acumula 1024 samples del DAW
2. Multiplica por ventana Hann (suaviza bordes)
3. Calcula FFT
4. Extrae magnitudes
5. Agrega al historial (máximo 512 frames)
6. Resetea y vuelve a empezar

El Editor lee el historial 30 veces por segundo y dibuja el espectrograma: cada frame es una línea, el color representa la energía.

---

## La diferencia clave con el SpectrumAnalyzer

El SpectrumAnalyzer guarda **un solo vector** de magnitudes — siempre se sobreescribe. Es una foto que se actualiza.

El STFT Analyzer guarda **512 vectores** en un historial. Es un video: ves cómo el espectro cambia en el tiempo.

Eso es la STFT: muchas FFTs con ventana, guardadas en secuencia.
