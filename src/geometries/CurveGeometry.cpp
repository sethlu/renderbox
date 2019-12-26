#include "Matrix.h"
#include "CurveGeometry.h"
#include "logging.h"


namespace renderbox {

    CurveGeometry::CurveGeometry(std::shared_ptr<Curve> curveDescriptor)
        : curveDescriptor(std::move(curveDescriptor)) {

    }

    void CurveGeometry::updateGeometry() {

        auto numCrossSectionVertices = crossSection.size();

        vertices.clear();
        faces.clear();
        uvs.clear();
        normals.clear();

        if (curveDescriptor->empty() || numCrossSectionVertices == 0) {
            return;
        }

        const auto &pointsAndTangents = curveDescriptor->getPointsAndTangentsWithInterval(4.f);

        for (int i = 0; i < pointsAndTangents.size(); i++) {

            vec3 up(0, 0, 1);
            vec3 point = pointsAndTangents[i].first;
            vec3 tangent = glm::normalize(pointsAndTangents[i].second);

            mat4 mat(vec4(glm::cross(up, -tangent), 0),
                     vec4(up, 0),
                     vec4(-tangent, 0),
                     vec4(point, 1));

            for (auto const &cross : crossSection) {
                vertices.emplace_back(mat * vec4(cross, 0, 1));
            }

            if (i > 0) {
                auto va = numCrossSectionVertices * (i - 1);
                auto vb = numCrossSectionVertices * i;

                for (auto v = 0; v < numCrossSectionVertices; v++) {
                    faces.emplace_back(vec3(
                            va + v,
                            vb + v,
                            vb + (v + 1) % numCrossSectionVertices));
                    faces.emplace_back(vec3(
                            vb + (v + 1) % numCrossSectionVertices,
                            va + (v + 1) % numCrossSectionVertices,
                            va + v));
                }
            }

        }

        regenerateNormals();

        // Update geometry version
        didUpdate();

    }

}
