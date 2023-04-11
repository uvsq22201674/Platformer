# Platformer

![shot-2023-04-11_19-59-48](https://user-images.githubusercontent.com/113366620/231249359-b00e4a78-bb4d-45fb-8a58-4709ec141a77.jpg)

## What ?
Simple platformer project in C++ using SFML.

## How ?
SFML is required ! Visit : <a>https://www.sfml-dev.org/</a>

Compile using :
<code>
g++ src/*.cpp -I[path to SFML's includes directory] -L[path to SFML's static libs directory] -lsfml-graphics -lsfml-window -lsfml-system
</code>

### N.B
Compiled code might not run if SFML's dynamics librairies aren't in PATH variables nor in the executable's directory !

