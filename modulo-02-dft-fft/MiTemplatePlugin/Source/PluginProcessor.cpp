// ============================================================
// PluginProcessor.cpp — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// IMPLEMENTACIÓN del Processor.
//
// Todo el DSP vive en processBlock().
// El resto es setup y boilerplate.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
// Se ejecuta UNA sola vez cuando Reaper carga el plugin.
// Acá reservamos memoria para todos los buffers.
// IMPORTANTE: nunca reservar memoria en processBlock — causa glitches.
// Es boilerplate — siempre es igual.
MiTemplatePluginAudioProcessor::MiTemplatePluginAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
        // entrada estéreo, salida estéreo
{   
    // Crea un vector de 2048 ceros. 
    // Acá vamos a ir guardando los samples que llegan del DAW, de a uno, 
    // hasta tener 2048. *1 Por que 2048 ?
    buffer_acumulador.resize(fft_size, 0.0f);
    // 2048 posiciones en cero — acá van los samples del DAW

    // Crea un vector de 1024 ceros. Acá van las magnitudes después de la FFT. 
    // Solo 1024 porque la segunda mitad del espectro es espejo. 
    // Este vector es el que lee el Editor para dibujar.
    spectrum.resize(fft_size / 2, 0.0f);
    // 1024 posiciones — solo hasta Nyquist (la mitad)
    
    
    // Crea un vector de 4096 ceros. porque crea esto? *2
    fft_buffer.resize(2 * fft_size, 0.0f);
    // 4096 posiciones — JUCE necesita el doble de espacio
}
/* Fin del constructor. Todo lo que reservamos acá queda disponible para siempre. 
Por eso lo hacemos acá y no en processBlock — si reservás memoria en processBlock, 
el audio se corta.
 */

 
// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
// Destructor vacío. Los vectores se liberan solos cuando el plugin se cierra.
MiTemplatePluginAudioProcessor::~MiTemplatePluginAudioProcessor() {}


/* Dos funciones que JUCE obliga a implementar. 
prepareToPlay se llama cuando Reaper está por empezar a mandar audio 
(te dice el sample rate y el tamaño de bloque). 
releaseResources cuando para.
En este plugin no necesitamos hacer nada en ninguna de las dos.*/

// ─── PREPARE TO PLAY ──────────────────────────────────────────────────────────
void MiTemplatePluginAudioProcessor::prepareToPlay(double, int) {}
// el DAW avisa a qué sample rate va a correr
// en este plugin no necesitamos hacer nada acá

// ─── RELEASE RESOURCES ────────────────────────────────────────────────────────
void MiTemplatePluginAudioProcessor::releaseResources() {}
// se llama al cerrar — nada que liberar


// ─── PROCESS BLOCK — EL CORAZÓN DEL PLUGIN ───────────────────────────────────
// El DAW llama a esta función ~86 veces por segundo.
// Cada vez manda un bloque de ~512 samples de audio.
//                                                        
// ║  Los pasos son:                                          ║
// ║  1. Leer samples del DAW                                 ║
// ║  2. Acumular hasta tener fft_size (2048)                 ║
// ║  3. Copiar al buffer de JUCE                             ║
// ║  4. Calcular FFT                                         ║
// ║  5. Extraer magnitudes → spectrum[]                      ║
// ║  6. Resetear y empezar de nuevo                          ║
// ╚══════════════════════════════════════════════════════════╝
//
void MiTemplatePluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer&)
{
    // TODO:
}

// ─── CREAR EDITOR ─────────────────────────────────────────────────────────────
// Crea la ventana del plugin (el Editor). Le pasa *this — 
// una referencia al Processor — para que el Editor pueda leer spectrum[].
juce::AudioProcessorEditor* MiTemplatePluginAudioProcessor::createEditor()
{
    return new MiTemplatePluginAudioProcessorEditor(*this);
    // *this = le pasa una referencia al Processor
    // el Editor la guarda para leer spectrum[]
}

// ─── PUNTO DE ENTRADA ─────────────────────────────────────────────────────────
// Punto de entrada. JUCE llama esta función para crear el plugin. 
// Siempre es igual, no se toca.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MiTemplatePluginAudioProcessor();
    // JUCE llama esto para crear el plugin — siempre es igual
}

// ============================================================
// ============================================================

/* *1 - Por qué 2048 ?
Porque es un balance entre resolución y velocidad:
Tiene que ser potencia de 2 (la FFT lo requiere): 256, 512, 1024, 2048, 4096...
Muy chico (256, 512): pocos bins, resolución en frecuencia mala — no distinguís notas cercanas
Muy grande (4096, 8192): muchos bins pero tarda más, se actualiza menos veces por segundo
2048: resolución de ~21 Hz por bin (44100/2048), suficiente para distinguir notas, y se calcula en menos de 1ms
*/

/* porque crea esto? *2
JUCE necesita el doble de espacio: las primeras 2048 posiciones para tu señal, 
las segundas 2048 para ceros.Después de la FFT,
acá quedan los resultados en formato intercalado (real, imaginario, real, imaginario...).

Es porque performRealOnlyForwardTransform de JUCE necesita un array más grande que tu señal. Le pasás un array de 4096 floats:
Antes de la FFT:

Posiciones 0 a 2047: tu señal (los 2048 samples)
Posiciones 2048 a 4095: ceros (espacio vacío que JUCE necesita para trabajar)

Después de la FFT:

JUCE sobreescribe ese mismo array con el resultado
El resultado son números complejos (cada uno tiene parte real + parte imaginaria)
Los pone intercalados: [real₀, imag₀, real₁, imag₁, real₂, imag₂, ...]
1024 números complejos × 2 valores cada uno = 2048 floats que usa para los resultados... pero JUCE pide el array de tamaño 4096 porque así fue diseñada la función

Es un detalle de cómo JUCE implementó su FFT — no es algo conceptual que tengas que 
entender en profundidad. Lo importante es: le das un array del doble de tamaño, ponés tu señal al principio, ceros al final, y JUCE hace su magia.
*/