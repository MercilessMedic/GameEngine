#pragma once

#include <vector>
#include "Shaders.h"
#include "VertexArray.h"
#include "Material.h"
#include "UnlitMaterial.h"
class Mesh {
public:
    //By default give the mesh unlitMaterial if not material type is provided.
    Mesh(std::shared_ptr<VertexArray> va)
        : vertexArray(va), material( make_shared<UnlitMaterial>() ) {}

    Mesh(std::shared_ptr<VertexArray> va, std::shared_ptr<Material> mat)
        : vertexArray(va), material(mat) {}

    const std::shared_ptr<VertexArray>& getVertexArray() const { return vertexArray; }
    const std::shared_ptr<Material>& getMaterial() const { return material; }

    void setMaterial(std::shared_ptr<Material> mat) { material = mat; }

private:
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Material> material;
};