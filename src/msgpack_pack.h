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

// std
#include <cstdlib>
#include <memory>

// qore
#include "qore/Qore.h"
#include "qore/qore_bitopts.h"

// mpack library
#include "mpack.h"

// module sources
#include "msgpack_enums.h"
#include "msgpack_extensions.h"
#include "MsgPackException.h"


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

// forward declarations
DLLLOCAL inline void msgpack_pack_qore_string(mpack_writer_t* writer, const QoreString* value, OperationMode mode, ExceptionSink* xsink);
DLLLOCAL inline void msgpack_pack_value(mpack_writer_t* writer, QoreValue value, OperationMode mode, ExceptionSink* xsink);

DLLLOCAL inline void msgpack_pack_qore_binary(mpack_writer_t* writer, const BinaryNode* value) {
    msgpack_pack_binary(writer, static_cast<const char*>(value->getPtr()), value->size());
}

DLLLOCAL inline void msgpack_pack_qore_bool(mpack_writer_t* writer, QoreValue value) {
    msgpack_pack_bool(writer, value.getAsBool());
}

DLLLOCAL inline void getDateStringRepr(const DateTimeNode* date, QoreString& str) {
    if (date->isAbsolute()) {
        if (date->getMicrosecond())
            date->format(str, "YYYY-MM-DDTHH:mm:SS.xxZ");
        else
            date->format(str, "YYYY-MM-DDTHH:mm:SSZ");
    }
    else {
        date->format(str, "IF");
    }
}

DLLLOCAL inline void msgpack_pack_qore_date(mpack_writer_t* writer, const DateTimeNode* value, OperationMode mode) {
    switch (mode) {
        case MSGPACK_SIMPLE_MODE: {
            QoreString str;
            getDateStringRepr(value, str);
            msgpack_pack_utf8(writer, str.c_str(), static_cast<uint32_t>(str.size()));
            break;
        }
        case MSGPACK_QORE_MODE:
            msgpack_pack_ext_date(writer, value);
            break;
        default:
            break;
    }
}

DLLLOCAL inline void msgpack_pack_qore_float(mpack_writer_t* writer, QoreValue value) {
    msgpack_pack_double(writer, value.getAsFloat());
}

DLLLOCAL inline void msgpack_pack_qore_hash(mpack_writer_t* writer, const QoreHashNode* value, OperationMode mode, ExceptionSink* xsink) {
    qore_size_t size = value->size();

    // start map writing
    mpack_start_map(writer, static_cast<uint32_t>(size));

    // write map elements
    HashIterator it(const_cast<QoreHashNode*>(value));
    while (it.next()) {
        // write key
        std::unique_ptr<QoreString> key(it.getKeyString());
        msgpack_pack_qore_string(writer, key.get(), mode, xsink);

        // write value
        msgpack_pack_value(writer, it.get(), mode, xsink);
    }

    // finish map writing
    mpack_finish_map(writer);
}

DLLLOCAL inline void msgpack_pack_qore_int(mpack_writer_t* writer, QoreValue value) {
    msgpack_pack_int(writer, value.getAsBigInt());
}

DLLLOCAL inline void msgpack_pack_qore_list(mpack_writer_t* writer, QoreValue value, OperationMode mode, ExceptionSink* xsink) {
    QoreListNode* listNode = dynamic_cast<QoreListNode*>(value.getInternalNode());
    if (listNode) {
        size_t size = listNode->size();

        // start array writing
        mpack_start_array(writer, static_cast<uint32_t>(size));

        // write array elements
        for (size_t i = 0; i < size; i++) {
            msgpack_pack_value(writer, listNode->retrieve_entry(i), mode, xsink);
        }
    }
    else {
        QoreValueList* list = dynamic_cast<QoreValueList*>(value.getInternalNode());
        if (list) {
            size_t size = list->size();

            // start array writing
            mpack_start_array(writer, static_cast<uint32_t>(size));

            // write array elements
            for (size_t i = 0; i < size; i++) {
                msgpack_pack_value(writer, list->retrieveEntry(i), mode, xsink);
            }
        }
        else {
            mpack_writer_flag_error(writer, mpack_error_data);
        }
    }

    // finish array writing
    mpack_finish_array(writer);
}

DLLLOCAL inline void msgpack_pack_qore_nothing(mpack_writer_t* writer) {
    msgpack_pack_nil(writer);
}

DLLLOCAL inline void msgpack_pack_qore_null(mpack_writer_t* writer, OperationMode mode) {
    switch (mode) {
        case MSGPACK_SIMPLE_MODE:
            msgpack_pack_nil(writer); break;
        case MSGPACK_QORE_MODE:
            msgpack_pack_ext_null(writer); break;
        default:
            break;
    }
}

DLLLOCAL inline void msgpack_pack_qore_number(mpack_writer_t* writer, const QoreNumberNode* value, OperationMode mode) {
    switch (mode) {
        case MSGPACK_SIMPLE_MODE: {
            //QoreStringValueHelper v(value);
            //msgpack_pack_utf8(writer, v->c_str(), static_cast<uint32_t>(v->size()));
            msgpack_pack_double(writer, value->getAsFloat());
            break;
        }
        case MSGPACK_QORE_MODE:
            msgpack_pack_ext_number(writer, value);
            break;
        default:
            break;
    }
}

DLLLOCAL inline void msgpack_pack_qore_string(mpack_writer_t* writer, const QoreString* value, OperationMode mode, ExceptionSink* xsink) {
    if (value->getEncoding() == QCS_UTF8) {
        msgpack_pack_utf8(writer, value->c_str(), value->size());
    }
    else {
        switch (mode) {
            case MSGPACK_SIMPLE_MODE: {
                TempEncodingHelper temp(value, QCS_UTF8, xsink);
                if (xsink && *xsink)
                    mpack_writer_flag_error(writer, mpack_error_data);
                if (*temp)
                    msgpack_pack_utf8(writer, temp->c_str(), temp->size());
                break;
            }
            case MSGPACK_QORE_MODE:
                msgpack_pack_ext_string(writer, value);
                break;
            default:
                break;
        }
    }
}

DLLLOCAL inline void msgpack_pack_value(mpack_writer_t* writer, QoreValue value, OperationMode mode, ExceptionSink* xsink) {
    switch (value.getType()) {
        case NT_BINARY:                     // BinaryNode
            msgpack_pack_qore_binary(writer, value.get<const BinaryNode>()); break;
        case NT_BOOLEAN:                    // QoreBoolNode
            msgpack_pack_qore_bool(writer, value); break;
        case NT_DATE:                       // DateTimeNode
            msgpack_pack_qore_date(writer, value.get<const DateTimeNode>(), mode); break;
        case NT_FLOAT:                      // QoreFloatNode
            msgpack_pack_qore_float(writer, value); break;
        case NT_HASH:                       // QoreHashNode
            msgpack_pack_qore_hash(writer, value.get<const QoreHashNode>(), mode, xsink); break;
        case NT_INT:                        // QoreBigIntNode
            msgpack_pack_qore_int(writer, value); break;
        case NT_LIST:                       // QoreListNode
            msgpack_pack_qore_list(writer, value, mode, xsink); break;
        case NT_NOTHING:                    // QoreNothingNode
            msgpack_pack_qore_nothing(writer); break;
        case NT_NULL:                       // QoreNullNode
            msgpack_pack_qore_null(writer, mode); break;
        case NT_NUMBER:                     // QoreNumberNode
            msgpack_pack_qore_number(writer, value.get<const QoreNumberNode>(), mode); break;
        case NT_STRING:                     // QoreStringNode
            msgpack_pack_qore_string(writer, value.get<const QoreStringNode>(), mode, xsink); break;
        default:
            break;
    }
}


//-----------------------
// msgpack_pack function
//-----------------------

DLLLOCAL inline QoreValue msgpack_pack(QoreValue& data, OperationMode mode, ExceptionSink* xsink) {
    size_t size = 0;
    char* buffer = nullptr;
    mpack_writer_t writer;

    // initialize writer
    mpack_writer_init_growable(&writer, &buffer, &size);

    // pack the data
    msgpack_pack_value(&writer, data, mode, xsink);

    // finish writing
    mpack_error_t result = mpack_writer_destroy(&writer);
    if (result != mpack_ok) {
        throw msgpack::getMsgPackException(result);
    }

    // return a binary node
    QoreValue bin(new BinaryNode(buffer, size));
    return bin;
}

} // namespace intern
} // namespace msgpack

#endif // _QORE_MODULE_MSGPACK_MSGPACK_PACK_H

