// ============================================================
// 01-crear-senal.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 1 — Señales digitales y C++ para DSP
// ============================================================
//
// Una señal de audio digital es una lista de números.
// Cada número representa la amplitud de la onda en un
// instante de tiempo — exactamente lo que hace el DAW
// cuando graba audio.
//
// Este ejercicio genera una senoidal de 440 Hz (un LA),
// imprime las primeras muestras en consola y exporta
// la señal a CSV para visualizarla.
//
// Compilar:  g++ -std=c++17 01-crear-senal.cpp -o crear-senal
// Ejecutar:  ./crear-senal
// Graficar:  gnuplot -e "set datafile separator ','; plot 'senal.csv' using 1:2 with lines title 'señal'; pause -1"
// ============================================================

#include <iostream>     // entrada/salida: std::cout para imprimir
#include <vector>       // std::vector: el array dinámico de C++ moderno
#include <fstream>      // std::ofstream: escribir archivos (el CSV)
#include <cmath>        // std::sin, M_PI: funciones matemáticas

int main() {
    // ----------------------------------------------------------
    // PARÁMETROS
    // Estos son los tres números que definen la señal.
    // Experimentá cambiándolos y observando cómo cambia el gráfico.
    // ----------------------------------------------------------
    int N            = 200;      // cantidad de muestras a calcular
                                 // con N=200 a 44100 Hz estás mirando
                                 // 200/44100 = 0.0045 segundos de audio
                                 // probá: 64, 100, 1024

    float sampleRate = 44100.0f; // frecuencia de muestreo en Hz
                                 // le dice al código cuánto "vale" cada casillero en tiempo
                                 // n=1 representa el instante 1/44100 segundos
                                 // estándar de audio — CD usa 44100, estudio usa 48000 o 96000

    float f0         = 440.0f;   // frecuencia de la senoidal en Hz
                                 // 440 = LA, 880 = LA una octava arriba, 220 = LA una abajo
                                 // probá: 880, 220, 1000, 4000
                                 // más Hz = onda oscila más rápido = tono más agudo

    // ----------------------------------------------------------
    // CREAR LA SEÑAL
    // std::vector<float> es una lista dinámica de números flotantes.
    // Reservamos N casilleros — uno por cada muestra.
    // En JUCE esto se convierte en AudioBuffer<float> — misma idea.
    // ----------------------------------------------------------
    std::vector<float> signal(N);

    for (int n = 0; n < N; n++) {
        // Para cada casillero n calculamos la amplitud de la onda en ese instante.
        // La fórmula descompone así:
        //   n / sampleRate       → en qué segundo estoy (ej: n=1 → 0.0000226 seg)
        //   f0 * tiempo          → cuántos ciclos completó la onda hasta ese instante
        //   2π * ciclos          → convertir ciclos a radianes (una vuelta = 2π radianes)
        //   sin(angulo)          → la altura de la onda en ese punto, siempre entre -1 y 1
        signal[n] = std::sin(2.0f * M_PI * f0 * n / sampleRate);
    }

    // ----------------------------------------------------------
    // EXPLORAR LOS VALORES
    // Imprimimos los valores para ver que son números reales.
    // signal[0] siempre es 0 — sin(0) = 0, la onda arranca en cero.
    // Los valores oscilan entre -1.0 y 1.0.
    // ----------------------------------------------------------
    std::cout << "Primeras 10 muestras:" << std::endl;
    for (int n = 0; n < 10; n++) {
        std::cout << "signal[" << n << "] = " << signal[n] << std::endl;
    }

    // ----------------------------------------------------------
    // EXPORTAR CSV
    // Guardamos la señal en un archivo de texto con formato CSV
    // para poder visualizarla con gnuplot.
    // Formato: n (índice de muestra), amplitud (valor entre -1 y 1)
    // Nota: cuando trabajemos con JUCE esto desaparece —
    // visualizamos directo en el plugin con paint().
    // ----------------------------------------------------------
    // exportar CSV — sin header para simplificar el comando de gnuplot
    // gnuplot -e "plot 'senal.csv' with lines; pause -1"
    std::ofstream file("senal.csv");
    for (int n = 0; n < N; n++) {
        file << n << "," << signal[n] << std::endl;
    }
    file.close();

    std::cout << "\nExportado a senal.csv" << std::endl;
    std::cout << "Para graficar corré el comando de gnuplot del header." << std::endl;
    return 0;
}