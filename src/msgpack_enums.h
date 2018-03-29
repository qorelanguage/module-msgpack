/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  msgpack_enums.h

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

#ifndef _QORE_MODULE_MSGPACK_MSGPACK_ENUMS_H
#define _QORE_MODULE_MSGPACK_MSGPACK_ENUMS_H

// qore
#include "qore/Qore.h"

namespace msgpack {

enum OperationMode {
    MSGPACK_SIMPLE_MODE = 0,
    MSGPACK_QORE_MODE = 1,
};

enum EncodingId {
    QE_USASCII     = 0,
    QE_UTF8        = 1,
    QE_UTF16       = 2,
    QE_UTF16BE     = 3,
    QE_UTF16LE     = 4,
    QE_ISO_8859_1  = 5,
    QE_ISO_8859_2  = 6,
    QE_ISO_8859_3  = 7,
    QE_ISO_8859_4  = 8,
    QE_ISO_8859_5  = 9,
    QE_ISO_8859_6  = 10,
    QE_ISO_8859_7  = 11,
    QE_ISO_8859_8  = 12,
    QE_ISO_8859_9  = 13,
    QE_ISO_8859_10 = 14,
    QE_ISO_8859_11 = 15,
    QE_ISO_8859_13 = 16,
    QE_ISO_8859_14 = 17,
    QE_ISO_8859_15 = 18,
    QE_ISO_8859_16 = 19,
    QE_KOI8_R      = 20,
    QE_KOI8_U      = 21,
    QE_KOI7        = 22,
};

DLLLOCAL bool checkOperationMode(int64 mode);
DLLLOCAL bool checkOperationMode(ExceptionSink* xsink, int64 mode);

DLLLOCAL EncodingId getEncodingId(const QoreEncoding* enc);
DLLLOCAL const QoreEncoding* getEncodingFromId(EncodingId encId);

} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_ENUMS_H

