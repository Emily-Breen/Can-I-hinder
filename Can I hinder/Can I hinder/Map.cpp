#include "Map.h"
#include <iostream>
#include <unordered_map>

bool MapRenderer::load(const std::string& tmxFilePath)
{
    m_mapPath = tmxFilePath;
    // Load the TMX map file
    tmx::Map mapData;
    if (!mapData.load(tmxFilePath))
    {
        std::cout << "Failed to load map: " << tmxFilePath << "\n";
        return false;
    }

    std::cout << "Successfully loaded map: " << tmxFilePath << "\n";

    // Get map properties
    const auto tileSize = mapData.getTileSize();
    const auto mapSize = mapData.getTileCount();

    // Clear old data
    m_layerGroups.clear();
    m_tilesetTextures.clear();
	m_collisionRects.clear();

    // Load all tileset textures
    for (const auto& tileset : mapData.getTilesets())
    {
        sf::Texture tilesetTexture;
        if (!tilesetTexture.loadFromFile(tileset.getImagePath()))
        {
            std::cout << "Failed to load tileset texture:" << tileset.getImagePath() << "\n";
            continue;
        }

        tilesetTexture.setSmooth(false);
        m_tilesetTextures[tileset.getImagePath()] = std::move(tilesetTexture);
    }

    // Process each tile layer
    for (const auto& layer : mapData.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();

		// Group tiles by texture for efficient rendering better than per-tile draw calls
        std::unordered_map<const sf::Texture*, sf::VertexArray> textureBatches;

        // Process each tile in the grid
        for (unsigned int y = 0; y < mapSize.y; ++y)
        {
            for (unsigned int x = 0; x < mapSize.x; ++x)
            {
                int index = y * mapSize.x + x;
                const auto& tile = tiles[index];

                if (tile.ID == 0) continue; // Empty tile

                // Find which tileset this tile belongs to
                const tmx::Tileset* tileset = nullptr;
                for (const auto& ts : mapData.getTilesets())
                {
                    if (tile.ID >= ts.getFirstGID())
                        tileset = &ts;
                }
                if (!tileset) continue;

                // Get the texture for this tileset
                auto it = m_tilesetTextures.find(tileset->getImagePath());
                if (it == m_tilesetTextures.end()) continue;

                const sf::Texture* texture = &it->second;
                sf::VertexArray& vertices = textureBatches[texture];
                vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

                // Calculate texture coordinates
                int localID = tile.ID - tileset->getFirstGID();
                int tilesPerRow = texture->getSize().x / tileSize.x;
                int tileX = localID % tilesPerRow;
                int tileY = localID / tilesPerRow;

                // World position
                float worldX = x * tileSize.x;
                float worldY = y * tileSize.y;

                // Texture position
                float texX = tileX * tileSize.x;
                float texY = tileY * tileSize.y;

                // Add 6 vertices (2 triangles) for this tile
                std::size_t startIdx = vertices.getVertexCount();
                vertices.resize(startIdx + 6);

                // Triangle 1
                vertices[startIdx + 0].position = { worldX, worldY };
                vertices[startIdx + 0].texCoords = { texX, texY };

                vertices[startIdx + 1].position = { worldX + tileSize.x, worldY };
                vertices[startIdx + 1].texCoords = { texX + tileSize.x, texY };

                vertices[startIdx + 2].position = { worldX + tileSize.x, worldY + tileSize.y };
                vertices[startIdx + 2].texCoords = { texX + tileSize.x, texY + tileSize.y };

                // Triangle 2
                vertices[startIdx + 3].position = { worldX, worldY };
                vertices[startIdx + 3].texCoords = { texX, texY };

                vertices[startIdx + 4].position = { worldX + tileSize.x, worldY + tileSize.y };
                vertices[startIdx + 4].texCoords = { texX + tileSize.x, texY + tileSize.y };

                vertices[startIdx + 5].position = { worldX, worldY + tileSize.y };
                vertices[startIdx + 5].texCoords = { texX, texY + tileSize.y };
            }
        }

        // Create layer group with all batches
        LayerGroup layerGroup;
        layerGroup.name = layer->getName();

        for (auto& [texture, vertices] : textureBatches)
        {
            TileBatch batch;
            batch.vertices = std::move(vertices);
            batch.texture = texture;
            layerGroup.batches.push_back(std::move(batch));
        }

        m_layerGroups.push_back(std::move(layerGroup));
    }
	// Process object layers for collision rectangles axis alligned as they arent rotating
    for (const auto& layer : mapData.getLayers())
    {
        if (layer->getType() == tmx::Layer::Type::Object &&
            layer->getName() == "Collision")
        {
            const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

            for (const auto& obj : objects)
            {
                auto aabb = obj.getAABB();

                m_collisionRects.push_back(
                    sf::FloatRect(
                        sf::Vector2f(static_cast<float>(aabb.left),
                            static_cast<float>(aabb.top)),
                        sf::Vector2f(static_cast<float>(aabb.width),
                            static_cast<float>(aabb.height))
                    )
                );
            }
        }
    }
    float mapScale = 3.f;

    for (auto& r : m_collisionRects)
    {
        r.position *= mapScale;
        r.size *= mapScale;
    }
    return true;
}

const std::string& MapRenderer::getMapPath() const
{
    return m_mapPath;
}

void MapRenderer::drawLayered(sf::RenderTarget& target, sf::RenderStates renderStates, bool drawAbove) const
{
    renderStates.transform.scale({ 3.f, 3.f });

    for (const auto& layer : m_layerGroups)
    {
        bool isAboveLayer = (layer.name == "Above");

        if (!drawAbove && isAboveLayer) continue;
        if (drawAbove && !isAboveLayer) continue;

        for (const auto& batch : layer.batches)
        {
            renderStates.texture = batch.texture;
            target.draw(batch.vertices, renderStates);
        }
    }
}

const std::vector<sf::FloatRect>& MapRenderer::getCollisionRects() const
{
	return m_collisionRects;
}

void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates renderState) const
{
      drawLayered(target, renderState, false); // draw background
    drawLayered(target, renderState, true); //draw above


}