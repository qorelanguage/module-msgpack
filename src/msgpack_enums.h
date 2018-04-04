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

DLLLOCAL inline bool checkOperationMode(int64 mode) {
    if (mode == (int64)msgpack::MSGPACK_SIMPLE_MODE)
        return false;
    if (mode == (int64)msgpack::MSGPACK_QORE_MODE)
        return false;
    return true;
}

DLLLOCAL inline bool checkOperationMode(ExceptionSink* xsink, int64 mode) {
    if (checkOperationMode(mode)) {
        xsink->raiseException("INVALID-MODE", "passed operation mode is invalid");
        return true;
    }
    return false;
}

DLLLOCAL inline EncodingId getEncodingId(const QoreEncoding* enc) {
    // cannot use switch here because of pointer comparison
    if (enc == QCS_USASCII) { return QE_USASCII; }
    else if (enc == QCS_UTF8) { return QE_UTF8; }
    else if (enc == QCS_UTF16) { return QE_UTF16; }
    else if (enc == QCS_UTF16BE) { return QE_UTF16BE; }
    else if (enc == QCS_UTF16LE) { return QE_UTF16LE; }
    else if (enc == QCS_ISO_8859_1) { return QE_ISO_8859_1; }
    else if (enc == QCS_ISO_8859_2) { return QE_ISO_8859_2; }
    else if (enc == QCS_ISO_8859_3) { return QE_ISO_8859_3; }
    else if (enc == QCS_ISO_8859_4) { return QE_ISO_8859_4; }
    else if (enc == QCS_ISO_8859_5) { return QE_ISO_8859_5; }
    else if (enc == QCS_ISO_8859_6) { return QE_ISO_8859_6; }
    else if (enc == QCS_ISO_8859_7) { return QE_ISO_8859_7; }
    else if (enc == QCS_ISO_8859_8) { return QE_ISO_8859_8; }
    else if (enc == QCS_ISO_8859_9) { return QE_ISO_8859_9; }
    else if (enc == QCS_ISO_8859_10) { return QE_ISO_8859_10; }
    else if (enc == QCS_ISO_8859_11) { return QE_ISO_8859_11; }
    else if (enc == QCS_ISO_8859_13) { return QE_ISO_8859_13; }
    else if (enc == QCS_ISO_8859_14) { return QE_ISO_8859_14; }
    else if (enc == QCS_ISO_8859_15) { return QE_ISO_8859_15; }
    else if (enc == QCS_ISO_8859_16) { return QE_ISO_8859_16; }
    else if (enc == QCS_KOI8_R) { return QE_KOI8_R; }
    else if (enc == QCS_KOI8_U) { return QE_KOI8_U; }
    else if (enc == QCS_KOI7) { return QE_KOI7; }

    return QE_UTF8;
}

DLLLOCAL inline const QoreEncoding* getEncodingFromId(EncodingId encId) {
    switch(encId) {
        case QE_USASCII: return QCS_USASCII;
        case QE_UTF8: return QCS_UTF8;
        case QE_UTF16: return QCS_UTF16;
        case QE_UTF16BE: return QCS_UTF16BE;
        case QE_UTF16LE: return QCS_UTF16LE;
        case QE_ISO_8859_1: return QCS_ISO_8859_1;
        case QE_ISO_8859_2: return QCS_ISO_8859_2;
        case QE_ISO_8859_3: return QCS_ISO_8859_3;
        case QE_ISO_8859_4: return QCS_ISO_8859_4;
        case QE_ISO_8859_5: return QCS_ISO_8859_5;
        case QE_ISO_8859_6: return QCS_ISO_8859_6;
        case QE_ISO_8859_7: return QCS_ISO_8859_7;
        case QE_ISO_8859_8: return QCS_ISO_8859_8;
        case QE_ISO_8859_9: return QCS_ISO_8859_9;
        case QE_ISO_8859_10: return QCS_ISO_8859_10;
        case QE_ISO_8859_11: return QCS_ISO_8859_11;
        case QE_ISO_8859_13: return QCS_ISO_8859_13;
        case QE_ISO_8859_14: return QCS_ISO_8859_14;
        case QE_ISO_8859_15: return QCS_ISO_8859_15;
        case QE_ISO_8859_16: return QCS_ISO_8859_16;
        case QE_KOI8_R: return QCS_KOI8_R;
        case QE_KOI8_U: return QCS_KOI8_U;
        case QE_KOI7: return QCS_KOI7;
        default: break;
    }

    return QCS_UTF8;
}

} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_ENUMS_H

