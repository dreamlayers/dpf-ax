/* Device wrapper for python
 *
 * (c) 2003-2010 Martin Strubel <hackfin@section5.ch>
 *
 * This file is part of the netpp library
 * 
 * Additions by BöserFisch for hacked DPF access
 *
 */

#include "Python.h"
#include "dpf.h"

#ifdef DEBUG
#	include <stdio.h>
#endif

#define CONCAT(x, y)   x##y
#define INITMODULE(x)  CONCAT(init, x)
#define RESOLVE(x)     #x
#define STRINGIFY(x)   RESOLVE(x)

////////////////////////////////////////////////////////////////////////////
// AUX

#define HANDLE_ERROR(err, msg) handleError(err, __LINE__)

PyObject *
handleError(int err, int line)
{
	char s[256];
	PyObject *exctype = PyExc_SystemError;

	// catch some special errors
	switch (err) {
		case DEVERR_OPEN:
			exctype = PyExc_TypeError;
			sprintf(s, "%d:%s", line, dev_errstr(err));
			break;
		default:
			sprintf(s, "%d:(%x): %s", line, err, dev_errstr(err));
	}
	PyErr_SetString(exctype, s);
	return NULL;
}

staticforward PyTypeObject DeviceType;

////////////////////////////////////////////////////////////////////////////
//
// Device object
//
////////////////////////////////////////////////////////////////////////////

#define FLAGS_VALID 0x0001

typedef struct {
	PyObject_HEAD
	DPFHANDLE       dpf;
	uint16_t        flags;
} DeviceObject;

staticforward PyTypeObject DeviceType;

#define DeviceObject_Check(v)	((v)->ob_type == &DeviceType)

////////////////////////////////////////////////////////////////////////////
//
// Device object method functions
//
////////////////////////////////////////////////////////////////////////////


static PyObject *
Device_close(DeviceObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	dpf_close(self->dpf);
	self->dpf = NULL;

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *
Device_probeflash(DeviceObject *self, PyObject *args)
{
	int error;
	unsigned char id[3];

	error = flash_probe(self->dpf, id);
	if (error < 0) return HANDLE_ERROR(error, "probeflash");

	return Py_BuildValue("(iii)", id[0], id[1], id[2]);
}

PyObject *
Device_readflash(DeviceObject *self, PyObject *args)
{
	int error;
	unsigned int count;
	ADDR addr;
	Py_ssize_t sz;
	PyObject *p;
	unsigned char *buf;

	if (!PyArg_ParseTuple(args, "II", &addr, &count))
		return NULL;

	p = PyBuffer_New(count);

	error = PyObject_AsWriteBuffer(p, (void**) &buf, &sz);
	while (error >= 0 && count > MAX_CHUNKSIZE) {
		error = flash_read(self->dpf, buf, addr, 0x10000);
		addr += MAX_CHUNKSIZE;
		count -= MAX_CHUNKSIZE;
		buf += MAX_CHUNKSIZE;
	}
	if (error >= 0) {
		error = flash_read(self->dpf, buf, addr, count);
	}
	if (error < 0) {
		Py_DECREF(p);
		return HANDLE_ERROR(error, "readflash");
	}
	return p;	
}

PyObject *
Device_eraseflash(DeviceObject *self, PyObject *args)
{
	int error;
	ADDR addr;

	if (!PyArg_ParseTuple(args, "I", &addr))
		return NULL;

	error = flash_erase(self->dpf, addr);
	if (error < 0) return HANDLE_ERROR(error, "eraseflash");

	Py_INCREF(Py_None);
	return Py_None;

}
PyObject *
Device_writeflash(DeviceObject *self, PyObject *args)
{
	int error;
	PyObject *p;
	ADDR addr;
	const void *buf;
	ssize_t len;

	if (!PyArg_ParseTuple(args, "IO", &addr, &p))
		return NULL;

	error = PyObject_AsReadBuffer(p, &buf, &len);
	if (error < 0) {
		PyErr_SetString(PyExc_TypeError,
			"Could not get pointer to buffer object");
		return NULL;
	}
	
	error = flash_write(self->dpf, (const unsigned char *) buf, addr, len);
	if (error < 0) return HANDLE_ERROR(error, "writeflash");

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *
Device_patchsector(DeviceObject *self, PyObject *args)
{
	int error;
	ADDR reloc;
	const char *hexfile;
	ADDR addr;

	if (!PyArg_ParseTuple(args, "IIs", &reloc, &addr, &hexfile))
		return NULL;

	if (addr < 0x10000) {
		PyErr_SetString(PyExc_ValueError,
			"Not allowed to write to first sector. It will brick your device.");
		return NULL;
	}
	
	error = patch_sector(self->dpf, reloc, addr, hexfile);
	if (error < 0) return HANDLE_ERROR(error, "patchsector");

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *
Device_setProperty(DeviceObject *self, PyObject *args)
{
	int error;
	PyObject *p;
	int prop;
	DPFValue v;

	if (!PyArg_ParseTuple(args, "iO", &prop, &p))
		return NULL;

	if (PyInt_Check(p)) {
		v.type = TYPE_INTEGER;
		v.value.integer = PyInt_AsLong(p);
	}

	error = dpf_setproperty(self->dpf, prop, &v);
	if (error < 0) return HANDLE_ERROR(error, "set_property");

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *
Device_writelcd(DeviceObject *self, PyObject *args)
{
	int error;
	PyObject *p;
	const void *buf;
	ssize_t len;

	if (!PyArg_ParseTuple(args, "O", &p))
		return NULL;

	error = PyObject_AsReadBuffer(p, &buf, &len);
	if (error < 0) {
		PyErr_SetString(PyExc_TypeError,
			"Could not get pointer to buffer object");
		return NULL;
	}
	
	error = write_screen(self->dpf, buf, len);
	if (error < 0) return HANDLE_ERROR(error, "writescreen");

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *
Device_readmem(DeviceObject *self, PyObject *args)
{
	int error;
	int count;
	ADDR addr;
	Py_ssize_t sz;
	PyObject *p;
	unsigned char *buf;

	if (!PyArg_ParseTuple(args, "Ii", &addr, &count))
		return NULL;

	p = PyBuffer_New(count);

	error = PyObject_AsWriteBuffer(p, (void**) &buf, &sz);
	if (error >= 0) {
		error = read_mem(self->dpf, buf, addr, count);
	}
	if (error < 0) {
		Py_DECREF(p);
		return HANDLE_ERROR(error, "readmem");
	}
	return p;	
}

PyObject *
Device_writemem(DeviceObject *self, PyObject *args)
{
	int error;
	char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	error = write_mem(self->dpf, name);
	if (error < 0) return HANDLE_ERROR(error, "hexfile");

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *
Device_exec(DeviceObject *self, PyObject *args)
{
	int error;
	ADDR addr;

	if (!PyArg_ParseTuple(args, "I", &addr))
		return NULL;

	error = code_go(self->dpf, addr);
	if (error < 0) return HANDLE_ERROR(error, "exec");

	Py_INCREF(Py_None);
	return Py_None;

}

PyObject *
Device_runapp(DeviceObject *self, PyObject *args)
{
	int error;
	const void *buf;
	ADDR addr;
	ADDR start;
	PyObject *p;
	ssize_t len;

	if (!PyArg_ParseTuple(args, "IOI", &addr, &p, &start))
		return NULL;

	error = PyObject_AsReadBuffer(p, &buf, &len);
	if (error < 0) return HANDLE_ERROR(error, "read buffer");

	error = dpf_bootstrap(self->dpf, addr, (unsigned char *) buf, len, start);
	if (error < 0) return HANDLE_ERROR(error, "runapp");

	Py_INCREF(Py_None);
	return Py_None;

}
static PyMethodDef Device_methods[] = 
{
	// BASIC ACCESS
	{"close",        (PyCFunction) Device_close,           METH_VARARGS},
	{"probeFlash",   (PyCFunction) Device_probeflash,       METH_VARARGS},
	{"readFlash",    (PyCFunction) Device_readflash,       METH_VARARGS},
	{"eraseFlash",   (PyCFunction) Device_eraseflash,      METH_VARARGS},
	{"writeFlash",   (PyCFunction) Device_writeflash,      METH_VARARGS},
	{"patchSector",  (PyCFunction) Device_patchsector,     METH_VARARGS},
	{"setProperty",  (PyCFunction) Device_setProperty,     METH_VARARGS},
	{"writeLCD",     (PyCFunction) Device_writelcd,        METH_VARARGS},
	{"readMemory",   (PyCFunction) Device_readmem,         METH_VARARGS},
	{"writeMemory",  (PyCFunction) Device_writemem,        METH_VARARGS},
	{"run",          (PyCFunction) Device_exec,            METH_VARARGS},
	{"runApp",       (PyCFunction) Device_runapp,          METH_VARARGS},
	{NULL,		NULL}		/* sentinel */
};

////////////////////////////////////////////////////////////////////////////
// Device type object standard methods
//


static void
Device_dealloc(DeviceObject *self)
{
	if (self->dpf)
		dpf_close(self->dpf);
	PyObject_Del(self);
}


static PyObject *
Device_getattr(DeviceObject *self, char *name)
{
	return Py_FindMethod(Device_methods, (PyObject *)self, name);
}

// finally, the object type definition

statichere PyTypeObject DeviceType = {
	/* The ob_type field must be initialized in the module init function
	 * to be portable to Windows without using C++. */
	PyObject_HEAD_INIT(NULL)
	0,			/*ob_size*/
	"device.Device",		/*tp_name*/
	sizeof(DeviceObject),	/*tp_basicsize*/
	0,			/*tp_itemsize*/
	/* methods */
	(destructor) Device_dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	(getattrfunc) Device_getattr, /*tp_getattr*/
	(setattrfunc) 0, /*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0,			/*tp_as_sequence*/
	0,			/*tp_as_mapping*/
	0,			/*tp_hash*/
        0,                      /*tp_call*/
        0,                      /*tp_str*/
        0,                      /*tp_getattro*/
        0,                      /*tp_setattro*/
        0,                      /*tp_as_buffer*/
        Py_TPFLAGS_DEFAULT,     /*tp_flags*/
        0,                      /*tp_doc*/
        0,                      /*tp_traverse*/
        0,                      /*tp_clear*/
        0,                      /*tp_richcompare*/
        0,                      /*tp_weaklistoffset*/
        0,                      /*tp_iter*/
        0,                      /*tp_iternext*/
        0,                      /*tp_methods*/
        0,                      /*tp_members*/
        0,                      /*tp_getset*/
        0,                      /*tp_base*/
        0,                      /*tp_dict*/
        0,                      /*tp_descr_get*/
        0,                      /*tp_descr_set*/
        0,                      /*tp_dictoffset*/
        0,                      /*tp_init*/
        0,                      /*tp_alloc*/
        0,                      /*tp_new*/
        0,                      /*tp_free*/
        0,                      /*tp_is_gc*/
};



//
// Create new device Python object and return it
//
static DeviceObject *
newDeviceObject(DPFHANDLE h)
{
	DeviceObject *self;
	self = PyObject_New(DeviceObject, &DeviceType);
	if (self == NULL)
		return NULL;
	self->dpf = h;    // device port handle
	self->flags = 0;
	return self;
}

////////////////////////////////////////////////////////////////////////////
//
// Device module definitions
//
////////////////////////////////////////////////////////////////////////////

static PyObject *
device_open(PyObject *self, PyObject *args)
{
	char *portname;
	DPFHANDLE h;
	int error;
	int raw = 0;
	char errstring[256];

	DeviceObject *d;

	if (!PyArg_ParseTuple(args, "s|i", &portname, &raw)) {
		return 0;
	}

	error = dpf_open(portname, &h);

	if (error == 0 || (raw && error > 0)) {
		d = newDeviceObject(h);
		// if no error or warning, device is valid
		if (error == 0) d->flags |= FLAGS_VALID;
		return (PyObject *) d;
	} else {
		// Warnings don't automatically close the device, so
		// we do it
		if (error > 0) {
			dpf_close(h);
		}
		sprintf(errstring, "Failed to open port:\n%s\n",
				dev_errstr(error));
		PyErr_SetString(PyExc_SystemError, errstring);
		return 0;
	} 
}

static PyMethodDef device_methods[] = {
	{"open",         device_open,                         METH_VARARGS },
	{NULL, NULL}
};


#ifdef __WIN32__
	__declspec(dllexport)
#endif

void
INITMODULE(MODULENAME)(void)
{
	// XXX
	// this only for windows portability
	// might be removed when using g++ or any other C++ compiler
	DeviceType.ob_type = &PyType_Type;
	//
	Py_InitModule("dpf", device_methods);

}
