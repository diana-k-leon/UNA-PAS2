// ============================================================
// 01-pitch-shift.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 5 — Pitch Shifting: efecto ardillita
// ============================================================
//
// Subir y bajar el pitch de una sinusoide:
//   1. Generar 440 Hz
//   2. FFT
//   3. Mover bins × factor (2.0 = subir octava, 0.5 = bajar octava)
//   4. Reconstruir espejo
//   5. IFFT
//   6. Exportar y comparar espectros
//
// Compilar:  g++ -std=c++17 01-pitch-shift.cpp ../dsp_utils.cpp -o pitch-shift
// Ejecutar:  ./pitch-shift
//
// Graficar espectros:
// gnuplot -e "set datafile separator ','; set title 'Pitch Shift: espectro original vs shifted'; set xrange [0:3000]; plot 'pitch-shift-espectro.csv' using 1:2 with lines lw 2 title 'original (440 Hz)', 'pitch-shift-espectro.csv' using 1:3 with lines lw 2 title 'shifted (880 Hz)'; pause -1"
//
// Graficar señales:
// gnuplot -e "set datafile separator ','; set title 'Pitch Shift: señal original vs shifted'; set xrange [0:200]; plot 'pitch-shift.csv' using 1:2 with lines lw 2 title 'original', 'pitch-shift.csv' using 1:3 with lines lw 2 title 'shifted'; pause -1"
//
// ============================================================

#include "../dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────
    int N            = 2048;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;
    float factor     = 2.0f;        // 2.0 = subir octava, 0.5 = bajar octava

    std::cout << "=== Pitch Shifting ===" << std::endl;
    std::cout << "Señal: " << f0 << " Hz" << std::endl;
    std::cout << "Factor: " << factor << " (resultado esperado: " << f0 * factor << " Hz)" << std::endl;
    std::cout << std::endl;

    // ─── paso 1: generar señal ────────────────────────────────────────────
    std::vector<std::complex<float>> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::complex<float>(std::sin(2.0f * PI * f0 * n / sampleRate), 0.0f);

    std::cout << "✓ Señal generada: " << f0 << " Hz" << std::endl;

    // ─── paso 2: FFT ──────────────────────────────────────────────────────
    auto espectro = fft(signal);

    std::cout << "✓ FFT calculada" << std::endl;

    // ─── paso 3: mover bins ───────────────────────────────────────────────
    std::vector<std::complex<float>> nuevo(N, {0.0f, 0.0f});

    for (int k = 0; k < N / 2; k++) {
        int nuevo_k = (int)(k * factor);
        if (nuevo_k < N / 2)
            nuevo[nuevo_k] = espectro[k];
    }

    std::cout << "✓ Bins desplazados × " << factor << std::endl;

    // ─── paso 4: reconstruir espejo ───────────────────────────────────────
    // La segunda mitad del espectro es el conjugado de la primera (señal real)
    for (int k = 1; k < N / 2; k++)
        nuevo[N - k] = std::conj(nuevo[k]);

    std::cout << "✓ Espejo reconstruido" << std::endl;

    // ─── paso 5: IFFT ─────────────────────────────────────────────────────
    auto resultado = ifft(nuevo);

    std::cout << "✓ IFFT calculada" << std::endl;

    // ─── paso 6: calcular espectros para comparar ─────────────────────────
    // magnitudes de la original
    std::vector<float> sigReal(N);
    for (int n = 0; n < N; n++)
        sigReal[n] = signal[n].real();
    auto magOriginal = magnitudes(sigReal);

    // magnitudes de la shifted
    std::vector<float> shiftedReal(N);
    for (int n = 0; n < N; n++)
        shiftedReal[n] = resultado[n].real();
    auto magShifted = magnitudes(shiftedReal);

    // ─── paso 7: exportar ─────────────────────────────────────────────────

    // señales en el tiempo
    std::ofstream file1("pitch-shift.csv");
    file1 << "sample,original,shifted" << std::endl;
    for (int n = 0; n < N; n++)
        file1 << n << "," << signal[n].real() << "," << resultado[n].real() << std::endl;
    file1.close();

    // espectros
    std::ofstream file2("pitch-shift-espectro.csv");
    file2 << "frecuencia_hz,original,shifted" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file2 << freq << "," << magOriginal[k] << "," << magShifted[k] << std::endl;
    }
    file2.close();

    std::cout << "✓ Exportado a pitch-shift.csv y pitch-shift-espectro.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Qué esperar:" << std::endl;
    std::cout << "- Original: pico en " << f0 << " Hz" << std::endl;
    std::cout << "- Shifted: pico en " << f0 * factor << " Hz" << std::endl;
    std::cout << std::endl;
    std::cout << "Probá cambiando factor a 0.5, 1.5, 3.0, etc." << std::endl;

    return 0;
}
