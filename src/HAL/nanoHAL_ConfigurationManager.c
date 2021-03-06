//
// Copyright (c) 2018 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <string.h>
#include <nanoHAL_v2.h>

uint32_t FindNextBlock(uint32_t startAddress, uint32_t endAddress, const unsigned char* marker)
{
    // all configuration markers are 4 bytes length

    unsigned char* cursor = (unsigned char*)startAddress;

    while(cursor < (unsigned char*)endAddress - 4)
    {
        if(memcmp(cursor, marker, 4 ) == 0)
        {
            // found one!
            break;
        }

        cursor++;
    }

    return (uint32_t)cursor;
}

uint32_t GetBlockCount(uint32_t startAddress, uint32_t endAddress, uint32_t blockSize, const unsigned char* marker)
{
    // all configuration markers are 4 bytes length
    int blockCount = 0;

    unsigned char* cursor = (unsigned char*)startAddress;

    while(cursor < (unsigned char*)endAddress - 4)
    {
        if(memcmp(cursor, marker, 4 ) == 0)
        {
            // found one!
            blockCount++;

            // bump cursor to the end of the config block size
            cursor +=blockSize;
        }
        else
        {
            cursor++;
        }
    }

    return blockCount;
}

__nfweak void* ConfigurationManager_FindNetworkConfigurationBlocks(uint32_t startAddress, uint32_t endAddress)
{
    uint32_t nextBlock = startAddress;

    // first pass: find out how many blocks of this type we have
    uint32_t blockCount = GetBlockCount(startAddress, endAddress, sizeof(HAL_Configuration_NetworkInterface), c_MARKER_CONFIGURATION_NETWORK_V1);

    // allocate config struct
    HAL_CONFIGURATION_NETWORK *networkConfigs = (HAL_CONFIGURATION_NETWORK *)platform_malloc(offsetof(HAL_CONFIGURATION_NETWORK, Configs) + blockCount * sizeof(networkConfigs->Configs[0]));

    // set collection count
    networkConfigs->Count = blockCount;

    if(blockCount > 0)
    {

        // second pass: get address of each config block
        for(uint32_t i = 0; i < blockCount; i++)
        {
            nextBlock = FindNextBlock(nextBlock, endAddress, c_MARKER_CONFIGURATION_NETWORK_V1);
            networkConfigs->Configs[i] = (HAL_Configuration_NetworkInterface*)nextBlock;
        }
    }

    return networkConfigs;
}

__nfweak void* ConfigurationManager_FindNetworkWireless80211ConfigurationBlocks(uint32_t startAddress, uint32_t endAddress)
{
    uint32_t nextBlock = startAddress;

    // first pass: find out how many blocks of this type we have
    uint32_t blockCount = GetBlockCount(startAddress, endAddress, sizeof(HAL_Configuration_Wireless80211), c_MARKER_CONFIGURATION_WIRELESS80211_V1);

    // allocate config struct
    HAL_CONFIGURATION_NETWORK_WIRELESS80211 *networkWirelessConfigs = (HAL_CONFIGURATION_NETWORK_WIRELESS80211 *)platform_malloc(offsetof(HAL_CONFIGURATION_NETWORK_WIRELESS80211, Configs) + blockCount * sizeof(networkWirelessConfigs->Configs[0]));

    // set collection count
    networkWirelessConfigs->Count = blockCount;

    if(blockCount > 0)
    {
        // second pass: get address of each config block
        for(uint32_t i = 0; i < blockCount; i++)
        {
            nextBlock = FindNextBlock(nextBlock, endAddress, c_MARKER_CONFIGURATION_WIRELESS80211_V1);
            networkWirelessConfigs->Configs[i] = (HAL_Configuration_Wireless80211*)nextBlock;
        }
    }

    return networkWirelessConfigs;
}

__nfweak HAL_Configuration_Wireless80211* ConfigurationManager_GetWirelessConfigurationFromId(uint32_t configurationId)
{
    for(int i = 0; i < g_TargetConfiguration.Wireless80211Configs->Count; i++)
    {
        if(g_TargetConfiguration.Wireless80211Configs->Configs[i]->Id == configurationId)
        {
            return g_TargetConfiguration.Wireless80211Configs->Configs[i];
        }
    }

    // not found
    return NULL;
}
