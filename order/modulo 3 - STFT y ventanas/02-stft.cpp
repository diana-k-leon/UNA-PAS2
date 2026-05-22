#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fstream>

const float PI = M_PI;

// ─── ventana Hann ─────────────────────────────────────────────────────────────
std::vector<float> hann(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.5f - 0.5f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

// ─── FFT ──────────────────────────────────────────────────────────────────────
std::vector<std::complex<float>> fft(std::vector<std::complex<float>> signal) {
    int N = signal.size();
    if (N <= 1) return signal;
    std::vector<std::complex<float>> pares(N / 2), impares(N / 2);
    for (int i = 0; i < N / 2; i++) {
        pares[i]   = signal[2 * i];
        impares[i] = signal[2 * i + 1];
    }
    auto FFT_pares   = fft(pares);
    auto FFT_impares = fft(impares);
    std::vector<std::complex<float>> resultado(N);
    for (int k = 0; k < N / 2; k++) {
        std::complex<float> twiddle = std::polar(1.0f, -2.0f * PI * k / N) * FFT_impares[k];
        resultado[k]         = FFT_pares[k] + twiddle;
        resultado[k + N / 2] = FFT_pares[k] - twiddle;
    }
    return resultado;
}

// ─── STFT ─────────────────────────────────────────────────────────────────────
// devuelve un espectrograma: vector de vectores de magnitudes
// cada fila es un instante de tiempo, cada columna es un bin de frecuencia
std::vector<std::vector<float>> stft(const std::vector<float>& signal,
                                      int windowSize,
                                      int hopSize) {
    auto ventana = hann(windowSize);
    std::vector<std::vector<float>> espectrograma;

    for (int start = 0; start + windowSize <= (int)signal.size(); start += hopSize) {
        // extraer y ventanear el tramo
        std::vector<std::complex<float>> frame(windowSize);
        for (int i = 0; i < windowSize; i++)
            frame[i] = std::complex<float>(signal[start + i] * ventana[i], 0.0f);

        // FFT del tramo
        auto espectro = fft(frame);

        // guardar magnitudes — solo mitad útil hasta Nyquist
        std::vector<float> magnitudes(windowSize / 2);
        for (int k = 0; k < windowSize / 2; k++)
            magnitudes[k] = std::abs(espectro[k]) / windowSize;

        espectrograma.push_back(magnitudes);
    }
    return espectrograma;
}

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────────
    int N            = 8192;       // duración de la señal en muestras
    float sampleRate = 44100.0f;
    int windowSize   = 1024;       // probá con 256, 512, 2048
    int hopSize      = 512;        // 50% overlap — probá con 256 (75%)

    // ─── generar señal de prueba ──────────────────────────────────────────────
    // tono de 440 Hz que cambia a 880 Hz a la mitad
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++) {
        float f = (n < N / 2) ? 440.0f : 880.0f;
        signal[n] = std::sin(2.0f * PI * f * n / sampleRate);
    }

    // ─── calcular STFT ────────────────────────────────────────────────────────
    auto espectrograma = stft(signal, windowSize, hopSize);

    int numVentanas   = espectrograma.size();
    int numBins       = windowSize / 2;

    std::cout << "Ventanas calculadas: " << numVentanas << std::endl;
    std::cout << "Bins de frecuencia:  " << numBins << std::endl;
    std::cout << "Resolución espectral: " << sampleRate / windowSize << " Hz por bin" << std::endl;
    std::cout << "Resolución temporal:  " << (float)hopSize / sampleRate * 1000.0f << " ms por ventana" << std::endl;

    // ─── exportar espectrograma a CSV ─────────────────────────────────────────
    // formato: una fila por ventana, una columna por bin
    std::ofstream file("stft.csv");

    // header con frecuencias
    file << "ventana";
    for (int k = 0; k < numBins; k++)
        file << "," << (int)(k * sampleRate / windowSize) << "Hz";
    file << std::endl;

    // datos
    for (int t = 0; t < numVentanas; t++) {
        file << t;
        for (int k = 0; k < numBins; k++)
            file << "," << espectrograma[t][k];
        file << std::endl;
    }
    file.close();

    std::cout << "\nEspectrograma exportado a stft.csv" << std::endl;
    std::cout << "Tip: cambiá windowSize para ver el trade-off tiempo/frecuencia" << std::endl;

    return 0;
}
