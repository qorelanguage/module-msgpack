/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  msgpack_extensions.cpp

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

#include "msgpack_extensions.h"

// std
#include <cmath>
#include <cstdint>
#include <memory>

// module sources
#include "msgpack_enums.h"

namespace msgpack {
namespace intern {

//-----------------------------
// Extension packing functions
//-----------------------------

void msgpack_pack_ext_date(mpack_writer_t* writer, const DateTimeNode* date) {
    char bytes[sizeof(int64_t)];

    if (date->isAbsolute()) {
        // get date parts
        char absolute = 1;
        qore_tm tm;
        date->getInfo(tm);
        int32_t offset = tm.utc_secs_east;
        int64_t epoch = date->getEpochSecondsUTC();
        int32_t us = date->getMicrosecond();

        // write header
        mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_DATE, 1 + sizeof(int64_t) + 2*sizeof(int32_t));
        mpack_write_bytes(writer, &absolute, 1);

        // write data
        mpack_store_u64(bytes, (uint64_t) epoch);
        mpack_write_bytes(writer, bytes, sizeof(int64_t));

        mpack_store_u32(bytes, (uint32_t) us);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) offset);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));
    }
    else { // relative date
        // get date parts
        char absolute = 0;
        int32_t year = date->getYear();
        int32_t month = date->getMonth();
        int32_t day = date->getDay();
        int32_t hour = date->getHour();
        int32_t minute = date->getMinute();
        int32_t second = date->getSecond();
        int32_t us = date->getMicrosecond();

        // write header
        mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_DATE, 1 + 7*sizeof(int32_t));
        mpack_write_bytes(writer, &absolute, 1);

        // write data
        mpack_store_u32(bytes, (uint32_t) year);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) month);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) day);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) hour);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) minute);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) second);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));

        mpack_store_u32(bytes, (uint32_t) us);
        mpack_write_bytes(writer, bytes, sizeof(int32_t));
    }

    mpack_finish_ext(writer);
}

void msgpack_pack_ext_ext(mpack_writer_t* writer, const MsgPackExtension* ext) {
    uint32_t size = ext->getSize();

    mpack_start_ext(writer, ext->getType(), size);
    mpack_write_bytes(writer, ext->getBuffer(), size);
    mpack_finish_ext(writer);
}

void msgpack_pack_ext_null(mpack_writer_t* writer) {
    // do not pass nullptr to mpack_write_ext() or it will assert
    mpack_write_ext(writer, (int8_t) MSGPACK_EXT_QORE_NULL, "", 0);
}

void msgpack_pack_ext_number(mpack_writer_t* writer, const QoreNumberNode* number) {
    if (number->nan()) {
        mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_NUMBER, 1);
        char numberType = (char) MSGPACK_NUMBER_NAN;
        mpack_write_bytes(writer, &numberType, 1);
    }
    else if (number->inf()) {
        mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_NUMBER, 1);
        char numberType = (number->sign() > 0) ? (char) MSGPACK_NUMBER_INF : (char) MSGPACK_NUMBER_NINF;
        mpack_write_bytes(writer, &numberType, 1);
    }
    else {
        // prepare number string
        QoreString str(QCS_USASCII);
        number->toString(str, QORE_NF_SCIENTIFIC|QORE_NF_RAW);

        // get precision
        uint32_t prec = number->getPrec();

        // write header
        mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_NUMBER, 1 + sizeof(uint32_t) + str.size());

        // writer number type
        char numberType = (char) MSGPACK_NUMBER_NORM;
        mpack_write_bytes(writer, &numberType, 1);

        // write number precision
        char bytes[sizeof(uint32_t)];
        mpack_store_u32(bytes, prec);
        mpack_write_bytes(writer, bytes, sizeof(uint32_t));

        // write number
        mpack_write_bytes(writer, str.c_str(), str.size());
    }

    mpack_finish_ext(writer);
}

void msgpack_pack_ext_string(mpack_writer_t* writer, const QoreString* str) {
    uint32_t size = str->size() + 1;
    char enc = (char) getEncodingId(str->getEncoding());

    mpack_start_ext(writer, (int8_t) MSGPACK_EXT_QORE_STRING, size);
    mpack_write_bytes(writer, &enc, 1);
    mpack_write_bytes(writer, str->c_str(), str->size());
    mpack_finish_ext(writer);
}

void msgpack_pack_ext_timestamp(mpack_writer_t* writer, const DateTimeNode* date) {
    int64_t seconds = date->getEpochSecondsUTC();
    int32_t ns = date->getMicrosecond() * 1000;

    if (ns == 0)
        mpack_write_timestamp_seconds(writer, seconds);
    else
        mpack_write_timestamp(writer, seconds, ns);
}


//-------------------------------
// Extension unpacking functions
//-------------------------------

AbstractQoreNode* msgpack_unpack_ext_timestamp(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    mpack_timestamp_t timestamp = mpack_read_timestamp(reader, mpack_tag_ext_length(&tag));
    return DateTimeNode::makeAbsolute(nullptr, timestamp.seconds, timestamp.nanoseconds / 1000);
}

AbstractQoreNode* msgpack_unpack_ext_date(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    char bytes[sizeof(int64_t)];
    SimpleRefHolder<DateTimeNode> result;

    // find out if the date is absolute or relative
    char absolute;
    mpack_read_bytes(reader, &absolute, 1);

    if (absolute == 1) {
        mpack_read_bytes(reader, bytes, sizeof(int64_t));
        int64_t epoch = (int64_t) mpack_load_u64(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t us = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t offset = (int32_t) mpack_load_u32(bytes);

        result = DateTimeNode::makeAbsolute(findCreateOffsetZone(offset), epoch, us);
    }
    else { // relative date
        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t year = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t month = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t day = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t hour = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t minute = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t second = (int32_t) mpack_load_u32(bytes);

        mpack_read_bytes(reader, bytes, sizeof(int32_t));
        int32_t us = (int32_t) mpack_load_u32(bytes);

        result = DateTimeNode::makeRelative(year, month, day, hour, minute, second, us);
    }

    mpack_done_ext(reader);
    return result.release();
}

AbstractQoreNode* msgpack_unpack_ext_null(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    mpack_done_ext(reader);
    return null();
}

AbstractQoreNode* msgpack_unpack_ext_number(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    // read number type
    char numberType;
    mpack_read_bytes(reader, &numberType, 1);

    // read/prepare number
    SimpleRefHolder<QoreNumberNode> result;
    switch ((NumberExtensionType) numberType) {
        case MSGPACK_NUMBER_NAN:
            result = new QoreNumberNode(nan(""));
            break;
        case MSGPACK_NUMBER_INF:
            result = new QoreNumberNode(std::numeric_limits<double>::infinity());
            break;
        case MSGPACK_NUMBER_NINF:
            result = new QoreNumberNode(-(std::numeric_limits<double>::infinity()));
            break;
        case MSGPACK_NUMBER_NORM: {
            // check extension size
            uint32_t size = mpack_tag_ext_length(&tag);
            if (size < 6) { // 1B numberType + 4B prec + 1B number string
                mpack_reader_flag_error(reader, mpack_error_data);
                mpack_done_ext(reader);
                return nullptr;
            }
            size -= 5;

            // read precision
            char bytes[sizeof(uint32_t)];
            mpack_read_bytes(reader, bytes, sizeof(uint32_t));
            uint32_t prec = mpack_load_u32(bytes);

            // read number string
            std::unique_ptr<char[]> str(new char[size+1]);
            mpack_read_bytes(reader, str.get(), size);
            str[size] = '\0';

            // prepare result number
            result = new QoreNumberNode(str.get(), prec);
            break;
        }
        default:
            mpack_reader_flag_error(reader, mpack_error_data);
            mpack_done_ext(reader);
            return nullptr;
    }

    mpack_done_ext(reader);
    return result.release();
}

AbstractQoreNode* msgpack_unpack_ext_string(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    uint32_t size = mpack_tag_ext_length(&tag);
    if (size < 1) {
        mpack_reader_flag_error(reader, mpack_error_data);
        // TODO add xsink exception
        return nullptr;
    }

    // get encoding
    char encId[1];
    mpack_read_bytes(reader, encId, 1);
    const QoreEncoding* enc = msgpack::getEncodingFromId(static_cast<msgpack::EncodingId>(*encId));

    // prepare string node and buffer for reading
    SimpleRefHolder<QoreStringNode> str(new QoreStringNode);
    str->allocate(size--);
    qore_size_t allocated = str->capacity();
    char* buffer = str->giveBuffer();

    // read string
    mpack_read_bytes(reader, buffer, size);
    buffer[size] = '\0';
    str->set(buffer, size, allocated, enc);

    mpack_done_ext(reader);
    return str.release();
}

} // namespace intern
} // namespace msgpack
