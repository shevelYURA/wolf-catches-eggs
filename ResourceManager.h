#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <map>
#include <vector>
#include <filesystem>
#include "resource.h"

using namespace sf;
using namespace std;

class ResourceManager {
private:
    static Texture& getFallbackTexture() {
        static Texture fallback;
        static bool initialized = false;
        if (!initialized) {
            Image img(Vector2u(64, 64), Color::Magenta);

            for (unsigned int i = 0; i < 64; i++) {
                img.setPixel(Vector2u(i, i), Color::Black);
                img.setPixel(Vector2u(i, 63 - i), Color::Black);
            }

            for (unsigned int i = 0; i < 64; i++) {
                img.setPixel(Vector2u(i, 0), Color::White);
                img.setPixel(Vector2u(i, 63), Color::White);
                img.setPixel(Vector2u(0, i), Color::White);
                img.setPixel(Vector2u(63, i), Color::White);
            }

            if (!fallback.loadFromImage(img)) {
                Image simpleImg(Vector2u(64, 64), Color::Magenta);
                bool backupResult = fallback.loadFromImage(simpleImg);
            }
            initialized = true;
        }
        return fallback;
    }

    static Font& getFallbackFont() {
        static Font fallback;
        static bool initialized = false;
        if (!initialized) {
            vector<string> fontPaths = {
                "C:/Windows/Fonts/arial.ttf",
                "C:/Windows/Fonts/segoeui.ttf",
                "C:/Windows/Fonts/calibri.ttf",
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                "/System/Library/Fonts/Helvetica.ttc"
            };

            bool loaded = false;
            for (const auto& path : fontPaths) {
                if (filesystem::exists(path)) {
                    if (fallback.openFromFile(path)) {
                        loaded = true;
                        break;
                    }
                }
            }

            initialized = true;
        }
        return fallback;
    }

    static string getResourceFileName(int resourceId) {
        static map<int, string> fileNames = {
            {IDB_PNG1, "wolf.png"},
            {IDB_PNG2, "basket.png"},
            {IDB_PNG3, "bomb.png"},
            {IDB_PNG4, "boss.png"},
            {IDB_PNG5, "egg.png"},
            {IDB_PNG6, "icon.png"},
        {IDB_POWERUP, "powerup.png"}
        };

        auto it = fileNames.find(resourceId);
        if (it != fileNames.end()) {
            return it->second;
        }
        return "resource_" + to_string(resourceId) + ".png";
    }

    static bool tryLoadFromFile(Texture& texture, const string& filename) {
        vvector<string> searchPaths = {
        filename,
        "assets/" + filename,
        "../assets/" + filename,
        "../../assets/" + filename,
        "resources/" + filename,
        "../resources/" + filename,
        "images/" + filename,           // ← ДОБАВИТЬ
        "../images/" + filename,        // ← ДОБАВИТЬ
        "../../images/" + filename      // ← ДОБАВИТЬ
        };

        for (const auto& path : searchPaths) {
            if (filesystem::exists(path)) {
                if (texture.loadFromFile(path)) {
                    return true;
                }
            }
        }
        return false;
    }

    static bool tryLoadFromResource(Texture& texture, int resourceId) {
        HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceId), L"PNG");
        if (!hRes) {
            hRes = FindResource(NULL, MAKEINTRESOURCE(resourceId), RT_RCDATA);
        }
        if (!hRes) {
            return false;
        }

        HGLOBAL hData = LoadResource(NULL, hRes);
        if (!hData) {
            return false;
        }

        void* pData = LockResource(hData);
        DWORD size = SizeofResource(NULL, hRes);

        if (!pData || size == 0) {
            return false;
        }

        return texture.loadFromMemory(pData, size);
    }

    static bool tryLoadFontFromResource(Font& font) {
        HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(FONT_ARCADE), RT_FONT);
        if (!hRes) {
            hRes = FindResource(NULL, MAKEINTRESOURCE(FONT_ARCADE), L"RCDATA");
        }
        if (!hRes) {
            hRes = FindResource(NULL, L"FONT_ARCADE", RT_FONT);
        }
        if (!hRes) {
            return false;
        }

        HGLOBAL hData = LoadResource(NULL, hRes);
        if (!hData) {
            return false;
        }

        void* pData = LockResource(hData);
        DWORD size = SizeofResource(NULL, hRes);

        if (!pData || size == 0) {
            return false;
        }

        return font.openFromMemory(pData, size);
    }

    static bool tryLoadFontFromFile(Font& font, const string& filename) {
        vector<string> searchPaths = {
            filename,
            "assets/" + filename,
            "../assets/" + filename,
            "../../assets/" + filename,
            "resources/" + filename,
            "../resources/" + filename,
            "assets/fonts/" + filename,
            "../assets/fonts/" + filename
        };

        for (const auto& path : searchPaths) {
            if (filesystem::exists(path)) {
                if (font.openFromFile(path)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    static Texture& getTexture(int resourceId) {
        static map<int, Texture> textures;

        if (textures.find(resourceId) == textures.end()) {
            Texture texture;
            bool loaded = false;
            string fileName = getResourceFileName(resourceId);

            if (tryLoadFromResource(texture, resourceId)) {
                loaded = true;
            }

            if (!loaded) {
                if (tryLoadFromFile(texture, fileName)) {
                    loaded = true;
                }
            }

            if (!loaded) {
                textures[resourceId] = getFallbackTexture();
            }
            else {
                textures[resourceId] = move(texture);
            }
        }
        return textures[resourceId];
    }

    static Font& getFont(int /*resourceId*/) {
        static Font font;
        static bool loaded = false;

        if (!loaded) {
            if (tryLoadFontFromResource(font)) {
                loaded = true;
            }

            if (!loaded) {
                vector<string> fontNames = {
                    "arcade.ttf",
                    "Arcade.ttf",
                    "ARCADE.TTF",
                    "arcade_classic.ttf",
                    "ARCADECLASSIC.TTF"
                };

                for (const auto& fontName : fontNames) {
                    if (tryLoadFontFromFile(font, fontName)) {
                        loaded = true;
                        break;
                    }
                }
            }

            if (!loaded) {
                font = getFallbackFont();
            }
        }
        return font;
    }

    static void preloadAllTextures() {
        vector<int> textureIds = {
            IDB_PNG1, IDB_PNG2, IDB_PNG3,
            IDB_PNG4, IDB_PNG5, IDB_PNG6,
        IDB_POWERUP
        };

        for (int id : textureIds) {
            getTexture(id);
        }
    }
};
