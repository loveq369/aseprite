/* Aseprite
 * Copyright (C) 2001-2013  David Capello
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

#ifndef APP_UI_EDITOR_MOVING_PIXELS_STATE_H_INCLUDED
#define APP_UI_EDITOR_MOVING_PIXELS_STATE_H_INCLUDED
#pragma once

#include "app/settings/settings_observers.h"
#include "app/ui/context_bar_observer.h"
#include "app/ui/editor/editor_observer.h"
#include "app/ui/editor/handle_type.h"
#include "app/ui/editor/pixels_movement.h"
#include "app/ui/editor/standby_state.h"
#include "app/ui/status_bar.h"
#include "base/connection.h"

namespace doc {
  class Image;
}

namespace app {
  class Command;
  class Editor;

  class MovingPixelsState
    : public StandbyState
    , EditorObserver
    , SelectionSettingsObserver
    , ContextBarObserver {
  public:
    MovingPixelsState(Editor* editor, ui::MouseMessage* msg, PixelsMovementPtr pixelsMovement, HandleType handle);
    virtual ~MovingPixelsState();

    void translate(const gfx::Point& delta);

    // EditorState
    virtual BeforeChangeAction onBeforeChangeState(Editor* editor, EditorState* newState) override;
    virtual void onCurrentToolChange(Editor* editor) override;
    virtual bool onMouseDown(Editor* editor, ui::MouseMessage* msg) override;
    virtual bool onMouseUp(Editor* editor, ui::MouseMessage* msg) override;
    virtual bool onMouseMove(Editor* editor, ui::MouseMessage* msg) override;
    virtual bool onSetCursor(Editor* editor) override;
    virtual bool onKeyDown(Editor* editor, ui::KeyMessage* msg) override;
    virtual bool onKeyUp(Editor* editor, ui::KeyMessage* msg) override;
    virtual bool onUpdateStatusBar(Editor* editor) override;
    virtual bool acceptQuickTool(tools::Tool* tool) override;
    virtual bool regenerateDrawingCursor() override { return false; }

    // EditorObserver
    virtual void onBeforeFrameChanged(Editor* editor) override;
    virtual void onBeforeLayerChanged(Editor* editor) override;

    // SettingsObserver
    virtual void onSetMoveTransparentColor(app::Color newColor) override;

    // ContextBarObserver
    virtual void onDropPixels(ContextBarObserver::DropAction action) override;

    virtual gfx::Transformation getTransformation(Editor* editor) override;

  private:
    // ContextObserver
    void onBeforeCommandExecution(Command* command);

    void setTransparentColor(const app::Color& color);
    void dropPixels(Editor* editor);

    bool isActiveDocument() const;
    bool isActiveEditor() const;

    // Helper member to move/translate selection and pixels.
    PixelsMovementPtr m_pixelsMovement;
    Editor* m_editor;

    // True if the image was discarded (e.g. when a "Cut" command was
    // used to remove the dragged image).
    bool m_discarded;

    Connection m_ctxConn;
  };

} // namespace app

#endif  // APP_UI_EDITOR_MOVING_PIXELS_STATE_H_INCLUDED
