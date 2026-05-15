// ============================================================
// 02-0-gain.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 1 — Señales digitales y C++ para DSP
// ============================================================
//
// Ganancia (gain) es el procesamiento más simple en audio:
// multiplicar cada sample por un número.
//
// Si gain = 0.5  → la onda baja a la mitad de amplitud (MÁS SILENCIOSA)
// Si gain = 2.0  → la onda sube al doble (MÁS FUERTE)
//
// Compilar:  g++ -std=c++17  02-0-gain.cpp -o aplicar-gain-1
// Ejecutar:  ./aplicar-gain-1
// Graficar:  gnuplot -e "set datafile separator ','; plot 'gain.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'original', 'gain.csv' using 1:3 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    // parámetros
    int N            = 64;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;
    float gain       = 1.5f;  // probá: 0.25, 2.0, 0.1, etc.

    std::cout << "=== Aplicar Gain ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  f0 = " << f0 << " Hz (LA)" << std::endl;
    std::cout << "  sampleRate = " << sampleRate << " Hz" << std::endl;
    std::cout << "  gain = " << gain << std::endl;
    std::cout << std::endl;

    // crear señal original
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        signal[n] = std::sin(2.0f * M_PI * f0 * n / sampleRate);
    }

    // aplicar gain
    std::vector<float> signalConGain(N);
    for (int n = 0; n < N; n++) {
        signalConGain[n] = signal[n] * gain;
    }

    // mostrar primeras muestras para verificar
    std::cout << "Primeras 5 muestras:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    for (int n = 0; n < 5; n++) {
        std::cout << "  n=" << n 
                  << " | original=" << signal[n] 
                  << " | con_gain=" << signalConGain[n] 
                  << " | ratio=" << (signalConGain[n] / signal[n]) 
                  << std::endl;
    }
    std::cout << std::endl;

    // exportar CSV con las dos señales
    std::ofstream file("gain.csv");
    file << "n,original,con_gain" << std::endl;
    for (int n = 0; n < N; n++) {
        file << n << "," << signal[n] << "," << signalConGain[n] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a gain.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Para graficar (VERDE = original, ROJO = con_gain):" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; plot 'gain.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'original', 'gain.csv' using 1:3 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1\"" << std::endl;

    return 0;
}