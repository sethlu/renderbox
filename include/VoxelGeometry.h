#ifndef VOX_VOXELGEOMETRY_H
#define VOX_VOXELGEOMETRY_H


#include <unordered_map>
#include "Geometry.h"

namespace renderbox {

    class VoxelGeometry : public Geometry {
    protected:

        static const int VOXEL_CHUNK_DIMENSION = 8;

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
                             std::vector<glm::vec3> &vertices,
                             std::vector<glm::vec3> &normals,
                             std::vector<glm::uvec3> &faces);

    public:

        bool isOccupied(int x, int y, int z);

        bool isOccupied(glm::ivec3 position);

        bool isOccupied(glm::vec3 position);

        float getOccupancy(int x, int y, int z);

        float getOccupancy(glm::ivec3 position);

        void setOccupancy(int x, int y, int z, float occupancy);

        void setOccupancy(glm::ivec3 position, float occupancy);

        void setOccupancy(glm::vec3 position, float occupancy);

        bool isConnected(int x, int y, int z, float isolevel);

        bool isConnected(glm::ivec3 position, float isolevel);

        void brush(glm::vec3 focus, float radius, float value, float isolevel);

        void updateGeometryByMarchingCube(float isolevel);

    };

}


#endif //VOX_VOXELGEOMETRY_H
