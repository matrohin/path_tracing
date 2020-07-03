#include "rendering/camera.h"
#include "rendering/scene.h"
#include "view/view.h"

#include <cstdint>

struct RenderingContext {
  Camera camera;
  Scene scene;
  uint32_t samples_num = 0;
  uint32_t max_depth = 0;
};

void render(View& view, const RenderingContext& context);
