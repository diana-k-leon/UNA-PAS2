// ============================================================
// 03-wavetable-sierra.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 4 — Wavetable: diente de sierra vs sinusoide
// ============================================================
//
// Generar 440 Hz con dos tablas distintas:
//   1. Sinusoide (tono puro)
//   2. Diente de sierra (todos los armónicos)
// Calcular FFT de ambas y comparar espectros.
//
// Compilar:  g++ -std=c++17 04-wavetable-sierra.cpp dsp_utils.cpp -o wavetable-sierra
// Ejecutar:  ./wavetable-sierra
//
// Graficar señales:
// gnuplot -e "set datafile separator ','; set title 'Sinusoide vs Sierra (tiempo)'; set xrange [0:200]; plot 'wavetable-sierra.csv' using 1:2 with lines lw 2 title 'sinusoide', 'wavetable-sierra.csv' using 1:3 with lines lw 2 title 'sierra'; pause -1"
//
// Graficar espectros:
// gnuplot -e "set datafile separator ','; set title 'Espectro: sinusoide vs sierra'; set xrange [0:5000]; plot 'wavetable-sierra-espectro.csv' using 1:2 with lines lw 2 title 'sinusoide', 'wavetable-sierra-espectro.csv' using 1:3 with lines lw 2 title 'sierra'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────
    int tableSize    = 1024;
    float f0         = 440.0f;
    float sampleRate = 44100.0f;
    int N            = 2048;        // potencia de 2 para FFT

    std::cout << "=== Wavetable: sinusoide vs diente de sierra ===" << std::endl;

    // ─── tabla 1: sinusoide ───────────────────────────────────────────────
    std::vector<float> tableSeno(tableSize);
    for (int i = 0; i < tableSize; i++)
        tableSeno[i] = std::sin(2.0f * PI * i / tableSize);

    // ─── tabla 2: diente de sierra ────────────────────────────────────────
    /*
        i / tableSize va de 0 a 1 mientras recorre la tabla.
        2.0f * i / tableSize va de 0 a 2.
        2.0f * i / tableSize - 1.0f va de -1 a +1.
        O sea es una línea recta que empieza en -1, sube hasta +1, 
        y cuando termina la tabla vuelve a empezar desde -1. Eso es exactamente la forma de onda diente de sierra.
    */
    std::vector<float> tableSierra(tableSize);
    for (int i = 0; i < tableSize; i++)
        tableSierra[i] = 2.0f * i / tableSize - 1.0f;  // rampa de -1 a +1

    std::cout << "✓ Tablas creadas: sinusoide y diente de sierra" << std::endl;

    // ─── generar señales ──────────────────────────────────────────────────
    std::vector<float> signalSeno(N);
    std::vector<float> signalSierra(N);

    float phase = 0.0f;
    float increment = f0 * tableSize / sampleRate;

    for (int n = 0; n < N; n++) {
        int idx = (int)phase % tableSize;
        signalSeno[n] = tableSeno[idx];
        signalSierra[n] = tableSierra[idx];
        phase += increment;
    }

    std::cout << "✓ Señales generadas: " << f0 << " Hz, " << N << " samples" << std::endl;

    // ─── calcular espectros ───────────────────────────────────────────────
    auto magSeno = magnitudes(signalSeno);
    auto magSierra = magnitudes(signalSierra);

    std::cout << "✓ FFT calculada" << std::endl;

    // ─── exportar señales ─────────────────────────────────────────────────
    std::ofstream file1("wavetable-sierra.csv");
    file1 << "sample,sinusoide,sierra" << std::endl;
    for (int n = 0; n < N; n++)
        file1 << n << "," << signalSeno[n] << "," << signalSierra[n] << std::endl;
    file1.close();

    // ─── exportar espectros ───────────────────────────────────────────────
    std::ofstream file2("wavetable-sierra-espectro.csv");
    file2 << "frecuencia_hz,sinusoide,sierra" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file2 << freq << "," << magSeno[k] << "," << magSierra[k] << std::endl;
    }
    file2.close();

    std::cout << "✓ Exportado a wavetable-sierra.csv y wavetable-sierra-espectro.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Qué esperar en el espectro:" << std::endl;
    std::cout << "- Sinusoide: un solo pico en 440 Hz" << std::endl;
    std::cout << "- Sierra: picos en 440, 880, 1320, 1760... (todos los armónicos)" << std::endl;
    std::cout << "  Eso es lo que la hace sonar más brillante." << std::endl;

    return 0;
}
