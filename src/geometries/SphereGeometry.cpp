#include "SphereGeometry.h"


namespace renderbox {

    SphereGeometry::SphereGeometry(float radius,
                                   unsigned int widthSegments, unsigned int heightSegments,
                                   float phiStart, float phiLength,
                                   float thetaStart, float thetaLength) {

        // TODO: May optimize slightly for the top/bottom vertex

        for (auto h = 0; h <= heightSegments; h++) {
            for (auto w = 0; w <= widthSegments; w++) {
                auto phi = phiStart + phiLength * w / widthSegments;
                auto theta = thetaStart + thetaLength * h / heightSegments;

                auto rst = radius * sin(theta);
                auto vertex = vec3(rst * cos(phi), rst * sin(phi), radius * cos(theta));

                vertices.push_back(vertex);
                normals.push_back(glm::normalize(vertex));

            }
        }

        for (auto h = 0; h < heightSegments; h++) {
            for (auto w = 0; w < widthSegments; w++) {
                auto a = h * (widthSegments + 1) + w;
                auto b = h * (widthSegments + 1) + w + 1;
                auto c = (h + 1) * (widthSegments + 1) + w;
                auto d = (h + 1) * (widthSegments + 1) + w + 1;

                faces.push_back(uvec3(a, c, d));
                faces.push_back(uvec3(d, b, a));

            }
        }

    }

}
