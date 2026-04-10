# LinkBridge VST Plugin

Plugin VST que lee el tempo y estado de reproducción del DAW y lo transmite via Ableton Link a otras aplicaciones en la red local.

**Autor:** Facundo Moreyra

## Características

- Lee BPM en tiempo real del DAW via AudioPlayHead
- Detecta estado Play/Stop del transporte
- Transmite tempo y estado via Ableton Link
- Sincronización de fase al iniciar reproducción
- Re-sincronización automática al cambiar tempo
- Muestra peers conectados en la sesión Link

## Requisitos

### Build
- CMake 3.22+
- Compilador C++17 (GCC 11+, Clang, MSVC 2019+)

### Linux
- GTK3
- ALSA

```bash
sudo apt install libgtk-3-dev libasound2-dev
```

### Windows
- Visual Studio 2022 (Community o superior) con "Desktop development with C++"
- CMake (incluido con Visual Studio o instalación separada)

## Compilación

### 1. Clonar con submodulos

```bash
git clone --recursive https://github.com/tu-usuario/ableton-link-vst.git
cd ableton-link-vst
```

Si ya clonaste sin `--recursive`:
```bash
git submodule update --init --recursive
```

### 2. Build en Linux

```bash
cd link-bridge
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j$(nproc)
```

Los artefactos se generan en:
- `build/LinkBridge_artefacts/Release/VST3/LinkBridge.vst3`
- `build/LinkBridge_artefacts/Release/Standalone/LinkBridge`

### 3. Build en Windows

#### Opción A: Línea de comandos (Developer Command Prompt)

```powershell
cd link-bridge
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

#### Opción B: Visual Studio GUI

1. Abrir Visual Studio 2022
2. Archivo → Abrir → CMake...
3. Seleccionar `link-bridge/CMakeLists.txt`
4. Esperar que CMake configure el proyecto
5. Compilar → Compilar todo (Ctrl+Shift+B)
6. Seleccionar configuración "Release" para el build final

Los artefactos se generan en:
- `build/LinkBridge_artefacts/Release/VST3/LinkBridge.vst3`
- `build/LinkBridge_artefacts/Release/Standalone/LinkBridge.exe`

## Instalación

### Linux
```bash
cp -r build/LinkBridge_artefacts/Release/VST3/LinkBridge.vst3 ~/.vst3/
```

### Windows
Copiar la carpeta `LinkBridge.vst3` a:
- `C:\Program Files\Common Files\VST3\` (todos los usuarios)
- `%LOCALAPPDATA%\Programs\Common\VST3\` (usuario actual)

## Uso

1. Carga el plugin en cualquier pista de tu DAW
2. El plugin mostrará:
   - BPM actual del proyecto
   - Estado Play/Stop
   - Conexión Link y peers conectados
3. Las aplicaciones con Ableton Link en tu red local recibirán el tempo y estado

## Limitaciones

- El tempo es **unidireccional** (DAW → Link). No se puede cambiar el tempo del DAW desde otras apps Link.
- Algunos DAWs pueden no reportar BPM si el proyecto no tiene un tempo track configurado.
- El estado Play/Stop puede tener algunos milisegundos de latencia.

## Estructura del Proyecto

```
link-bridge/
├── CMakeLists.txt          # Configuración del build
├── Source/
│   ├── PluginProcessor.h   # Procesador con AudioPlayHead + Link
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h      # Editor GUI nativa
│   └── PluginEditor.cpp
└── README.md
```

## Licencia

MIT
