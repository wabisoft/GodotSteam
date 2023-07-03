#!python
import os

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = SConscript("../godot-cpp/SConstruct")

addon_target = "../unchiinu/addons/godotsteam/"
# Define our options
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', addon_target, PathVariable.PathAccept))
opts.Add(PathVariable('target_name', 'The library name.', 'godotsteam', PathVariable.PathAccept))

# Local dependency paths, adapt them to your setup
steam_lib_path = "godotsteam/sdk/1.57/redistributable_bin"

# Updates the environment with the option variables.
opts.Update(env)
# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags
arch_path_prefix = "win64/"

# Check our platform specifics
if env['platform'] == "macos":
    # Set the correct Steam library
    arch_path_prefix = "osx/"
    steam_lib_path += "/osx"
    steamworks_library = 'libsteam_api.dylib'

elif env['platform'] in ('linuxbsd', 'linux'):
    # Set correct Steam library
    arch_path_prefix = "linux64/"
    steam_lib_path += "/linux64"
    steamworks_library = 'libsteam_api.so'

elif env['platform'] == "windows":
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    # env.Append(ENV=os.environ)

    # Set correct Steam library
    arch_path_prefix = "win64/"
    steam_lib_path += "/win64"
    steamworks_library = 'steam_api64.dll'

# make sure our binding library is properly includes
env.Append(LIBPATH=[steam_lib_path])
env.Append(CPPPATH=['godotsteam/sdk/1.57/public'])
env.Append(LIBS=[
    steamworks_library.replace(".dll", "")
])

gdextension_path = "godotsteam.gdextension"
# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['godotsteam/'])
sources = Glob('godotsteam/*.cpp')

p = f"{env['target_path']}{arch_path_prefix}{env['target_name']}{env['suffix']}{env['SHLIBSUFFIX']}"
breakpoint()
library = env.SharedLibrary(target=p, source=sources)
env.Depends(library, Command(addon_target + arch_path_prefix + steamworks_library, steam_lib_path + "/" + steamworks_library, Copy("$TARGET", "$SOURCE")))
env.Depends(library, Command(addon_target + gdextension_path, gdextension_path, Copy("$TARGET", "$SOURCE")))

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
