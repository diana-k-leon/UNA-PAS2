// ============================================================
// dsp_utils.h
// Programación Aplicada al Sonido II — UNA
// Utilidades compartidas: FFT, espectro en dB, exportar CSV
// ============================================================
//
// Declaraciones. La implementación está en dsp_utils.cpp.
// En tus ejercicios:
//   #include "dsp_utils.h"
//
// Compilar junto con dsp_utils.cpp:
//   g++ -std=c++17 tu_ejercicio.cpp dsp_utils.cpp -o tu_ejercicio
//
// ============================================================

#ifndef DSP_UTILS_H
#define DSP_UTILS_H

#include <vector>
#include <complex>
#include <string>

extern const float PI;

// ─── FFT (Cooley-Tukey, N debe ser potencia de 2) ────────────────────────────
std::vector<std::complex<float>> fft(std::vector<std::complex<float>> signal);

// ─── aplicar ventana ──────────────────────────────────────────────────────────
std::vector<float> aplicarVentana(const std::vector<float>& signal,
                                   const std::vector<float>& ventana);

// ─── magnitudes de una señal real ─────────────────────────────────────────────
std::vector<float> magnitudes(const std::vector<float>& signal);

// ─── espectro de una ventana en dB (estilo Xavier Serra) ──────────────────────
struct WindowSpectrum {
    std::vector<float> dB;
    int mainLobeWidth;
    float highestSidelobe;
};

WindowSpectrum windowSpectrum(const std::vector<float>& ventana, int fftSize);

// ─── exportar espectro de ventana a CSV ───────────────────────────────────────
void exportarWindowSpectrum(const std::string& filename,
                            const std::string& columnName,
                            const WindowSpectrum& ws,
                            int M, int fftSize, int rangeBins = 22);

#endif // DSP_UTILS_H
