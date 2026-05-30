#include "Shaders.h"

namespace Shaders 
{
    std::shared_ptr<Shader> PBR = nullptr;
    std::shared_ptr<Shader> Phong = nullptr;
    std::shared_ptr<Shader> Unlit = nullptr;
    std::shared_ptr<Shader> Skybox = nullptr;
    std::shared_ptr<Shader> EquirectangularToCubemap = nullptr;
}