/* indent-tabs-mode: nil -*- */
/*
  msgpack-module.cpp

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

// qore
#include "qore/Qore.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// module sources
#include "QC_MsgPack.h"
#include "QC_MsgPackExtension.h"

void init_msgpack_functions(QoreNamespace& ns);
void init_msgpack_constants(QoreNamespace& ns);

QoreStringNode *msgpack_module_init();
void msgpack_module_ns_init(QoreNamespace *rns, QoreNamespace *qns);
void msgpack_module_delete();

// qore module symbols
DLLEXPORT char qore_module_name[] = "msgpack";
DLLEXPORT char qore_module_version[] = PACKAGE_VERSION;
DLLEXPORT char qore_module_description[] = "MessagePack module";
DLLEXPORT char qore_module_author[] = "Ondrej Musil <ondrej.musil@qoretechnologies.com>";
DLLEXPORT char qore_module_url[] = "https://github.com/qorelanguage/module-msgpack";
DLLEXPORT int qore_module_api_major = QORE_MODULE_API_MAJOR;
DLLEXPORT int qore_module_api_minor = QORE_MODULE_API_MINOR;
DLLEXPORT qore_module_init_t qore_module_init = msgpack_module_init;
DLLEXPORT qore_module_ns_init_t qore_module_ns_init = msgpack_module_ns_init;
DLLEXPORT qore_module_delete_t qore_module_delete = msgpack_module_delete;

DLLEXPORT qore_license_t qore_module_license = QL_MIT;
DLLEXPORT char qore_module_license_str[] = "MIT";

QoreNamespace MsgPackNS("msgpack");

QoreStringNode *msgpack_module_init() {
    MsgPackNS.addSystemClass(initMsgPackClass(MsgPackNS));
    MsgPackNS.addSystemClass(initMsgPackExtensionClass(MsgPackNS));
    init_msgpack_functions(MsgPackNS);
    init_msgpack_constants(MsgPackNS);

    return 0;
}

void msgpack_module_ns_init(QoreNamespace *rns, QoreNamespace *qns) {
    qns->addNamespace(MsgPackNS.copy());
}

void msgpack_module_delete() {
    // nothing to do here in this case
}

