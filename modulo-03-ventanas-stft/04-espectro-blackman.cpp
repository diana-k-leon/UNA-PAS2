// ============================================================
// 04-espectro-blackman.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 3 — El espectro de la ventana Blackman
// ============================================================
//
// Compilar:  g++ -std=c++17 04-espectro-blackman.cpp dsp_utils.cpp -o ventana-blackman
// Ejecutar:  ./ventana-blackman
//
// Graficar (solo Blackman):
// gnuplot -e "set datafile separator ','; set title 'Espectro de ventana Blackman'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-100:5]; plot 'ventana-blackman.csv' using 1:2 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
//
// Graficar (las 4 juntas):
// gnuplot -e "set datafile separator ','; set title 'Rectangular vs Hamming vs Hann vs Blackman'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-100:5]; plot 'ventana-rectangular.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', 'ventana-hamming.csv' using 1:2 with lines lc rgb '#0000FF' lw 2 title 'Hamming', 'ventana-hann.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'Hann', 'ventana-blackman.csv' using 1:2 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>

// ─── ventana Blackman ─────────────────────────────────────────────────────────
// w[n] = 0.42 - 0.5 * cos(2π * n / (N-1)) + 0.08 * cos(4π * n / (N-1))
// Tres cosenos sumados. Lóbulos laterales muy bajos, pero lóbulo principal ancho.
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
    int M       = 63;
    int fftSize = 512;

    std::cout << "=== Espectro de la ventana Blackman ===" << std::endl;

    auto resultado = windowSpectrum(blackman(M), fftSize);

    std::cout << "  Lóbulo principal: " << resultado.mainLobeWidth << " bins" << std::endl;
    std::cout << "  Lóbulo lateral:   " << (int)std::round(resultado.highestSidelobe) << " dB" << std::endl;
    std::cout << std::endl;
    std::cout << "  → El más ancho (peor resolución)" << std::endl;
    std::cout << "  → Pero laterales MUY bajos (casi sin leakage)" << std::endl;
    std::cout << "  → Ideal cuando hay señales débiles cerca de señales fuertes" << std::endl;

    exportarWindowSpectrum("ventana-blackman.csv", "blackman", resultado, M, fftSize);
    std::cout << "✓ Exportado a ventana-blackman.csv" << std::endl;

    return 0;
}
