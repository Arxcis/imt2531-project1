#pragma once
#include <vector>
#include <map>
#include <utility>
#include "glm/glm.hpp"
#include "./Shader.hpp"
#include "Entity.h"
#include "./logger.h"


namespace ost
{

    enum TileType : int {
        FLOOR            = 0b00000000, // 0   //THESE ARE VERY HACKY FLAGS
        WALL             = 0b00000001, // 1   //AS WE DONT ALLOW ANYTHING ELSE TO APPEAR WITH WALL
        FOOD             = 0b00000010, // 2
        SUPERFOOD        = 0b00000011, // 3   //THAT'S WHY WE USE THE WALL-bit FOR OTHER TILE FLAGS AS WELL

        PACMAN_START     = 0b00000100, // 4   //
        GHOST_START      = 0b00001000, // 8
        PORTAL_KEY_SHIFT = 0b00001000, // 8

        //PORTALS  > 16
        PORTAL_UP        = 0b00010000, //16
        PORTAL_DOWN      = 0b00100000, //32
        PORTAL_RIGHT     = 0b01000000, //64
        PORTAL_LEFT      = 0b10000000 //128
        //FOR PORTAL INDEX: TileType >> GHOST_START


        //ALL BYTES IN FRONT OF PORTAL ie 1 << n for n > 6 will be considered ids of portal pairs
    };
    using Grid = const std::vector<std::vector<TileType>>;
    struct Pacman;

    using namespace glm;
    class Level {

    public:
        const std::vector<std::vector<TileType>> grid;
        const std::vector<vec2> vertices;
        const glm::ivec2 size;
        const int biggestSize;
        const float levelUnit;

        glm::ivec2 pacmanSpawnTile;
        std::vector<glm::ivec2> ghostSpawnTiles;
        std::map<TileType, Portal> portals;

        Level(const std::vector<vec2> _vertices, const ivec2 _size, Grid _grid)
        :grid(_grid)
        ,vertices(_vertices)
        ,size(_size)
        ,biggestSize((_size.x > _size.y) ? _size.x : _size.y)
        ,levelUnit(2.0f/(biggestSize))
        {
            auto tileIndex = glm::ivec2{0,1};
            for(auto& row : grid) {
                for(auto& tile : row) {
                    // LOG_DEBUG("%d", (int)tile);
                    if((tile & PACMAN_START) == PACMAN_START) {
                        pacmanSpawnTile = tileIndex;
                        LOG_DEBUG("Pacman pos set to (%d, %d)", tileIndex.x,tileIndex.y);
                    }
                    if((tile & GHOST_START) == GHOST_START) {
                        ghostSpawnTiles.push_back(tileIndex);

                        LOG_DEBUG("Ghost pos set to (%d, %d)", tileIndex.x,tileIndex.y);
                    }
                    if(tile >= PORTAL_UP)//PORTALS
                    {
                        LOG_DEBUG("FOUND PORTAL: %d", tile);

                        if((tile & PORTAL_UP) == PORTAL_UP) {
                            LOG_DEBUG("FOUND PORTAL_UP: %d at (%d,%d)", tile, tileIndex.x, tileIndex.y)
                            portals.insert(
                                std::pair<TileType, Portal>(
                                    tile,
                                    ost::Portal{
                                        glm::ivec2{0,1},
                                        tileIndex
                                    }
                                )
                            );
                        }
                        if((tile & PORTAL_DOWN) == PORTAL_DOWN) {
                            LOG_DEBUG("FOUND PORTAL_DOWN: %d at (%d,%d)", tile, tileIndex.x, tileIndex.y)
                            portals.insert(
                                std::pair<TileType, Portal>(
                                    tile,
                                    ost::Portal{
                                        glm::ivec2{0,-1},
                                        tileIndex
                                    }
                                )
                            );
                        }
                        if((tile & PORTAL_LEFT) == PORTAL_LEFT) {
                            LOG_DEBUG("FOUND PORTAL_LEFT: %d at (%d,%d)", tile, tileIndex.x, tileIndex.y)
                            portals.insert(std::pair<TileType, Portal>(
                                    tile,
                                    ost::Portal{
                                        glm::ivec2{-2,0},
                                        tileIndex
                                    }
                                )
                            );
                        }
                        if((tile & PORTAL_RIGHT) == PORTAL_RIGHT) {
                            LOG_DEBUG("FOUND PORTAL_RIGHT: %d at (%d,%d)", tile, tileIndex.x, tileIndex.y)
                            portals.insert(std::pair<TileType, Portal>(
                                    tile,
                                    ost::Portal{
                                        glm::ivec2{1,0},
                                        tileIndex
                                    }
                                )
                            );
                        }

                    }
                    tileIndex.x++;
                }
                tileIndex.x = 0;
                tileIndex.y++;
            }
            for(auto& portal : portals) {
                int portalPairKey = (portal.first >> 8);
                LOG_DEBUG("Portal p1: tile=%d, key=%d", portal.first, portalPairKey);
                for(auto& p2 : portals) {
                    int p2PortalPairKey = (p2.first >> 8);
                    LOG_DEBUG("Portal p2: tile=%d, key=%d", p2.first, p2PortalPairKey);
                    //if the two portals are supposed to be connected, but are not the same portal
                    if((p2PortalPairKey == portalPairKey) && (portal.second.tileIndex != p2.second.tileIndex)) {
                        portal.second.destination = &p2.second;
                        p2.second.destination = &portal.second;
                        LOG_DEBUG("Connected portals at tile (%d,%d), (%d,%d)", portal.second.tileIndex.x, portal.second.tileIndex.y, p2.second.tileIndex.x, p2.second.tileIndex.y);
                    }
                }
            }

        }

        static glm::mat4 getScaleMatrix(float levelUnit) {
            return mat4{
                levelUnit,  0,              0,      0,
                0,          levelUnit,      0,      0,
                0,          0,              1,      0,
                0,          0,              0,      1
            };
        }

        static glm::mat4 getMoveMatrix(glm::vec2 size) {
            return mat4{
                1,             0,           0,      0,
                0,             1,           0,      0,
                0,             0,           1,      0,
                -size.x*0.5, -size.y*0.5, 0,      1
            };
        }

        //
        // BUFFER COMPONENT
        //
        void bindBufferVertices(Mesh::Mesh mesh) const {
            auto vbo = mesh.VBObegin;
            for(auto v : vertices) {
                vbo[0].position = v;
                vbo += 1;
            }
        }

        // static glm::ivec2 getIndexForTileOfType(const Grid& grid, TileType tileType) {
        //
        // }

        static bool canWalkToward(const Grid& grid, glm::vec2 coordinate, glm::vec2 size, glm::ivec2 direction)
        {
            const auto center = Level::getCenterPosition(coordinate, size);
            const auto gridIndex = glm::ivec2{center.x, center.y};

            const auto wantTile = grid[gridIndex.y +  direction.y ][gridIndex.x + direction.x];

        //    LOG_DEBUG("cx: %.2f cy: %.2f ix: %d, iy: %d  dx: %.2f, dy: %.2f, tt: %d , wt: %d", center.x, center.y, gridIndex.x, gridIndex.y, direction.x*0.5, direction.y*0.5, grid[gridIndex.y][gridIndex.x],  grid[gridIndex.y+direction.y][gridIndex.x+direction.x]);

            if (wantTile == ost::WALL) {
                if (Level::isCloseEnoughToTheMiddleOfTile(direction, gridIndex, center)) {
                    return false;
                }
            }
            return true;
        }

        static bool canChangeDirection(const Grid& grid, glm::vec2 coordinate, glm::vec2 size, glm::ivec2 direction, glm::ivec2 wantedDirection)
        {
            const auto center = Level::getCenterPosition(coordinate, size);
            const auto gridIndex = glm::ivec2{center.x, center.y};

            if (!canWalkToward(grid, coordinate, size, wantedDirection)) {
                return false;
            }

            return (Level::isCloseEnoughToTheMiddleOfTile(direction, gridIndex, center));
        }


        static glm::vec2 getCenterPosition(glm::vec2 coordinate, glm::vec2 size)
        {
            return glm::vec2{coordinate.x+(size.x*0.5f), coordinate.y-(size.y*0.5f)};
        }

        static glm::vec2 getTileSnapPosition(glm::vec2 coordinate, glm::vec2 size)
        {
            const auto center = getCenterPosition(coordinate, size);
            return glm::vec2{ int(center.x), int(center.y)+1 };
        }

        //isInSameTile - used to check if two objects are in the same tile, useful for Entity Collision
        static bool isInSameTile(const glm::vec2 aCoord, const glm::vec2 aSize, const glm::vec2 bCoord, const glm::vec2 bSize) {
            return glm::ivec2{Level::getCenterPosition(aCoord, aSize)} == glm::ivec2{Level::getCenterPosition(bCoord, bSize)};
        }

        static bool isCloseEnoughToTheMiddleOfTile(const glm::ivec2 direction, const glm::ivec2 gridIndex, const glm::vec2 center)
        {
            const float margin = 0.1;

#ifdef OST_DEBUG
            if (direction.y > 0 && direction.x > 0) {
                LOG_ERROR("OMG, direction.x and direction.y is both > 0. DIAGONAL movement not supported!");
            }

            if( direction.y == 0 && direction.x == 0){
                LOG_ERROR("ENTITY does not have any direction. Undefined behaviour!");
            }
#endif

            if (direction.x) {
                return abs(center.x - (gridIndex.x+0.5)) < margin;
            } else {
                return abs(center.y - (gridIndex.y+0.5) < margin);
            }

            LOG_ERROR("HOW DA FUCK DID YOU GET ALL THE WAY HERE !??????? ");
        }
    };


inline bool Portal::tryTeleport(const Pacman& pacman) {

    return Level::isInSameTile(tileIndex+entryDirection, vec2{0.25f,0.25f}, pacman.pos,  vec2{0.25f,0.25f});
}

inline void Portal::teleport(Pacman& pacman) {
    auto const tpFrom = tileIndex+entryDirection;
    auto const tpTo   = destination->tileIndex;
    LOG_DEBUG("teleport from (%d, %d) to (%d, %d)", tpFrom.x, tpFrom.y, tpTo.x, tpTo.y);

    pacman.pos = tpTo;
    pacman.direction = destination->exitDirection();
}

}
