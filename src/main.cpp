#include <vendor_respawn_manager.h>
#include "config.h"
#include "main.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;



/**
 * Setup logging.
 *
 * <p>
 * Logging is important to track issues. CommonLibSSE bundles functionality for spdlog, a common C++ logging
 * framework. Here we initialize it, using values from the configuration file. This includes support for a debug
 * logger that shows output in your IDE when it has a debugger attached to Skyrim, as well as a file logger which
 * writes data to the standard SKSE logging directory at <code>Documents/My Games/Skyrim Special Edition/SKSE</code>
 * (or <code>Skyrim VR</code> if you are using VR).
 * </p>
 */
void InitializeLogging() {
    auto path = log_directory();
    if (!path) {
        report_and_fail("Unable to lookup SKSE logs directory.");
    }
    *path /= PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    if (IsDebuggerPresent()) {
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    }
    else {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
    }
    const auto& debugConfig = Sample::Config::GetSingleton().GetDebug();
    log->set_level(debugConfig.GetLogLevel());
    log->flush_on(debugConfig.GetFlushLevel());

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}


/**
 * Initialize the SKSE cosave system for our plugin.
 *
 * <p>
 * SKSE comes with a feature called a <em>cosave</em>, an additional save file kept alongside the original Skyrim
 * save file. SKSE plugins can write their own data to this file, and load it again when the save game is loaded,
 * allowing them to keep custom data along with a player's save. Each plugin must have a unique ID, which is four
 * characters long (similar to the record names used by forms in ESP files). Note however this is little-endian, so
 * technically the 'SMPL' here ends up as 'LPMS' in the save file, unless we use a byte order swap.
 * </p>
 *
 * <p>
 * There can only be one serialization callback for save, revert (called on new game and before a load), and load
 * for the entire plugin.
 * </p>
*/
void InitializeSerialization() {
    log::trace("Initializing cosave serialization...");
    auto* serde = GetSerializationInterface();
    serde->SetUniqueID(vendor_fix::VendorRespawnManager::interfaceId);
    serde->SetSaveCallback(vendor_fix::VendorRespawnManager::OnGameSaved);
    serde->SetRevertCallback(vendor_fix::VendorRespawnManager::OnRevert);
    serde->SetLoadCallback(vendor_fix::VendorRespawnManager::OnGameLoaded);
    log::trace("Cosave serialization initialized.");
}

/**
 * This if the main callback for initializing your SKSE plugin, called just before Skyrim runs its main function.
 *
 * <p>
 * This is your main entry point to your plugin, where you should initialize everything you need. Many things can't be
 * done yet here, since Skyrim has not initialized and the Windows loader lock is not released (so don't do any
 * multithreading). But you can register to listen for messages for later stages of Skyrim startup to perform such
 * tasks.
 * </p>
 */
SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);
    /* if (g_interface001.GetMerges()) {
        g_mergeMapperInterface = &g_interface001;
        InitializeMessaging();
    }
    else
        log::info("{} disabled because no merges found.", plugin->GetName());*/
    InitializeSerialization();
    // InitializePapyrus();

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}