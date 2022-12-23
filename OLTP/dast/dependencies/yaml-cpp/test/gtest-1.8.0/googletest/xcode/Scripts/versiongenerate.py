// !$*UTF8*$!
{
	archiveVersion = 1;
	classes = {
	};
	objectVersion = 46;
	objects = {

/* Begin PBXAggregateTarget section */
		3B238F5F0E828B5400846E11 /* Check */ = {
			isa = PBXAggregateTarget;
			buildConfigurationList = 3B238FA30E828BB600846E11 /* Build configuration list for PBXAggregateTarget "Check" */;
			buildPhases = (
				3B238F5E0E828B5400846E11 /* ShellScript */,
			);
			dependencies = (
				40899F9D0FFA740F000B29AE /* PBXTargetDependency */,
				40C849F7101A43440083642A /* PBXTargetDependency */,
				4089A0980FFAD34A000B29AE /* PBXTargetDependency */,
				40C849F9101A43490083642A /* PBXTargetDependency */,
			);
			name = Check;
			productName = Check;
		};
		40C44ADC0E3798F4008FCC51 /* Version Info */ = {
			isa = PBXAggregateTarget;
			buildConfigurationList = 40C44AE40E379905008FCC51 /* Build configuration list for PBXAggregateTarget "Version Info" */;
			buildPhases = (
				40C44ADB0E3798F4008FCC51 /* Generate Version.h */,
			);
			comments = "The generation of Version.h must be performed in its own target. Since the Info.plist is preprocessed before any of the other build phases in gtest, the Version.h file would not be ready if included as a build phase of that target.";
			dependencies = (
			);
			name = "Version Info";
			productName = Version.h;
		};
/* End PBXAggregateTarget section */

/* Begin PBXBuildFile section */
		224A12A30E9EADCC00BD17FD /* gtest-test-part.h in Headers */ = {isa = PBXBuildFile; fileRef = 224A12A20E9EADCC00BD17FD /* gtest-test-part.h */; settings = {ATTRIBUTES = (Public, ); }; };
		3BF6F2A00E79B5AD000F2EEE /* gtest-type-util.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 3BF6F29F0E79B5AD000F2EEE /* gtest-type-util.h */; };
		3BF6F2A50E79B616000F2EEE /* gtest-typed-test.h in Headers */ = {isa = PBXBuildFile; fileRef = 3BF6F2A40E79B616000F2EEE /* gtest-typed-test.h */; settings = {ATTRIBUTES = (Public, ); }; };
		404884380E2F799B00CF7658 /* gtest-death-test.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883DB0E2F799B00CF7658 /* gtest-death-test.h */; settings = {ATTRIBUTES = (Public, ); }; };
		404884390E2F799B00CF7658 /* gtest-message.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883DC0E2F799B00CF7658 /* gtest-message.h */; settings = {ATTRIBUTES = (Public, ); }; };
		4048843A0E2F799B00CF7658 /* gtest-spi.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883DD0E2F799B00CF7658 /* gtest-spi.h */; settings = {ATTRIBUTES = (Public, ); }; };
		4048843B0E2F799B00CF7658 /* gtest.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883DE0E2F799B00CF7658 /* gtest.h */; settings = {ATTRIBUTES = (Public, ); }; };
		4048843C0E2F799B00CF7658 /* gtest_pred_impl.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883DF0E2F799B00CF7658 /* gtest_pred_impl.h */; settings = {ATTRIBUTES = (Public, ); }; };
		4048843D0E2F799B00CF7658 /* gtest_prod.h in Headers */ = {isa = PBXBuildFile; fileRef = 404883E00E2F799B00CF7658 /* gtest_prod.h */; settings = {ATTRIBUTES = (Public, ); }; };
		404884500E2F799B00CF7658 /* README.md in Resources */ = {isa = PBXBuildFile; fileRef = 404883F60E2F799B00CF7658 /* README.md */; };
		404884A00E2F7BE600CF7658 /* gtest-death-test-internal.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 404883E20E2F799B00CF7658 /* gtest-death-test-internal.h */; };
		404884A10E2F7BE600CF7658 /* gtest-filepath.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 404883E30E2F799B00CF7658 /* gtest-filepath.h */; };
		404884A20E2F7BE600CF7658 /* gtest-internal.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 404883E40E2F799B00CF7658 /* gtest-internal.h */; };
		404884A30E2F7BE600CF7658 /* gtest-port.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 404883E50E2F799B00CF7658 /* gtest-port.h */; };
		404884A40E2F7BE600CF7658 /* gtest-string.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 404883E60E2F799B00CF7658 /* gtest-string.h */; };
		404884AC0E2F7CD900CF7658 /* CHANGES in Resources */ = {isa = PBXBuildFile; fileRef = 404884A90E2F7CD900CF7658 /* CHANGES */; };
		404884AD0E2F7CD900CF7658 /* CONTRIBUTORS in Resources */ = {isa = PBXBuildFile; fileRef = 404884AA0E2F7CD900CF7658 /* CONTRIBUTORS */; };
		404884AE0E2F7CD900CF7658 /* LICENSE in Resources */ = {isa = PBXBuildFile; fileRef = 404884AB0E2F7CD900CF7658 /* LICENSE */; };
		40899F3A0FFA70D4000B29AE /* gtest-all.cc in Sources */ = {isa = PBXBuildFile; fileRef = 224A12A10E9EADA700BD17FD /* gtest-all.cc */; };
		40899F500FFA7281000B29AE /* gtest-tuple.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 40899F4D0FFA7271000B29AE /* gtest-tuple.h */; };
		40899F530FFA72A0000B29AE /* gtest_unittest.cc in Sources */ = {isa = PBXBuildFile; fileRef = 3B238C120E7FE13C00846E11 /* gtest_unittest.cc */; };
		4089A0440FFAD1BE000B29AE /* sample1.cc in Sources */ = {isa = PBXBuildFile; fileRef = 4089A02C0FFACF7F000B29AE /* sample1.cc */; };
		4089A0460FFAD1BE000B29AE /* sample1_unittest.cc in Sources */ = {isa = PBXBuildFile; fileRef = 4089A02E0FFACF7F000B29AE /* sample1_unittest.cc */; };
		40C848FF101A21150083642A /* gtest-all.cc in Sources */ = {isa = PBXBuildFile; fileRef = 224A12A10E9EADA700BD17FD /* gtest-all.cc */; };
		40C84915101A21DF0083642A /* gtest_main.cc in Sources */ = {isa = PBXBuildFile; fileRef = 4048840D0E2F799B00CF7658 /* gtest_main.cc */; };
		40C84916101A235B0083642A /* libgtest_main.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C8490B101A217E0083642A /* libgtest_main.a */; };
		40C84921101A23AD0083642A /* libgtest_main.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C8490B101A217E0083642A /* libgtest_main.a */; };
		40C84978101A36540083642A /* libgtest_main.a in Resources */ = {isa = PBXBuildFile; fileRef = 40C8490B101A217E0083642A /* libgtest_main.a */; };
		40C84980101A36850083642A /* gtest_unittest.cc in Sources */ = {isa = PBXBuildFile; fileRef = 3B238C120E7FE13C00846E11 /* gtest_unittest.cc */; };
		40C84982101A36850083642A /* libgtest.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C848FA101A209C0083642A /* libgtest.a */; };
		40C84983101A36850083642A /* libgtest_main.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C8490B101A217E0083642A /* libgtest_main.a */; };
		40C8498F101A36A60083642A /* sample1.cc in Sources */ = {isa = PBXBuildFile; fileRef = 4089A02C0FFACF7F000B29AE /* sample1.cc */; };
		40C84990101A36A60083642A /* sample1_unittest.cc in Sources */ = {isa = PBXBuildFile; fileRef = 4089A02E0FFACF7F000B29AE /* sample1_unittest.cc */; };
		40C84992101A36A60083642A /* libgtest.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C848FA101A209C0083642A /* libgtest.a */; };
		40C84993101A36A60083642A /* libgtest_main.a in Frameworks */ = {isa = PBXBuildFile; fileRef = 40C8490B101A217E0083642A /* libgtest_main.a */; };
		40C849A2101A37050083642A /* gtest.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 4539C8FF0EC27F6400A70F4C /* gtest.framework */; };
		40C849A4101A37150083642A /* gtest.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 4539C8FF0EC27F6400A70F4C /* gtest.framework */; };
		4539C9340EC280AE00A70F4C /* gtest-param-test.h in Headers */ = {isa = PBXBuildFile; fileRef = 4539C9330EC280AE00A70F4C /* gtest-param-test.h */; settings = {ATTRIBUTES = (Public, ); }; };
		4539C9380EC280E200A70F4C /* gtest-linked_ptr.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 4539C9350EC280E200A70F4C /* gtest-linked_ptr.h */; };
		4539C9390EC280E200A70F4C /* gtest-param-util-generated.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 4539C9360EC280E200A70F4C /* gtest-param-util-generated.h */; };
		4539C93A0EC280E200A70F4C /* gtest-param-util.h in Copy Headers Internal */ = {isa = PBXBuildFile; fileRef = 4539C9370EC280E200A70F4C /* gtest-param-util.h */; };
		4567C8181264FF71007740BE /* gtest-printers.h in Headers */ = {isa = PBXBuildFile; fileRef = 4567C8171264FF71007740BE /* gtest-printers.h */; settings = {ATTRIBUTES = (Public, ); }; };
/* End PBXBuildFile section */

/* Begin PBXContainerItemProxy section */
		40899F9C0FFA740F000B29AE /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40899F420FFA7184000B29AE;
			remoteInfo = gtest_unittest;
		};
		4089A0970FFAD34A000B29AE /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 4089A0120FFACEFC000B29AE;
			remoteInfo = sample1_unittest;
		};
		408BEC0F1046CFE900DEF522 /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C848F9101A209C0083642A;
			remoteInfo = "gtest-static";
		};
		40C44AE50E379922008FCC51 /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C44ADC0E3798F4008FCC51;
			remoteInfo = Version.h;
		};
		40C8497C101A36850083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C848F9101A209C0083642A;
			remoteInfo = "gtest-static";
		};
		40C8497E101A36850083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C8490A101A217E0083642A;
			remoteInfo = "gtest_main-static";
		};
		40C8498B101A36A60083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C848F9101A209C0083642A;
			remoteInfo = "gtest-static";
		};
		40C8498D101A36A60083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C8490A101A217E0083642A;
			remoteInfo = "gtest_main-static";
		};
		40C8499B101A36DC0083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C8490A101A217E0083642A;
			remoteInfo = "gtest_main-static";
		};
		40C8499D101A36E50083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 8D07F2BC0486CC7A007CD1D0;
			remoteInfo = "gtest-framework";
		};
		40C8499F101A36F10083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 8D07F2BC0486CC7A007CD1D0;
			remoteInfo = "gtest-framework";
		};
		40C849F6101A43440083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C8497A101A36850083642A;
			remoteInfo = "gtest_unittest-static";
		};
		40C849F8101A43490083642A /* PBXContainerItemProxy */ = {
			isa = PBXContainerItemProxy;
			containerPortal = 0867D690FE84028FC02AAC07 /* Project object */;
			proxyType = 1;
			remoteGlobalIDString = 40C84989101A36A60083642A;
			remoteInfo = "sample1_unittest-static";
		};
/* End PBXContainerItemProxy section */

/* Begin PBXCopyFilesBuildPhase section */
		404884A50E2F7C0400CF7658 /* Copy Headers Internal */ = {
			isa = PBXCopyFilesBuildPhase;
			buildActionMask = 2147483647;
			dstPath = Headers/internal;
			dstSubfolderSpec = 6;
			files = (
				404884A00E2F7BE600CF7658 /* gtest-death-test-internal.h in Copy Headers Internal */,
				404884A10E2F7BE600CF7658 /* gtest-filepath.h in Copy Headers Internal */,
				404884A20E2F7BE600CF7658 /* gtest-internal.h in Copy Headers Internal */,
				4539C9380EC280E200A70F4C /* gtest-linked_ptr.h in Copy Headers Internal */,
				4539C9390EC280E200A70F4C /* gtest-param-util-generated.h in Copy Headers Internal */,
				4539C93A0EC280E200A70F4C /* gtest-param-util.h in Copy Headers Internal */,
				404884A30E2F7BE600CF7658 /* gtest-port.h in Copy Headers Internal */,
				404884A40E2F7BE600CF7658 /* gtest-string.h in Copy Headers Internal */,
				40899F500FFA7281000B29AE /* gtest-tuple.h in Copy Headers Internal */,
				3BF6F2A00E79B5AD000F2EEE /* gtest-type-util.h in Copy Headers Internal */,
			);
			name = "Copy Headers Internal";
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXCopyFilesBuildPhase section */

/* Begin PBXFileReference section */
		224A12A10E9EADA700BD17FD /* gtest-all.cc */ = {isa = PBXFileReference; fileEncoding = 30; lastKnownFileType = sourcecode.cpp.cpp; path = "gtest-all.cc"; sourceTree = "<group>"; };
		224A12A20E9EADCC00BD17FD /* gtest-test-part.h */ = {isa = PBXFileReference; fileEncoding = 30; lastKnownFileType = sourcecode.c.h; path = "gtest-test-part.h"; sourceTree = "<group>"; };
		3B238C120E7FE13C00846E11 /* gtest_unittest.cc */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; path = gtest_unittest.cc; sourceTree = "<group>"; };
		3B87D2100E96B92E000D1852 /* runtests.sh */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.script.sh; path = runtests.sh; sourceTree = "<group>"; };
		3BF6F29F0E79B5AD000F2EEE /* gtest-type-util.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-type-util.h"; sourceTree = "<group>"; };
		3BF6F2A40E79B616000F2EEE /* gtest-typed-test.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-typed-test.h"; sourceTree = "<group>"; };
		403EE37C0E377822004BD1E2 /* versiongenerate.py */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.script.python; path = versiongenerate.py; sourceTree = "<group>"; };
		404883DB0E2F799B00CF7658 /* gtest-death-test.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-death-test.h"; sourceTree = "<group>"; };
		404883DC0E2F799B00CF7658 /* gtest-message.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-message.h"; sourceTree = "<group>"; };
		404883DD0E2F799B00CF7658 /* gtest-spi.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-spi.h"; sourceTree = "<group>"; };
		404883DE0E2F799B00CF7658 /* gtest.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = gtest.h; sourceTree = "<group>"; };
		404883DF0E2F799B00CF7658 /* gtest_pred_impl.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = gtest_pred_impl.h; sourceTree = "<group>"; };
		404883E00E2F799B00CF7658 /* gtest_prod.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = gtest_prod.h; sourceTree = "<group>"; };
		404883E20E2F799B00CF7658 /* gtest-death-test-internal.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-death-test-internal.h"; sourceTree = "<group>"; };
		404883E30E2F799B00CF7658 /* gtest-filepath.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-filepath.h"; sourceTree = "<group>"; };
		404883E40E2F799B00CF7658 /* gtest-internal.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-internal.h"; sourceTree = "<group>"; };
		404883E50E2F799B00CF7658 /* gtest-port.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-port.h"; sourceTree = "<group>"; };
		404883E60E2F799B00CF7658 /* gtest-string.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-string.h"; sourceTree = "<group>"; };
		404883F60E2F799B00CF7658 /* README.md */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text; name = README.md; path = ../README.md; sourceTree = SOURCE_ROOT; };
		4048840D0E2F799B00CF7658 /* gtest_main.cc */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; path = gtest_main.cc; sourceTree = "<group>"; };
		404884A90E2F7CD900CF7658 /* CHANGES */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text; name = CHANGES; path = ../CHANGES; sourceTree = SOURCE_ROOT; };
		404884AA0E2F7CD900CF7658 /* CONTRIBUTORS */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text; name = CONTRIBUTORS; path = ../CONTRIBUTORS; sourceTree = SOURCE_ROOT; };
		404884AB0E2F7CD900CF7658 /* LICENSE */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text; name = LICENSE; path = ../LICENSE; sourceTree = SOURCE_ROOT; };
		40899F430FFA7184000B29AE /* gtest_unittest-framework */ = {isa = PBXFileReference; explicitFileType = "compiled.mach-o.executable"; includeInIndex = 0; path = "gtest_unittest-framework"; sourceTree = BUILT_PRODUCTS_DIR; };
		40899F4D0FFA7271000B29AE /* gtest-tuple.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-tuple.h"; sourceTree = "<group>"; };
		40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.xcconfig; path = StaticLibraryTarget.xcconfig; sourceTree = "<group>"; };
		4089A0130FFACEFC000B29AE /* sample1_unittest-framework */ = {isa = PBXFileReference; explicitFileType = "compiled.mach-o.executable"; includeInIndex = 0; path = "sample1_unittest-framework"; sourceTree = BUILT_PRODUCTS_DIR; };
		4089A02C0FFACF7F000B29AE /* sample1.cc */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; path = sample1.cc; sourceTree = "<group>"; };
		4089A02D0FFACF7F000B29AE /* sample1.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = sample1.h; sourceTree = "<group>"; };
		4089A02E0FFACF7F000B29AE /* sample1_unittest.cc */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; path = sample1_unittest.cc; sourceTree = "<group>"; };
		40C848FA101A209C0083642A /* libgtest.a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = libgtest.a; sourceTree = BUILT_PRODUCTS_DIR; };
		40C8490B101A217E0083642A /* libgtest_main.a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = libgtest_main.a; sourceTree = BUILT_PRODUCTS_DIR; };
		40C84987101A36850083642A /* gtest_unittest */ = {isa = PBXFileReference; explicitFileType = "compiled.mach-o.executable"; includeInIndex = 0; path = gtest_unittest; sourceTree = BUILT_PRODUCTS_DIR; };
		40C84997101A36A60083642A /* sample1_unittest-static */ = {isa = PBXFileReference; explicitFileType = "compiled.mach-o.executable"; includeInIndex = 0; path = "sample1_unittest-static"; sourceTree = BUILT_PRODUCTS_DIR; };
		40D4CDF10E30E07400294801 /* DebugProject.xcconfig */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.xcconfig; path = DebugProject.xcconfig; sourceTree = "<group>"; };
		40D4CDF20E30E07400294801 /* FrameworkTarget.xcconfig */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.xcconfig; path = FrameworkTarget.xcconfig; sourceTree = "<group>"; };
		40D4CDF30E30E07400294801 /* General.xcconfig */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.xcconfig; path = General.xcconfig; sourceTree = "<group>"; };
		40D4CDF40E30E07400294801 /* ReleaseProject.xcconfig */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.xcconfig; path = ReleaseProject.xcconfig; sourceTree = "<group>"; };
		40D4CF510E30F5E200294801 /* Info.plist */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = text.plist.xml; path = Info.plist; sourceTree = "<group>"; };
		4539C8FF0EC27F6400A70F4C /* gtest.framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = gtest.framework; sourceTree = BUILT_PRODUCTS_DIR; };
		4539C9330EC280AE00A70F4C /* gtest-param-test.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-param-test.h"; sourceTree = "<group>"; };
		4539C9350EC280E200A70F4C /* gtest-linked_ptr.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-linked_ptr.h"; sourceTree = "<group>"; };
		4539C9360EC280E200A70F4C /* gtest-param-util-generated.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-param-util-generated.h"; sourceTree = "<group>"; };
		4539C9370EC280E200A70F4C /* gtest-param-util.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-param-util.h"; sourceTree = "<group>"; };
		4567C8171264FF71007740BE /* gtest-printers.h */ = {isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.c.h; path = "gtest-printers.h"; sourceTree = "<group>"; };
/* End PBXFileReference section */

/* Begin PBXFrameworksBuildPhase section */
		40899F410FFA7184000B29AE /* Frameworks */ = {
			isa = PBXFrameworksBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C849A4101A37150083642A /* gtest.framework in Frameworks */,
				40C84916101A235B0083642A /* libgtest_main.a in Frameworks */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		4089A0110FFACEFC000B29AE /* Frameworks */ = {
			isa = PBXFrameworksBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C849A2101A37050083642A /* gtest.framework in Frameworks */,
				40C84921101A23AD0083642A /* libgtest_main.a in Frameworks */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C84981101A36850083642A /* Frameworks */ = {
			isa = PBXFrameworksBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C84982101A36850083642A /* libgtest.a in Frameworks */,
				40C84983101A36850083642A /* libgtest_main.a in Frameworks */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C84991101A36A60083642A /* Frameworks */ = {
			isa = PBXFrameworksBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C84992101A36A60083642A /* libgtest.a in Frameworks */,
				40C84993101A36A60083642A /* libgtest_main.a in Frameworks */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXFrameworksBuildPhase section */

/* Begin PBXGroup section */
		034768DDFF38A45A11DB9C8B /* Products */ = {
			isa = PBXGroup;
			children = (
				4539C8FF0EC27F6400A70F4C /* gtest.framework */,
				40C848FA101A209C0083642A /* libgtest.a */,
				40C8490B101A217E0083642A /* libgtest_main.a */,
				40899F430FFA7184000B29AE /* gtest_unittest-framework */,
				40C84987101A36850083642A /* gtest_unittest */,
				4089A0130FFACEFC000B29AE /* sample1_unittest-framework */,
				40C84997101A36A60083642A /* sample1_unittest-static */,
			);
			name = Products;
			sourceTree = "<group>";
		};
		0867D691FE84028FC02AAC07 /* gtest */ = {
			isa = PBXGroup;
			children = (
				40D4CDF00E30E07400294801 /* Config */,
				08FB77ACFE841707C02AAC07 /* Source */,
				40D4CF4E0E30F5E200294801 /* Resources */,
				403EE37B0E377822004BD1E2 /* Scripts */,
				034768DDFF38A45A11DB9C8B /* Products */,
			);
			name = gtest;
			sourceTree = "<group>";
		};
		08FB77ACFE841707C02AAC07 /* Source */ = {
			isa = PBXGroup;
			children = (
				404884A90E2F7CD900CF7658 /* CHANGES */,
				404884AA0E2F7CD900CF7658 /* CONTRIBUTORS */,
				404884AB0E2F7CD900CF7658 /* LICENSE */,
				404883F60E2F799B00CF7658 /* README.md */,
				404883D90E2F799B00CF7658 /* include */,
				4089A02F0FFACF84000B29AE /* samples */,
				404884070E2F799B00CF7658 /* src */,
				3B238BF00E7FE13B00846E11 /* test */,
			);
			name = Source;
			sourceTree = "<group>";
		};
		3B238BF00E7FE13B00846E11 /* test */ = {
			isa = PBXGroup;
			children = (
				3B238C120E7FE13C00846E11 /* gtest_unittest.cc */,
			);
			name = test;
			path = ../test;
			sourceTree = SOURCE_ROOT;
		};
		403EE37B0E377822004BD1E2 /* Scripts */ = {
			isa = PBXGroup;
			children = (
				403EE37C0E377822004BD1E2 /* versiongenerate.py */,
				3B87D2100E96B92E000D1852 /* runtests.sh */,
			);
			path = Scripts;
			sourceTree = "<group>";
		};
		404883D90E2F799B00CF7658 /* include */ = {
			isa = PBXGroup;
			children = (
				404883DA0E2F799B00CF7658 /* gtest */,
			);
			name = include;
			path = ../include;
			sourceTree = SOURCE_ROOT;
		};
		404883DA0E2F799B00CF7658 /* gtest */ = {
			isa = PBXGroup;
			children = (
				404883E10E2F799B00CF7658 /* internal */,
				224A12A20E9EADCC00BD17FD /* gtest-test-part.h */,
				404883DB0E2F799B00CF7658 /* gtest-death-test.h */,
				404883DC0E2F799B00CF7658 /* gtest-message.h */,
				4539C9330EC280AE00A70F4C /* gtest-param-test.h */,
				4567C8171264FF71007740BE /* gtest-printers.h */,
				404883DD0E2F799B00CF7658 /* gtest-spi.h */,
				404883DE0E2F799B00CF7658 /* gtest.h */,
				404883DF0E2F799B00CF7658 /* gtest_pred_impl.h */,
				404883E00E2F799B00CF7658 /* gtest_prod.h */,
				3BF6F2A40E79B616000F2EEE /* gtest-typed-test.h */,
			);
			path = gtest;
			sourceTree = "<group>";
		};
		404883E10E2F799B00CF7658 /* internal */ = {
			isa = PBXGroup;
			children = (
				404883E20E2F799B00CF7658 /* gtest-death-test-internal.h */,
				404883E30E2F799B00CF7658 /* gtest-filepath.h */,
				404883E40E2F799B00CF7658 /* gtest-internal.h */,
				4539C9350EC280E200A70F4C /* gtest-linked_ptr.h */,
				4539C9360EC280E200A70F4C /* gtest-param-util-generated.h */,
				4539C9370EC280E200A70F4C /* gtest-param-util.h */,
				404883E50E2F799B00CF7658 /* gtest-port.h */,
				404883E60E2F799B00CF7658 /* gtest-string.h */,
				40899F4D0FFA7271000B29AE /* gtest-tuple.h */,
				3BF6F29F0E79B5AD000F2EEE /* gtest-type-util.h */,
			);
			path = internal;
			sourceTree = "<group>";
		};
		404884070E2F799B00CF7658 /* src */ = {
			isa = PBXGroup;
			children = (
				224A12A10E9EADA700BD17FD /* gtest-all.cc */,
				4048840D0E2F799B00CF7658 /* gtest_main.cc */,
			);
			name = src;
			path = ../src;
			sourceTree = SOURCE_ROOT;
		};
		4089A02F0FFACF84000B29AE /* samples */ = {
			isa = PBXGroup;
			children = (
				4089A02C0FFACF7F000B29AE /* sample1.cc */,
				4089A02D0FFACF7F000B29AE /* sample1.h */,
				4089A02E0FFACF7F000B29AE /* sample1_unittest.cc */,
			);
			name = samples;
			path = ../samples;
			sourceTree = SOURCE_ROOT;
		};
		40D4CDF00E30E07400294801 /* Config */ = {
			isa = PBXGroup;
			children = (
				40D4CDF10E30E07400294801 /* DebugProject.xcconfig */,
				40D4CDF20E30E07400294801 /* FrameworkTarget.xcconfig */,
				40D4CDF30E30E07400294801 /* General.xcconfig */,
				40D4CDF40E30E07400294801 /* ReleaseProject.xcconfig */,
				40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */,
			);
			path = Config;
			sourceTree = "<group>";
		};
		40D4CF4E0E30F5E200294801 /* Resources */ = {
			isa = PBXGroup;
			children = (
				40D4CF510E30F5E200294801 /* Info.plist */,
			);
			path = Resources;
			sourceTree = "<group>";
		};
/* End PBXGroup section */

/* Begin PBXHeadersBuildPhase section */
		8D07F2BD0486CC7A007CD1D0 /* Headers */ = {
			isa = PBXHeadersBuildPhase;
			buildActionMask = 2147483647;
			files = (
				404884380E2F799B00CF7658 /* gtest-death-test.h in Headers */,
				404884390E2F799B00CF7658 /* gtest-message.h in Headers */,
				4539C9340EC280AE00A70F4C /* gtest-param-test.h in Headers */,
				4567C8181264FF71007740BE /* gtest-printers.h in Headers */,
				3BF6F2A50E79B616000F2EEE /* gtest-typed-test.h in Headers */,
				4048843A0E2F799B00CF7658 /* gtest-spi.h in Headers */,
				4048843B0E2F799B00CF7658 /* gtest.h in Headers */,
				4048843C0E2F799B00CF7658 /* gtest_pred_impl.h in Headers */,
				4048843D0E2F799B00CF7658 /* gtest_prod.h in Headers */,
				224A12A30E9EADCC00BD17FD /* gtest-test-part.h in Headers */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXHeadersBuildPhase section */

/* Begin PBXNativeTarget section */
		40899F420FFA7184000B29AE /* gtest_unittest-framework */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 40899F4A0FFA71BC000B29AE /* Build configuration list for PBXNativeTarget "gtest_unittest-framework" */;
			buildPhases = (
				40899F400FFA7184000B29AE /* Sources */,
				40899F410FFA7184000B29AE /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
				40C849A0101A36F10083642A /* PBXTargetDependency */,
			);
			name = "gtest_unittest-framework";
			productName = gtest_unittest;
			productReference = 40899F430FFA7184000B29AE /* gtest_unittest-framework */;
			productType = "com.apple.product-type.tool";
		};
		4089A0120FFACEFC000B29AE /* sample1_unittest-framework */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 4089A0240FFACF01000B29AE /* Build configuration list for PBXNativeTarget "sample1_unittest-framework" */;
			buildPhases = (
				4089A0100FFACEFC000B29AE /* Sources */,
				4089A0110FFACEFC000B29AE /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
				40C8499E101A36E50083642A /* PBXTargetDependency */,
			);
			name = "sample1_unittest-framework";
			productName = sample1_unittest;
			productReference = 4089A0130FFACEFC000B29AE /* sample1_unittest-framework */;
			productType = "com.apple.product-type.tool";
		};
		40C848F9101A209C0083642A /* gtest-static */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 40C84902101A212E0083642A /* Build configuration list for PBXNativeTarget "gtest-static" */;
			buildPhases = (
				40C848F7101A209C0083642A /* Sources */,
			);
			buildRules = (
			);
			dependencies = (
			);
			name = "gtest-static";
			productName = "gtest-static";
			productReference = 40C848FA101A209C0083642A /* libgtest.a */;
			productType = "com.apple.product-type.library.static";
		};
		40C8490A101A217E0083642A /* gtest_main-static */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 40C84912101A21D20083642A /* Build configuration list for PBXNativeTarget "gtest_main-static" */;
			buildPhases = (
				40C84908101A217E0083642A /* Sources */,
			);
			buildRules = (
			);
			dependencies = (
			);
			name = "gtest_main-static";
			productName = "gtest_main-static";
			productReference = 40C8490B101A217E0083642A /* libgtest_main.a */;
			productType = "com.apple.product-type.library.static";
		};
		40C8497A101A36850083642A /* gtest_unittest-static */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 40C84984101A36850083642A /* Build configuration list for PBXNativeTarget "gtest_unittest-static" */;
			buildPhases = (
				40C8497F101A36850083642A /* Sources */,
				40C84981101A36850083642A /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
				40C8497B101A36850083642A /* PBXTargetDependency */,
				40C8497D101A36850083642A /* PBXTargetDependency */,
			);
			name = "gtest_unittest-static";
			productName = gtest_unittest;
			productReference = 40C84987101A36850083642A /* gtest_unittest */;
			productType = "com.apple.product-type.tool";
		};
		40C84989101A36A60083642A /* sample1_unittest-static */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 40C84994101A36A60083642A /* Build configuration list for PBXNativeTarget "sample1_unittest-static" */;
			buildPhases = (
				40C8498E101A36A60083642A /* Sources */,
				40C84991101A36A60083642A /* Frameworks */,
			);
			buildRules = (
			);
			dependencies = (
				40C8498A101A36A60083642A /* PBXTargetDependency */,
				40C8498C101A36A60083642A /* PBXTargetDependency */,
			);
			name = "sample1_unittest-static";
			productName = sample1_unittest;
			productReference = 40C84997101A36A60083642A /* sample1_unittest-static */;
			productType = "com.apple.product-type.tool";
		};
		8D07F2BC0486CC7A007CD1D0 /* gtest-framework */ = {
			isa = PBXNativeTarget;
			buildConfigurationList = 4FADC24208B4156D00ABE55E /* Build configuration list for PBXNativeTarget "gtest-framework" */;
			buildPhases = (
				8D07F2C10486CC7A007CD1D0 /* Sources */,
				8D07F2BD0486CC7A007CD1D0 /* Headers */,
				404884A50E2F7C0400CF7658 /* Copy Headers Internal */,
				8D07F2BF0486CC7A007CD1D0 /* Resources */,
			);
			buildRules = (
			);
			dependencies = (
				40C44AE60E379922008FCC51 /* PBXTargetDependency */,
				408BEC101046CFE900DEF522 /* PBXTargetDependency */,
				40C8499C101A36DC0083642A /* PBXTargetDependency */,
			);
			name = "gtest-framework";
			productInstallPath = "$(HOME)/Library/Frameworks";
			productName = gtest;
			productReference = 4539C8FF0EC27F6400A70F4C /* gtest.framework */;
			productType = "com.apple.product-type.framework";
		};
/* End PBXNativeTarget section */

/* Begin PBXProject section */
		0867D690FE84028FC02AAC07 /* Project object */ = {
			isa = PBXProject;
			attributes = {
				LastUpgradeCheck = 0460;
			};
			buildConfigurationList = 4FADC24608B4156D00ABE55E /* Build configuration list for PBXProject "gtest" */;
			compatibilityVersion = "Xcode 3.2";
			developmentRegion = English;
			hasScannedForEncodings = 1;
			knownRegions = (
				English,
				Japanese,
				French,
				German,
				en,
			);
			mainGroup = 0867D691FE84028FC02AAC07 /* gtest */;
			productRefGroup = 034768DDFF38A45A11DB9C8B /* Products */;
			projectDirPath = "";
			projectRoot = "";
			targets = (
				8D07F2BC0486CC7A007CD1D0 /* gtest-framework */,
				40C848F9101A209C0083642A /* gtest-static */,
				40C8490A101A217E0083642A /* gtest_main-static */,
				40899F420FFA7184000B29AE /* gtest_unittest-framework */,
				40C8497A101A36850083642A /* gtest_unittest-static */,
				4089A0120FFACEFC000B29AE /* sample1_unittest-framework */,
				40C84989101A36A60083642A /* sample1_unittest-static */,
				3B238F5F0E828B5400846E11 /* Check */,
				40C44ADC0E3798F4008FCC51 /* Version Info */,
			);
		};
/* End PBXProject section */

/* Begin PBXResourcesBuildPhase section */
		8D07F2BF0486CC7A007CD1D0 /* Resources */ = {
			isa = PBXResourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				404884500E2F799B00CF7658 /* README.md in Resources */,
				404884AC0E2F7CD900CF7658 /* CHANGES in Resources */,
				404884AD0E2F7CD900CF7658 /* CONTRIBUTORS in Resources */,
				404884AE0E2F7CD900CF7658 /* LICENSE in Resources */,
				40C84978101A36540083642A /* libgtest_main.a in Resources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXResourcesBuildPhase section */

/* Begin PBXShellScriptBuildPhase section */
		3B238F5E0E828B5400846E11 /* ShellScript */ = {
			isa = PBXShellScriptBuildPhase;
			buildActionMask = 2147483647;
			files = (
			);
			inputPaths = (
			);
			outputPaths = (
			);
			runOnlyForDeploymentPostprocessing = 0;
			shellPath = /bin/sh;
			shellScript = "# Remember, this \"Run Script\" build phase will be executed from $SRCROOT\n/bin/bash Scripts/runtests.sh";
		};
		40C44ADB0E3798F4008FCC51 /* Generate Version.h */ = {
			isa = PBXShellScriptBuildPhase;
			buildActionMask = 2147483647;
			files = (
			);
			inputPaths = (
				"$(SRCROOT)/Scripts/versiongenerate.py",
				"$(SRCROOT)/../configure.ac",
			);
			name = "Generate Version.h";
			outputPaths = (
				"$(PROJECT_TEMP_DIR)/Version.h",
			);
			runOnlyForDeploymentPostprocessing = 0;
			shellPath = /bin/sh;
			shellScript = "# Remember, this \"Run Script\" build phase will be executed from $SRCROOT\n/usr/bin/python Scripts/versiongenerate.py ../ $PROJECT_TEMP_DIR";
		};
/* End PBXShellScriptBuildPhase section */

/* Begin PBXSourcesBuildPhase section */
		40899F400FFA7184000B29AE /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40899F530FFA72A0000B29AE /* gtest_unittest.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		4089A0100FFACEFC000B29AE /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				4089A0440FFAD1BE000B29AE /* sample1.cc in Sources */,
				4089A0460FFAD1BE000B29AE /* sample1_unittest.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C848F7101A209C0083642A /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C848FF101A21150083642A /* gtest-all.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C84908101A217E0083642A /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C84915101A21DF0083642A /* gtest_main.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C8497F101A36850083642A /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C84980101A36850083642A /* gtest_unittest.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		40C8498E101A36A60083642A /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40C8498F101A36A60083642A /* sample1.cc in Sources */,
				40C84990101A36A60083642A /* sample1_unittest.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
		8D07F2C10486CC7A007CD1D0 /* Sources */ = {
			isa = PBXSourcesBuildPhase;
			buildActionMask = 2147483647;
			files = (
				40899F3A0FFA70D4000B29AE /* gtest-all.cc in Sources */,
			);
			runOnlyForDeploymentPostprocessing = 0;
		};
/* End PBXSourcesBuildPhase section */

/* Begin PBXTargetDependency section */
		40899F9D0FFA740F000B29AE /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40899F420FFA7184000B29AE /* gtest_unittest-framework */;
			targetProxy = 40899F9C0FFA740F000B29AE /* PBXContainerItemProxy */;
		};
		4089A0980FFAD34A000B29AE /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 4089A0120FFACEFC000B29AE /* sample1_unittest-framework */;
			targetProxy = 4089A0970FFAD34A000B29AE /* PBXContainerItemProxy */;
		};
		408BEC101046CFE900DEF522 /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C848F9101A209C0083642A /* gtest-static */;
			targetProxy = 408BEC0F1046CFE900DEF522 /* PBXContainerItemProxy */;
		};
		40C44AE60E379922008FCC51 /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C44ADC0E3798F4008FCC51 /* Version Info */;
			targetProxy = 40C44AE50E379922008FCC51 /* PBXContainerItemProxy */;
		};
		40C8497B101A36850083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C848F9101A209C0083642A /* gtest-static */;
			targetProxy = 40C8497C101A36850083642A /* PBXContainerItemProxy */;
		};
		40C8497D101A36850083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C8490A101A217E0083642A /* gtest_main-static */;
			targetProxy = 40C8497E101A36850083642A /* PBXContainerItemProxy */;
		};
		40C8498A101A36A60083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C848F9101A209C0083642A /* gtest-static */;
			targetProxy = 40C8498B101A36A60083642A /* PBXContainerItemProxy */;
		};
		40C8498C101A36A60083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C8490A101A217E0083642A /* gtest_main-static */;
			targetProxy = 40C8498D101A36A60083642A /* PBXContainerItemProxy */;
		};
		40C8499C101A36DC0083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C8490A101A217E0083642A /* gtest_main-static */;
			targetProxy = 40C8499B101A36DC0083642A /* PBXContainerItemProxy */;
		};
		40C8499E101A36E50083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 8D07F2BC0486CC7A007CD1D0 /* gtest-framework */;
			targetProxy = 40C8499D101A36E50083642A /* PBXContainerItemProxy */;
		};
		40C849A0101A36F10083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 8D07F2BC0486CC7A007CD1D0 /* gtest-framework */;
			targetProxy = 40C8499F101A36F10083642A /* PBXContainerItemProxy */;
		};
		40C849F7101A43440083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C8497A101A36850083642A /* gtest_unittest-static */;
			targetProxy = 40C849F6101A43440083642A /* PBXContainerItemProxy */;
		};
		40C849F9101A43490083642A /* PBXTargetDependency */ = {
			isa = PBXTargetDependency;
			target = 40C84989101A36A60083642A /* sample1_unittest-static */;
			targetProxy = 40C849F8101A43490083642A /* PBXContainerItemProxy */;
		};
/* End PBXTargetDependency section */

/* Begin XCBuildConfiguration section */
		3B238F600E828B5400846E11 /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				COPY_PHASE_STRIP = NO;
				GCC_DYNAMIC_NO_PIC = NO;
				GCC_OPTIMIZATION_LEVEL = 0;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = Check;
				SDKROOT = macosx;
			};
			name = Debug;
		};
		3B238F610E828B5400846E11 /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				COPY_PHASE_STRIP = YES;
				DEBUG_INFORMATION_FORMAT = "dwarf-with-dsym";
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = Check;
				SDKROOT = macosx;
				ZERO_LINK = NO;
			};
			name = Release;
		};
		40899F450FFA7185000B29AE /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = ../;
				PRODUCT_NAME = "gtest_unittest-framework";
				SDKROOT = macosx;
			};
			name = Debug;
		};
		40899F460FFA7185000B29AE /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = ../;
				PRODUCT_NAME = "gtest_unittest-framework";
				SDKROOT = macosx;
			};
			name = Release;
		};
		4089A0150FFACEFD000B29AE /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = "sample1_unittest-framework";
				SDKROOT = macosx;
			};
			name = Debug;
		};
		4089A0160FFACEFD000B29AE /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = "sample1_unittest-framework";
				SDKROOT = macosx;
			};
			name = Release;
		};
		40C44ADF0E3798F4008FCC51 /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				MACOSX_DEPLOYMENT_TARGET = 10.7;
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
				TARGET_NAME = gtest;
			};
			name = Debug;
		};
		40C44AE00E3798F4008FCC51 /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				MACOSX_DEPLOYMENT_TARGET = 10.7;
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
				TARGET_NAME = gtest;
			};
			name = Release;
		};
		40C848FB101A209D0083642A /* Debug */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_INLINES_ARE_PRIVATE_EXTERN = YES;
				GCC_SYMBOLS_PRIVATE_EXTERN = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
			};
			name = Debug;
		};
		40C848FC101A209D0083642A /* Release */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_INLINES_ARE_PRIVATE_EXTERN = YES;
				GCC_SYMBOLS_PRIVATE_EXTERN = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
			};
			name = Release;
		};
		40C8490E101A217F0083642A /* Debug */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				PRODUCT_NAME = gtest_main;
				SDKROOT = macosx;
			};
			name = Debug;
		};
		40C8490F101A217F0083642A /* Release */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40899FB30FFA7567000B29AE /* StaticLibraryTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				PRODUCT_NAME = gtest_main;
				SDKROOT = macosx;
			};
			name = Release;
		};
		40C84985101A36850083642A /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = ../;
				PRODUCT_NAME = gtest_unittest;
				SDKROOT = macosx;
			};
			name = Debug;
		};
		40C84986101A36850083642A /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = ../;
				PRODUCT_NAME = gtest_unittest;
				SDKROOT = macosx;
			};
			name = Release;
		};
		40C84995101A36A60083642A /* Debug */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = "sample1_unittest-static";
				SDKROOT = macosx;
			};
			name = Debug;
		};
		40C84996101A36A60083642A /* Release */ = {
			isa = XCBuildConfiguration;
			buildSettings = {
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				PRODUCT_NAME = "sample1_unittest-static";
				SDKROOT = macosx;
			};
			name = Release;
		};
		4FADC24308B4156D00ABE55E /* Debug */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40D4CDF20E30E07400294801 /* FrameworkTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				DYLIB_COMPATIBILITY_VERSION = 1;
				DYLIB_CURRENT_VERSION = 1;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				INFOPLIST_FILE = Resources/Info.plist;
				INFOPLIST_PREFIX_HEADER = "$(PROJECT_TEMP_DIR)/Version.h";
				INFOPLIST_PREPROCESS = YES;
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
				VERSIONING_SYSTEM = "apple-generic";
			};
			name = Debug;
		};
		4FADC24408B4156D00ABE55E /* Release */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40D4CDF20E30E07400294801 /* FrameworkTarget.xcconfig */;
			buildSettings = {
				COMBINE_HIDPI_IMAGES = YES;
				DYLIB_COMPATIBILITY_VERSION = 1;
				DYLIB_CURRENT_VERSION = 1;
				GCC_VERSION = com.apple.compilers.llvm.clang.1_0;
				HEADER_SEARCH_PATHS = (
					../,
					../include/,
				);
				INFOPLIST_FILE = Resources/Info.plist;
				INFOPLIST_PREFIX_HEADER = "$(PROJECT_TEMP_DIR)/Version.h";
				INFOPLIST_PREPROCESS = YES;
				PRODUCT_NAME = gtest;
				SDKROOT = macosx;
				VERSIONING_SYSTEM = "apple-generic";
			};
			name = Release;
		};
		4FADC24708B4156D00ABE55E /* Debug */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40D4CDF10E30E07400294801 /* DebugProject.xcconfig */;
			buildSettings = {
			};
			name = Debug;
		};
		4FADC24808B4156D00ABE55E /* Release */ = {
			isa = XCBuildConfiguration;
			baseConfigurationReference = 40D4CDF40E30E07400294801 /* ReleaseProject.xcconfig */;
			buildSettings = {
			};
			name = Release;
		};
/* End XCBuildConfiguration section */

/* Begin XCConfigurationList section */
		3B238FA30E828BB600846E11 /* Build configuration list for PBXAggregateTarget "Check" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				3B238F600E828B5400846E11 /* Debug */,
				3B238F610E828B5400846E11 /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40899F4A0FFA71BC000B29AE /* Build configuration list for PBXNativeTarget "gtest_unittest-framework" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40899F450FFA7185000B29AE /* Debug */,
				40899F460FFA7185000B29AE /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		4089A0240FFACF01000B29AE /* Build configuration list for PBXNativeTarget "sample1_unittest-framework" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				4089A0150FFACEFD000B29AE /* Debug */,
				4089A0160FFACEFD000B29AE /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40C44AE40E379905008FCC51 /* Build configuration list for PBXAggregateTarget "Version Info" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40C44ADF0E3798F4008FCC51 /* Debug */,
				40C44AE00E3798F4008FCC51 /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40C84902101A212E0083642A /* Build configuration list for PBXNativeTarget "gtest-static" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40C848FB101A209D0083642A /* Debug */,
				40C848FC101A209D0083642A /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40C84912101A21D20083642A /* Build configuration list for PBXNativeTarget "gtest_main-static" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40C8490E101A217F0083642A /* Debug */,
				40C8490F101A217F0083642A /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40C84984101A36850083642A /* Build configuration list for PBXNativeTarget "gtest_unittest-static" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40C84985101A36850083642A /* Debug */,
				40C84986101A36850083642A /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		40C84994101A36A60083642A /* Build configuration list for PBXNativeTarget "sample1_unittest-static" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				40C84995101A36A60083642A /* Debug */,
				40C84996101A36A60083642A /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		4FADC24208B4156D00ABE55E /* Build configuration list for PBXNativeTarget "gtest-framework" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				4FADC24308B4156D00ABE55E /* Debug */,
				4FADC24408B4156D00ABE55E /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
		4FADC24608B4156D00ABE55E /* Build configuration list for PBXProject "gtest" */ = {
			isa = XCConfigurationList;
			buildConfigurations = (
				4FADC24708B4156D00ABE55E /* Debug */,
				4FADC24808B4156D00ABE55E /* Release */,
			);
			defaultConfigurationIsVisible = 0;
			defaultConfigurationName = Release;
		};
/* End XCConfigurationList section */
	};
	rootObject = 0867D690FE84028FC02AAC07 /* Project object */;
}
