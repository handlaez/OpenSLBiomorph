# Biomorph Generator

A tiny, early-stage C/C++ project for generating \[Biomorphs](https://en.wikipedia.org/wiki/Pickover\_stalk#Biomorphs) according to specific mathematical parameters. 
Currently, the project runs headlessly. It processes the given parameters, renders the biomorph using OpenSL, and exports the result directly to an image file.

## Current Features

* **Parameter-based Generation:** Define the genetic parameters of your biomorph to influence its shape and structure.
* **OpenSL Rendering:** Uses OpenSL to handle the generation. (it would take absurd amounts of time otherwise)
* **Image Export:** Utilizes the lightweight `stb\_image\_write` library to export the generated biomorphs straight to disk (e.g., as PNG/BMP).
* **Headless:** CLI-only at the moment, making it fast and lightweight.

## Dependencies
To build and run this project, you will need:
* A standard C/C++ compiler
* **OpenSL** (Ensure you have the development headers installed)

## Building and Running

I'll make some instructions once the project is in good-enough state.
