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

#ifndef APP_TRANSACTION_H_INCLUDED
#define APP_TRANSACTION_H_INCLUDED
#pragma once

#include <string>

namespace app {

  class Cmd;
  class CmdTransaction;
  class Context;
  class DocumentUndo;

  enum Modification {
    ModifyDocument,      // This item changes the "saved status" of the document.
    DoesntModifyDocument // This item doesn't modify the document.
  };

  // High-level class to group a set of commands to modify the
  // document atomically, with enough information to rollback the
  // whole operation if something fails (e.g. an exceptions is thrown)
  // in the middle of the procedure.
  //
  // You have to wrap every call to an transaction with a
  // ContextWriter. The preferred usage is as follows:
  //
  // {
  //   ContextWriter writer(context);
  //   Transaction transaction(context, "My big operation");
  //   ...
  //   transaction.commit();
  // }
  //
  class Transaction {
  public:
    // Starts a undoable sequence of operations in a transaction that
    // can be committed or rollbacked.  All the operations will be
    // grouped in the sprite's undo as an atomic operation.
    Transaction(Context* ctx, const std::string& label, Modification mod = ModifyDocument);
    virtual ~Transaction();

    // This must be called to commit all the changes, so the undo will
    // be finally added in the sprite.
    //
    // If you don't use this routine, all the changes will be discarded
    // (if the sprite's undo was enabled when the Transaction was
    // created).
    void commit();

    void execute(Cmd* cmd);

  private:
    void rollback();

    Context* m_ctx;
    DocumentUndo* m_undo;
    CmdTransaction* m_cmds;
  };

} // namespace app

#endif
