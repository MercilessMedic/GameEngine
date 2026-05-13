#pragma once
#include <memory>
#include "Shader.h"

namespace Shaders
{
    extern std::shared_ptr<Shader> PBR;
    extern std::shared_ptr<Shader> Phong;
    extern std::shared_ptr<Shader> Unlit;
}