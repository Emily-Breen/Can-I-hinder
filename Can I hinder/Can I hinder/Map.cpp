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

    m_tileSize = sf::Vector2u{ tileSize.x, tileSize.y };
    m_mapSize = sf::Vector2u{ mapSize.x,  mapSize.y };
    m_walkable.assign(m_mapSize.x * m_mapSize.y, 0);
    // Clear old data
    m_floorSpawns.clear();
    m_layerGroups.clear();
    m_tilesetTextures.clear();
	m_collisionRects.clear();
    m_doors.clear(); //this fecker I forgot to add it in when creating the door mechanic and caused me HOURS of debugging :'O

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
		// If this is the floor layer, mark walkable tiles and store floor spawn points
        if (layer->getName() == "floor")
        {
            for (unsigned tileY = 0; tileY < mapSize.y; ++tileY)
            {
                for (unsigned tileX = 0; tileX < mapSize.x; ++tileX)
                {
                    const auto& tile = tiles[tileY * mapSize.x + tileX];

                    // Walkable if this floor layer HAS a tile here
                    if (tile.ID != 0)
                    {
                        m_walkable[tileY * mapSize.x + tileX] = 1;

                        // only store floor tile centers
                        sf::Vector2f tileCenter{
                            (tileX + 0.5f) * tileSize.x * mapScale,
                            (tileY + 0.5f) * tileSize.y * mapScale
                        };
                        m_floorSpawns.push_back(tileCenter);
                    }
                }
            }
        }

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
        //Le door this basically is for the rectangle (I love me some AABB) for the object "Doors" in tiled so it I set properties for the door when the player hits the door with 3 keys moves it to the next map so cool!

        if (layer->getType() == tmx::Layer::Type::Object &&
            layer->getName() == "Doors")
        {
            const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

            for (const auto& obj : objects)
            {
                DoorData door;

                auto aabb = obj.getAABB();
                door.rect = sf::FloatRect(
                    sf::Vector2f(static_cast<float>(aabb.left),
                        static_cast<float>(aabb.top)),
                    sf::Vector2f(static_cast<float>(aabb.width),
                        static_cast<float>(aabb.height))
                );
                
                for (const auto& prop : obj.getProperties())
                {
                    if (prop.getName() == "requiredKeys") 
                        door.requiredKeys = prop.getIntValue();
                    else if (prop.getName() == "nextMap") 
                        door.nextMap = prop.getStringValue();
                    else if (prop.getName() == "spawnX") 
                        door.spawn.x = prop.getFloatValue();
                    else if (prop.getName() == "spawnY") 
                        door.spawn.y = prop.getFloatValue();
                    
                }

                m_doors.push_back(door);
            }
        }

    }
    

    for (auto& rect : m_collisionRects)
    {
        rect.position *= mapScale;
        rect.size *= mapScale;

    }
    for (auto& door : m_doors)
    {
        door.rect.position *= mapScale;
        door.rect.size *= mapScale;
   
       
    }
	// Mark tiles as non-walkable if they intersect with any collision rectangle
    const float worldTileWidth = static_cast<float>(m_tileSize.x) * mapScale;
    const float worldTileHeight = static_cast<float>(m_tileSize.y) * mapScale;
	// Iterate over each tile and check for intersection with collision rectangles
    for (unsigned tileY = 0; tileY < m_mapSize.y; ++tileY)
        for (unsigned tileX = 0; tileX < m_mapSize.x; ++tileX)
        {
            if (!m_walkable[tileY * m_mapSize.x + tileX]) 
                continue;

            sf::FloatRect tileRect(
                sf::Vector2f(tileX * worldTileWidth, tileY * worldTileHeight),
                sf::Vector2f(worldTileWidth, worldTileHeight)
            );
			// Check if this tile intersects with any collision rectangle
            for (const auto& c : m_collisionRects)
            {
                if (tileRect.findIntersection(c).has_value())
                {
                    m_walkable[tileY * m_mapSize.x + tileX] = 0;
                    break;
                }
            }
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

        if (!drawAbove && isAboveLayer) 
            continue;
        if (drawAbove && !isAboveLayer) 
            continue;

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

const std::vector<DoorData>& MapRenderer::getDoors() const
{
    return m_doors;
}
// to help stop the enemy spawning on top of the player and outside the map will probably move this to another class later.
sf::Vector2f MapRenderer::getFloorSpawn(const sf::Vector2f& entitySize, const sf::Vector2f& avoidPos, float avoidRadius) const
{ 
    // If no walkable tiles, just spawn to the right of avoidPos
    if (m_walkable.empty())
        return avoidPos + sf::Vector2f(avoidRadius, 0.f);
	// Get map dimensions
    const int mapWidth = static_cast<int>(m_mapSize.x);
    const int mapHeight = static_cast<int>(m_mapSize.y);
	// Convert avoidPos to tile coordinates
    auto startTile = worldToTile(avoidPos);
	// If the starting tile is out of bounds, just spawn to the right of avoidPos
    if (startTile.x < 0 || startTile.y < 0 || startTile.x >= mapWidth || startTile.y >= mapHeight)
        return avoidPos + sf::Vector2f(avoidRadius, 0.f);
	// Lambda to check if a tile is walkable
    auto isTileWalkable = [&](int x, int y) -> uint8_t {
        return m_walkable[y * mapWidth + x];
        };
	// If the starting tile is not walkable, just spawn to the right of avoidPos
    if (!isTileWalkable(startTile.x, startTile.y))
    {
        return avoidPos + sf::Vector2f(avoidRadius, 0.f);
    }
	// BFS to find all reachable tiles from the starting tile
    std::vector<uint8_t> visitedTiles(mapWidth * mapHeight, 0);
    std::vector<sf::Vector2i> reachableTilesQueue;
    reachableTilesQueue.reserve(mapWidth * mapHeight);
	// Lambda to add a tile to the BFS queue if it's valid
    auto addTileToQueue = [&](int x, int y)
        {
            if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight) 
                return;
            const int i = y * mapWidth + x;
            if (visitedTiles[i]) 
                return;
            if (!isTileWalkable(x, y)) 
                return;
            visitedTiles[i] = 1;
            reachableTilesQueue.push_back({ x, y });
        };
	// Start BFS from the tile under avoidPos
    addTileToQueue(startTile.x, startTile.y);

    // BFS fills only the player's connected area
    for (std::size_t i = 0; i < reachableTilesQueue.size(); ++i)
    {
        auto [tileX, tileY] = reachableTilesQueue[i];
        addTileToQueue(tileX + 1, tileY);
        addTileToQueue(tileX - 1, tileY);
        addTileToQueue(tileX, tileY + 1);
        addTileToQueue(tileX, tileY - 1);
    }

    // Choose nearest reachable tile outside radius
    float closestValidDistance = std::numeric_limits<float>::max();
    sf::Vector2f bestSpawnPos = avoidPos + sf::Vector2f(avoidRadius, 0.f);
	// Iterate over all reachable tiles and find the one closest to avoidPos but outside avoidRadius
    for (auto [tileX, tileY] : reachableTilesQueue)
    {
        sf::Vector2f tileCenterPos = tileCenter(tileX, tileY);
        float distanceFromCenter = MathUtils::vectorLength(tileCenterPos - avoidPos);

        if (distanceFromCenter < avoidRadius) 
            continue;

        if (distanceFromCenter < closestValidDistance)
        {
            closestValidDistance = distanceFromCenter;
            bestSpawnPos = tileCenterPos;
        }
    }

    return bestSpawnPos;
}

bool MapRenderer::rectHitsCollision(const sf::FloatRect& test, const std::vector<sf::FloatRect>& colliders)
{
    for (const auto& c : colliders)
    {
        if (test.findIntersection(c).has_value())
            return true;
    }
    return false;
}
// Convert world coordinates to tile coordinates
sf::Vector2i MapRenderer::worldToTile(const sf::Vector2f& p) const
{
    const float worldTileWidth = static_cast<float>(m_tileSize.x) * mapScale;
    const float worldTileHeight = static_cast<float>(m_tileSize.y) * mapScale;
    return { static_cast<int>(p.x / worldTileWidth), static_cast<int>(p.y / worldTileHeight) };
}
// Get the world coordinates of the center of a tile given its tile coordinates
sf::Vector2f MapRenderer::tileCenter(int tx, int ty) const
{
    const float worldTileWidth = static_cast<float>(m_tileSize.x) * mapScale;
    const float worldTileHeight = static_cast<float>(m_tileSize.y) * mapScale;
    return { (tx + 0.5f) * worldTileWidth, (ty + 0.5f) * worldTileHeight };
}


