// ============================================================
// 03-windows-two-tones.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2 — Ventanas: resolvabilidad de dos tonos cercanos
// ============================================================
//
// Objetivo: Ver cómo las ventanas afectan la capacidad
// de separar dos sinusoides cercanas en frecuencia.
//
// Señal: 440 Hz + 460 Hz (20 Hz de diferencia)
// Con la DFT de 1024 muestras a 44100 Hz, cada bin = 44100/1024 ≈ 43 Hz
// Entonces 440 Hz y 460 Hz caen en bins cercanos pero distintos.
//
// Sin ventana: el leakage de 440 Hz puede enmascarar a 460 Hz
// Con ventana: la supresión de lóbulos deja más clara la separación
//
// Este ejercicio:
// - Genera dos sinusoides: 440 Hz + 460 Hz
// - Calcula FFT con 4 ventanas
// - Exporta CSV para comparar
// - Comprobá cuál ventana "ve" dos picos claros
//
// Compilar:  g++ -std=c++17 03-windows-two-tones.cpp -o 03-windows-two-tones
// Ejecutar:  ./03-windows-two-tones
//
// Graficar (zoom en la región de interés):
// gnuplot -e "set datafile separator ','; set title 'Dos Tonos (440+460 Hz): ¿Cuál ventana los separa?'; set xrange [300:600]; plot 'two-tones.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', 'two-tones.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', 'two-tones.csv' using 2:5 with lines lc rgb '#0000FF' lw 2 title 'Hamming', 'two-tones.csv' using 2:6 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1"
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
    float f1         = 440.0f;   // LA
    float f2         = 460.0f;   // 20 Hz arriba (casi un semitono)

    std::cout << "=== Dos tonos cercanos: resolvabilidad ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  f1 = " << f1 << " Hz" << std::endl;
    std::cout << "  f2 = " << f2 << " Hz (diferencia: " << (f2 - f1) << " Hz)" << std::endl;
    std::cout << "  sr = " << sampleRate << " Hz" << std::endl;
    std::cout << "  Bin resolution = " << (sampleRate / N) << " Hz/bin" << std::endl;
    std::cout << std::endl;

    // Generar señal: suma de dos sinusoides
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        float t = (float)n / sampleRate;
        signal[n] = std::sin(2.0f * PI * f1 * t) + std::sin(2.0f * PI * f2 * t);
    }

    std::cout << "Calculando espectros..." << std::endl;
    
    auto spec_rectangular = magnitudes(aplicarVentana(signal, rectangular(N)));
    auto spec_hann        = magnitudes(aplicarVentana(signal, hann(N)));
    auto spec_hamming     = magnitudes(aplicarVentana(signal, hamming(N)));
    auto spec_blackman    = magnitudes(aplicarVentana(signal, blackman(N)));
    
    std::cout << "✓ FFT calculadas" << std::endl;

    // Exportar CSV
    std::ofstream file("two-tones.csv");
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

    std::cout << "✓ Exportado a two-tones.csv" << std::endl;
    std::cout << std::endl;

    std::cout << "Para graficar (zoom en 300-600 Hz, donde están los tonos):" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; set title 'Dos Tonos (440+460 Hz)'; set xrange [300:600]; plot 'two-tones.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'Rectangular', 'two-tones.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hann', 'two-tones.csv' using 2:5 with lines lc rgb '#0000FF' lw 2 title 'Hamming', 'two-tones.csv' using 2:6 with lines lc rgb '#FF8800' lw 2 title 'Blackman'; pause -1\"" << std::endl;
    std::cout << std::endl;

    std::cout << "Qué observar:" << std::endl;
    std::cout << "- ¿Ves dos picos separados o uno solo?" << std::endl;
    std::cout << "- Rectangular: los ve pero hay mucho leakage" << std::endl;
    std::cout << "- Hann/Hamming: dos picos claros" << std::endl;
    std::cout << "- Blackman: dos picos claros, menos leakage" << std::endl;
    std::cout << std::endl;

    return 0;
}
