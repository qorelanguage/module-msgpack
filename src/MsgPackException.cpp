/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  MsgPackException.cpp

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

#include "MsgPackException.h"

namespace msgpack {

// error messages for mpack errors
const char* MpackErrorIO            = "The reader or writer failed to fill or flush, or some other file or socket error occurred.";
const char* MpackErrorInvalid       = "The data read is not valid MessagePack.";
const char* MpackErrorType          = "The type or value range did not match what was expected by the caller.";
const char* MpackErrorTooBig        = "A read or write was bigger than the maximum size allowed for that operation.";
const char* MpackErrorMemory        = "An allocation failure occurred.";
const char* MpackErrorBug           = "The MPack API was used incorrectly. (This will always assert in debug mode.)";
const char* MpackErrorData          = "The contained data is not valid.";
const char* MpackErrorUnknownIntern = "Unknown error occured.";

MsgPackException getMsgPackException(mpack_error_t error) {
    switch (error) {
        case mpack_error_io:
            return MsgPackException(MpackErrorIO);
        case mpack_error_invalid:
            return MsgPackException(MpackErrorInvalid);
        case mpack_error_type:
            return MsgPackException(MpackErrorType);
        case mpack_error_too_big:
            return MsgPackException(MpackErrorTooBig);
        case mpack_error_memory:
            return MsgPackException(MpackErrorMemory);
        case mpack_error_bug:
            return MsgPackException(MpackErrorBug);
        case mpack_error_data:
            return MsgPackException(MpackErrorData);
        default:
            return MsgPackException(MpackErrorUnknownIntern);
    }
}

} // namespace msgpack
