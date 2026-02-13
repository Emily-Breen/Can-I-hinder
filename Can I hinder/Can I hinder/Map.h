#pragma once
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <unordered_map>

#include "MathUtils.h"

// One batch of tiles using a single texture
struct TileBatch
{
    sf::VertexArray vertices;
    const sf::Texture* texture;
};
// for the door and controlling when player can traverse to next level - it is absolutely the coolest thing ever!!!!
struct DoorData
{
    sf::FloatRect rect;
    int requiredKeys = 3;
    std::string nextMap;
    sf::Vector2f spawn = { 0.f, 0.f };
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
    const std::string& getMapPath() const;
    void drawLayered(sf::RenderTarget& target,sf::RenderStates renderStates,bool drawAbove) const;
    const std::vector<sf::FloatRect>& getCollisionRects() const;
    const std::vector<DoorData>& getDoors() const;
	sf::Vector2f getFloorSpawn(const sf::Vector2f& entitySize,const sf::Vector2f& avoidPos, float avoidRadius) const;
    static bool rectHitsCollision(const sf::FloatRect& test,const std::vector<sf::FloatRect>& colliders);
    sf::Vector2i worldToTile(const sf::Vector2f& p) const;
    sf::Vector2f tileCenter(int tx, int ty) const;

 
private:
    std::unordered_map<std::string, sf::Texture> m_tilesetTextures;
    std::vector<LayerGroup> m_layerGroups;
    std::vector<sf::FloatRect> m_collisionRects;
    std::string m_mapPath;
    std::vector<DoorData> m_doors;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::vector<uint8_t> m_walkable; // 0 for non-walkable, 1 for walkable
    std::vector<sf::Vector2f> m_floorSpawns;
    sf::Vector2u m_tileSize{ 0,0 };
    sf::Vector2u m_mapSize{ 0,0 };
    float mapScale = 3.f;
};