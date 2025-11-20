#pragma once
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <unordered_map>

// One batch of tiles using a single texture
struct TileBatch
{
    sf::VertexArray vertices;
    const sf::Texture* texture;
};

// One map layer (e.g., "Floor", "Walls", etc.)
// can contain tiles from multiple tilesets
struct LayerGroup
{
    std::string name;
    std::vector<TileBatch> batches;
};

class MapRenderer : public sf::Drawable
{
public:
    bool load(const std::string& tmxPath);
    void drawLayered(sf::RenderTarget& target,sf::RenderStates renderStates,bool drawAbove) const;
    const std::vector<sf::FloatRect>& getCollisionRects() const;

private:
    std::unordered_map<std::string, sf::Texture> m_tilesetTextures;
    std::vector<LayerGroup> m_layerGroups;
    std::vector<sf::FloatRect> m_collisionRects;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};