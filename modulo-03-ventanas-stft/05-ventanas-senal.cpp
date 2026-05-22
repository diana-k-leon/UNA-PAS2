// ============================================================
// 05-ventanas-senal.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 3 — Ventanas aplicadas a una sinusoide real
// ============================================================
//
// Ejercicio anterior: vimos la FFT de cada ventana SOLA.
// Este ejercicio: vemos qué pasa cuando USAMOS esas ventanas
// para analizar una sinusoide de 440 Hz.
//
// Lo que vas a ver: el pico de 440 Hz con la "forma" de cada
// ventana pegada alrededor. Rectangular tiene pico angosto
// pero mucho ruido. Blackman tiene pico gordo pero limpio.
//
// Compilar:  g++ -std=c++17 05-ventanas-senal.cpp dsp_utils.cpp -o ventanas-senal
// Ejecutar:  ./ventanas-senal
//
// Graficar (zoom en 440 Hz):
// gnuplot -e "set datafile separator ','; set title 'Sinusoide 440 Hz con distintas ventanas'; set xrange [0:2000]; plot 'ventanas-senal.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', 'ventanas-senal.csv' using 2:4 with lines lc rgb '#0000FF' lw 2 title 'Hamming', 'ventanas-senal.csv' using 2:5 with lines lc rgb '#00AA00' lw 2 title 'Hann', 'ventanas-senal.csv' using 2:6 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
//
// Probá con f0 = 441.0 para ver el efecto del leakage.
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>

// ─── ventanas ─────────────────────────────────────────────────────────────────

std::vector<float> hamming(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.54f - 0.46f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

std::vector<float> hann(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.5f - 0.5f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

std::vector<float> blackman(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++) {
        float t = (float)n / (N - 1);
        w[n] = 0.42f - 0.5f * std::cos(2.0f * PI * t)
                      + 0.08f * std::cos(4.0f * PI * t);
    }
    return w;
}

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────
    int N            = 1024;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;      // probá con 441.0 para ver leakage

    // ─── generar señal ────────────────────────────────────────────────────
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "=== Sinusoide " << f0 << " Hz con distintas ventanas ===" << std::endl;

    // ─── calcular espectros ───────────────────────────────────────────────
    auto sinVentana = magnitudes(signal);
    auto conHamming = magnitudes(aplicarVentana(signal, hamming(N)));
    auto conHann    = magnitudes(aplicarVentana(signal, hann(N)));
    auto conBlackman = magnitudes(aplicarVentana(signal, blackman(N)));

    // ─── exportar CSV ─────────────────────────────────────────────────────
    std::ofstream file("ventanas-senal.csv");
    file << "bin,frecuencia_hz,sin_ventana,hamming,hann,blackman" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file << k << ","
             << freq << ","
             << sinVentana[k] << ","
             << conHamming[k] << ","
             << conHann[k] << ","
             << conBlackman[k] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a ventanas-senal.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Tip: cambiá f0 a 441.0 Hz para ver el efecto del leakage" << std::endl;

    return 0;
}
