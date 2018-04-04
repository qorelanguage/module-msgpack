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

#include "qore/Qore.h"

// mpack library
#include "mpack.h"

namespace msgpack {

// error messages for mpack errors
extern const char* MpackErrorIO;
extern const char* MpackErrorInvalid;
extern const char* MpackErrorType;
extern const char* MpackErrorTooBig;
extern const char* MpackErrorMemory;
extern const char* MpackErrorBug;
extern const char* MpackErrorData;
extern const char* MpackErrorUnknownIntern;

//! MessagePack module exception class.
class MsgPackException {
public:
    DLLLOCAL MsgPackException(const char* nerr = MpackErrorUnknownIntern) : err(nerr) {}

    const char* err;
};

DLLLOCAL MsgPackException getMsgPackException(mpack_error_t error);

} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACKEXCEPTION_H

