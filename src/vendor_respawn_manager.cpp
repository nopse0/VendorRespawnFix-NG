#undef GetForm

#include <SKSE/SKSE.h>
#include <vendor_respawn_manager.h>

using namespace RE;
using namespace vendor_fix;
using namespace SKSE;

VendorRespawnManager& VendorRespawnManager::GetSingleton() noexcept {
    static VendorRespawnManager instance;
    return instance;
}



void VendorRespawnManager::OnRevert(SerializationInterface* serde) {

}

void VendorRespawnManager::OnGameLoaded(SerializationInterface* serde) {
    std::uint32_t type, version, length;

    log::info("OnGameLoaded started");

    auto factions = TESDataHandler::GetSingleton()->formArrays[stl::to_underlying(FormType::Faction)];

    while (serde->GetNextRecordInfo(type, version, length)) {

        if (recordId == type) {
            if (recordVersion == version && sizeof(LastDayReset_Record) == length) {
                auto record = LastDayReset_Record(0, 0);
                serde->ReadRecordData(&record, sizeof(LastDayReset_Record));
                log::info("Restoring form id: {}, lastDayReset: {}", record.formID, record.lastDayReset);

                RE:TESForm* form = 0;
                if (serde->ResolveFormID(record.formID, record.formID)) {
                    for (auto* tmp : factions) {
                        if (tmp->formID == record.formID) {
                            form = tmp;
                            break;
                        }
                    }
                }

                if (0 != form) {
                    auto faction = static_cast<TESFaction*>(form);
                    if (!faction || !faction->vendorData.merchantContainer) {
                        log::error("Form isn't a TESFaction or hasn't a merchantContainer");
                        continue;
                    }
                    faction->vendorData.lastDayReset = record.lastDayReset;
                    log::info("Restored form id: {}, lastDayReset: {}", record.formID, record.lastDayReset);
                }
                else {
                    log::info("Form not found");
                }



            }
        }

    }

    log::info("OnGameLoaded stopped");
}

void VendorRespawnManager::OnGameSaved(SerializationInterface* serde) {

    log::info("OnGameSaved started");

    const auto& factions = TESDataHandler::GetSingleton()->formArrays[stl::to_underlying(FormType::Faction)];

    for (auto* form : factions) {

        auto faction = static_cast<TESFaction*>(form);

        if (!faction) {
            log::error("Faction not found.");
            continue;
        }

        if (faction->vendorData.merchantContainer) {
            auto record = LastDayReset_Record(faction->formID, faction->vendorData.lastDayReset);

            log::info("Saving form id: {}, lastDayReset: {}", record.formID, record.lastDayReset);

            serde->WriteRecord(recordId, recordVersion, record);   
        }

    }

    log::info("OnGameSaved finished");
 
}
