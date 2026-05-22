// ============================================================
// 03-espectro-hann.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 3 — El espectro de la ventana Hann
// ============================================================
//
// Le hacemos FFT a la ventana Hann SOLA (sin señal).
// Comparar con Rectangular: lóbulo principal más ancho
// pero laterales mucho más bajos.
//
// Compilar:  g++ -std=c++17 03-espectro-hann.cpp dsp_utils.cpp -o ventana-hann
// Ejecutar:  ./ventana-hann
//
// Graficar (solo Hann):
// gnuplot -e "set datafile separator ','; set title 'Espectro de ventana Hann'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-80:5]; plot 'ventana-hann.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'Hann'; pause -1"
//
// Graficar (Hann vs Rectangular):
// gnuplot -e "set datafile separator ','; set title 'Rectangular vs Hann'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-80:5]; plot 'ventana-rectangular.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', 'ventana-hann.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'Hann'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>

// ─── ventana Hann ─────────────────────────────────────────────────────────────
// w[n] = 0.5 - 0.5 * cos(2π * n / (N-1))
// Coseno elevado. Cae exactamente a cero en los bordes.
std::vector<float> hann(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.5f - 0.5f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

int main() {
    int M       = 63;
    int fftSize = 512;

    std::cout << "=== Espectro de la ventana Hann ===" << std::endl;

    auto resultado = windowSpectrum(hann(M), fftSize);

    std::cout << "  Lóbulo principal: " << resultado.mainLobeWidth << " bins" << std::endl;
    std::cout << "  Lóbulo lateral:   " << (int)std::round(resultado.highestSidelobe) << " dB" << std::endl;
    std::cout << std::endl;
    std::cout << "  → Más ancho que Rectangular (peor resolución)" << std::endl;
    std::cout << "  → Pero laterales MUCHO más bajos (menos leakage)" << std::endl;

    exportarWindowSpectrum("ventana-hann.csv", "hann", resultado, M, fftSize);
    std::cout << "✓ Exportado a ventana-hann.csv" << std::endl;

    return 0;
}
