// ============================================================
// 04-windows-nonperiodic.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2 — Ventanas: tono no periódico y su limpieza
// ============================================================
//
// Objetivo: Ver el efecto DRAMÁTICO de las ventanas cuando
// tu señal NO cabe exactamente en N muestras.
//
// Caso de estudio:
// - Tono de 441 Hz con N=1024 muestras a 44100 Hz
// - 441 Hz no es una frecuencia "natural" en la grilla DFT
//   (grilla: 0, 43, 86, 129, 172, 215, ... Hz)
// - Sin ventana: espectro lleno de ruido (leakage)
// - Con ventana: leakage suprimido, pico limpio
//
// Este ejercicio demuestra por qué SIEMPRE usás ventanas
// en análisis espectral de señales reales.
//
// Compilar:  g++ -std=c++17 04-windows-nonperiodic.cpp -o 04-windows-nonperiodic
// Ejecutar:  ./04-windows-nonperiodic
//
// Graficar (zoom para ver el leakage):
// gnuplot -e "set datafile separator ','; set title 'Tono No Periódico (441 Hz): Leakage vs Ventanas'; set xrange [0:2000]; plot '441hz.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana (leakage)', '441hz.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', '441hz.csv' using 2:5 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
//
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fstream>

const float PI = M_PI;

// ----------------------------------------------------------
// VENTANAS
// ----------------------------------------------------------
std::vector<float> rectangular(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 1.0f;
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
        w[n] = 0.42f - 0.5f * std::cos(2.0f * PI * t) + 0.08f * std::cos(4.0f * PI * t);
    }
    return w;
}

// ----------------------------------------------------------
// APLICAR VENTANA
// ----------------------------------------------------------
std::vector<float> aplicarVentana(const std::vector<float>& signal,
                                  const std::vector<float>& ventana) {
    std::vector<float> resultado(signal.size());
    for (int n = 0; n < (int)signal.size(); n++)
        resultado[n] = signal[n] * ventana[n];
    return resultado;
}

// ----------------------------------------------------------
// FFT
// ----------------------------------------------------------
std::vector<std::complex<float>> fft(std::vector<std::complex<float>> signal) {
    int N = signal.size();
    if (N <= 1) return signal;
    
    std::vector<std::complex<float>> pares(N / 2), impares(N / 2);
    for (int i = 0; i < N / 2; i++) {
        pares[i]   = signal[2 * i];
        impares[i] = signal[2 * i + 1];
    }
    
    auto FFT_pares   = fft(pares);
    auto FFT_impares = fft(impares);
    
    std::vector<std::complex<float>> resultado(N);
    for (int k = 0; k < N / 2; k++) {
        std::complex<float> twiddle = std::polar(1.0f, -2.0f * PI * k / N) * FFT_impares[k];
        resultado[k]         = FFT_pares[k] + twiddle;
        resultado[k + N / 2] = FFT_pares[k] - twiddle;
    }
    return resultado;
}

// ----------------------------------------------------------
// CALCULAR MAGNITUDES
// ----------------------------------------------------------
std::vector<float> magnitudes(const std::vector<float>& signal) {
    int N = signal.size();
    std::vector<std::complex<float>> compleja(N);
    for (int n = 0; n < N; n++)
        compleja[n] = std::complex<float>(signal[n], 0.0f);
    
    auto espectro = fft(compleja);
    std::vector<float> mag(N / 2);
    for (int k = 0; k < N / 2; k++)
        mag[k] = std::abs(espectro[k]) / N;
    
    return mag;
}

int main() {
    int N            = 1024;
    float sampleRate = 44100.0f;
    float f0         = 441.0f;   // NO es múltiplo exacto de 43.066 Hz (bin resolution)

    std::cout << "=== Tono NO periódico: efecto del leakage ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  f0 = " << f0 << " Hz (¡NO cabe exactamente en la DFT!)" << std::endl;
    std::cout << "  sr = " << sampleRate << " Hz" << std::endl;
    std::cout << "  Bin resolution = " << (sampleRate / N) << " Hz/bin" << std::endl;
    std::cout << "  ⚠️  441 Hz cae entre bins, causando leakage" << std::endl;
    std::cout << std::endl;

    // Generar señal
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "Calculando espectros..." << std::endl;
    
    auto spec_rectangular = magnitudes(aplicarVentana(signal, rectangular(N)));
    auto spec_hann        = magnitudes(aplicarVentana(signal, hann(N)));
    auto spec_blackman    = magnitudes(aplicarVentana(signal, blackman(N)));
    
    std::cout << "✓ FFT calculadas" << std::endl;

    // Exportar CSV
    std::ofstream file("441hz.csv");
    file << "bin,frecuencia_hz,rectangular,hann,blackman" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file << k << ","
             << freq << ","
             << spec_rectangular[k] << ","
             << spec_hann[k] << ","
             << spec_blackman[k] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a 441hz.csv" << std::endl;
    std::cout << std::endl;

    std::cout << "Para graficar:" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; set title 'Tono No Periódico (441 Hz)'; set xrange [0:2000]; plot '441hz.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', '441hz.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', '441hz.csv' using 2:5 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1\"" << std::endl;
    std::cout << std::endl;

    std::cout << "Qué observar:" << std::endl;
    std::cout << "- Sin ventana (rojo): espectro LLENO DE RUIDO alrededor de 441 Hz" << std::endl;
    std::cout << "  (es leakage: la energía se derrama a todas partes)" << std::endl;
    std::cout << "- Hann (verde): mucho más limpio, pico claro en 441 Hz" << std::endl;
    std::cout << "- Blackman (naranja): aún más limpio que Hann" << std::endl;
    std::cout << std::endl;
    std::cout << "⚠️  Este es el caso real: tus señales NO caen exactamente en bins." << std::endl;
    std::cout << "   Por eso SIEMPRE usás ventanas en análisis espectral." << std::endl;
    std::cout << std::endl;

    return 0;
}
