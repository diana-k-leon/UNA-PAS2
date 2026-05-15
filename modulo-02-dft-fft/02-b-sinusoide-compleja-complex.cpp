// ============================================================
// 02-sinusoide-compleja.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: Sinusoide compleja en dominio discreto
// ============================================================
//
// A diferencia de la sinusoide anterior, esta trabaja en
// dominio discreto — no hay sample rate ni segundos,
// solo índices de muestra y bins de frecuencia.
// Es la base matemática de la DFT.
//
// Compilar:  g++ -std=c++17 02-b-sinusoide-compleja-complex.cpp -o sinusoide-compleja-b
// Ejecutar:  ./sinusoide-compleja-b
// Graficar:  gnuplot -e "set datafile separator ','; plot 'sinusoide-compleja.csv' using 1:2 with lines title 'real', 'sinusoide-compleja.csv' using 1:3 with lines title 'imag'; pause -1"
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
    int N = 64;   // cantidad de muestras (potencia de 2, como en la DFT)
    int k = 3;    // bin — k ciclos completos en N muestras

    // ----------------------------------------------------------
    // GENERAR LA SINUSOIDE COMPLEJA
    // ----------------------------------------------------------
    // Fórmula: s[n] = e^(j * 2π * k * n / N)
    //                = cos(2π * k * n / N) + j * sin(2π * k * n / N)
    //
    // std::complex<float> guarda los dos valores juntos:
    //   z.real() → coseno → parte real
    //   z.imag() → seno   → parte imaginaria

    std::vector<std::complex<float>> s(N);

    for (int n = 0; n < N; ++n) {
        float exponent = 2.0f * M_PI * k * n / N;
        s[n] = std::complex<float>(std::cos(exponent), std::sin(exponent));
    }

    // ----------------------------------------------------------
    // EXPORTAR A CSV
    // ----------------------------------------------------------
    // Columna 1: índice n
    // Columna 2: parte real (coseno)
    // Columna 3: parte imaginaria (seno)

    std::ofstream file("sinusoide-compleja.csv");
    for (int n = 0; n < N; ++n) {
        file << n << "," << s[n].real() << "," << s[n].imag() << "\n";
    }
    file.close();

    std::cout << "Generadas " << N << " muestras\n";
    std::cout << "Guardado en sinusoide-compleja.csv\n";

    return 0;
}

// ----------------------------------------------------------
// PARA EXPLORAR
// ----------------------------------------------------------
// k=1  → un ciclo en 64 muestras
// k=8  → ocho ciclos — frecuencia más alta
// k=32 → Nyquist — el límite
// En la DFT: cada bin k usa exactamente esta sinusoide como regla de medición