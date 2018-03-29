/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  msgpack_pack.h

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

#ifndef _QORE_MODULE_MSGPACK_MSGPACK_PACK_H
#define _QORE_MODULE_MSGPACK_MSGPACK_PACK_H

// qore
#include "qore/Qore.h"

// mpack library
#include "mpack.h"

// module sources
#include "msgpack_enums.h"

namespace msgpack {
namespace intern {

//-----------------------------
// Raw value writing functions
//-----------------------------

DLLLOCAL inline void msgpack_pack_binary(mpack_writer_t* writer, const char* value, uint32_t size) {
    mpack_write_bin(writer, value, size);
}

DLLLOCAL inline void msgpack_pack_bool(mpack_writer_t* writer, bool value) {
    mpack_write_bool(writer, value);
}

DLLLOCAL inline void msgpack_pack_double(mpack_writer_t* writer, double value) {
    mpack_write_double(writer, value);
}

DLLLOCAL inline void msgpack_pack_int(mpack_writer_t* writer, int64 value) {
    mpack_write_i64(writer, value);
}

DLLLOCAL inline void msgpack_pack_nil(mpack_writer_t* writer) {
    mpack_write_nil(writer);
}

DLLLOCAL inline void msgpack_pack_utf8(mpack_writer_t* writer, const char* value, uint32_t size) {
    mpack_write_utf8(writer, value, size);
}


//-------------------------------------
// Qore nodes/values writing functions
//-------------------------------------

DLLLOCAL void msgpack_pack_qore_binary(mpack_writer_t* writer, const BinaryNode* value);
DLLLOCAL void msgpack_pack_qore_bool(mpack_writer_t* writer, QoreValue value);
DLLLOCAL void msgpack_pack_qore_date(mpack_writer_t* writer, const DateTimeNode* value, OperationMode mode);
DLLLOCAL void msgpack_pack_qore_float(mpack_writer_t* writer, QoreValue value);
DLLLOCAL void msgpack_pack_qore_hash(mpack_writer_t* writer, const QoreHashNode* value, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL void msgpack_pack_qore_int(mpack_writer_t* writer, QoreValue value);
DLLLOCAL void msgpack_pack_qore_list(mpack_writer_t* writer, QoreValue value, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL void msgpack_pack_qore_nothing(mpack_writer_t* writer);
DLLLOCAL void msgpack_pack_qore_null(mpack_writer_t* writer, OperationMode mode);
DLLLOCAL void msgpack_pack_qore_number(mpack_writer_t* writer, const QoreNumberNode* value, OperationMode mode);
DLLLOCAL void msgpack_pack_qore_string(mpack_writer_t* writer, const QoreString* value, OperationMode mode, ExceptionSink* xsink);

DLLLOCAL void msgpack_pack_qore_node(mpack_writer_t* writer, const AbstractQoreNode* value, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL void msgpack_pack_qore_value(mpack_writer_t* writer, QoreValue value, OperationMode mode, ExceptionSink* xsink);


//-----------------------
// msgpack_pack function
//-----------------------

DLLLOCAL QoreValue msgpack_pack(QoreValue& data, OperationMode mode, ExceptionSink* xsink);

} // namespace intern
} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_PACK_H

