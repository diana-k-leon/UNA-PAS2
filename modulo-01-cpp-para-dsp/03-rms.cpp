// ============================================================
// 03-rms.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 1 — Señales digitales y C++ para DSP
// ============================================================
//
// RMS = Root Mean Square — la amplitud "promedio" de una señal.
//
// Es lo que ves en el medidor de volumen de cualquier DAW.
// No es el máximo (peak), sino la energía general de la onda.
//
// Pasos:
// 1. Elevar cada sample al cuadrado
// 2. Calcular el promedio de esos cuadrados
// 3. Sacar la raíz cuadrada del resultado
//
// Fórmula:
//   RMS = sqrt( (1/N) * suma(sample[n]^2) )
//
// Compilar:  g++ -std=c++17 03-rms.cpp -o calcular-rms
// Ejecutar:  ./calcular-rms
// ============================================================

#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <cmath>        // std::sin, std::sqrt, M_PI

int main() {
    // ----------------------------------------------------------
    // PARÁMETROS
    // ----------------------------------------------------------
    int N            = 4096;       // muestras
    float sampleRate = 44100.0f; // Hz
    float f0         = 440.0f;   // LA
    float amplitude  = 1.0f;     // amplitud de la sinusoide (probá: 0.5, 2.0, etc.)

    // ----------------------------------------------------------
    // CREAR SEÑAL
    // ----------------------------------------------------------
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        signal[n] = amplitude * std::sin(2.0f * M_PI * f0 * n / sampleRate);
    }

    // ----------------------------------------------------------
    // CALCULAR RMS
    // Paso 1: acumular los cuadrados de cada sample
    // ----------------------------------------------------------
    float sumOfSquares = 0.0f;
    for (const auto& sample : signal) {
        sumOfSquares += sample * sample;
    }

    // Paso 2: calcular el promedio (dividir por N)
    float mean = sumOfSquares / N;                      // promedio de los cuadrados

    // Paso 3: raíz cuadrada
    float rms = std::sqrt(mean);

    // ----------------------------------------------------------
    // MOSTRAR RESULTADOS
    // ----------------------------------------------------------
    std::cout << "=== RMS de la señal ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  f0 = " << f0 << " Hz" << std::endl;
    std::cout << "  amplitud = " << amplitude << std::endl;
    std::cout << std::endl;

    std::cout << "Cálculo:" << std::endl;
    std::cout << "  suma de cuadrados = " << sumOfSquares << std::endl;
    std::cout << "  media de cuadrados = " << mean << std::endl;
    std::cout << "  RMS = " << rms << std::endl;
    std::cout << std::endl;

    // ----------------------------------------------------------
    // VERIFICACIÓN TEÓRICA
    // Para una sinusoide pura: RMS = amplitud / sqrt(2) ≈ 0.707 * amplitud
    // Esto debería coincidir con lo que calculamos
    // ----------------------------------------------------------
    float theoreticalRMS = amplitude / std::sqrt(2.0f);
    std::cout << "Verificación:" << std::endl;
    std::cout << "  RMS teórico para sinusoide = amplitud / sqrt(2)" << std::endl;
    std::cout << "  RMS teórico = " << amplitude << " / sqrt(2) = " << theoreticalRMS << std::endl;
    std::cout << "  RMS calculado = " << rms << std::endl;
    std::cout << "  Diferencia = " << std::abs(rms - theoreticalRMS) << std::endl;

    return 0;
}