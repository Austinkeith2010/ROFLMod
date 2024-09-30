#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

$execute
{
    // Memory patch addresses for different platforms
    std::vector<geode::Patch*> patches = {};

    #ifdef GEODE_IS_WINDOWS
    // These patches need to ensure proper boundary handling without pushing objects to the end
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x5ed33c), { 0x90, 0x90, 0x90, 0x90 }));  // NOP out unwanted constraints or incorrect logic
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x5ed340), { 0x90, 0x90, 0x90, 0x90 }));  // NOP (No Operation) instructions to prevent forced limits
    #endif

    #ifdef GEODE_IS_ANDROID32
    // Same idea for Android 32-bit platform
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x37e678 - 0x10000)), { 0x90, 0x90, 0x90, 0x90 }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x37ea1c - 0x10000)), { 0x90, 0x90, 0x90, 0x90 }));
    #endif

    #ifdef GEODE_IS_ANDROID64
    // Patches for Android 64-bit platform
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x76cdb4 - 0x100000)), { 0x90, 0x90, 0x90, 0x90 }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x76d2fc - 0x100000)), { 0x90, 0x90, 0x90, 0x90 }));
    #endif

    #ifdef GEODE_IS_INTEL_MAC
    // Patches for macOS Intel-based systems
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x822770), { 0x90, 0x90, 0x90, 0x90 }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x82258c), { 0x90, 0x90, 0x90, 0x90 }));
    #endif

    #ifdef GEODE_IS_ARM_MAC
    // Placeholder for macOS ARM-based systems
    #endif

    // Apply the patches in the main thread
    Loader::get()->queueInMainThread([patches]{
        for (auto patch : patches)
        {
            // Apply the patches to modify object positioning behavior
            Client::GetModule("editor-extension")->addPatch(patch);
        }
    });
};
