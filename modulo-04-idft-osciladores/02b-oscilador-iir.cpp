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
int main()
{
    const int   sampleRate = 44100;
    const int   numSamples = sampleRate;  // 1 segundo

    // tres frecuencias del acorde Do-Mi-Sol
    const float freqs[3] = { 261.0f, 330.0f, 392.0f };

    std::vector<float> buffer(numSamples, 0.0f);

    for (int i = 0; i < 3; i++)
    {
        float omega = 2.0f * M_PI * freqs[i] / sampleRate;
        float a     = 2.0f * std::cos(omega);
        float y1    = std::sin(-omega);
        float y2    = std::sin(-2.0f * omega);

        for (int n = 0; n < numSamples; n++)
        {
            float y  = a * y1 - y2;
            // (y1/3) + (y2/3) + (y3/3) = (y1 + y2 + y3) / 3
            // buffer[n] += y / 3.0f;
            buffer[n] += y; 
            y2 = y1;
            y1 = y;
        }
    }
    for (int n = 0; n < numSamples; n++)
        buffer[n] /= 3.0f;
    
    write_wav("02-oscilador-iir.wav", buffer, sampleRate);
    return 0;
}
/*
En la práctica las tres ondas no llegan a su máximo al mismo tiempo — Do, Mi y Sol están desfasadas. Entonces el máximo real de la suma es menor que 3. Pero en el peor caso teórico sí podría llegar a 3, por eso dividís preventivamente.
El clipping es cuando la onda "choca" contra el límite del rango y se aplana — en vez de una curva suave se ve una línea recta en los picos. Eso genera armónicos no deseados y suena distorsionado, como un amplificador saturado.
En audio profesional esto se evita con normalización — escalar la señal entera al máximo posible sin clipear. Dividir por 3 es una versión simplificada de eso.
*/