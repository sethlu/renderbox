#ifndef VOX_VOXELGEOMETRY_H
#define VOX_VOXELGEOMETRY_H


#include <unordered_map>
#include "Geometry.h"

namespace renderbox {

    unsigned const VOXEL_CHUNK_DIMENSION = 4; // Greater than one to prevent non-updated vertex gradients
    unsigned constexpr VOXEL_CHUNK_DIMENSION2 = VOXEL_CHUNK_DIMENSION * VOXEL_CHUNK_DIMENSION;
    unsigned constexpr VOXEL_CHUNK_DIMENSION3 = VOXEL_CHUNK_DIMENSION2 * VOXEL_CHUNK_DIMENSION;

    class VoxelGeometry : public Geometry {
    public:

        bool isOccupied(int x, int y, int z);

        bool isOccupied(glm::ivec3 position);

        bool isOccupied(glm::vec3 position);

        float getOccupancy(int x, int y, int z);

        float getOccupancy(glm::ivec3 position);

        void setOccupancy(int x, int y, int z, float occupancy);

        void setOccupancy(glm::ivec3 position, float occupancy);

        void setOccupancy(glm::vec3 position, float occupancy);

        glm::vec3 getGradient(int x, int y, int z);

        bool isConnected(int x, int y, int z, float isolevel);

        bool isConnected(glm::ivec3 position, float isolevel);

        void brush(glm::vec3 focus, float radius, float value, float isolevel);

        void updateGeometryByMarchingCube(float isolevel, bool force = false);

    protected:

        struct Voxel {
            float occupancy;
        };

        struct VoxelChunk {
            int index;
            Voxel voxels[VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION];

            // Cache

            bool cacheNeedsUpdating = false;
            bool edgeCacheNeedsUpdating = false;

            std::vector<glm::vec3> cacheVertices;
            std::vector<glm::vec3> cacheNormals;
            std::vector<glm::uvec3> cacheFaces;

            std::vector<glm::vec3> edgeCacheVertices;
            std::vector<glm::vec3> edgeCacheNormals;
            std::vector<glm::uvec3> edgeCacheFaces;
        };

        struct SparseVoxelChunkList {
            int index;
            std::unordered_map<int, VoxelChunk *> voxelChunks;
        };

        struct SparseVoxelChunkPlane {
            int index;
            std::unordered_map<int, SparseVoxelChunkList *> voxelChunkLines;
        };

        std::unordered_map<int, SparseVoxelChunkPlane *> voxelChunkPlanes;

        VoxelChunk *getVoxelChunk(int cx, int cy, int cz, bool createIfNotExists = false);

        VoxelChunk *getVoxelChunkByVoxel(int x, int y, int z, bool createIfNotExists = false);

        void addMarchingCube(int x, int y, int z,
                             float isolevel,
                             std::vector<glm::vec3> &cacheVertices,
                             std::vector<glm::vec3> &cacheNormals,
                             std::vector<glm::uvec3> &cacheFaces);

    };

}


#endif //VOX_VOXELGEOMETRY_H
