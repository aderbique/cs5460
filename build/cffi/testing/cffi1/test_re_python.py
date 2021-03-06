import sys
import py
from cffi import FFI
from cffi import recompiler, ffiplatform, VerificationMissing
from testing.udir import udir


def setup_module(mod):
    SRC = """
    #include <string.h>
    #define FOOBAR (-42)
    static const int FOOBAZ = -43;
    #define BIGPOS 420000000000L
    #define BIGNEG -420000000000L
    int add42(int x) { return x + 42; }
    int add43(int x, ...) { return x; }
    int globalvar42 = 1234;
    const int globalconst42 = 4321;
    const char *const globalconsthello = "hello";
    struct foo_s;
    typedef struct bar_s { int x; signed char a[]; } bar_t;
    enum foo_e { AA, BB, CC };

    void init_test_re_python(void) { }      /* windows hack */
    void PyInit__test_re_python(void) { }   /* windows hack */
    """
    tmpdir = udir.join('test_re_python')
    tmpdir.ensure(dir=1)
    c_file = tmpdir.join('_test_re_python.c')
    c_file.write(SRC)
    ext = ffiplatform.get_extension(
        str(c_file),
        '_test_re_python',
        export_symbols=['add42', 'add43', 'globalvar42',
                        'globalconst42', 'globalconsthello']
    )
    outputfilename = ffiplatform.compile(str(tmpdir), ext)
    mod.extmod = outputfilename
    mod.tmpdir = tmpdir
    #
    ffi = FFI()
    ffi.cdef("""
    #define FOOBAR -42
    static const int FOOBAZ = -43;
    #define BIGPOS 420000000000L
    #define BIGNEG -420000000000L
    int add42(int);
    int add43(int, ...);
    int globalvar42;
    const int globalconst42;
    const char *const globalconsthello = "hello";
    int no_such_function(int);
    int no_such_globalvar;
    struct foo_s;
    typedef struct bar_s { int x; signed char a[]; } bar_t;
    enum foo_e { AA, BB, CC };
    int strlen(const char *);
    """)
    ffi.set_source('re_python_pysrc', None)
    ffi.emit_python_code(str(tmpdir.join('re_python_pysrc.py')))
    mod.original_ffi = ffi
    #
    sys.path.insert(0, str(tmpdir))


def test_constant():
    from re_python_pysrc import ffi
    assert ffi.integer_const('FOOBAR') == -42
    assert ffi.integer_const('FOOBAZ') == -43

def test_large_constant():
    from re_python_pysrc import ffi
    assert ffi.integer_const('BIGPOS') == 420000000000
    assert ffi.integer_const('BIGNEG') == -420000000000

def test_function():
    import _cffi_backend
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    assert lib.add42(-10) == 32
    assert type(lib.add42) is _cffi_backend.FFI.CData

def test_function_with_varargs():
    import _cffi_backend
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod, 0)
    assert lib.add43(45, ffi.cast("int", -5)) == 45
    assert type(lib.add43) is _cffi_backend.FFI.CData

def test_dlopen_none():
    import _cffi_backend
    from re_python_pysrc import ffi
    name = None
    if sys.platform == 'win32':
        import ctypes.util
        name = ctypes.util.find_msvcrt()
        if name is None:
            py.test.skip("dlopen(None) cannot work on Windows with Python 3")
    lib = ffi.dlopen(name)
    assert lib.strlen(b"hello") == 5

def test_dlclose():
    import _cffi_backend
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    ffi.dlclose(lib)
    e = py.test.raises(ffi.error, ffi.dlclose, lib)
    assert str(e.value).startswith(
        "library '%s' is already closed" % (extmod,))
    e = py.test.raises(ffi.error, getattr, lib, 'add42')
    assert str(e.value) == (
        "library '%s' has been closed" % (extmod,))

def test_constant_via_lib():
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    assert lib.FOOBAR == -42
    assert lib.FOOBAZ == -43

def test_opaque_struct():
    from re_python_pysrc import ffi
    ffi.cast("struct foo_s *", 0)
    py.test.raises(TypeError, ffi.new, "struct foo_s *")

def test_nonopaque_struct():
    from re_python_pysrc import ffi
    for p in [ffi.new("struct bar_s *", [5, b"foobar"]),
              ffi.new("bar_t *", [5, b"foobar"])]:
        assert p.x == 5
        assert p.a[0] == ord('f')
        assert p.a[5] == ord('r')

def test_enum():
    from re_python_pysrc import ffi
    assert ffi.integer_const("BB") == 1
    e = ffi.cast("enum foo_e", 2)
    assert ffi.string(e) == "CC"

def test_include_1():
    sub_ffi = FFI()
    sub_ffi.cdef("static const int k2 = 121212;")
    sub_ffi.include(original_ffi)
    assert 'macro FOOBAR' in original_ffi._parser._declarations
    assert 'macro FOOBAZ' in original_ffi._parser._declarations
    sub_ffi.set_source('re_python_pysrc', None)
    sub_ffi.emit_python_code(str(tmpdir.join('_re_include_1.py')))
    #
    if sys.version_info[:2] >= (3, 3):
        import importlib
        importlib.invalidate_caches()  # issue 197 (but can't reproduce myself)
    #
    from _re_include_1 import ffi
    assert ffi.integer_const('FOOBAR') == -42
    assert ffi.integer_const('FOOBAZ') == -43
    assert ffi.integer_const('k2') == 121212
    lib = ffi.dlopen(extmod)     # <- a random unrelated library would be fine
    assert lib.FOOBAR == -42
    assert lib.FOOBAZ == -43
    assert lib.k2 == 121212
    #
    p = ffi.new("bar_t *", [5, b"foobar"])
    assert p.a[4] == ord('a')

def test_global_var():
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    assert lib.globalvar42 == 1234
    p = ffi.addressof(lib, 'globalvar42')
    lib.globalvar42 += 5
    assert p[0] == 1239
    p[0] -= 1
    assert lib.globalvar42 == 1238

def test_global_const_int():
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    assert lib.globalconst42 == 4321
    py.test.raises(AttributeError, ffi.addressof, lib, 'globalconst42')

def test_global_const_nonint():
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    assert ffi.string(lib.globalconsthello, 8) == b"hello"
    py.test.raises(AttributeError, ffi.addressof, lib, 'globalconsthello')

def test_rtld_constants():
    from re_python_pysrc import ffi
    ffi.RTLD_NOW    # check that we have the attributes
    ffi.RTLD_LAZY
    ffi.RTLD_GLOBAL

def test_no_such_function_or_global_var():
    from re_python_pysrc import ffi
    lib = ffi.dlopen(extmod)
    e = py.test.raises(ffi.error, getattr, lib, 'no_such_function')
    assert str(e.value).startswith(
        "symbol 'no_such_function' not found in library '")
    e = py.test.raises(ffi.error, getattr, lib, 'no_such_globalvar')
    assert str(e.value).startswith(
        "symbol 'no_such_globalvar' not found in library '")

def test_check_version():
    import _cffi_backend
    e = py.test.raises(ImportError, _cffi_backend.FFI,
                       "foobar", _version=0x2594)
    assert str(e.value).startswith(
        "cffi out-of-line Python module 'foobar' has unknown version")

def test_partial_enum():
    ffi = FFI()
    ffi.cdef("enum foo { A, B, ... };")
    ffi.set_source('test_partial_enum', None)
    py.test.raises(VerificationMissing, ffi.emit_python_code,
                   str(tmpdir.join('test_partial_enum.py')))
