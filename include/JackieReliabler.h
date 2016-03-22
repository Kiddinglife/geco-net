#ifndef ReliabilityLayer_H_
#define ReliabilityLayer_H_

#include "geco-time.h"
#include "geco-namesapces.h"
#include "geco-export.h"
#include "geco-features.h"
#include "geco-basic-type.h"
#if ENABLE_SECURE_HAND_SHAKE==1
#include "geco-secure-hand-shake.h"
#endif

GECO_NET_BEGIN_NSPACE

class JackieRemoteSystem;
class JISRecvParams;
class JackieApplication;
class GecoMemoryStream;
struct ReliableSendParams;

class GECO_EXPORT JackieReliabler
{
    private:
    JackieRemoteSystem* remoteEndpoint;

#if ENABLE_SECURE_HAND_SHAKE == 1
    public:
    cat::AuthenticatedEncryption* GetAuthenticatedEncryption(void) { return &auth_enc; }
    protected:
    cat::AuthenticatedEncryption auth_enc;
    bool useSecurity;
#endif // ENABLE_SECURE_HAND_SHAKE

    public:
    JackieReliabler();
    ~JackieReliabler();

    void ApplyNetworkSimulator(double _packetloss, unsigned short _minExtraPing, unsigned short _extraPingVariance);

    // Packets are read directly from the socket layer and skip the reliability
    //layer  because unconnected players do not use the reliability layer
    // This function takes packet data after a player has been confirmed as
    //connected.  The game should not use that data directly
    // because some data is used internally, such as packet acknowledgment and
    //split packets
    bool ProcessOneConnectedRecvParams(JackieApplication* serverApp,
        JISRecvParams* recvParams, unsigned mtuSize);
    void Reset(bool param1, int MTUSize, bool client_has_security);
    void SetSplitMessageProgressInterval(int splitMessageProgressInterval);
    void SetUnreliableTimeout(TimeMS unreliableTimeout);
    void SetTimeoutTime(TimeMS defaultTimeoutTime);
    bool Send(ReliableSendParams& sendParams);
};

GECO_NET_END_NSPACE
#endif


