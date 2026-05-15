// ============================================================
// 02-aplicar-gain-v2-inplace.cpp
// Programación Aplicada al Sonido II — UNA
// VERSIÓN MEJORADA A: Modificar la señal directamente
// ============================================================
//
// En lugar de crear un nuevo vector, modificamos la señal original.
// Esto es más eficiente: usa la mitad de memoria.
//
// Lo que cambia:
// - Usamos for (auto& sample : signal) para modificar cada elemento
// - No necesitamos signalConGain
// - Guardamos una sola señal en el CSV, pero podemos verificar que cambió
//
// Compilar:  g++ -std=c++17 02-aplicar-gain-v2-inplace.cpp -o aplicar-gain-v2
// Ejecutar:  ./aplicar-gain-v2
// Graficar:  gnuplot -e "set datafile separator ','; plot 'gain_v2.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    // parámetros
    int N            = 64;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;
    float gain       = 0.5f;

    std::cout << "=== Aplicar Gain (IN-PLACE) ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  gain = " << gain << std::endl;
    std::cout << std::endl;

    // crear señal
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        signal[n] = std::sin(2.0f * M_PI * f0 * n / sampleRate);
    }

    // VERSIÓN MEJORADA: modificar in-place
    // for (auto& sample : signal) accede cada elemento directamente
    // sin necesidad de índice — y el & permite modificarlo
    for (auto& sample : signal) {
        sample *= gain;
    }

    // mostrar primeras muestras
    std::cout << "Primeras 5 muestras con gain aplicado:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    for (int n = 0; n < 5; n++) {
        std::cout << "  n=" << n << " | con_gain=" << signal[n] << std::endl;
    }
    std::cout << std::endl;

    // exportar CSV
    std::ofstream file("gain_v2.csv");
    file << "n,con_gain" << std::endl;
    for (int n = 0; n < N; n++) {
        file << n << "," << signal[n] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a gain_v2.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Ventajas: usa la mitad de memoria, código más limpio" << std::endl;
    std::cout << "Para graficar (ROJO): gnuplot -e \"set datafile separator ','; plot 'gain_v2.csv' using 1:2 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1\"" << std::endl;

    return 0;
}