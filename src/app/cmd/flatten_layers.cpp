/* Aseprite
 * Copyright (C) 2001-2015  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/cmd/flatten_layers.h"

#include "app/cmd/add_layer.h"
#include "app/cmd/configure_background.h"
#include "app/cmd/copy_rect.h"
#include "app/cmd/remove_layer.h"
#include "app/cmd/remove_layer.h"
#include "app/document.h"
#include "doc/cel.h"
#include "doc/layer.h"
#include "doc/primitives.h"
#include "doc/sprite.h"
#include "render/render.h"

namespace app {
namespace cmd {

FlattenLayers::FlattenLayers(Sprite* sprite)
  : WithSprite(sprite)
{
}

void FlattenLayers::onExecute()
{
  Sprite* sprite = this->sprite();
  app::Document* doc = static_cast<app::Document*>(sprite->document());

  // Create a temporary image.
  ImageRef image(Image::create(sprite->pixelFormat(),
      sprite->width(),
      sprite->height()));

  // If there aren't a background layer we must to create the background.
  LayerImage* background = sprite->backgroundLayer();
  if (!background) {
    background = new LayerImage(sprite);
    executeAndAdd(new cmd::AddLayer(sprite->folder(), background, nullptr));
    executeAndAdd(new cmd::ConfigureBackground(background));
  }

  render::Render render;
  render.setBgType(render::BgType::NONE);
  color_t bgcolor = doc->bgColor(background);

  // Copy all frames to the background.
  for (frame_t frame(0); frame<sprite->totalFrames(); ++frame) {
    // Clear the image and render this frame.
    clear_image(image, bgcolor);
    render.renderSprite(image, sprite, frame);

    ImageRef cel_image;
    Cel* cel = background->cel(frame);
    if (cel) {
      cel_image = cel->imageRef();
      ASSERT(cel_image);

      executeAndAdd(new cmd::CopyRect(cel_image, image,
          gfx::Clip(0, 0, image->bounds())));
    }
    else {
      cel_image.reset(Image::createCopy(image));
      cel = new Cel(frame, cel_image);
      background->addCel(cel);
    }
  }

  // Delete old layers.
  LayerList layers = sprite->folder()->getLayersList();
  for (Layer* layer : layers)
    if (layer != background)
      executeAndAdd(new cmd::RemoveLayer(layer));
}

} // namespace cmd
} // namespace app
