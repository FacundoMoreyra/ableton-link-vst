FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-22.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # Core Build Tools
    build-essential cmake git pkg-config \
    # Official JUCE Dependencies
    libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype6-dev libfontconfig1-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev \
    # Explicit GTK fix
    libgtk-3-dev \
    # Audio Utilities
    alsa-utils libasound2-plugins pulseaudio-utils

WORKDIR /workspaces/