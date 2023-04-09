# Platformer

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
