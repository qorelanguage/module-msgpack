/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  MsgPackExtension.h

  Qore MessagePack module

  Copyright (C) 2018 Qore Technologies, s.r.o.

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/

#ifndef _QORE_MODULE_MSGPACK_MSGPACKEXTENSION_H
#define _QORE_MODULE_MSGPACK_MSGPACKEXTENSION_H

// qore
#include "qore/Qore.h"

// mpack library
#include "mpack.h"

namespace msgpack {

//! MessagePack extension wrapper class.
class MsgPackExtension : public AbstractPrivateData {
private:
    int8_t extType = 0;
    SimpleRefHolder<BinaryNode> data;

public:
    DLLLOCAL MsgPackExtension() {}
    DLLLOCAL MsgPackExtension(int8_t exttype, BinaryNode* bin) :
        extType(exttype),
        data(bin) {}

    //! Get extension type.
    DLLLOCAL int8_t getType() const { return extType; }

    //! Get extension's data.
    DLLLOCAL BinaryNode* getReferencedData() { data->ref(); return *data; }

    //! Get extension's data (unreferenced).
    DLLLOCAL BinaryNode* getUnreferencedData() { return *data; }

    //! Get extension's data buffer.
    DLLLOCAL const char* getBuffer() const {
        return data ? static_cast<const char*>(data->getPtr()) : nullptr;
    }

    //! Get extension's data size.
    DLLLOCAL size_t getSize() const { return data ? data->size() : 0; };
};

} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACKEXTENSION_H

