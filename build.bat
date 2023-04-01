rmdir /s /q build
cmake -B build . -DCMAKE_CONFIGURATION_TYPES=Release -A Win32
cmake --build build --config Release
