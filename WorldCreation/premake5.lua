project "WorldCreation"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    runtime "Debug"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.c",
        "dependencies/ofxImGui/src/**.h",
        "dependencies/ofxImGui/src/**.cpp",
        "dependencies/ofxImGui/src/**.hpp",
        "dependencies/ofxImGui/src/**.c",
        "dependencies/ofxImGui/libs/imgui/src/**.h",
        "dependencies/ofxImGui/libs/imgui/src/**.cpp",
        "dependencies/ofxImGui/libs/imgui/src/**.hpp",
        "dependencies/ofxImGui/libs/imgui/src/**.c",
        "dependencies/ofxAssimpModelLoader/src/**.h",
        "dependencies/ofxAssimpModelLoader/src/**.cpp",
        "dependencies/ofxAssimpModelLoader/src/**.hpp",
        "dependencies/ofxAssimpModelLoader/src/**.c",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/**.h",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/**.cpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/**.hpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/**.c",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/Compiler/**.h",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/Compiler/**.cpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/Compiler/**.hpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/Compiler/**.c",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/port/**.h",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/port/**.cpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/port/**.hpp",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include/assimp/port/**.c",
    }

    includedirs
    {
        "src",
        "src/Core",
        "dependencies/ofxImGui/src",
        "dependencies/ofxImGui/libs/imgui/src",
        "dependencies/ofxAssimpModelLoader/libs/assimp/include",
        "dependencies/ofxAssimpModelLoader/src",
        -- OpenFrameworks include dirs
        "../OpenFrameworks/Src/",
        "../OpenFrameworks/Src/graphics",
        "../OpenFrameworks/Src/app",
        "../OpenFrameworks/Src/sound",
        "../OpenFrameworks/Src/utils",
        "../OpenFrameworks/Src/communication",
        "../OpenFrameworks/Src/video",
        "../OpenFrameworks/Src/types",
        "../OpenFrameworks/Src/math",
        "../OpenFrameworks/Src/3d",
        "../OpenFrameworks/Src/gl",
        "../OpenFrameworks/Src/events",
        "../OpenFrameworks/Dependencies/glut/include",
        "../OpenFrameworks/Dependencies/glm/include",
        "../OpenFrameworks/Dependencies/rtAudio/include",
        "../OpenFrameworks/Dependencies/quicktime/include",
        "../OpenFrameworks/Dependencies/freetype/include",
        "../OpenFrameworks/Dependencies/freetype/include/freetype2",
        "../OpenFrameworks/Dependencies/freeImage/include",
        "../OpenFrameworks/Dependencies/fmod/include",
        "../OpenFrameworks/Dependencies/videoInput/include",
        "../OpenFrameworks/Dependencies/glew/include/",
        "../OpenFrameworks/Dependencies/glu/include",
        "../OpenFrameworks/Dependencies/tess2/include",
        "../OpenFrameworks/Dependencies/cairo/include/cairo",
        "../OpenFrameworks/Dependencies/poco/include",
        "../OpenFrameworks/Dependencies/glfw/include",
        "../OpenFrameworks/Dependencies/openssl/include",
        "../OpenFrameworks/Dependencies/utf8/include",
        "../OpenFrameworks/Dependencies/boost/include",
        "../OpenFrameworks/Dependencies/json/include",
        "../OpenFrameworks/Dependencies/curl/include",
        "../OpenFrameworks/Dependencies/uriparser/include",
        "../OpenFrameworks/Dependencies/pugixml/include",
    }

    libdirs 
    {
        "dependencies/ofxAssimpModelLoader/libs/assimp/lib/vs/x64",
        "../OpenFrameworks/Dependencies/glfw/lib/vs/x64",
        "../OpenFrameworks/Dependencies/rtAudio/lib/vs/x64",
        "../OpenFrameworks/Dependencies/FreeImage/lib/vs/x64",
        "../OpenFrameworks/Dependencies/freetype/lib/vs/x64",
        "../OpenFrameworks/Dependencies/fmod/lib/vs/x64",
        "../OpenFrameworks/Dependencies/videoInput/lib/vs/x64",
        "../OpenFrameworks/Dependencies/cairo/lib/vs/x64",
        "../OpenFrameworks/Dependencies/glew/lib/vs/x64",
        "../OpenFrameworks/Dependencies/glu/lib/vs/x64",
        "../OpenFrameworks/Dependencies/openssl/lib/vs/x64",
        "../OpenFrameworks/Dependencies/curl/lib/vs/x64",
        "../OpenFrameworks/Dependencies/tess2/lib/vs/x64",
        "../OpenFrameworks/Dependencies/boost/lib/vs/x64",
        "../OpenFrameworks/Dependencies/uriparser/lib/vs/x64",
        "../OpenFrameworks/Dependencies/pugixml/lib/vs/x64",
        "C:/Program Files (x86)/Visual Leak Detector/lib/Win64",
    }

    links
    {
        "OpenFrameworks",
        "cairo-static.lib",
        "pixman-1.lib",
        "libpng.lib",
        "zlib.lib",
        "msimg32.lib",
        "OpenGL32.lib",
        "kernel32.lib",
        "setupapi.lib",
        "Vfw32.lib",
        "comctl32.lib",
        "rtAudioD.lib",
        "videoInputD.lib",
        "libfreetype.lib",
        "FreeImage.lib",
        "dsound.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "glew32s.lib",
        "fmod64_vc.lib",
        "libssl.lib",
        "libcrypto.lib",
        "crypt32.lib",
        "libcurl.lib",
        "uriparser.lib",
        "pugixmld.lib",
        "Ws2_32.lib",
        "tess2.lib",
        "glfw3.lib",
        "winmm.lib",
        "odbc32.lib",
        "odbccp32.lib",
        "wldap32.lib",
        "assimp64.lib",
    }

    flags
    { 
        "MultiProcessorCompile",
    }

    postbuildcommands 
    { 
        "{COPY} dependencies/ofxAssimpModelLoader/libs/assimp/lib/vs/x64/assimp64.dll ../bin/Debug-windows-x86_64/WorldCreation",
        "{COPY} ../OpenFrameworks/Dependencies/FreeImage/lib/vs/x64/FreeImage.dll ../bin/Debug-windows-x86_64/WorldCreation",
        "{COPY} ../OpenFrameworks/Dependencies/fmod/lib/vs/x64/fmod.dll ../bin/Debug-windows-x86_64/WorldCreation",
        "{COPY} ../data ../bin/Debug-windows-x86_64/WorldCreation/data",
    }

    debugdir "../bin/Debug-windows-x86_64/WorldCreation"

    filter "system:windows"
        cppdialect "C++14"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines 
        {
            "WC_DEBUG",
            "WIN32",
            "_DEBUG",
            "_CONSOLE",
            "POCO_STATIC",
            "CAIRO_WIN32_STATIC_BUILD",
            "DISABLE_SOME_FLOATING_POINT",
            "CURL_STATICLIB",
            "_UNICODE",
            "UNICODE",
        }
        symbols "On"
        buildoptions "/MDd"

    filter "configurations:Release"
        defines 
        {
            "WC_RELEASE",
            "WIN32",
            "_DEBUG",
            "_CONSOLE",
            "POCO_STATIC",
            "CAIRO_WIN32_STATIC_BUILD",
            "DISABLE_SOME_FLOATING_POINT",
            "CURL_STATICLIB",
            "_UNICODE",
            "UNICODE",
        }
        optimize "On"

    filter "configurations:Dist"
        defines 
        {
            "WC_DIST",
            "WIN32",
            "_DEBUG",
            "_CONSOLE",
            "POCO_STATIC",
            "CAIRO_WIN32_STATIC_BUILD",
            "DISABLE_SOME_FLOATING_POINT",
            "CURL_STATICLIB",
            "_UNICODE",
            "UNICODE",
        }
        symbols "On"