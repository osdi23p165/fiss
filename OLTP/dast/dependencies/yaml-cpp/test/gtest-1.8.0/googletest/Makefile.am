# Automake file

ACLOCAL_AMFLAGS = -I m4

# Nonstandard package files for distribution
EXTRA_DIST = \
  CHANGES \
  CONTRIBUTORS \
  LICENSE \
  include/gtest/gtest-param-test.h.pump \
  include/gtest/internal/gtest-param-util-generated.h.pump \
  include/gtest/internal/gtest-tuple.h.pump \
  include/gtest/internal/gtest-type-util.h.pump \
  make/Makefile \
  scripts/fuse_gtest_files.py \
  scripts/gen_gtest_pred_impl.py \
  scripts/pump.py \
  scripts/test/Makefile

# gtest source files that we don't compile directly.  They are
# #included by gtest-all.cc.
GTEST_SRC = \
  src/gtest-death-test.cc \
  src/gtest-filepath.cc \
  src/gtest-internal-inl.h \
  src/gtest-port.cc \
  src/gtest-printers.cc \
  src/gtest-test-part.cc \
  src/gtest-typed-test.cc \
  src/gtest.cc

EXTRA_DIST += $(GTEST_SRC)

# Sample files that we don't compile.
EXTRA_DIST += \
  samples/prime_tables.h \
  samples/sample2_unittest.cc \
  samples/sample3_unittest.cc \
  samples/sample4_unittest.cc \
  samples/sample5_unittest.cc \
  samples/sample6_unittest.cc \
  samples/sample7_unittest.cc \
  samples/sample8_unittest.cc \
  samples/sample9_unittest.cc

# C++ test files that we don't compile directly.
EXTRA_DIST += \
  test/gtest-death-test_ex_test.cc \
  test/gtest-death-test_test.cc \
  test/gtest-filepath_test.cc \
  test/gtest-linked_ptr_test.cc \
  test/gtest-listener_test.cc \
  test/gtest-message_test.cc \
  test/gtest-options_test.cc \
  test/gtest-param-test2_test.cc \
  test/gtest-param-test2_test.cc \
  test/gtest-param-test_test.cc \
  test/gtest-param-test_test.cc \
  test/gtest-param-test_test.h \
  test/gtest-port_test.cc \
  test/gtest_premature_exit_test.cc \
  test/gtest-printers_test.cc \
  test/gtest-test-part_test.cc \
  test/gtest-tuple_test.cc \
  test/gtest-typed-test2_test.cc \
  test/gtest-typed-test_test.cc \
  test/gtest-typed-test_test.h \
  test/gtest-unittest-api_test.cc \
  test/gtest_break_on_failure_unittest_.cc \
  test/gtest_catch_exceptions_test_.cc \
  test/gtest_color_test_.cc \
  test/gtest_env_var_test_.cc \
  test/gtest_environment_test.cc \
  test/gtest_filter_unittest_.cc \
  test/gtest_help_test_.cc \
  test/gtest_list_tests_unittest_.cc \
  test/gtest_main_unittest.cc \
  test/gtest_no_test_unittest.cc \
  test/gtest_output_test_.cc \
  test/gtest_pred_impl_unittest.cc \
  test/gtest_prod_test.cc \
  test/gtest_repeat_test.cc \
  test/gtest_shuffle_test_.cc \
  test/gtest_sole_header_test.cc \
  test/gtest_stress_test.cc \
  test/gtest_throw_on_failure_ex_test.cc \
  test/gtest_throw_on_failure_test_.cc \
  test/gtest_uninitialized_test_.cc \
  test/gtest_unittest.cc \
  test/gtest_unittest.cc \
  test/gtest_xml_outfile1_test_.cc \
  test/gtest_xml_outfile2_test_.cc \
  test/gtest_xml_output_unittest_.cc \
  test/production.cc \
  test/production.h

# Python tests that we don't run.
EXTRA_DIST += \
  test/gtest_break_on_failure_unittest.py \
  test/gtest_catch_exceptions_test.py \
  test/gtest_color_test.py \
  test/gtest_env_var_test.py \
  test/gtest_filter_unittest.py \
  test/gtest_help_test.py \
  test/gtest_list_tests_unittest.py \
  test/gtest_output_test.py \
  test/gtest_output_test_golden_lin.txt \
  test/gtest_shuffle_test.py \
  test/gtest_test_utils.py \
  test/gtest_throw_on_failure_test.py \
  test/gtest_uninitialized_test.py \
  test/gtest_xml_outfiles_test.py \
  test/gtest_xml_output_unittest.py \
  test/gtest_xml_test_utils.py

# CMake script
EXTRA_DIST += \
  CMakeLists.txt \
  cmake/internal_utils.cmake

# MSVC project files
EXTRA_DIST += \
  msvc/gtest-md.sln \
  msvc/gtest-md.vcproj \
  msvc/gtest.sln \
  msvc/gtest.vcproj \
  msvc/gtest_main-md.vcproj \
  msvc/gtest_main.vcproj \
  msvc/gtest_prod_test-md.vcproj \
  msvc/gtest_prod_test.vcproj \
  msvc/gtest_unittest-md.vcproj \
  msvc/gtest_unittest.vcproj

# xcode project files
EXTRA_DIST += \
  xcode/Config/DebugProject.xcconfig \
  xcode/Config/FrameworkTarget.xcconfig \
  xcode/Config/General.xcconfig \
  xcode/Config/ReleaseProject.xcconfig \
  xcode/Config/StaticLibraryTarget.xcconfig \
  xcode/Config/TestTarget.xcconfig \
  xcode/Resources/Info.plist \
  xcode/Scripts/runtests.sh \
  xcode/Scripts/versiongenerate.py \
  xcode/gtest.xcodeproj/project.pbxproj

# xcode sample files
EXTRA_DIST += \
  xcode/Samples/FrameworkSample/Info.plist \
  xcode/Samples/FrameworkSample/WidgetFramework.xcodeproj/project.pbxproj \
  xcode/Samples/FrameworkSample/runtests.sh \
  xcode/Samples/FrameworkSample/widget.cc \
  xcode/Samples/FrameworkSample/widget.h \
  xcode/Samples/FrameworkSample/widget_test.cc

# C++Builder project files
EXTRA_DIST += \
  codegear/gtest.cbproj \
  codegear/gtest.groupproj \
  codegear/gtest_all.cc \
  codegear/gtest_link.cc \
  codegear/gtest_main.cbproj \
  codegear/gtest_unittest.cbproj

# Distribute and install M4 macro
m4datadir = $(datadir)/aclocal
m4data_DATA = m4/gtest.m4
EXTRA_DIST += $(m4data_DATA)

# We define the global AM_CPPFLAGS as everything we compile includes from these
# directories.
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include

# Modifies compiler and linker flags for pthreads compatibility.
if HAVE_PTHREADS
  AM_CXXFLAGS = @PTHREAD_CFLAGS@ -DGTEST_HAS_PTHREAD=1
  AM_LIBS = @PTHREAD_LIBS@
else
  AM_CXXFLAGS = -DGTEST_HAS_PTHREAD=0
endif

# Build