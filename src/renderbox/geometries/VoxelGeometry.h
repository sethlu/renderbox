#ifndef PARTICLESIM_VOXELGEOMETRY_H
#define PARTICLESIM_VOXELGEOMETRY_H


#include <unordered_map>
#include "Geometry.h"

namespace renderbox {

    class VoxelGeometry : public Geometry {

        static const int VOXEL_CHUNK_DIMENSION = 16;

        struct Voxel {
            float occupancy;
        };

        struct VoxelChunk {
            int index;
            Voxel voxels[VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION][VOXEL_CHUNK_DIMENSION];
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

        void addMarchingCube(int x, int y, int z, float isolevel);
        void addCube(int x, int y, int z);

    public:
        bool isOccupied(int x, int y, int z);
        bool isOccupied(glm::ivec3 position);
        bool isOccupied(glm::vec3 position);
        float getOccupancy(int x, int y, int z);
        void setOccupancy(int x, int y, int z, float occupancy);
        void setOccupancy(glm::ivec3 position, float occupancy);
        void setOccupancy(glm::vec3 position, float occupancy);
        void updateGeometry(float isolevel);
    };

}


#endif //PARTICLESIM_VOXELGEOMETRY_H
