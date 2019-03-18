#pragma once

#include <BitStream.h>

class ISerializable
{
public:
    virtual ~ISerializable() {}

    virtual void writeUpdate(RakNet::BitStream& bs) const = 0;
    virtual void readUpdate(RakNet::BitStream& bs) = 0;

    virtual void writeCreate(RakNet::BitStream& bs) const = 0;
    virtual void readCreate(RakNet::BitStream& bs) = 0;
};