#include "Core.h"
#include "NetworkRPC.h"

void NetworkRPC::invokeRPC(RakNet::BitStream & bitStream)
{
    STRCODE functionName;
    bitStream.Read(functionName);

    auto iter = rpcFunctions.find(functionName);
    if (iter != rpcFunctions.end())
    {
        (*iter).second(bitStream);
    }
}

void NetworkRPC::registerRPC(STRCODE functionName, std::function<void(RakNet::BitStream&)> functionPtr)
{
    rpcFunctions.insert(std::pair<STRCODE, std::function<void(RakNet::BitStream&)>>(functionName, functionPtr));
}
