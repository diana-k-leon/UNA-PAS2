// ============================================================
// 02-espectro-hammingg.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 3 — El espectro de la ventana Hamming
// ============================================================
//
// Le hacemos FFT a la ventana Hamming SOLA (sin señal).
//
// Compilar:  g++ -std=c++17 02-espectro-hamming.cpp dsp_utils.cpp -o ventana-hamming
// Ejecutar:  ./ventana-hamming
//
// Graficar (solo Hamming):
// gnuplot -e "set datafile separator ','; set title 'Espectro de ventana Hamming'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-80:5]; plot 'ventana-hamming.csv' using 1:2 with lines lc rgb '#0000FF' lw 2 title 'Hamming'; pause -1"
//
// Graficar (Hamming vs Rectangular):
// gnuplot -e "set datafile separator ','; set title 'Rectangular vs Hamming'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-80:5]; plot 'ventana-rectangular.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', 'ventana-hamming.csv' using 1:2 with lines lc rgb '#0000FF' lw 2 title 'Hamming'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>

// ─── ventana Hamming ──────────────────────────────────────────────────────────
// w[n] = 0.54 - 0.46 * cos(2π * n / (N-1))
// Parecida a Hann pero NO cae exactamente a cero en los bordes.
// Los bordes quedan en ~0.08 en vez de 0.
std::vector<float> hamming(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.54f - 0.46f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

int main() {
    int M       = 63;
    int fftSize = 512;

    std::cout << "=== Espectro de la ventana Hamming ===" << std::endl;

    auto resultado = windowSpectrum(hamming(M), fftSize);

    std::cout << "  Lóbulo principal: " << resultado.mainLobeWidth << " bins" << std::endl;
    std::cout << "  Lóbulo lateral:   " << (int)std::round(resultado.highestSidelobe) << " dB" << std::endl;
    std::cout << std::endl;
    std::cout << "  → Mismo ancho que Hann (4 bins)" << std::endl;
    std::cout << "  → Pero laterales AÚN más bajos que Hann (-42 vs -31 dB)" << std::endl;
    std::cout << "  → La más usada en audio" << std::endl;

    exportarWindowSpectrum("ventana-hamming.csv", "hamming", resultado, M, fftSize);
    std::cout << "✓ Exportado a ventana-hamming.csv" << std::endl;

    return 0;
}
