/* dpf wrapper for python 3
 * Completely rewritten for python 3
 * Original by Martin Strubel and BöserFisch
 */

#include "Python.h"
#include "dpf.h"

#define HANDLE_ERROR(err) handleError(err, __LINE__)

static PyObject*
handleError(int err, int line) {
    PyObject* exctype = PyExc_SystemError;
    char s[256];

    // catch some special errors
    switch (err) {
        case DEVERR_OPEN:
            exctype = PyExc_TypeError;
            snprintf(s, sizeof(s), "%d:%s", line, dev_errstr(err));
            break;
        default:
            snprintf(s, sizeof(s), "%d:(%x): %s", line, err, dev_errstr(err));
            break;
    }

    PyErr_SetString(exctype, s);
    return NULL;
}

typedef struct {
    PyObject_HEAD
    DPFHANDLE dpf;
} dpfObject;

static PyObject*
dpfClose(dpfObject* self, PyObject* args) {
    if(!PyArg_ParseTuple(args, ""))
        return NULL;

    if(self->dpf) {
        dpf_close(self->dpf);
        self->dpf = NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfSetProperty(dpfObject* self, PyObject* args) {
    int prop;
    PyObject* p;
    DPFValue v;
    int error;

    if(!PyArg_ParseTuple(args, "iO", &prop, &p))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    if(PyLong_Check(p)) {
        v.type = TYPE_INTEGER;
        v.value.integer = PyLong_AsLong(p);
    }

    error = dpf_setproperty(self->dpf, prop, &v);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfRun(dpfObject* self, PyObject* args) {
    ADDR addr;
    int error;

    if(!PyArg_ParseTuple(args, "I", &addr))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = code_go(self->dpf, addr);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfProbeFlash(dpfObject* self, PyObject* args) {
    int error;
    unsigned char id[3];

    if(!PyArg_ParseTuple(args, ""))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = flash_probe(self->dpf, id);
    if (error < 0)
        return HANDLE_ERROR(error);

    return Py_BuildValue("(iii)", id[0], id[1], id[2]);
}

static PyObject*
dpfEraseFlash(dpfObject* self, PyObject* args) {
    ADDR addr = 0xffffffff;
    int error;

    if(!PyArg_ParseTuple(args, "|I", &addr))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = (addr == 0xffffffff)        ?
            flash_erase_full(self->dpf) :
            flash_erase(self->dpf, addr);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfPatchSector(dpfObject* self, PyObject* args) {
    ADDR reloc;
    ADDR addr;
    const char* hexfile;
    int error;

    if(!PyArg_ParseTuple(args, "IIs", &reloc, &addr, &hexfile))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    if(addr < 0x10000) {
        PyErr_SetString(PyExc_ValueError,
            "Not allowed to write to first sector. It will brick your device.");
        return NULL;
    }

    error = patch_sector(self->dpf, reloc, addr, hexfile);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfLoadApp(dpfObject* self, PyObject* args) {
    char* name;
    int error;

    if(!PyArg_ParseTuple(args, "s", &name))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = load_hexfile(self->dpf, name);
    if (error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfReadFlash(dpfObject* self, PyObject* args) {
    ADDR addr;
    unsigned int count;
    PyObject* bytes;
    unsigned char* buf;
    int error = 0;

    if(!PyArg_ParseTuple(args, "II", &addr, &count))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    bytes = PyBytes_FromStringAndSize(NULL, count);
    if(bytes == NULL)
        return NULL;

    buf = (unsigned char*) PyBytes_AsString(bytes);

    while(error >= 0 && count > MAX_CHUNKSIZE) {
        error = flash_read(self->dpf, buf, addr, MAX_CHUNKSIZE);
        buf += MAX_CHUNKSIZE;
        addr += MAX_CHUNKSIZE;
        count -= MAX_CHUNKSIZE;
    }

    if(error >= 0) {
        error = flash_read(self->dpf, buf, addr, count);
    }

    if(error < 0) {
        PyObject_Del(bytes);
        return HANDLE_ERROR(error);
    }

    return bytes;
}

static PyObject*
dpfReadMemory(dpfObject* self, PyObject* args) {
    ADDR addr;
    unsigned int count;
    PyObject* bytes;
    unsigned char* buf;
    int error;

    if(!PyArg_ParseTuple(args, "II", &addr, &count))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    bytes = PyBytes_FromStringAndSize(NULL, count);
    if(bytes == NULL)
        return NULL;

    buf = (unsigned char*) PyBytes_AsString(bytes);

    error = read_mem(self->dpf, buf, addr, count);
    if(error < 0) {
        PyObject_Del(bytes);
        return HANDLE_ERROR(error);
    }

    return bytes;
}

static PyObject*
dpfWriteFlash(dpfObject* self, PyObject* args) {
    ADDR addr;
    PyObject* bytes;
    int error;
    char* buf;
    Py_ssize_t size;

    if(!PyArg_ParseTuple(args, "IO", &addr, &bytes))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = PyBytes_AsStringAndSize(bytes, &buf, &size);
    if(error < 0)
        return NULL;

    error = flash_write(self->dpf, (unsigned char*) buf, addr, size);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
dpfRunApp(dpfObject* self, PyObject* args) {
    ADDR addr;
    PyObject* bytes;
    ADDR start;
    int error;
    char* buf;
    Py_ssize_t size;

    if(!PyArg_ParseTuple(args, "IOI", &addr, &bytes, &start))
        return NULL;

    if(!self->dpf) {
        PyErr_SetString(PyExc_SystemError, "attempted to use closed dpf handle");
        return NULL;
    }

    error = PyBytes_AsStringAndSize(bytes, &buf, &size);
    if(error < 0)
        return NULL;

    error = dpf_bootstrap(self->dpf, addr, (unsigned char *) buf, size, start);
    if(error < 0)
        return HANDLE_ERROR(error);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef dpfMethods[] = {
    {      "close",       (PyCFunction) dpfClose, METH_VARARGS},
    {"setProperty", (PyCFunction) dpfSetProperty, METH_VARARGS},
    {        "run",         (PyCFunction) dpfRun, METH_VARARGS},
    { "probeFlash",  (PyCFunction) dpfProbeFlash, METH_VARARGS},
    { "eraseFlash",  (PyCFunction) dpfEraseFlash, METH_VARARGS},
    {"patchSector", (PyCFunction) dpfPatchSector, METH_VARARGS},
    {    "loadApp",     (PyCFunction) dpfLoadApp, METH_VARARGS},
    {  "readFlash",   (PyCFunction) dpfReadFlash, METH_VARARGS},
    { "readMemory",  (PyCFunction) dpfReadMemory, METH_VARARGS},
    { "writeFlash",  (PyCFunction) dpfWriteFlash, METH_VARARGS},
    {     "runApp",      (PyCFunction) dpfRunApp, METH_VARARGS},
    {         NULL,                         NULL,            0},
};

static void
dpfDealloc(dpfObject* self) {
    if(self->dpf)
        dpf_close(self->dpf);
    PyObject_Del(self);
}

static PyTypeObject dpfType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dpf.DPF",
    .tp_basicsize = sizeof(dpfObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_methods = dpfMethods,
    .tp_dealloc = (destructor) dpfDealloc,
};

static PyObject*
moduleOpen(PyObject* self, PyObject* args) {
    const char* portname;
    DPFHANDLE h;
    int error;
    char errstring[256];
    dpfObject* dpf;

    if(!PyArg_ParseTuple(args, "s", &portname)) {
        return NULL;
    }

    error = dpf_open(portname, &h);

    if(error != 0) {
        if(error > 0)
            dpf_close(h);
        snprintf(errstring, sizeof(errstring),
            "Failed to open port:\n%s\n", dev_errstr(error));
        PyErr_SetString(PyExc_SystemError, errstring);
        return NULL;
    }

    dpf = PyObject_New(dpfObject, &dpfType);
    if(dpf == NULL) {
        dpf_close(h);
        return NULL;
    }

    dpf->dpf = h;

    return (PyObject *) dpf;
}

static PyMethodDef moduleMethods[] = {
    {"open", moduleOpen, METH_VARARGS},
    { NULL,        NULL,            0},
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "dpf",
    NULL,
    -1,
    moduleMethods,
};

PyMODINIT_FUNC
PyInit_dpf(void) {
    return PyModule_Create(&module);
}
