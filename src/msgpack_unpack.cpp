/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
    msgpack_unpack.cpp

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

#include "msgpack_unpack.h"

// std
#include <climits>
#include <cstdint>

// module sources
#include "msgpack_extensions.h"
#include "MsgPackException.h"
#include "MsgPackExtension.h"
#include "QC_MsgPackExtension.h"

namespace msgpack {
namespace intern {

QoreListNode* msgpack_unpack_array(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink) {
    // prepare list
    ReferenceHolder<QoreListNode> list(new QoreListNode, xsink);

    // read all elements
    uint32_t size = tag.v.n;
    for (uint32_t i = 0; i < size; i++) {
        list->push(msgpack_unpack_value(reader, mode, xsink), xsink);
    }

    mpack_done_array(reader);
    return list.release();
}

BinaryNode* msgpack_unpack_binary(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    uint32_t size = tag.v.l;

    // prepare binary node
    SimpleRefHolder<BinaryNode> bin(new BinaryNode);
    bin->preallocate(size);

    // read binary
    mpack_read_bytes(reader, static_cast<char*>(const_cast<void*>(bin->getPtr())), size);

    mpack_done_bin(reader);
    return bin.release();
}

AbstractQoreNode* msgpack_unpack_ext(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink) {
    switch (mode) {
        case MSGPACK_SIMPLE_MODE: {
            uint32_t size = tag.v.ext.length;
            SimpleRefHolder<BinaryNode> bin(new BinaryNode);
            bin->preallocate(size);
            mpack_read_bytes(reader, (char*) bin->getPtr(), size);
            mpack_done_ext(reader);
            return new QoreObject(QC_MSGPACKEXTENSION, getProgram(), new MsgPackExtension(tag.v.ext.exttype, bin.release()));
            break;
        }
        case MSGPACK_QORE_MODE: {
            switch (tag.v.ext.exttype) {
                case MSGPACK_EXT_QORE_NULL:
                    return msgpack_unpack_ext_null(reader, tag, xsink);
                case MSGPACK_EXT_QORE_DATE:
                    return msgpack_unpack_ext_date(reader, tag, xsink);
                case MSGPACK_EXT_QORE_NUMBER:
                    return msgpack_unpack_ext_number(reader, tag, xsink);
                case MSGPACK_EXT_QORE_STRING:
                    return msgpack_unpack_ext_string(reader, tag, xsink);
                default:
                    mpack_reader_flag_error(reader, mpack_error_data);
                    break;
            }
            break;
        }
        default:
            break;
    }

    return nullptr;
}

QoreHashNode* msgpack_unpack_map(mpack_reader_t* reader, mpack_tag_t tag, OperationMode mode, ExceptionSink* xsink) {
    // prepare hash
    ReferenceHolder<QoreHashNode> hash(new QoreHashNode, xsink);

    // read all elements
    uint32_t size = tag.v.n;
    for (uint32_t i = 0; i < size; i++) {
        // read key and value
        ValueHolder key(msgpack_unpack_value(reader, mode, xsink), xsink);
        ValueHolder value(msgpack_unpack_value(reader, mode, xsink), xsink);

        // check key and value
        if (!(*key || *value) || key->getType() != NT_STRING) {
            mpack_reader_flag_error(reader, mpack_error_data);
            break;
        }

        // add element to hash
        hash->setKeyValue(key->get<QoreStringNode>()->c_str(), value.release(), xsink);
    }

    mpack_done_map(reader);
    return hash.release();
}

QoreStringNode* msgpack_unpack_string(mpack_reader_t* reader, mpack_tag_t tag, ExceptionSink* xsink) {
    uint32_t size = tag.v.l;

    // prepare string node and buffer for reading
    SimpleRefHolder<QoreStringNode> str(new QoreStringNode);
    str->allocate(size+1);
    qore_size_t allocated = str->capacity();
    char* buffer = str->giveBuffer();

    // read string
    mpack_read_utf8(reader, buffer, size);
    buffer[size] = '\0';
    str->set(buffer, size, allocated, QCS_UTF8);

    mpack_done_str(reader);
    return str.release();
}


QoreValue msgpack_unpack_value(mpack_reader_t* reader, OperationMode mode, ExceptionSink* xsink) {
    mpack_tag_t tag = mpack_read_tag(reader);

    switch (tag.type) {
        case mpack_type_array:
            return msgpack_unpack_array(reader, tag, mode, xsink);
        case mpack_type_bin:
            return msgpack_unpack_binary(reader, tag, xsink);
        case mpack_type_bool:
            return tag.v.b;
        case mpack_type_double:
            return tag.v.d;
        case mpack_type_ext:
            return msgpack_unpack_ext(reader, tag, mode, xsink);
        case mpack_type_float:
            return tag.v.f;
        case mpack_type_int:
            return tag.v.i;
        case mpack_type_map:
            return msgpack_unpack_map(reader, tag, mode, xsink);
        case mpack_type_nil:
            return QoreValue();
        case mpack_type_str:
            return msgpack_unpack_string(reader, tag, xsink);
        case mpack_type_uint:
            if (tag.v.u <= LLONG_MAX)
                return static_cast<int64>(tag.v.u);
            return new QoreNumberNode(static_cast<double>(tag.v.u));
        default:
            mpack_reader_flag_error(reader, mpack_error_data);
            break;
    }
    return QoreValue();
}


//-------------------------
// msgpack_unpack function
//-------------------------

QoreValue msgpack_unpack(const BinaryNode* data, OperationMode mode, ExceptionSink* xsink) {
    ValueHolder unpacked(xsink);
    const char* dataCheck = nullptr;
    const char* buffer = static_cast<const char*>(data->getPtr());
    size_t remaining = 0;
    size_t size = data->size();
    mpack_reader_t reader;

    // return nothing if no data
    if (buffer == nullptr || size == 0)
        return QoreValue();

    // initialize reader
    mpack_reader_init_data(&reader, buffer, size);

    // unpack the data
    do {
        QoreValue node = msgpack_unpack_value(&reader, mode, xsink);
        if (*unpacked) {
            ReferenceHolder<QoreListNode> list(xsink);
            if (unpacked->getType() == NT_LIST)
                list = unpacked.release().get<QoreListNode>();
            else
                list = new QoreListNode(autoTypeInfo);
            list->push(node, xsink);
            unpacked = list.release();
        }
        else {
            unpacked = node;
        }
        remaining = mpack_reader_remaining(&reader, &dataCheck);
    }
    while (remaining && dataCheck);

    // finish reading
    mpack_error_t result = mpack_reader_destroy(&reader);
    if (result != mpack_ok) {
        throw msgpack::getMsgPackException(result);
    }

    // return unpacked Qore node
    return unpacked.release();
}

} // namespace intern
} // namespace msgpack
