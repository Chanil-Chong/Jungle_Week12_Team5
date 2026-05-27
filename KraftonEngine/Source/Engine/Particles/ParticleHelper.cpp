#include "Particles/ParticleHelper.h"
#include "Particles/ParticleMemory.h"

#include <cassert>
#include <cstring>

FParticleDataContainer::~FParticleDataContainer()
{
    Free();
}

FParticleDataContainer::FParticleDataContainer(FParticleDataContainer&& Other) noexcept
{
    MemBlockSize = Other.MemBlockSize;
    ParticleDataNumBytes = Other.ParticleDataNumBytes;
    ParticleIndicesNum = Other.ParticleIndicesNum;
    ParticleData = Other.ParticleData;
    ParticleIndices = Other.ParticleIndices;

    Other.MemBlockSize = 0;
    Other.ParticleDataNumBytes = 0;
    Other.ParticleIndicesNum = 0;
    Other.ParticleData = nullptr;
    Other.ParticleIndices = nullptr;
}

FParticleDataContainer& FParticleDataContainer::operator=(FParticleDataContainer&& Other) noexcept
{
    if (this == &Other)
    {
        return *this;
    }

    Free();

    MemBlockSize = Other.MemBlockSize;
    ParticleDataNumBytes = Other.ParticleDataNumBytes;
    ParticleIndicesNum = Other.ParticleIndicesNum;
    ParticleData = Other.ParticleData;
    ParticleIndices = Other.ParticleIndices;

    Other.MemBlockSize = 0;
    Other.ParticleDataNumBytes = 0;
    Other.ParticleIndicesNum = 0;
    Other.ParticleData = nullptr;
    Other.ParticleIndices = nullptr;

    return *this;
}

void FParticleDataContainer::Alloc(int32 InParticleDataNumBytes, int32 InParticleIndicesNum)
{
    Free();

    assert(InParticleDataNumBytes >= 0);
    assert(InParticleIndicesNum >= 0);

    ParticleDataNumBytes =
        static_cast<int32>(ParticleMemory::AlignSize(static_cast<size_t>(InParticleDataNumBytes)));

    ParticleIndicesNum = InParticleIndicesNum;

    MemBlockSize =
        ParticleDataNumBytes +
        ParticleIndicesNum * static_cast<int32>(sizeof(uint32));

    if (MemBlockSize <= 0)
    {
        return;
    }

    ParticleData = static_cast<uint8*>(
        ParticleMemory::Malloc(static_cast<size_t>(MemBlockSize)));

    assert(ParticleData != nullptr);

    std::memset(ParticleData, 0, static_cast<size_t>(MemBlockSize));

    ParticleIndices =
        reinterpret_cast<uint32*>(ParticleData + ParticleDataNumBytes);
}

void FParticleDataContainer::Free()
{
    if (!ParticleData)
    {
        MemBlockSize = 0;
        ParticleDataNumBytes = 0;
        ParticleIndicesNum = 0;
        ParticleIndices = nullptr;
        return;
    }

    ParticleMemory::Free(ParticleData, static_cast<size_t>(MemBlockSize));

    MemBlockSize = 0;
    ParticleDataNumBytes = 0;
    ParticleIndicesNum = 0;
    ParticleData = nullptr;
    ParticleIndices = nullptr;
}
