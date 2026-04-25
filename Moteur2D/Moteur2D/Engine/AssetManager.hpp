#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <unordered_map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <XkLogs.hpp>

namespace xk {
	template<typename T>
	struct AssetManager
	{
		static std::unordered_map<std::string, T> data;

		static T& Load(const std::string& name, const std::string& filepath) {
			auto it = data.find(name);

			if (it != data.end()) {
				return it->second;
			}

			T ressource;
			if (ressource.loadFromFile(filepath)) {
				data[name] = ressource;
				XK_SUCCESS("AssetManager : Ressource '{0}' chargée depuis [{1}]", name, filepath);
				return data[name];
			}

			XK_ERROR("AssetManager : Impossible de charger '{0}' à l'adresse : {1}", name, filepath);
			return GetDefault();
		}

		static T& Get(const std::string& name) {
			auto it = data.find(name);
			if (it != data.end()) {
				return it->second;
			}

			XK_WARN("AssetManager : Tentative d'accès à une ressource inexistante : '{0}'. Renvoi du fallback.", name);
			return GetDefault();
		}

		static T& GetDefault() {
			static T fallback;
			return fallback;
		}

		static void unloadALL() {
			size_t count = data.size();
			if (count > 0) {
				data.clear();
				XK_INFO("AssetManager : Nettoyage de {0} ressources effectue.", count);
			}
		}
	};

	template<typename T>
	std::unordered_map<std::string, T> AssetManager<T>::data;

	template<>
	inline sf::Texture& AssetManager<sf::Texture>::GetDefault() {
		static sf::Texture fallbackTexture;
		static bool init = false;
		sf::Vector2u fallbackSize = { 64,64 };
		sf::Image fallbackImage(fallbackSize, sf::Color::Black);
		if (!init) {
			for (int y = 0; y < fallbackSize.y; ++y) {
				for (int x = 0; x < fallbackSize.x; ++x) {
					if (((x / 32) + (y / 32)) % 2 != 0) {
						fallbackImage.setPixel(sf::Vector2u(x, y), sf::Color::Magenta);
					}
				}
			}
			fallbackTexture.loadFromImage(fallbackImage);
			fallbackTexture.setRepeated(true);
			init = true;
		}
		return fallbackTexture;
	}
}

#endif