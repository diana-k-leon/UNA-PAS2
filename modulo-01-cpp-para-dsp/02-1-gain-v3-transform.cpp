// ============================================================
// 02-aplicar-gain-v3-transform.cpp
// Programación Aplicada al Sonido II — UNA
// VERSIÓN MEJORADA B: Usar std::transform (funcional/declarativo)
// ============================================================
//
// std::transform es una función de la librería <algorithm> que
// aplica una operación a cada elemento de un contenedor.
//
// Es más "declarativo" — no escribís el loop, solo describes
// qué quieres: "transforma cada sample multiplicándolo por gain"
//
// Nota: requiere entender lambdas [gain](float s) { return s * gain; }
// Por ahora es solo para ver que existen otras formas elegantes.
//
// Compilar:  g++ -std=c++17 02-aplicar-gain-v3-transform.cpp -o aplicar-gain-v3
// Ejecutar:  ./aplicar-gain-v3
// Graficar:  gnuplot -e "set datafile separator ','; plot 'gain_v3.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'original', 'gain_v3.csv' using 1:3 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>  // std::transform
#include <iomanip>

int main() {
    // parámetros
    int N            = 64;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;
    float gain       = 0.5f;

    std::cout << "=== Aplicar Gain (std::transform) ===" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  N = " << N << " muestras" << std::endl;
    std::cout << "  gain = " << gain << std::endl;
    std::cout << std::endl;

    // crear señal original
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        signal[n] = std::sin(2.0f * M_PI * f0 * n / sampleRate);
    }

    // VERSIÓN MEJORADA: std::transform
    // std::transform(inicio_origen, fin_origen, inicio_destino, función)
    // La función [gain](float s) { return s * gain; } es una LAMBDA
    // que dice: "toma cada sample s, multiplícalo por gain, devuelve el resultado"
    std::vector<float> signalConGain(N);
    std::transform(signal.begin(), signal.end(),
                   signalConGain.begin(),
                   [gain](float s) { return s * gain; });

    // mostrar primeras muestras
    std::cout << "Primeras 5 muestras:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    for (int n = 0; n < 5; n++) {
        std::cout << "  n=" << n 
                  << " | original=" << signal[n] 
                  << " | con_gain=" << signalConGain[n] 
                  << std::endl;
    }
    std::cout << std::endl;

    // exportar CSV
    std::ofstream file("gain_v3.csv");
    file << "n,original,con_gain" << std::endl;
    for (int n = 0; n < N; n++) {
        file << n << "," << signal[n] << "," << signalConGain[n] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a gain_v3.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Ventajas: declarativo, funciona con cualquier contenedor STL" << std::endl;
    std::cout << std::endl;
    std::cout << "Para graficar (copia y pega este comando):" << std::endl;
    std::cout << "gnuplot -e \"set datafile separator ','; set title 'Ganancia'; plot 'gain_v3.csv' using 1:2 with lines lc rgb '#00AA00' lw 2 title 'original', 'gain_v3.csv' using 1:3 with lines lc rgb '#FF0000' lw 2 title 'con gain'; pause -1\"" << std::endl;

    return 0;
}