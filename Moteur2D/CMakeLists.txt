cmake_minimum_required(VERSION 3.15)
project(Dave_The_Miners)

# 1. Standard C++ (Requis pour LDtk et filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Configuration SFML 3.1.0
set(SFML_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-3.1.0")
include_directories("${SFML_PATH}/include")
link_directories("${SFML_PATH}/lib")

# 3. Configuration LDtkLoader
# On ajoute le sous-répertoire. Vérifie que le nom du dossier est exact.
add_subdirectory(libs/LDtkLoader-1.5.3.1)

# 4. Recherche automatique des fichiers
# On scanne 'include' pour les .hpp et 'src' pour les .cpp
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 5. Création de l'exécutable
# On regroupe tout (Sources + Headers) pour qu'ils apparaissent dans Visual Studio
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 6. Gestion des chemins d'inclusion (HEADERS)
# Cela permet de faire : #include "Core/mon_fichier.hpp"
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Si tu as encore des .hpp dans le dossier src, on l'ajoute aussi par sécurité
target_include_directories(${PROJECT_NAME} PRIVATE src)

# 7. Lien avec les bibliothèques
# LDtkLoader::LDtkLoader est la cible officielle de la lib
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    LDtkLoader::LDtkLoader
)

# --- NOTE POUR LE DEBUG ---
# Si tu as des erreurs de lien (LNK2019) en mode Debug, 
# il faudra peut-être renommer les libs SFML en 'sfml-graphics-d', etc.