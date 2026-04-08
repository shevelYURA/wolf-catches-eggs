#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <map>
#include <vector>
#include <iostream>
#include "resource.h"

class ResourceManager {
private:
    static void logError(const std::string& message) {
#ifdef _DEBUG
        MessageBoxA(NULL, message.c_str(), "Resource Error", MB_OK | MB_ICONERROR);
#endif
        std::cerr << "ERROR: " << message << std::endl;
    }

    static sf::Texture& getFallbackTexture() {
        static sf::Texture fallback;
        static bool initialized = false;
        if (!initialized) {
            // SFML 3: Image создаётся через конструктор с размерами
            sf::Image img(sf::Vector2u(32, 32), sf::Color::Magenta);

            // Добавим чёрный крест для заметности (SFML 3 синтаксис)
            for (unsigned int i = 0; i < 32; i++) {
                img.setPixel(sf::Vector2u(i, i), sf::Color::Black);
                img.setPixel(sf::Vector2u(i, 31 - i), sf::Color::Black);
            }

            if (!fallback.loadFromImage(img)) {
                logError("Failed to create fallback texture");
            }
            initialized = true;
        }
        return fallback;
    }

    static sf::Font& getFallbackFont() {
        static sf::Font fallback;
        static bool initialized = false;
        if (!initialized) {
            if (!fallback.openFromFile("C:/Windows/Fonts/arial.ttf")) {
                logError("Failed to load fallback font! Text may not be visible.");
            }
            initialized = true;
        }
        return fallback;
    }

public:
    static sf::Texture& getTexture(int resourceId) {
        static std::map<int, sf::Texture> textures;

        if (textures.find(resourceId) == textures.end()) {
            HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceId), L"PNG");
            if (!hRes) {
                logError("Texture resource not found for ID: " + std::to_string(resourceId));
                textures[resourceId] = getFallbackTexture();
                return textures[resourceId];
            }

            HGLOBAL hData = LoadResource(NULL, hRes);
            if (!hData) {
                logError("Failed to load texture data for ID: " + std::to_string(resourceId));
                textures[resourceId] = getFallbackTexture();
                return textures[resourceId];
            }

            void* pData = LockResource(hData);
            DWORD size = SizeofResource(NULL, hRes);

            if (!pData || size == 0) {
                logError("Invalid texture data for ID: " + std::to_string(resourceId));
                textures[resourceId] = getFallbackTexture();
                return textures[resourceId];
            }

            if (!textures[resourceId].loadFromMemory(pData, size)) {
                logError("Failed to load texture from memory for ID: " + std::to_string(resourceId));
                textures[resourceId] = getFallbackTexture();
            }
        }
        return textures[resourceId];
    }

    static sf::Font& getFont(int /*resourceId*/) {
        static sf::Font font;
        static bool loaded = false;
        static std::vector<char> fontData;

        if (!loaded) {
            HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(FONT_ARCADE), RT_FONT);
            if (!hRes) {
                hRes = FindResource(NULL, MAKEINTRESOURCE(FONT_ARCADE), L"RCDATA");
            }
            if (!hRes) {
                hRes = FindResource(NULL, L"FONT_ARCADE", RT_FONT);
            }

            if (!hRes) {
                logError("Font resource not found for ID: " + std::to_string(FONT_ARCADE));
                return getFallbackFont();
            }

            HGLOBAL hData = LoadResource(NULL, hRes);
            if (!hData) {
                logError("Failed to load font data");
                return getFallbackFont();
            }

            void* pData = LockResource(hData);
            DWORD size = SizeofResource(NULL, hRes);

            if (!pData || size == 0) {
                logError("Invalid font data");
                return getFallbackFont();
            }

            fontData.assign((char*)pData, (char*)pData + size);

            if (!font.openFromMemory(fontData.data(), fontData.size())) {
                logError("Failed to load font from memory");
                return getFallbackFont();
            }

            loaded = true;
        }
        return font;
    }
};