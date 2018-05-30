/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
    msgpack_unpack.h

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

#ifndef _QORE_MODULE_MSGPACK_MSGPACK_UNPACK_H
#define _QORE_MODULE_MSGPACK_MSGPACK_UNPACK_H

// qore
#include "qore/Qore.h"

// mpack library
#include "mpack.h"

// module sources
#include "msgpack_enums.h"

namespace msgpack {
namespace intern {

DLLLOCAL QoreListNode* msgpack_unpack_array(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL BinaryNode* msgpack_unpack_binary(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);
DLLLOCAL AbstractQoreNode* msgpack_unpack_ext(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL QoreHashNode* msgpack_unpack_map(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL QoreStringNode* msgpack_unpack_string(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);

DLLLOCAL QoreValue msgpack_unpack_value(mpack_reader_t* reader, OperationMode mode, ExceptionSink* xsink);

DLLLOCAL QoreValue msgpack_unpack(const BinaryNode* data, OperationMode mode, ExceptionSink* xsink);

} // namespace intern
} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_UNPACK_H

