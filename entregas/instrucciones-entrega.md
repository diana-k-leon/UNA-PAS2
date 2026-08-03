# Entrega del trabajo final — Instrucciones

El repositorio del curso es:
https://github.com/diana-k-leon/UNA-PAS2

La entrega se hace mediante un **fork + Pull Request**: no necesitás pedir permisos a nadie, subís tus archivos a tu propia copia del repo y después mandás un Pull Request para que quede incorporado al repositorio del curso.

---

## Opción 1 — Ya tenés el repo clonado (venimos trabajando con él en clase)

1. Entrar a la página del repositorio: https://github.com/diana-k-leon/UNA-PAS2
2. Hacer clic en el botón **"Fork"** (arriba a la derecha), para crear una copia en tu cuenta de GitHub.
3. Abrir una terminal en la carpeta donde ya tenés clonado el repo `UNA-PAS2`.
4. Agregar tu fork como un nuevo remoto (reemplazando `SU_USUARIO` por tu usuario de GitHub):
   ```
   git remote add mifork https://github.com/SU_USUARIO/UNA-PAS2.git
   ```
5. Asegurarte de tener la última versión del repo original:
   ```
   git pull origin main
   ```
6. Entrar a la carpeta `entregas`:
   ```
   cd entregas
   ```
7. Crear una carpeta con tu **Nombre y Apellido** (sin espacios, usando guion o guion bajo, por ejemplo `Juan_Perez`):
   ```
   mkdir Juan_Perez
   ```
8. Dentro de esa carpeta, colocar:
   - El **plugin VST3** compilado (para poder probarlo en Reaper).
   - El **repositorio con los archivos fuente** del proyecto (código completo, no solo el ejecutable).
9. Agregar los cambios, hacer commit y subirlos a **tu fork** (no al repo original):
   ```
   git add .
   git commit -m "Entrega final - Juan Perez"
   git push mifork main
   ```
10. Ir a tu fork en GitHub (`https://github.com/SU_USUARIO/UNA-PAS2`). Va a aparecer un botón **"Contribute"** o **"Compare & pull request"**. Hacer clic ahí.
11. Completar el título del Pull Request (por ejemplo "Entrega Juan Perez") y hacer clic en **"Create pull request"**.

---

## Opción 2 — Nunca clonaste el repo

1. Entrar a la página del repositorio: https://github.com/diana-k-leon/UNA-PAS2
2. Hacer clic en el botón **"Fork"** (arriba a la derecha), para crear una copia en tu cuenta de GitHub.
3. Abrir una terminal desde donde se pueda usar git.
4. Posicionarse en la carpeta en la que se desea clonar el repo.
5. Ejecutar (reemplazando `SU_USUARIO` por tu usuario de GitHub):
   ```
   git clone https://github.com/SU_USUARIO/UNA-PAS2.git
   ```
6. Entrar a la carpeta del repo y luego a la carpeta `entregas`:
   ```
   cd UNA-PAS2/entregas
   ```
7. Crear una carpeta con tu **Nombre y Apellido** (sin espacios, usando guion o guion bajo, por ejemplo `Juan_Perez`):
   ```
   mkdir Juan_Perez
   ```
8. Dentro de esa carpeta, colocar:
   - El **plugin VST3** compilado (para poder probarlo en Reaper).
   - El **repositorio con los archivos fuente** del proyecto.
9. Agregar los cambios, hacer commit y subirlos a tu fork:
   ```
   git add .
   git commit -m "Entrega final - Juan Perez"
   git push origin main
   ```
10. Ir a tu repositorio en GitHub (tu fork, `https://github.com/SU_USUARIO/UNA-PAS2`). Va a aparecer un botón **"Contribute"** o **"Compare & pull request"**. Hacer clic ahí.
11. Completar el título del Pull Request (por ejemplo "Entrega Juan Perez") y hacer clic en **"Create pull request"**.

---

✅ En ambos casos, la docente va a revisar el Pull Request y lo va a aprobar (merge) para incorporarlo al repositorio del curso.

⚠️ **Importante:**
- Usar siempre el nombre de la carpeta como **Nombre_Apellido**, sin espacios.
- No modificar ni borrar archivos de otros compañeros.
- Si tenés dudas o algún comando da error, consultar antes de la fecha límite de entrega.
