#pragma once

class NetworkRPC
{
private:
    std::map<STRCODE, std::function<void(RakNet::BitStream&)>> rpcFunctions;

public:
    void invokeRPC(RakNet::BitStream& bitStream);

protected:
    void registerRPC(STRCODE functionName, std::function<void(RakNet::BitStream&)> functionPtr);
};

