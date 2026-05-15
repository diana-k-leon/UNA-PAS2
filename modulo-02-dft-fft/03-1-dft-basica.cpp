// ============================================================
// 03-1-dft-basica.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: Implementación de la DFT desde cero
// ============================================================
//
// La DFT compara la señal con sinusoides complejas de distintas
// frecuencias. Para cada frecuencia genera una sinusoide de
// referencia y la compara con la señal — si se parecen, ese
// bin tiene valor alto.
//
// Compilar:  g++ -std=c++17 03-1-dft-basica.cpp -o dft-basica
// Ejecutar:  ./dft-basica
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

int main() {

    // ----------------------------------------------------------
    // SEÑAL DE ENTRADA — sinusoide compleja en bin 7
    // ----------------------------------------------------------
    // Usamos una sinusoide compleja pura porque sabemos de antemano
    // que la DFT debería encontrar energía exactamente en bin 7.
    // Es un test controlado — verificamos que el algoritmo funciona.

    int N  = 64;
    int k0 = 7;   // bin de la señal de entrada

    std::vector<std::complex<float>> senal(N);
    for (int muestra = 0; muestra < N; ++muestra) {
        float angulo = 2.0f * M_PI * k0 * muestra / N;
        senal[muestra] = std::complex<float>(std::cos(angulo), std::sin(angulo));
    }

    // ----------------------------------------------------------
    // DFT
    // ----------------------------------------------------------
    // Para cada bin de frecuencia:
    //   1. Genera la sinusoide de referencia de ese bin
    //   2. Compara con la señal — producto interno
    //   3. Guarda el resultado en el espectro
    //
    // Complejidad: O(N²) — N bins × N muestras

    //Un vector de 64 números complejos, todos en cero. Acá va a quedar el resultado.
    std::vector<std::complex<float>> espectro(N);

    // Recorre las 64 frecuencias posibles. 
    // En cada vuelta pregunta: ¿hay energía en esta frecuencia?
    for (int bin = 0; bin < N; ++bin) {  

        std::complex<float> suma(0.0f, 0.0f);
        // Para cada muestra de la señal hace tres cosas
        for (int muestra = 0; muestra < N; ++muestra) { 
            // Calcula el angulo de la sinusoide de referencia de ese bin
            float angulo = 2.0f * M_PI * bin * muestra / N;
            //Genera la referencia — la sinusoide compleja de ese bin en esa muestra
            std::complex<float> referencia(std::cos(angulo), std::sin(angulo));
            //Multiplica senal[muestra] * conj(referencia) — la comparación — y lo suma al acumulador
            suma += senal[muestra] * std::conj(referencia);  // comparación
        }
        //Cuando terminó de recorrer todas las muestras, guarda el resultado en ese bin.
        espectro[bin] = suma;
    }

    // ----------------------------------------------------------
    // RESULTADO — magnitud de cada bin
    // ----------------------------------------------------------
    // std::abs() calcula el módulo del número complejo — igual que
    // en la slide de std::complex. Si es alto, hay energía en ese bin.

    std::cout << "Bin\tMagnitud\n";
    for (int bin = 0; bin < N; ++bin) {
        float magnitud = std::abs(espectro[bin]);
        if (magnitud > 0.5f) {
            std::cout << bin << "\t" << magnitud << "\n";
        }
    }

    // resultado esperado: solo bin 7 con magnitud ≈ 64 (= N)

    return 0;
}

// ----------------------------------------------------------
// PARA EXPLORAR
// ----------------------------------------------------------
// Cambiar k0 — verificar que el pico se mueve al bin correcto
// Cambiar N  — ver que la magnitud del pico cambia con él