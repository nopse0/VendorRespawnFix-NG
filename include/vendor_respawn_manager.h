#pragma once

#include <stddef.h>
#include <RE/Skyrim.h>


namespace vendor_fix {
#pragma warning(push)
#pragma warning(disable : 4251)

    struct LastDayReset_Record
    {
        RE::FormID    formID;       // 0
        std::uint32_t lastDayReset; // 4
    };
    static_assert(offsetof(LastDayReset_Record, formID) == 0x0);
    static_assert(offsetof(LastDayReset_Record, lastDayReset) == 0x4);
    static_assert(sizeof(LastDayReset_Record) == 0x8);



    /**
     * The class which tracks hit count information.
     *
     * <p>
     * All aspects of this are done in a singleton since SKSE cannot create instances of new Papyrus types such as a
     * hit counter.
     * </p>
     *
     * <p>
     * Hit count information must be tracked in the SKSE cosave when the game is save and loaded. Therefore the main
     * <code>SKSEQuery_Load</code> call must be sure to initialize the serialization handlers such that they will call
     * this class when it needs to save or load data.
     * </p>
     */
    class __declspec(dllexport) VendorRespawnManager {
    public:

        enum : std::uint32_t
        {
            interfaceId = 'VRFN',
            recordId = 'VRFR',
            recordVersion = 1
        };

        /**
         * Get the singleton instance of the <code>HitCounterManager</code>.
         */
        [[nodiscard]] static VendorRespawnManager& GetSingleton() noexcept;

        /**
         * The serialization handler for reverting game state.
         *
         * <p>
         * This is called as the handler for revert. Revert is called by SKSE on a plugin that is registered for
         * serialization handling on a new game or before a save game is loaded. It should be used to revert the state
         * of the plugin back to its default.
         * </p>
         */
        static void OnRevert(SKSE::SerializationInterface*);

        /**
         * The serialization handler for saving data to the cosave.
         *
         * @param serde The serialization interface used to write data.
         */
        static void OnGameSaved(SKSE::SerializationInterface* serde);

        /**
         * The serialization handler for loading data from a cosave.
         *
         * @param serde  The serialization interface used to read data.
         */
        static void OnGameLoaded(SKSE::SerializationInterface* serde);

    private:
        VendorRespawnManager() = default;

        mutable std::mutex _lock;
    };
#pragma warning(pop)
}  // namespace Sample
