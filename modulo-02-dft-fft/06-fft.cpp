// ============================================================
// 04-fft.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: FFT Cooley-Tukey desde cero + comparación con DFT
// ============================================================
//
// Implementa la DFT O(N²) y la FFT O(N·log₂N) sobre la misma
// señal y compara tiempos. Los resultados deben ser idénticos.
//
// Compilar:  g++ -std=c++17 04-fft.cpp -o fft
// Ejecutar:  ./fft
// Graficar:  gnuplot -e "set datafile separator ','; plot 'fft.csv' using 2:3 with lines title 'espectro'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <chrono>
#include <fstream>

const float PI = M_PI;

// ─────────────────────────────────────────────────────────────
// DFT — O(N²)
// ─────────────────────────────────────────────────────────────
// Igual que antes — está acá solo para comparar tiempos.
// Entrada: señal real (vector de floats)
// Salida:  espectro complejo (vector de complex<float>)

std::vector<std::complex<float>> dft(const std::vector<float>& senal) {
    int N = senal.size();
    std::vector<std::complex<float>> espectro(N);

    for (int bin = 0; bin < N; ++bin) {
        std::complex<float> suma(0.0f, 0.0f);
        for (int muestra = 0; muestra < N; ++muestra) {
            float angulo = 2.0f * PI * bin * muestra / N;
            suma += senal[muestra] * std::complex<float>(std::cos(angulo), -std::sin(angulo));
        }
        espectro[bin] = suma;
    }
    return espectro;
}

// ─────────────────────────────────────────────────────────────
// FFT Cooley-Tukey Radix-2 — O(N·log₂N)
// ─────────────────────────────────────────────────────────────
// Divide y conquistá recursivo — igual que mergesort.
// Entrada: señal compleja (Im=0 para señal real)
// Salida:  espectro complejo
//
// IMPORTANTE: N debe ser potencia de 2

std::vector<std::complex<float>> fft(std::vector<std::complex<float>> senal) {
    int N = senal.size();

    // ── caso base ──────────────────────────────────────────
    // DFT de 1 punto = el mismo valor — no hay nada que calcular
    if (N <= 1) return senal;

    // ── validar potencia de 2 ──────────────────────────────
    // N & (N-1) == 0 solo cuando N es potencia de 2
    // Ej: N=8 → 1000 & 0111 = 0 ✓ · N=6 → 0110 & 0101 = 4 ✗
    if (N & (N - 1)) {
        std::cerr << "Error: N debe ser potencia de 2\n";
        return senal;
    }

    // ── dividir en pares e impares ─────────────────────────
    // pares   → muestras en posición 0, 2, 4, 6...
    // impares → muestras en posición 1, 3, 5, 7...
    std::vector<std::complex<float>> pares(N / 2), impares(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        pares[i]   = senal[2 * i];
        impares[i] = senal[2 * i + 1];
    }

    // ── conquistar — llamadas recursivas ───────────────────
    // Cada llamada divide a la mitad hasta llegar al caso base
    // log₂(N) niveles de recursión en total
    auto FFT_pares   = fft(pares);
    auto FFT_impares = fft(impares);

    // ── combinar — butterfly ───────────────────────────────
    // Para cada bin k:
    //   factorDeRotacion = e^(-2πi·k/N) × FFT_impares[k]
    //   resultado[k]       = FFT_pares[k] + factorDeRotacion
    //   resultado[k + N/2] = FFT_pares[k] - factorDeRotacion
    //
    // std::polar(1.0f, angulo) genera un número complejo
    // de módulo 1 y el ángulo dado

   std::vector<std::complex<float>> resultado(N);
    for (int k = 0; k < N / 2; ++k) {
        std::complex<float> factorDeRotacion = std::polar(1.0f, -2.0f * PI * k / N)
                                               * FFT_impares[k];
        resultado[k]         = FFT_pares[k] + factorDeRotacion;
        resultado[k + N / 2] = FFT_pares[k] - factorDeRotacion;
    }
    return resultado;
}

// ─────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────

int main() {

    // ── parámetros ─────────────────────────────────────────
    int   N          = 1024;      // potencia de 2
    float sr         = 44100.0f;
    float f0         = 440.0f;    // La4

    // ── generar señal ──────────────────────────────────────
    std::vector<float> senal(N);
    for (int n = 0; n < N; ++n)
        senal[n] = std::sin(2.0f * PI * f0 * n / sr);

    // convertir a complejo para la FFT (Im = 0)
    std::vector<std::complex<float>> senalCompleja(N);
    for (int n = 0; n < N; ++n)
        senalCompleja[n] = std::complex<float>(senal[n], 0.0f);

    // ── medir tiempo DFT ───────────────────────────────────
    auto t1 = std::chrono::high_resolution_clock::now();
    auto espectroDFT = dft(senal);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duracionDFT = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    // ── medir tiempo FFT ───────────────────────────────────
    auto t3 = std::chrono::high_resolution_clock::now();
    auto espectroFFT = fft(senalCompleja);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto duracionFFT = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    // ── comparar tiempos ───────────────────────────────────
    std::cout << "N = " << N << "\n";
    std::cout << "DFT: " << duracionDFT << " μs\n";
    std::cout << "FFT: " << duracionFFT << " μs\n";
    std::cout << "FFT es " << (float)duracionDFT / duracionFFT << "x más rápida\n";

    // ── verificar que los resultados son iguales ───────────
    // Si la FFT está bien implementada, el error debe ser
    // casi cero — solo pequeñas diferencias de punto flotante
    float errorMax = 0.0f;
    for (int k = 0; k < N; ++k) {
        float error = std::abs(espectroDFT[k] - espectroFFT[k]);
        errorMax = std::max(errorMax, error);
    }
    std::cout << "Error máximo DFT vs FFT: " << errorMax << "\n";

    // ── exportar espectro FFT a CSV ────────────────────────
    // Solo la primera mitad — hasta Nyquist
    // Magnitud normalizada por N
    std::ofstream file("fft.csv");
    for (int k = 0; k < N / 2; ++k) {
        float frecuencia = k * sr / N;
        float magnitud   = std::abs(espectroFFT[k]) / N;
        file << k << "," << frecuencia << "," << magnitud << "\n";
    }
    file.close();
    std::cout << "Espectro exportado a fft.csv\n";

    return 0;
}

// ─────────────────────────────────────────────────────────────
// PARA EXPLORAR
// ─────────────────────────────────────────────────────────────
// Cambiar N a 64, 256, 4096 — ver cómo crecen los tiempos
// Cambiar f0 — verificar que el pico se mueve
// El error DFT vs FFT debe ser siempre < 0.01 — si no, hay un bug