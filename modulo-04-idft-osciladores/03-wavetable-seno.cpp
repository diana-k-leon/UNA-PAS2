// ============================================================
// 02-wavetable-seno.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 4 — Wavetable: sinusoide
// ============================================================
//
// Generar 440 Hz de dos formas:
//   1. Wavetable: pre-calcular un ciclo, recorrerlo
//   2. Cálculo directo: sin() para cada sample
// Exportar ambas y verificar que son (casi) iguales.
//
// Compilar:  g++ -std=c++17 03-wavetable-seno.cpp dsp_utils.cpp -o wavetable-seno
// Ejecutar:  ./wavetable-seno
//
// Graficar:
// gnuplot -e "set datafile separator ','; set title 'Wavetable vs Directo'; set xrange [0:200]; plot 'wavetable-seno.csv' using 1:2 with lines lw 2 title 'wavetable', 'wavetable-seno.csv' using 1:3 with lines lw 1 title 'directo'; pause -1"
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
    int N            = 44100;       // 1 segundo

    std::cout << "=== Wavetable vs Cálculo directo ===" << std::endl;

    // ─── paso 1: llenar la tabla con un ciclo de sinusoide ────────────────
    std::vector<float> table(tableSize);
    for (int i = 0; i < tableSize; i++)
        table[i] = std::sin(2.0f * PI * i / tableSize);

    std::cout << "✓ Tabla creada: " << tableSize << " samples, un ciclo de sinusoide" << std::endl;

    // ─── paso 2: generar señal recorriendo la tabla ───────────────────────
    std::vector<float> wavetableSignal(N);
    float phase = 0.0f;
    float increment = f0 * tableSize / sampleRate;
    // increment = cuánto avanza el índice por sample
    // 440 * 1024 / 44100 = ~10.22 posiciones por sample

    for (int n = 0; n < N; n++) {
        int idx = (int)phase;
        wavetableSignal[n] = table[idx % tableSize];
        phase += increment;
    }

    std::cout << "✓ Señal wavetable generada: " << f0 << " Hz, " << N << " samples" << std::endl;
    std::cout << "  Increment: " << increment << " posiciones por sample" << std::endl;

    // ─── paso 3: generar la misma señal con cálculo directo ───────────────
    std::vector<float> directSignal(N);
    for (int n = 0; n < N; n++)
        directSignal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "✓ Señal directa generada: " << f0 << " Hz, " << N << " samples" << std::endl;

    // ─── paso 4: comparar ─────────────────────────────────────────────────
    /*  el error no es aleatorio, tiene un patrón. 
        Es mayor cuando phase tiene mucha parte decimal (cerca de .5) y 
        menor cuando está cerca de un entero. Eso es exactamente lo que resuelve la interpolación lineal.
    */
    float maxError = 0.0f;
    for (int n = 0; n < N; n++) {
        float error = std::abs(wavetableSignal[n] - directSignal[n]);
        maxError = std::max(maxError, error);
    }

    std::cout << "  Error máximo: " << maxError << std::endl;
    std::cout << "  (debería ser pequeño — la diferencia es por truncar el índice)" << std::endl;

    // ─── paso 5: exportar ─────────────────────────────────────────────────
    std::ofstream file("wavetable-seno.csv");
    file << "sample,wavetable,directo" << std::endl;
    for (int n = 0; n < N; n++)
        file << n << "," << wavetableSignal[n] << "," << directSignal[n] << std::endl;
    file.close();

    std::cout << "✓ Exportado a wavetable-seno.csv" << std::endl;

    return 0;
}
