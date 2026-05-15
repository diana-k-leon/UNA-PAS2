// ============================================================
// 05-suma-sinusoides.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: Suma de dos sinusoides — detectar dos picos
// ============================================================
//
// Genera una señal con dos frecuencias y verifica que
// la DFT detecta los dos picos correctamente.
//
// Compilar:  g++ -std=c++17 05-suma-sinusoides.cpp -o suma-sinusoides
// Ejecutar:  ./suma-sinusoides
// Graficar:  gnuplot -e "set datafile separator ','; plot 'suma-sinusoides.csv' using 1:2 with lines title 'espectro'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fstream>

int main() {

    // ----------------------------------------------------------
    // PARÁMETROS
    // ----------------------------------------------------------
    int   N  = 512;
    float sr = 44100.0f;
    float f1 = 440.0f;    // La4
    float f2 = 1760.0f;   // La6 — cuatro octavas arriba

    // ----------------------------------------------------------
    // SEÑAL — suma de dos sinusoides
    // ----------------------------------------------------------
    // Amplitudes distintas para que se vea la diferencia en el espectro
    std::vector<float> senal(N);
    for (int n = 0; n < N; ++n) {
        senal[n] = 0.7f * std::cos(2.0f * M_PI * f1 / sr * n)
                 + 0.3f * std::cos(2.0f * M_PI * f2 / sr * n);
    }

    // ----------------------------------------------------------
    // DFT
    // ----------------------------------------------------------
    std::vector<std::complex<float>> espectro(N);
    for (int bin = 0; bin < N; ++bin) {
        std::complex<float> suma(0.0f, 0.0f);
        for (int muestra = 0; muestra < N; ++muestra) {
            float angulo = 2.0f * M_PI * bin * muestra / N;
            std::complex<float> referencia(std::cos(angulo), std::sin(angulo));
            suma += senal[muestra] * std::conj(referencia);
        }
        espectro[bin] = suma;
    }

    // ----------------------------------------------------------
    // RESULTADO — mostrar picos significativos
    // ----------------------------------------------------------
    std::cout << "Bin\tFrecuencia (Hz)\tMagnitud\n";
    for (int bin = 0; bin < N / 2; ++bin) {   // solo primera mitad
        float magnitud = std::abs(espectro[bin]) / N;
        if (magnitud > 0.05f) {
            float frecuencia = bin * sr / N;
            std::cout << bin << "\t" << frecuencia << "\t\t" << magnitud << "\n";
        }
    }

    // ----------------------------------------------------------
    // EXPORTAR A CSV
    // ----------------------------------------------------------
    std::ofstream file("suma-sinusoides.csv");
    for (int bin = 0; bin < N / 2; ++bin) {
        float frecuencia = bin * sr / N;
        float magnitud   = std::abs(espectro[bin]) / N;
        file << frecuencia << "," << magnitud << "\n";
    }
    file.close();
    std::cout << "\nExportado a suma-sinusoides.csv\n";

    return 0;
}

// ----------------------------------------------------------
// PARA EXPLORAR
// ----------------------------------------------------------
// Cambiar f1 y f2 — ¿la DFT siempre detecta los dos picos?
// Cambiar las amplitudes — ¿se refleja en la magnitud de cada pico?
// Poner f1 y f2 muy cerca — ¿puede la DFT distinguirlas?