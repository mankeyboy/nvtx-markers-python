#include <stdio.h>
#include <wchar.h>
#include <nvToolsExt.h>
#include <Python.h>
#include <cuda_profiler_api.h>

int cid = 0;
const uint32_t colors[] = { 0x0000ff00, 0x000000ff, 0x00ffff00, 0x00ff00ff, 0x0000ffff, 0x00ff0000, 0x00ffffff};
const int num_colors = sizeof(colors)/sizeof(uint32_t);
#define NVTX_RANGE_PUSH(name,cid) { int color_id = cid; color_id = color_id%num_colors; nvtxEventAttributes_t eventAttrib = {0};  eventAttrib.version = NVTX_VERSION; eventAttrib.size = NVTX_EVENT_ATTRIB_STRUCT_SIZE; eventAttrib.colorType = NVTX_COLOR_ARGB; eventAttrib.color = colors[color_id]; eventAttrib.messageType = NVTX_MESSAGE_TYPE_ASCII; eventAttrib.message.ascii = name; nvtxRangePushEx(&eventAttrib); }

#define NVTX_RANGE_POP nvtxRangePop();

/*
 * Function to be called from Python 
*/
static PyObject* py_profilerStart(PyObject* self, PyObject* args)
{
   cudaProfilerStart();
   char *s;
   if (!PyArg_ParseTuple(args, "s", &s)) {
      return NULL;
   }
   return Py_BuildValue("s", s);
}

/*
 * Function to be called from Python
*/
static PyObject* py_profilerStop(PyObject* self, PyObject* args)
{
   cudaProfilerStop();
   char *s;
   if (!PyArg_ParseTuple(args, "s", &s)) {
      return NULL;
   }
   return Py_BuildValue("s", s);

}

static PyObject* py_profilerRangePush(PyObject* self, PyObject* args)
{
   char *s;
   if (!PyArg_ParseTuple(args, "s", &s)) {
      return NULL;
   }
   NVTX_RANGE_PUSH(s, (cid++))
   return Py_BuildValue("s", s);
}

static PyObject* py_profilerRangePop(PyObject* self, PyObject* args)
{
   char *s;
   if (!PyArg_ParseTuple(args, "s", &s)) {
      return NULL;
   }
   NVTX_RANGE_POP
   //nvtxRangePop();
   return Py_BuildValue("s", s);
}

/*
 * Bind Python function names to our C functions
*/
static PyMethodDef profilerModule_methods[] = {
      {"profilerStart", py_profilerStart, METH_VARARGS},
      {"profilerStop", py_profilerStop, METH_VARARGS},
      {"profilerRangePush", py_profilerRangePush, METH_VARARGS},
      {"profilerRangePop", py_profilerRangePop, METH_VARARGS},
      {NULL, NULL}
  };

static struct PyModuleDef profilerModule =
{
    PyModuleDef_HEAD_INIT,
    "profilerModule", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    profilerModule_methods
};

PyMODINIT_FUNC PyInit_profilerModule(void)
{
    return PyModule_Create(&profilerModule);
}


// /*
//  * Python calls this to let us initialize our module
// */
// void initprofilerModule()
// {
//       (void) Py_Initialize();
// }
int
main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("profilerModule", PyInit_profilerModule);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyImport_ImportModule("profilerModule");

    PyMem_RawFree(program);
    return 0;
}