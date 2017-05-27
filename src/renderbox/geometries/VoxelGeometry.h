#ifndef PARTICLESIM_VOXELGEOMETRY_H
#define PARTICLESIM_VOXELGEOMETRY_H


#include <unordered_map>
#include "Geometry.h"

namespace renderbox {

    class VoxelGeometry : public Geometry {

        static const int VOXEL_CHUNK_DIMENSION = 8;

        struct Voxel {
            float occupancy;
        };

        struct VoxelChunk {
            int index;
            Voxel voxels[VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION];

            std::vector<glm::vec3> vertices;
            std::vector<glm::vec3> normals;
            std::vector<glm::uvec3> faces;

            std::vector<glm::vec3> edgeVertices;
            std::vector<glm::vec3> edgeNormals;
            std::vector<glm::uvec3> edgeFaces;

            bool cacheNeedsUpdating = false;
            bool edgeCacheNeedsUpdating = false;
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

        void addCube(int x, int y, int z,
                     float isolevel,
                     std::vector<glm::vec3> &vertices,
                     std::vector<glm::vec3> &normals,
                     std::vector<glm::uvec3> &faces);

    public:

        bool isOccupied(int x, int y, int z);
        bool isOccupied(glm::ivec3 position);
        bool isOccupied(glm::vec3 position);
        float getOccupancy(int x, int y, int z);
        void setOccupancy(int x, int y, int z, float occupancy);
        void setOccupancy(glm::ivec3 position, float occupancy);
        void setOccupancy(glm::vec3 position, float occupancy);

        void brush(glm::vec3 focus, float radius, float value);

        void updateGeometry(float isolevel);

    };

}


#endif //PARTICLESIM_VOXELGEOMETRY_H
