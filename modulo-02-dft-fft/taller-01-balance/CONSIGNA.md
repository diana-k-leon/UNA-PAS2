# Taller 01 · Balance L/R

## Objetivo
Agregar un slider de balance L/R al GainRMS plugin.

## Qué tiene que hacer el slider
- Rango: -1.0 (todo izquierdo) a +1.0 (todo derecho)
- Centro (0.0): audio igual en los dos canales
- El cambio tiene que escucharse en tiempo real en Reaper

## Archivos a modificar
- `PluginProcessor.h` — agregar la variable `balance`
- `PluginProcessor.cpp` — usar `balance` en `processBlock()`
- `PluginEditor.h` — declarar el slider
- `PluginEditor.cpp` — crear el slider y conectarlo al Processor

## Entrega
Subir tu versión modificada a `entregas/tu-nombre/taller-01/`
