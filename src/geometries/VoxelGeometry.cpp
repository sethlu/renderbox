// Marching cube algorithm based on code from http://paulbourke.net/geometry/polygonise/

#include <iostream>
#include <cfloat>
#include <glm/geometric.hpp>
#include "VoxelGeometry.h"


namespace renderbox {

    int div(int a, int b) {
        return (int) floor((double) a / (double) b);
    }

    int remainder(int a, int b) {
        return a - div(a, b) * b;
    }

    VoxelGeometry::VoxelChunk *VoxelGeometry::getVoxelChunk(int cx, int cy, int cz, bool createIfNotExists) {

        auto it0 = voxelChunkPlanes.find(cz);
        SparseVoxelChunkPlane *chunkPlane;
        if (it0 == voxelChunkPlanes.end()) {
            if (!createIfNotExists) {
                return nullptr;
            } else {
                chunkPlane = new SparseVoxelChunkPlane();
                chunkPlane->index = cz;
                voxelChunkPlanes[cz] = chunkPlane;
            }
        } else {
            chunkPlane = it0->second;
        }

        auto it1 = chunkPlane->voxelChunkLines.find(cy);
        SparseVoxelChunkList *chunkList;
        if (it1 == chunkPlane->voxelChunkLines.end()) {
            if (!createIfNotExists) {
                return nullptr;
            } else {
                chunkList = new SparseVoxelChunkList();
                chunkList->index = cy;
                chunkPlane->voxelChunkLines[cy] = chunkList;
            }
        } else {
            chunkList = it1->second;
        }

        auto it2 = chunkList->voxelChunks.find(cx);
        VoxelChunk *voxelChunk;
        if (it2 == chunkList->voxelChunks.end()) {
            if (!createIfNotExists) {
                return nullptr;
            } else {
                voxelChunk = new VoxelChunk();
                voxelChunk->index = cx;
                chunkList->voxelChunks[cx] = voxelChunk;
            }
        } else {
            voxelChunk = it2->second;
        }

        return voxelChunk;

    }

    VoxelGeometry::VoxelChunk *VoxelGeometry::getVoxelChunkByVoxel(int x, int y, int z, bool createIfNotExists) {

        return getVoxelChunk(div(x, VOXEL_CHUNK_DIMENSION),
                             div(y, VOXEL_CHUNK_DIMENSION),
                             div(z, VOXEL_CHUNK_DIMENSION),
                             createIfNotExists);

    }

    float VoxelGeometry::getOccupancy(int x, int y, int z) {

        VoxelChunk *voxelChunk = getVoxelChunkByVoxel(x, y, z);
        if (voxelChunk == nullptr) {
            return false;
        }

        int ix = remainder(x, VOXEL_CHUNK_DIMENSION);
        int iy = remainder(y, VOXEL_CHUNK_DIMENSION);
        int iz = remainder(z, VOXEL_CHUNK_DIMENSION);

        return voxelChunk->voxels[ix][iy][iz].occupancy;

    }

    float VoxelGeometry::getOccupancy(glm::ivec3 position) {
        return getOccupancy(position.x, position.y, position.z);
    }

    bool VoxelGeometry::isOccupied(int x, int y, int z) {
        return fabs(getOccupancy(x, y, z)) > FLT_EPSILON;
    }

    bool VoxelGeometry::isOccupied(glm::ivec3 position) {
        return isOccupied(position.x, position.y, position.z);
    }

    bool VoxelGeometry::isOccupied(glm::vec3 position) {
        return isOccupied(glm::ivec3(position));
    }

    void VoxelGeometry::setOccupancy(int x, int y, int z, float occupancy) {

        VoxelChunk *voxelChunk = getVoxelChunkByVoxel(x, y, z, true);

        int ix = remainder(x, VOXEL_CHUNK_DIMENSION);
        int iy = remainder(y, VOXEL_CHUNK_DIMENSION);
        int iz = remainder(z, VOXEL_CHUNK_DIMENSION);

        voxelChunk->voxels[ix][iy][iz].occupancy = occupancy;
        voxelChunk->cacheNeedsUpdating = true;

        // Pre populate neighboring chunks for geometry generation of overlapping voxel chunk edges

        getVoxelChunkByVoxel(x + 1, y + 1, z + 1, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x + 1, y + 1, z, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x, y + 1, z + 1, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x, y + 1, z, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x + 1, y, z + 1, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x + 1, y, z, true)->edgeCacheNeedsUpdating = true;
        getVoxelChunkByVoxel(x, y, z + 1, true)->edgeCacheNeedsUpdating = true;

    }

    void VoxelGeometry::setOccupancy(glm::ivec3 position, float occupancy) {
        setOccupancy(position.x, position.y, position.z, occupancy);
    }

    void VoxelGeometry::setOccupancy(glm::vec3 position, float occupancy) {
        setOccupancy(glm::ivec3(position), occupancy);
    }

    glm::vec3 VoxelGeometry::getGradient(int x, int y, int z) {
        return glm::vec3(getOccupancy(x + 1, y,     z    ) - getOccupancy(x - 1, y,     z    ),
                         getOccupancy(x,     y + 1, z    ) - getOccupancy(x,     y - 1, z    ),
                         getOccupancy(x,     y,     z + 1) - getOccupancy(x,     y,     z - 1));
    }

    float normalDistributionPDF(float x) {
        return (float) (exp(- 1.0 / 2.0 * x * x) / sqrt(M_PI_2));
    }

    float normalDistributionPDF(float x, float mu, float delta) {
        return normalDistributionPDF((x - mu) / delta) / delta;
    }

    bool VoxelGeometry::isConnected(int x, int y, int z, float isolevel) {
        return getOccupancy(x, y, z) > isolevel
                || getOccupancy(x + 1, y, z) > isolevel
                || getOccupancy(x - 1, y, z) > isolevel
                || getOccupancy(x, y + 1, z) > isolevel
                || getOccupancy(x, y - 1, z) > isolevel
                || getOccupancy(x, y, z + 1) > isolevel
                || getOccupancy(x, y, z - 1) > isolevel;
    }

    bool VoxelGeometry::isConnected(glm::ivec3 position, float isolevel) {
        return isConnected(position.x, position.y, position.z, isolevel);
    }

    void VoxelGeometry::brush(glm::vec3 focus, float radius, float value, float isolevel) {
        for (int x = (int) floorf(focus.x - radius); x <= (int) ceilf(focus.x + radius); ++x) {
            for (int y = (int) floorf(focus.y - radius); y <= (int) ceilf(focus.y + radius); ++y) {
                for (int z = (int) floorf(focus.z - radius); z <= (int) ceilf(focus.z + radius); ++z) {
                    float distance = glm::length(glm::vec3(x, y, z) - focus);
                    if (distance > radius) continue;

                    if (!isConnected(x, y, z, isolevel)) continue;

                    float occupancy = getOccupancy(x, y, z) + value * normalDistributionPDF(distance, 0, radius / 3);
                    if (occupancy > 1.0f) occupancy = 1.0f;
                    setOccupancy(x, y, z, occupancy);
                }
            }
        }
    }

    unsigned edgeTable[256] = {
            0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
            0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
            0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
            0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
            0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
            0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
            0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
            0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
            0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
            0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
            0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
            0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
            0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
            0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
            0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
            0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
            0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
            0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
            0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
            0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
            0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
            0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
            0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
            0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
            0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
            0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
            0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
            0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
            0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
            0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
            0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
            0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
    };

    int triTable[256][16] = {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
            {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
            {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
            {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
            {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
            {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
            {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
            {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
            {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
            {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
            {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
            {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
            {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
            {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
            {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
            {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
            {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
            {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
            {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
            {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
            {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
            {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
            {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
            {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
            {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
            {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
            {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
            {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
            {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
            {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
            {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
            {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
            {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
            {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
            {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
            {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
            {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
            {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
            {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
            {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
            {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
            {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
            {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
            {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
            {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
            {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
            {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
            {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
            {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
            {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
            {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
            {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
            {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
            {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
            {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
            {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
            {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
            {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
            {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
            {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
            {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
            {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
            {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
            {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
            {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
            {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
            {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
            {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
            {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
            {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
            {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
            {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
            {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
            {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
            {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
            {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
            {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
            {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
            {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
            {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
            {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
            {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
            {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
            {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
            {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
            {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
            {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
            {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
            {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
            {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
            {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
            {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
            {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
            {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
            {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
            {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
            {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
            {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
            {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
            {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
            {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
            {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
            {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
            {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
            {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
            {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
            {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
            {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
            {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
            {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
            {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
            {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
            {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
            {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
            {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
            {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
            {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
            {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
            {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
            {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
            {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
            {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
            {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
            {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
            {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
            {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
            {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
            {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
            {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
            {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
            {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
            {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
            {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
            {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
            {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
            {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
            {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
            {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
            {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
            {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
            {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
            {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
            {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
            {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
            {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
            {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
            {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
            {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
            {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
            {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
            {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
            {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
            {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
            {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
            {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
            {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    };

    glm::vec3 interpolateVector(glm::vec3 p1, glm::vec3 p2, float v1, float v2, float isolevel) {
        if (fabs(v1 - isolevel) < FLT_EPSILON) return p1;
        if (fabs(v2 - isolevel) < FLT_EPSILON) return p2;
        if (fabs(v1 - v2) < FLT_EPSILON) return p1;
        return p1 + (p2 - p1) * (isolevel - v1) / (v2 - v1);
    }

    void VoxelGeometry::addMarchingCube(int x, int y, int z,
                                        float isolevel,
                                        std::vector<glm::vec3> &cacheVertices,
                                        std::vector<glm::vec3> &cacheNormals,
                                        std::vector<glm::uvec3> &cacheFaces) {

        float occupancies[8]{
            getOccupancy(x - 1, y    , z - 1),
            getOccupancy(x,     y    , z - 1),
            getOccupancy(x,     y - 1, z - 1),
            getOccupancy(x - 1, y - 1, z - 1),
            getOccupancy(x - 1, y    , z    ),
            getOccupancy(x,     y    , z    ),
            getOccupancy(x,     y - 1, z    ),
            getOccupancy(x - 1, y - 1, z    ),
        };

        unsigned char cubeIndex = 0;
        if (occupancies[0] < isolevel) cubeIndex |= 0b1;
        if (occupancies[1] < isolevel) cubeIndex |= 0b10;
        if (occupancies[2] < isolevel) cubeIndex |= 0b100;
        if (occupancies[3] < isolevel) cubeIndex |= 0b1000;
        if (occupancies[4] < isolevel) cubeIndex |= 0b10000;
        if (occupancies[5] < isolevel) cubeIndex |= 0b100000;
        if (occupancies[6] < isolevel) cubeIndex |= 0b1000000;
        if (occupancies[7] < isolevel) cubeIndex |= 0b10000000;

        if (edgeTable[cubeIndex] == 0) return;

        glm::vec3 cubeVertices[8]{
            glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f),
            glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f),
            glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f),
            glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f),
            glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f),
            glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f),
            glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f),
            glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f),
        };
        glm::vec3 cubeGradients[8]{
            getGradient(x - 1, y,     z - 1),
            getGradient(x,     y,     z - 1),
            getGradient(x,     y - 1, z - 1),
            getGradient(x - 1, y - 1, z - 1),
            getGradient(x - 1, y,     z    ),
            getGradient(x,     y,     z    ),
            getGradient(x,     y - 1, z    ),
            getGradient(x - 1, y - 1, z    ),
        };

        glm::vec3 vertices[12], normals[12];
        unsigned edges = edgeTable[cubeIndex];
        if (edges & 0b1) {
            vertices[0] = interpolateVector(cubeVertices[0], cubeVertices[1], occupancies[0], occupancies[1], isolevel);
            normals[0] = - interpolateVector(cubeGradients[0], cubeGradients[1], occupancies[0], occupancies[1], isolevel);
        }
        if (edges & 0b10) {
            vertices[1] = interpolateVector(cubeVertices[1], cubeVertices[2], occupancies[1], occupancies[2], isolevel);
            normals[1] = - interpolateVector(cubeGradients[1], cubeGradients[2], occupancies[1], occupancies[2], isolevel);
        }
        if (edges & 0b100) {
            vertices[2] = interpolateVector(cubeVertices[2], cubeVertices[3], occupancies[2], occupancies[3], isolevel);
            normals[2] = - interpolateVector(cubeGradients[2], cubeGradients[3], occupancies[2], occupancies[3], isolevel);
        }
        if (edges & 0b1000) {
            vertices[3] = interpolateVector(cubeVertices[3], cubeVertices[0], occupancies[3], occupancies[0], isolevel);
            normals[3] = - interpolateVector(cubeGradients[3], cubeGradients[0], occupancies[3], occupancies[0], isolevel);
        }
        if (edges & 0b10000) {
            vertices[4] = interpolateVector(cubeVertices[4], cubeVertices[5], occupancies[4], occupancies[5], isolevel);
            normals[4] = - interpolateVector(cubeGradients[4], cubeGradients[5], occupancies[4], occupancies[5], isolevel);
        }
        if (edges & 0b100000) {
            vertices[5] = interpolateVector(cubeVertices[5], cubeVertices[6], occupancies[5], occupancies[6], isolevel);
            normals[5] = - interpolateVector(cubeGradients[5], cubeGradients[6], occupancies[5], occupancies[6], isolevel);
        }
        if (edges & 0b1000000) {
            vertices[6] = interpolateVector(cubeVertices[6], cubeVertices[7], occupancies[6], occupancies[7], isolevel);
            normals[6] = - interpolateVector(cubeGradients[6], cubeGradients[7], occupancies[6], occupancies[7], isolevel);
        }
        if (edges & 0b10000000) {
            vertices[7] = interpolateVector(cubeVertices[7], cubeVertices[4], occupancies[7], occupancies[4], isolevel);
            normals[7] = - interpolateVector(cubeGradients[7], cubeGradients[4], occupancies[7], occupancies[4], isolevel);
        }
        if (edges & 0b100000000) {
            vertices[8] = interpolateVector(cubeVertices[0], cubeVertices[4], occupancies[0], occupancies[4], isolevel);
            normals[8] = - interpolateVector(cubeGradients[0], cubeGradients[4], occupancies[0], occupancies[4], isolevel);
        }
        if (edges & 0b1000000000) {
            vertices[9] = interpolateVector(cubeVertices[1], cubeVertices[5], occupancies[1], occupancies[5], isolevel);
            normals[9] = - interpolateVector(cubeGradients[1], cubeGradients[5], occupancies[1], occupancies[5], isolevel);
        }
        if (edges & 0b10000000000) {
            vertices[10] = interpolateVector(cubeVertices[2], cubeVertices[6], occupancies[2], occupancies[6], isolevel);
            normals[10] = - interpolateVector(cubeGradients[2], cubeGradients[6], occupancies[2], occupancies[6], isolevel);
        }
        if (edges & 0b100000000000) {
            vertices[11] = interpolateVector(cubeVertices[3], cubeVertices[7], occupancies[3], occupancies[7], isolevel);
            normals[11] = - interpolateVector(cubeGradients[3], cubeGradients[7], occupancies[3], occupancies[7], isolevel);
        }

        for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
            unsigned long n = cacheVertices.size();

            cacheVertices.push_back(vertices[triTable[cubeIndex][i]]);
            cacheVertices.push_back(vertices[triTable[cubeIndex][i + 1]]);
            cacheVertices.push_back(vertices[triTable[cubeIndex][i + 2]]);

            cacheNormals.push_back(glm::normalize(normals[triTable[cubeIndex][i]]));
            cacheNormals.push_back(glm::normalize(normals[triTable[cubeIndex][i + 1]]));
            cacheNormals.push_back(glm::normalize(normals[triTable[cubeIndex][i + 2]]));

            cacheFaces.emplace_back(n, n + 1, n + 2);
        }

    }

    void VoxelGeometry::updateGeometryByMarchingCube(float isolevel, bool force) {

        vertices.clear();
        normals.clear();
        faces.clear();

        unsigned edgeVertices[VOXEL_CHUNK_DIMENSION3 * 3];

        for (auto &voxelChunkPlane : voxelChunkPlanes) {
            SparseVoxelChunkPlane *chunkPlane = voxelChunkPlane.second;
            int cz = chunkPlane->index;

            for (auto &voxelChunkLine : chunkPlane->voxelChunkLines) {
                SparseVoxelChunkList *chunkList = voxelChunkLine.second;
                int cy = chunkList->index;

                for (auto &it2 : chunkList->voxelChunks) {
                    VoxelChunk *voxelChunk = it2.second;
                    int cx = voxelChunk->index;

                    // Cache non-edge voxel geometry

                    if (voxelChunk->cacheNeedsUpdating || force) {
                        voxelChunk->cacheNeedsUpdating = false;

                        voxelChunk->cacheVertices.clear();
                        voxelChunk->cacheNormals.clear();
                        voxelChunk->cacheFaces.clear();

#define EDGE(X, Y, Z, I) (3 * VOXEL_CHUNK_DIMENSION2 * (X) + 3 * VOXEL_CHUNK_DIMENSION * (Y) + 3 * (Z) + (I))
#define FROM_EDGE(E, X, Y, Z, I) \
    (I) = (E) % 3; \
    (Z) = (E) / 3 % VOXEL_CHUNK_DIMENSION; \
    (Y) = (E) / (3 * VOXEL_CHUNK_DIMENSION) % VOXEL_CHUNK_DIMENSION; \
    (X) = (E) / (3 * VOXEL_CHUNK_DIMENSION2); // This macro is for now left as reference

                        for (auto &edgeVertex : edgeVertices) edgeVertex = INT_MAX;
                        unsigned vertexIndex = 0;

                        for (unsigned ix = 1; ix < VOXEL_CHUNK_DIMENSION; ++ix) {
                            for (unsigned iy = 1; iy < VOXEL_CHUNK_DIMENSION; ++iy) {
                                for (unsigned iz = 1; iz < VOXEL_CHUNK_DIMENSION; ++iz) {

                                    int x = cx * VOXEL_CHUNK_DIMENSION + ix;
                                    int y = cy * VOXEL_CHUNK_DIMENSION + iy;
                                    int z = cz * VOXEL_CHUNK_DIMENSION + iz;

                                    float occupancies[8]{
                                        getOccupancy(x - 1, y    , z - 1),
                                        getOccupancy(x,     y    , z - 1),
                                        getOccupancy(x,     y - 1, z - 1),
                                        getOccupancy(x - 1, y - 1, z - 1),
                                        getOccupancy(x - 1, y    , z    ),
                                        getOccupancy(x,     y    , z    ),
                                        getOccupancy(x,     y - 1, z    ),
                                        getOccupancy(x - 1, y - 1, z    )
                                    };

                                    unsigned char cubeIndex = 0;
                                    if (occupancies[0] < isolevel) cubeIndex |= 0b1;
                                    if (occupancies[1] < isolevel) cubeIndex |= 0b10;
                                    if (occupancies[2] < isolevel) cubeIndex |= 0b100;
                                    if (occupancies[3] < isolevel) cubeIndex |= 0b1000;
                                    if (occupancies[4] < isolevel) cubeIndex |= 0b10000;
                                    if (occupancies[5] < isolevel) cubeIndex |= 0b100000;
                                    if (occupancies[6] < isolevel) cubeIndex |= 0b1000000;
                                    if (occupancies[7] < isolevel) cubeIndex |= 0b10000000;

                                    if (edgeTable[cubeIndex] == 0) continue;

#define USE_VERTEX(VERTEX, Ax, Ay, Az, Bx, By, Bz) \
    if (edgeVertices[VERTEX] == INT_MAX) { \
        edgeVertices[VERTEX] = vertexIndex++; \
        voxelChunk->cacheVertices.emplace_back( \
            interpolateVector(glm::vec3((Ax) + 0.5f, (Ay) + 0.5f, (Az) + 0.5f), glm::vec3((Bx) + 0.5f, (By) + 0.5f, (Bz) + 0.5f), \
                              getOccupancy((Ax), (Ay), (Az)), getOccupancy((Bx), (By), (Bz)), isolevel)); \
        voxelChunk->cacheNormals.emplace_back( \
            - glm::normalize(interpolateVector(getGradient((Ax), (Ay), (Az)), getGradient((Bx), (By), (Bz)), \
                                               getOccupancy((Ax), (Ay), (Az)), getOccupancy((Bx), (By), (Bz)), isolevel))); \
    }

                                    unsigned jx = ix - 1, jy = iy - 1, jz = iz - 1;
                                    unsigned edgeToVertex[12]{
                                        EDGE(jx, iy, jz, 0),
                                        EDGE(ix, jy, jz, 1),
                                        EDGE(jx, jy, jz, 0),
                                        EDGE(jx, jy, jz, 1),
                                        EDGE(jx, iy, iz, 0),
                                        EDGE(ix, jy, iz, 1),
                                        EDGE(jx, jy, iz, 0),
                                        EDGE(jx, jy, iz, 1),
                                        EDGE(jx, iy, jz, 2),
                                        EDGE(ix, iy, jz, 2),
                                        EDGE(ix, jy, jz, 2),
                                        EDGE(jx, jy, jz, 2),
                                    };

                                    unsigned edges = edgeTable[cubeIndex];
                                    if (edges & 0b1)            USE_VERTEX(edgeToVertex[0],  x - 1, y,     z - 1, x,     y,     z - 1);
                                    if (edges & 0b10)           USE_VERTEX(edgeToVertex[1],  x,     y,     z - 1, x,     y - 1, z - 1);
                                    if (edges & 0b100)          USE_VERTEX(edgeToVertex[2],  x,     y - 1, z - 1, x - 1, y - 1, z - 1);
                                    if (edges & 0b1000)         USE_VERTEX(edgeToVertex[3],  x - 1, y - 1, z - 1, x - 1, y,     z - 1);
                                    if (edges & 0b10000)        USE_VERTEX(edgeToVertex[4],  x - 1, y,     z,     x,     y,     z    );
                                    if (edges & 0b100000)       USE_VERTEX(edgeToVertex[5],  x,     y,     z,     x,     y - 1, z    );
                                    if (edges & 0b1000000)      USE_VERTEX(edgeToVertex[6],  x,     y - 1, z,     x - 1, y - 1, z    );
                                    if (edges & 0b10000000)     USE_VERTEX(edgeToVertex[7],  x - 1, y - 1, z,     x - 1, y,     z    );
                                    if (edges & 0b100000000)    USE_VERTEX(edgeToVertex[8],  x - 1, y,     z - 1, x - 1, y,     z    );
                                    if (edges & 0b1000000000)   USE_VERTEX(edgeToVertex[9],  x,     y,     z - 1, x,     y,     z    );
                                    if (edges & 0b10000000000)  USE_VERTEX(edgeToVertex[10], x,     y - 1, z - 1, x,     y - 1, z    );
                                    if (edges & 0b100000000000) USE_VERTEX(edgeToVertex[11], x - 1, y - 1, z - 1, x - 1, y - 1, z    );

                                    for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
                                        voxelChunk->cacheFaces.emplace_back(
                                            edgeVertices[edgeToVertex[triTable[cubeIndex][i    ]]],
                                            edgeVertices[edgeToVertex[triTable[cubeIndex][i + 1]]],
                                            edgeVertices[edgeToVertex[triTable[cubeIndex][i + 2]]]);
                                    }

                                }
                            }
                        }

#undef USE_VERTEX
#undef FROM_EDGE
#undef EDGE

                    }

                    if (voxelChunk->edgeCacheNeedsUpdating || force) {
                        voxelChunk->edgeCacheNeedsUpdating = false;

                        voxelChunk->edgeCacheVertices.clear();
                        voxelChunk->edgeCacheNormals.clear();
                        voxelChunk->edgeCacheFaces.clear();

                        for (int ix = 0; ix < VOXEL_CHUNK_DIMENSION; ++ix) {
                            for (int iy = 0; iy < VOXEL_CHUNK_DIMENSION; ++iy) {
                                addMarchingCube(cx * VOXEL_CHUNK_DIMENSION + ix,
                                                cy * VOXEL_CHUNK_DIMENSION + iy,
                                                cz * VOXEL_CHUNK_DIMENSION,
                                                isolevel,
                                                voxelChunk->edgeCacheVertices,
                                                voxelChunk->edgeCacheNormals,
                                                voxelChunk->edgeCacheFaces);
                            }
                        }
                        for (int ix = 0; ix < VOXEL_CHUNK_DIMENSION; ++ix) {
                            for (int iz = 1; iz < VOXEL_CHUNK_DIMENSION; ++iz) {
                                addMarchingCube(cx * VOXEL_CHUNK_DIMENSION + ix,
                                                cy * VOXEL_CHUNK_DIMENSION,
                                                cz * VOXEL_CHUNK_DIMENSION + iz,
                                                isolevel,
                                                voxelChunk->edgeCacheVertices,
                                                voxelChunk->edgeCacheNormals,
                                                voxelChunk->edgeCacheFaces);
                            }
                        }
                        for (int iy = 1; iy < VOXEL_CHUNK_DIMENSION; ++iy) {
                            for (int iz = 1; iz < VOXEL_CHUNK_DIMENSION; ++iz) {
                                addMarchingCube(cx * VOXEL_CHUNK_DIMENSION,
                                                cy * VOXEL_CHUNK_DIMENSION + iy,
                                                cz * VOXEL_CHUNK_DIMENSION + iz,
                                                isolevel,
                                                voxelChunk->edgeCacheVertices,
                                                voxelChunk->edgeCacheNormals,
                                                voxelChunk->edgeCacheFaces);
                            }
                        }
                    }

                    // Add all vertices in chunk to the vertices array

                    auto numVertices = (unsigned int) vertices.size();
                    vertices.insert(vertices.end(), voxelChunk->cacheVertices.begin(), voxelChunk->cacheVertices.end());
                    normals.insert(normals.end(), voxelChunk->cacheNormals.begin(), voxelChunk->cacheNormals.end());
                    for (glm::uvec3 face : voxelChunk->cacheFaces) {
                        faces.push_back(glm::uvec3(numVertices) + face);
                    }

                    numVertices = (unsigned int) vertices.size();
                    vertices.insert(vertices.end(), voxelChunk->edgeCacheVertices.begin(), voxelChunk->edgeCacheVertices.end());
                    normals.insert(normals.end(), voxelChunk->edgeCacheNormals.begin(), voxelChunk->edgeCacheNormals.end());
                    for (glm::uvec3 face : voxelChunk->edgeCacheFaces) {
                        faces.push_back(glm::uvec3(numVertices) + face);
                    }

                }
            }
        }

    }

}