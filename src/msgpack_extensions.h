/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  msgpack_extensions.h

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

#ifndef _QORE_MODULE_MSGPACK_MSGPACK_EXTENSIONS_H
#define _QORE_MODULE_MSGPACK_MSGPACK_EXTENSIONS_H

// qore
#include "qore/Qore.h"

// mpack library
#include "mpack.h"

// module sources
#include "MsgPackExtension.h"

namespace msgpack {

// extension types 0-127 are available for use by Qore
enum QoreExtensionType {
    MSGPACK_EXT_QORE_NULL   = 0,
    MSGPACK_EXT_QORE_DATE   = 1,
    MSGPACK_EXT_QORE_NUMBER = 2,
    MSGPACK_EXT_QORE_STRING = 3,
};

enum NumberExtensionType {
    MSGPACK_NUMBER_NAN  = 0,
    MSGPACK_NUMBER_INF  = 1,
    MSGPACK_NUMBER_NINF = 2,
    MSGPACK_NUMBER_NORM = 3,
};

namespace intern {

//-----------------------------
// Extension packing functions
//-----------------------------

/*
    Qore date extension type uses the following format:
    (all data saved in network byte order - big-endian)

    Absolute dates:
    @verbatim
    +-------+-----------+-------------------+----------------+
    |   1   |   epoch   |   micro-seconds   |   UTC offset   |
    +-------+-----------+-------------------+----------------+
    |  1B   |    8B     |        4B         |       4B       |
    @endverbatim

    Relative dates:
    @verbatim
    +-----+--------+---------+-------+--------+----------+--------+----------+
    |  0  |  year  |  month  |  day  |  hour  |  minute  |  secs  |  u-secs  |
    +-----+--------+---------+-------+--------+----------+--------+----------+
    |  1B |   4B   |   4B    |  4B   |   4B   |    4B    |   4B   |    4B    |
    @endverbatim
*/
DLLLOCAL void msgpack_pack_ext_date(mpack_writer_t* writer, const DateTimeNode* date);

DLLLOCAL void msgpack_pack_ext_ext(mpack_writer_t* writer, const MsgPackExtension* ext);

DLLLOCAL void msgpack_pack_ext_null(mpack_writer_t* writer);

/*
    Qore number extension type uses the following format:
    (all data saved in network byte order - big-endian)

    NaN:
    @verbatim
    +----------------------+
    |  MSGPACK_NUMBER_NAN  |
    +----------------------+
    |          1B          |
    @endverbatim

    Infinity:
    @verbatim
    +----------------------+
    |  MSGPACK_NUMBER_INF  |
    +----------------------+
    |          1B          |
    @endverbatim

    Negative infinity:
    @verbatim
    +-----------------------+
    |  MSGPACK_NUMBER_NINF  |
    +-----------------------+
    |          1B           |
    @endverbatim

    Normal numbers:
    @verbatim
    +-----------------------+-------------+-----------------+
    |  MSGPACK_NUMBER_NORM  |  precision  |  number string  |
    +-----------------------+-------------+-----------------+
    |          1B           |     4B      |      1+ B       |
    @endverbatim
*/
DLLLOCAL void msgpack_pack_ext_number(mpack_writer_t* writer, const QoreNumberNode* number);

/*
    Qore string extension type uses the following format:

    @verbatim
    +-------+-------------------------+
    |  enc  |   ... string data ...   |
    +-------+-------------------------+
    |  1B   |     ext. size - 1B      |
    @endverbatim

    I.e. 1 byte for encoding and the rest for string data.
*/
DLLLOCAL void msgpack_pack_ext_string(mpack_writer_t* writer, const QoreString* str);


//-------------------------------
// Extension unpacking functions
//-------------------------------

DLLLOCAL AbstractQoreNode* msgpack_unpack_ext_date(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);
DLLLOCAL AbstractQoreNode* msgpack_unpack_ext_null(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);
DLLLOCAL AbstractQoreNode* msgpack_unpack_ext_number(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);
DLLLOCAL AbstractQoreNode* msgpack_unpack_ext_string(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink);

} // namespace intern
} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_EXTENSIONS_H

