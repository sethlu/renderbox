#include <iostream>
#include <cfloat>
#include "VoxelGeometry.h"
#include "../renderbox.h"


namespace renderbox {

    int div(int a, int b) {
        return (int) floor((double) a / (double) b);
    }

    int remainder(int a, int b) {
        return a - div(a, b) * b;
    }

    float VoxelGeometry::getOccupancy(int x, int y, int z) {

        int cx = div(x, VOXEL_CHUNK_DIMENSION);
        int cy = div(y, VOXEL_CHUNK_DIMENSION);
        int cz = div(z, VOXEL_CHUNK_DIMENSION);
        int ix = remainder(x, VOXEL_CHUNK_DIMENSION);
        int iy = remainder(y, VOXEL_CHUNK_DIMENSION);
        int iz = remainder(z, VOXEL_CHUNK_DIMENSION);

        auto it0 = voxelChunkPlanes.find(cy);
        if (it0 == voxelChunkPlanes.end()) {
            return false;
        }
        SparseVoxelChunkPlane *chunkPlane = it0->second;

        auto it1 = chunkPlane->voxelChunkLines.find(cx);
        if (it1 == chunkPlane->voxelChunkLines.end()) {
            return false;
        }
        SparseVoxelChunkList *chunkList = it1->second;

        auto it2 = chunkList->voxelChunks.find(cz);
        if (it2 == chunkList->voxelChunks.end()) {
            return false;
        }
        VoxelChunk *voxelChunk = it2->second;

        return voxelChunk->voxels[ix][iy][iz].occupancy;

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

        int cx = div(x, VOXEL_CHUNK_DIMENSION);
        int cy = div(y, VOXEL_CHUNK_DIMENSION);
        int cz = div(z, VOXEL_CHUNK_DIMENSION);
        int ix = remainder(x, VOXEL_CHUNK_DIMENSION);
        int iy = remainder(y, VOXEL_CHUNK_DIMENSION);
        int iz = remainder(z, VOXEL_CHUNK_DIMENSION);

        auto it0 = voxelChunkPlanes.find(cy);
        SparseVoxelChunkPlane *chunkPlane;
        if (it0 == voxelChunkPlanes.end()) {
            chunkPlane = new SparseVoxelChunkPlane();
            voxelChunkPlanes[cy] = chunkPlane;
        } else {
            chunkPlane = it0->second;
        }

        auto it1 = chunkPlane->voxelChunkLines.find(cx);
        SparseVoxelChunkList *chunkList;
        if (it1 == chunkPlane->voxelChunkLines.end()) {
            chunkList = new SparseVoxelChunkList();
            chunkPlane->voxelChunkLines[cx] = chunkList;
        } else {
            chunkList = it1->second;
        }

        auto it2 = chunkList->voxelChunks.find(cz);
        VoxelChunk *voxelChunk;
        if (it2 == chunkList->voxelChunks.end()) {
            voxelChunk = new VoxelChunk();
            chunkList->voxelChunks[cz] = voxelChunk;
        } else {
            voxelChunk = it2->second;
        }

        voxelChunk->voxels[ix][iy][iz].occupancy = occupancy;

    }

    void VoxelGeometry::setOccupancy(glm::ivec3 position, float occupancy) {
        setOccupancy(position.x, position.y, position.z, occupancy);
    }

    void VoxelGeometry::setOccupancy(glm::vec3 position, float occupancy) {
        setOccupancy(glm::ivec3(position), occupancy);
    }

    void VoxelGeometry::addCube(int x, int y, int z) {

        if (!isOccupied(x, y, z)) {return;}

        float length = 1.0f, width = 1.0f, height = 1.0f;
        float length2 = length / 2, width2 = width / 2, height2 = height / 2;
        float px = length * x + length2, py = width * y + width2, pz = height * z + height2;

        unsigned long i = vertices.size();

        vertices.push_back(glm::vec3(px + length2, py + width2, pz + height2));
        vertices.push_back(glm::vec3(px - length2, py + width2, pz + height2));
        vertices.push_back(glm::vec3(px - length2, py - width2, pz + height2));
        vertices.push_back(glm::vec3(px + length2, py - width2, pz + height2));
        vertices.push_back(glm::vec3(px + length2, py + width2, pz - height2));
        vertices.push_back(glm::vec3(px - length2, py + width2, pz - height2));
        vertices.push_back(glm::vec3(px - length2, py - width2, pz - height2));
        vertices.push_back(glm::vec3(px + length2, py - width2, pz - height2));

        faces.push_back(glm::vec3(i + 0, i + 1, i + 2));
        faces.push_back(glm::vec3(i + 2, i + 3, i + 0));

        faces.push_back(glm::vec3(i + 3, i + 2, i + 6));
        faces.push_back(glm::vec3(i + 6, i + 7, i + 3));

        faces.push_back(glm::vec3(i + 7, i + 6, i + 5));
        faces.push_back(glm::vec3(i + 5, i + 4, i + 7));

        faces.push_back(glm::vec3(i + 4, i + 5, i + 1));
        faces.push_back(glm::vec3(i + 1, i + 0, i + 4));

        faces.push_back(glm::vec3(i + 4, i + 0, i + 3));
        faces.push_back(glm::vec3(i + 3, i + 7, i + 4));

        faces.push_back(glm::vec3(i + 1, i + 5, i + 6));
        faces.push_back(glm::vec3(i + 6, i + 2, i + 1));

    }

    void VoxelGeometry::updateGeometry(float isolevel) {

        vertices.clear();
        faces.clear();

        for (int x = -20; x < 20; ++x) {
            for (int y = -20; y < 20; ++y) {
                for (int z = -20; z < 20; ++z) {
                    addCube(x, y, z);
                }
            }
        }

    }

}