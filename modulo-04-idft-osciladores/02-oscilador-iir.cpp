// 02-oscilador-iir.cpp
// Genera 1 segundo de 440 Hz usando un oscilador IIR de 2° orden
// y lo exporta como WAV mono.
//
// Compilar:
//   g++ -std=c++17 -o 02-oscilador-iir 02-oscilador-iir.cpp
// Ejecutar:
//   ./02-oscilador-iir

#include <cmath>
#include <cstdint>
#include <fstream>
#include <vector>

// Escribe un archivo WAV mono 16-bit.
// No se preocupen por esta función — es infraestructura.
// Ya la vieron en PAS I. El foco está en el oscilador.
static void write_wav(const std::string& path,
                      const std::vector<float>& samples,
                      int sampleRate)
{
    int numSamples  = (int)samples.size();
    int byteRate    = sampleRate * 2;          // 16-bit mono
    int dataSize    = numSamples * 2;
    int chunkSize   = 36 + dataSize;

    std::ofstream f(path, std::ios::binary);

    auto w32 = [&](int v)   { f.write(reinterpret_cast<char*>(&v), 4); };
    auto w16 = [&](short v) { f.write(reinterpret_cast<char*>(&v), 2); };

    f.write("RIFF", 4);  w32(chunkSize);
    f.write("WAVE", 4);
    f.write("fmt ", 4);  w32(16);  w16(1);  w16(1);
    w32(sampleRate);     w32(byteRate);  w16(2);  w16(16);
    f.write("data", 4);  w32(dataSize);

    for (float s : samples) {
        short pcm = (short)(s * 32767.0f);
        f.write(reinterpret_cast<char*>(&pcm), 2);
    }
}
// Genera 1 segundo de 440 Hz usando el método recursivo.
// sin() se llama exactamente dos veces — solo para inicializar.
// El resto del loop no usa ninguna función trigonométrica.
int main()
{
    const int   sampleRate = 44100;
    const float freq       = 440.0f;
    const int   numSamples = sampleRate;       // 1 segundo

    const float omega = 2.0f * M_PI * freq / sampleRate;
    const float a     = 2.0f * std::cos(omega);

    // La fórmula necesita dos samples para arrancar.
    // Le damos los valores que habrían existido antes del sample 0.
    // Como el loop genera sin(0), sin(ω), sin(2ω)...
    // el paso anterior es sin(-ω) y dos pasos atrás sin(-2ω).
    float y1 = std::sin(-omega);               // y[n-1]
    float y2 = std::sin(-2.0f * omega);        // y[n-2]

    std::vector<float> buffer(numSamples);

    for (int n = 0; n < numSamples; n++) {
        // identidad trigonométrica: sin(nω) = 2cos(ω)·sin((n-1)ω) - sin((n-2)ω)
        float y = a * y1 - y2;              
        buffer[n] = y;
        y2 = y1; // correr ventana: y[n-2] ← y[n-1]
        y1 = y; // correr ventana: y[n-1] ← y[n]
    }
    
    // Exporta el buffer como WAV para escuchar el resultado.
    write_wav("02-oscilador-iir.wav", buffer, sampleRate);

    return 0;
}