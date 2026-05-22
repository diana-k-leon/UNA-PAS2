// ============================================================
// 01-espectro-rectangular.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 3 — El espectro de la ventana rectangular
// ============================================================
//
// Le hacemos FFT a la ventana rectangular SOLA (sin señal).
// Así vemos la forma que le impone a cualquier señal.
//
// Compilar:  g++ -std=c++17 01-espectro-rectangular.cpp dsp_utils.cpp -o ventana-rectangular
// Ejecutar:  ./ventana-rectangular
//
// Graficar:
// gnuplot -e "set datafile separator ','; set title 'Espectro de ventana Rectangular'; set ylabel 'dB'; set xlabel 'bins'; set yrange [-80:5]; plot 'ventana-rectangular.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'Rectangular'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>

// ─── ventana rectangular ──────────────────────────────────────────────────────
// w[n] = 1.0 para todo n
// Es "no hacer nada" — la señal entra tal cual.
std::vector<float> rectangular(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 1.0f;
    return w;
}

int main() {
    int M       = 63;     // tamaño de la ventana
    int fftSize = 512;    // zero-padding para más detalle visual

    std::cout << "=== Espectro de la ventana Rectangular ===" << std::endl;

    // calcular espectro de la ventana
    auto resultado = windowSpectrum(rectangular(M), fftSize);

    // mostrar mediciones
    std::cout << "  Lóbulo principal: " << resultado.mainLobeWidth << " bins" << std::endl;
    std::cout << "  Lóbulo lateral:   " << (int)std::round(resultado.highestSidelobe) << " dB" << std::endl;
    std::cout << std::endl;
    std::cout << "  → Angosto (buena resolución) pero laterales ALTOS (mucho leakage)" << std::endl;

    // exportar CSV
    exportarWindowSpectrum("ventana-rectangular.csv", "rectangular", resultado, M, fftSize);
    std::cout << "✓ Exportado a ventana-rectangular.csv" << std::endl;

    return 0;
}
