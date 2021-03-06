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

#include "app/cmd/deselect_mask.h"

#include "app/cmd/set_mask.h"
#include "app/document.h"
#include "doc/mask.h"

namespace app {
namespace cmd {

DeselectMask::DeselectMask(Document* doc)
  : WithDocument(doc)
{
}

void DeselectMask::onExecute()
{
  app::Document* doc = document();
  m_oldMask.reset(doc->isMaskVisible() ? new Mask(*doc->mask()): nullptr);
  doc->setMaskVisible(false);
}

void DeselectMask::onUndo()
{
  app::Document* doc = document();

  doc->setMask(m_oldMask);
  doc->setMaskVisible(true);

  m_oldMask.reset(nullptr);
}

size_t DeselectMask::onMemSize() const
{
  return sizeof(*this) + (m_oldMask ? m_oldMask->getMemSize(): 0);
}

} // namespace cmd
} // namespace app
