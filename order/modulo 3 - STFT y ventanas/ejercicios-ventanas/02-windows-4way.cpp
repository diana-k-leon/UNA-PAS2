// ============================================================
// 02-windows-4way.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2 — Ventanas: comparación de 4 tipos
// ============================================================
//
// Objetivo: Ver en un solo gráfico cómo se comportan
// las 4 ventanas principales con la MISMA señal.
//
// Preguntas que responde:
// - ¿Cuál tiene el pico más angosto? (Rectangular)
// - ¿Cuál suprime mejor los lóbulos? (Blackman)
// - ¿Cuál es el mejor balance general? (Hann)
//
// Este ejercicio:
// - Genera una senoidal de 440 Hz
// - Aplica 4 ventanas diferentes
// - Calcula FFT para cada una
// - Exporta CSV con todos los espectros
// - Visualiza todas en un gráfico
//
// Compilar:  g++ -std=c++17 02-windows-4way.cpp -o 02-windows-4way
// Ejecutar:  ./02-windows-4way
//
// Graficar (zoom recomendado para ver bien la diferencia):
// gnuplot -e "set datafile separator ','; set title '4 Ventanas: Rectangular vs Hann vs Hamming vs Blackman'; set xrange [0:2000]; plot '4windows.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', '4windows.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', '4windows.csv' using 2:5 with lines lc rgb '#0000FF' lw 2 title 'Hamming', '4windows.csv' using 2:6 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
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

std::vector<float> hamming(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.54f - 0.46f * std::cos(2.0f * PI * n / (N - 1));
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
    float f0         = 440.0f;

    // Generar señal
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "=== Comparar 4 ventanas ===" << std::endl;
    std::cout << "Parámetros: N=" << N << ", f0=" << f0 << " Hz, sr=" << sampleRate << " Hz" << std::endl;
    std::cout << std::endl;

    // Calcular espectros
    std::cout << "Calculando espectros..." << std::endl;
    
    auto spec_rectangular = magnitudes(aplicarVentana(signal, rectangular(N)));
    auto spec_hann        = magnitudes(aplicarVentana(signal, hann(N)));
    auto spec_hamming     = magnitudes(aplicarVentana(signal, hamming(N)));
    auto spec_blackman    = magnitudes(aplicarVentana(signal, blackman(N)));
    
    std::cout << "✓ FFT calculadas" << std::endl;

    // Exportar CSV
    std::ofstream file("4windows.csv");
    file << "bin,frecuencia_hz,rectangular,hann,hamming,blackman" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file << k << ","
             << freq << ","
             << spec_rectangular[k] << ","
             << spec_hann[k] << ","
             << spec_hamming[k] << ","
             << spec_blackman[k] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a 4windows.csv" << std::endl;
    std::cout << std::endl;

    std::cout << "Para graficar:" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; set title '4 Ventanas'; set xrange [0:2000]; plot '4windows.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', '4windows.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', '4windows.csv' using 2:5 with lines lc rgb '#0000FF' lw 2 title 'Hamming', '4windows.csv' using 2:6 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1\"" << std::endl;
    std::cout << std::endl;

    std::cout << "Qué observar:" << std::endl;
    std::cout << "- Rectangular (rojo): pico MÁS ANGOSTO, pero lóbulos ALTÍSIMOS" << std::endl;
    std::cout << "- Hann (verde): balance: lóbulos bajos, pico moderado" << std::endl;
    std::cout << "- Hamming (azul): parecido a Hann, un poco mejor en lóbulos" << std::endl;
    std::cout << "- Blackman (naranja): lóbulos MÁS BAJOS, pero pico MÁS ANCHO" << std::endl;
    std::cout << std::endl;

    return 0;
}
