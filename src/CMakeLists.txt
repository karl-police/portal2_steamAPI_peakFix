# CMakeList.txt : CMake project, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (portal2_steamAPI_peakFix SHARED "dll_plugin.cpp")

# Add /include as header file paths
target_include_directories(portal2_steamAPI_peakFix PRIVATE "include")
target_include_directories(portal2_steamAPI_peakFix PRIVATE "modules")


# Source SDK 2013 Multiplayer Source files
# unused since SDK 2013 is wrong for Portal 2
#target_include_directories(portal2_steamAPI_peakFix PRIVATE "${SOURCE_SDK_2013}/mp/src")

#target_link_libraries(portal2_steamAPI_peakFix PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/steam_api.lib")


# Detours
target_include_directories(portal2_steamAPI_peakFix PRIVATE "${DETOURS_PATH}/include")
target_link_libraries(portal2_steamAPI_peakFix PRIVATE "${DETOURS_PATH}/lib.X86/detours.lib")



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET portal2_steamAPI_peakFix PROPERTY CXX_STANDARD 20)
endif()


write_file("portal2_steamAPI_peakFix.vdf"
[["Plugin"
{
	"file" "addons\portal2_steamAPI_peakFix"
}]]
)

# Add tests and install targets if needed.