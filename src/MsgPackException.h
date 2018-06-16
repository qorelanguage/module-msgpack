/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  MsgPackException.h

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

#ifndef _QORE_MODULE_MSGPACK_MSGPACKEXCEPTION_H
#define _QORE_MODULE_MSGPACK_MSGPACKEXCEPTION_H

// qore
#include "qore/Qore.h"

// mpack library
#include "mpack.h"

#include <stdarg.h>

namespace msgpack {

// error messages for mpack errors
DLLLOCAL extern const char* MpackErrorIO;
DLLLOCAL extern const char* MpackErrorInvalid;
DLLLOCAL extern const char* MpackErrorType;
DLLLOCAL extern const char* MpackErrorTooBig;
DLLLOCAL extern const char* MpackErrorMemory;
DLLLOCAL extern const char* MpackErrorBug;
DLLLOCAL extern const char* MpackErrorData;
DLLLOCAL extern const char* MpackErrorUnknownIntern;

//! MessagePack module exception class.
class MsgPackException {
public:
    DLLLOCAL MsgPackException(const char* nerr = MpackErrorUnknownIntern) : err(nerr) {}

    const char* err;
};

class MsgPackExceptionMaker : public MsgPackException {
public:
    DLLLOCAL MsgPackExceptionMaker(const char* fmt, ...) : MsgPackException(nullptr) {
        va_list args;
        while (true) {
            va_start(args, fmt);
            int rc = buf.vsprintf(fmt, args);
            va_end(args);
            if (!rc)
                break;
        }
        err = buf.c_str();
    }

private:
    QoreString buf;
};

DLLLOCAL MsgPackException getMsgPackException(mpack_error_t error);

} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACKEXCEPTION_H

