// ============================================================
// 02-sinusoide-compleja.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: Sinusoide compleja en dominio discreto
// ============================================================
//
// A diferencia de la sinusoide anterior, esta trabaja en
// dominio discreto — no hay sample rate ni segundos,
// solo índices de muestra y bins de frecuencia.
// La herramienta matemática que usa la DFT — no suena, mide
// e^(j·2π·k·n/N) = cos(2π·k·n/N) + j·sin(2π·k·n/N)
//  La metáfora de la regla
// La sinusoide compleja es la regla con la que la DFT mide tu señal. 
// La DFT tiene N de estas reglas — una por cada bin de frecuencia. 
// Para cada bin pregunta: ¿cuánto se parece mi señal a esta regla? 
// Si se parecen mucho, el bin tiene valor alto. Si no se parecen, valor casi cero.

// Compilar:  g++ -std=c++17 02-sinusoide-compleja.cpp -o sinusoide-compleja
// Ejecutar:  ./sinusoide-compleja
// Graficar:  gnuplot -e "set datafile separator ','; plot 'sinusoide-compleja.csv' using 1:2 with lines title 'real'; pause -1"
// ============================================================

#include <iostream>
#include <vector>
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
    // Usamos dos vectores separados — parte real e imaginaria —
    // para que sea claro qué es cada cosa antes de introducir std::complex.
    //
    // parte real → coseno
    // parte imaginaria → seno

    std::vector<float> s_real(N);
    std::vector<float> s_imag(N);

    for (int n = 0; n < N; ++n) {
        float exponent = 2.0f * M_PI * k * n / N;
        s_real[n] = std::cos(exponent);
        s_imag[n] = std::sin(exponent);
    }

    // ----------------------------------------------------------
    // EXPORTAR A CSV
    // ----------------------------------------------------------
    // Exportamos parte real e imaginaria para poder graficar ambas.
    // gnuplot muestra la parte real por defecto (columna 2).
    // Para ver la imaginaria cambiá "using 1:2" por "using 1:3".

    std::ofstream file("sinusoide-compleja.csv");
    for (int n = 0; n < N; ++n) {
        file << n << "," << s_real[n] << "," << s_imag[n] << "\n";
    }
    file.close();

    std::cout << "Generadas " << N << " muestras\n";
    std::cout << "Guardado en sinusoide-compleja.csv\n";

    return 0;
}

// ----------------------------------------------------------
// PARA EXPLORAR
// ----------------------------------------------------------
// Cambiar k=1  → un ciclo en 64 muestras
// Cambiar k=8  → ocho ciclos — frecuencia más alta
// Cambiar k=32 → Nyquist — el límite
// En la DFT: cada bin k usa exactamente esta sinusoide como regla de medición