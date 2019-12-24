# Create defines based on platform
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (ARCH Windows)
	set (PLATFORM_WINDOWS 1)
	add_definitions (-DPLATFORM_WINDOWS=1) 
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (ARCH Linux)
	set (PLATFORM_LINUX 1)
	add_definitions (-DPLATFORM_LINUX=1)
endif ()

# Determine target architecture
if ((${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86") AND ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set (ARCH x64)
	set (ARCH_X64 1)
	add_definitions (-DARCH_X64=1)
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64" AND ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	set (ARCH x86)
	set (ARCH_X86 1)
	add_definitions (-DARCH_X86=1)
endif ()

# Determine compiler
if (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
	set (COMPILER MSVC)
	set (COMPILER_MSVC 1)
	add_definitions (-DCOMPILER_MSVC=1)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set (COMPILER Clang)
	set (COMPILER_CLANG 1)
	add_definitions (-DCOMPILER_CLANG=1)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	set (COMPILER GNU)
	set (COMPILER_GCC 1)
	add_definitions (-DCOMPILER_GCC=1)
endif()