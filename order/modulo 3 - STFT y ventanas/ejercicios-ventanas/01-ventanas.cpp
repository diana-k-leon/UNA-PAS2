// ============================================================
// 01-hamming-espectro.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2 — Ventanas y espectrograma
// ============================================================
//
// Una ventana es una función matemática que multiplica tu señal
// para suavizar los bordes. Sin ventana, la FFT ve un "salto" abrupto
// que crea artefactos espectrales falsos (lóbulos laterales).
// Con ventana, el espectro es más limpio.
//
// Este ejercicio:
// - Genera una senoidal de 440 Hz
// - Calcula FFT SIN ventana
// - Calcula FFT CON ventana Hamming
// - Exporta CSV para comparar visualmente los dos espectros
// - Observa la diferencia en los lóbulos laterales
//
// Compilar:  g++ -std=c++17 ventanas.cpp -o ventanas
// Ejecutar:  ./ventanas
// GRAFICAR — 3 opciones:
//
// 1. Vista completa (0-25000 Hz):
// gnuplot -e "set datafile separator ','; set title 'Espectro: sin ventana vs Hamming'; plot 'hamming.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', 'hamming.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hamming'; pause -1"
//
// 2. Zoom en la región relevante (0-2000 Hz) — RECOMENDADO PARA VER LA DIFERENCIA:
// gnuplot -e "set datafile separator ','; set title 'Espectro: sin ventana vs Hamming (zoom 440 Hz)'; set xrange [0:2000]; plot 'hamming.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', 'hamming.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hamming'; pause -1"
//
// 3. Zoom ultra-cerrado en 440 Hz ± 200 Hz (para ver claramente los lóbulos laterales):
// gnuplot -e "set datafile separator ','; set title 'Lóbulos laterales (zoom en 440 Hz)'; set xrange [300:600]; set yrange [0:0.1]; plot 'hamming.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', 'hamming.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hamming'; pause -1"
//
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fstream>

const float PI = M_PI;

// ----------------------------------------------------------
// VENTANA HAMMING
// Fórmula: w[n] = 0.54 - 0.46 * cos(2π * n / (N-1))
// Resultado: un array donde cada elemento vale entre ~0 y 1
// Los bordes valen casi 0, el centro vale ~1
// ----------------------------------------------------------
std::vector<float> hamming(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.54f - 0.46f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

// ----------------------------------------------------------
// APLICAR VENTANA
// Multiplica cada muestra de la señal por su correspondiente valor de ventana
// señal_ventaneada[n] = señal[n] * ventana[n]
// ----------------------------------------------------------
std::vector<float> aplicarVentana(const std::vector<float>& signal,
                                  const std::vector<float>& ventana) {
    std::vector<float> resultado(signal.size());
    for (int n = 0; n < (int)signal.size(); n++)
        resultado[n] = signal[n] * ventana[n];
    return resultado;
}

// ----------------------------------------------------------
// FFT — TRANSFORMADA RÁPIDA DE FOURIER
// Implementación recursiva de Cooley-Tukey
// Convierte una señal del dominio del tiempo al dominio de la frecuencia
// Entrada: vector de números complejos
// Salida: vector de números complejos (espectro, cada uno representa una frecuencia)
// ----------------------------------------------------------
std::vector<std::complex<float>> fft(std::vector<std::complex<float>> signal) {
    int N = signal.size();
    
    // Caso base: si N <= 1, ya está "transformado"
    if (N <= 1) return signal;
    
    // Dividir en dos mitades: índices pares e impares
    std::vector<std::complex<float>> pares(N / 2), impares(N / 2);
    for (int i = 0; i < N / 2; i++) {
        pares[i]   = signal[2 * i];       // muestras 0, 2, 4, ...
        impares[i] = signal[2 * i + 1];   // muestras 1, 3, 5, ...
    }
    
    // Recursión: calcular FFT de cada mitad
    auto FFT_pares   = fft(pares);
    auto FFT_impares = fft(impares);
    
    // Combinar resultados con "twiddle factors" (factores de rotación)
    std::vector<std::complex<float>> resultado(N);
    for (int k = 0; k < N / 2; k++) {
        // twiddle = e^(-2πi * k / N) — una rotación en el plano complejo
        std::complex<float> twiddle = std::polar(1.0f, -2.0f * PI * k / N) * FFT_impares[k];
        resultado[k]         = FFT_pares[k] + twiddle;          // primera mitad
        resultado[k + N / 2] = FFT_pares[k] - twiddle;          // segunda mitad
    }
    return resultado;
}

// ----------------------------------------------------------
// CALCULAR MAGNITUDES DEL ESPECTRO
// Toma una señal real (como audio), la convierte a compleja,
// calcula FFT, y devuelve la magnitud de cada bin de frecuencia.
// Magnitud = |número_complejo| = sqrt(real² + imag²)
// ----------------------------------------------------------
std::vector<float> magnitudes(const std::vector<float>& signal) {
    int N = signal.size();
    
    // Convertir señal real a compleja (parte imaginaria = 0)
    std::vector<std::complex<float>> compleja(N);
    for (int n = 0; n < N; n++)
        compleja[n] = std::complex<float>(signal[n], 0.0f);
    
    // Calcular FFT
    auto espectro = fft(compleja);
    
    // Extraer magnitudes de la primera mitad (frecuencias positivas)
    // y normalizar por N
    std::vector<float> mag(N / 2);
    for (int k = 0; k < N / 2; k++)
        mag[k] = std::abs(espectro[k]) / N;
    
    return mag;
}

int main() {
    // ----------------------------------------------------------
    // PARÁMETROS
    // Experimentá cambiando f0 para ver cómo afecta el leakage
    // ----------------------------------------------------------
    int N            = 1024;       // tamaño de la ventana (número de muestras)
    float sampleRate = 44100.0f;   // frecuencia de muestreo estándar (CD)
    float f0         = 440.0f;     // frecuencia de la senoidal (LA)
                                   // probá con 441.0 para ver el efecto del leakage

    // ----------------------------------------------------------
    // GENERAR SEÑAL
    // Una sinusoide pura: y[n] = sin(2π * f0 * n / sampleRate)
    // ----------------------------------------------------------
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "=== Comparar espectro: sin ventana vs Hamming ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  f0 = " << f0 << " Hz (LA)" << std::endl;
    std::cout << "  sampleRate = " << sampleRate << " Hz" << std::endl;
    std::cout << std::endl;

    // ----------------------------------------------------------
    // CALCULAR ESPECTROS
    // ----------------------------------------------------------
    std::cout << "Calculando espectros..." << std::endl;
    
    // Espectro sin ventana
    auto sinVentana = magnitudes(signal);
    
    // Espectro con ventana Hamming
    auto ventanaHamming = hamming(N);
    auto conHamming = magnitudes(aplicarVentana(signal, ventanaHamming));
    
    std::cout << "✓ FFT calculadas" << std::endl;

    // ----------------------------------------------------------
    // EXPORTAR CSV
    // Columnas: número de bin, frecuencia en Hz, magnitud sin ventana, magnitud con Hamming
    // ----------------------------------------------------------
    std::ofstream file("hamming.csv");
    file << "bin,frecuencia_hz,sin_ventana,con_hamming" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file << k << ","
             << freq << ","
             << sinVentana[k] << ","
             << conHamming[k] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a hamming.csv" << std::endl;
    std::cout << std::endl;

    // ----------------------------------------------------------
    // INSTRUCCIONES PARA VISUALIZAR
    // ----------------------------------------------------------
    std::cout << "Para graficar (copia y pega este comando):" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; set title 'Espectro: sin ventana vs Hamming'; plot 'hamming.csv' using 2:3 with lines lc rgb '#FF0000' lw 2 title 'sin ventana', 'hamming.csv' using 2:4 with lines lc rgb '#00AA00' lw 2 title 'Hamming'; pause -1\"" << std::endl;
    std::cout << std::endl;

    std::cout << "Qué esperar:" << std::endl;
    std::cout << "- Rojo (sin ventana): pico central agudo pero lóbulos laterales MUY altos" << std::endl;
    std::cout << "- Verde (Hamming): pico central más ancho pero lóbulos laterales SUPRIMIDOS" << std::endl;
    std::cout << std::endl;

    std::cout << "Probá también con f0=441.0 Hz para ver el efecto del leakage" << std::endl;

    return 0;
}