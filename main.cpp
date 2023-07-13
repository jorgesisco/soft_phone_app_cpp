#include <pjsua2.hpp>
#include <iostream>
#include <cstdlib>  // for getenv
#include <thread>
#include <chrono>
#include <pjsua-lib/pjsua.h>

using namespace pj;

class MyCall : public pj::Call
{
public:
    MyCall(pj::Account &acc, int call_id, pj::Endpoint &ep) 
        : Call(acc, call_id), account(acc), ep(ep), recorder(nullptr), player(nullptr) {}

    virtual void onCallState(pj::OnCallStateParam &prm)
    {
        pj::CallInfo ci = getInfo();

        if (ci.state == PJSIP_INV_STATE_DISCONNECTED)
        {
            if (recorder)
            {
                recorder = nullptr;

                // Remove this call from the account's list of calls
                // This will need to be handled in your account management system
                // account.calls.remove(this); 
            }
        }
    }

    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm)
    {
        pj::CallInfo ci = getInfo();

        for (auto &mi : ci.media)
        {
            if (mi.type == PJMEDIA_TYPE_AUDIO && mi.status == PJSUA_CALL_MEDIA_ACTIVE)
            {
                pj::AudioMedia audioMedia = getAudioMedia(mi.index);
                

                // Create a reference to playback device media
                const pj::AudioMedia& playbackMedia = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();

                // Transmit the call's audio media to the capture device (for you to hear the caller)
                audioMedia.startTransmit(playbackMedia);

                // Transmit from the local machine's microphone to the call's audio media
                pj::Endpoint::instance().audDevManager().getCaptureDevMedia().startTransmit(audioMedia);
            }
        }
    };
private:
    pj::Account &account;
    pj::Endpoint &ep;
    pj::AudioMediaRecorder *recorder;
    pj::AudioMediaPlayer *player;
};

class MyAccount : public Account {
public:
    MyAccount(Endpoint &ep) : ep(ep) {}

    virtual void onRegState(OnRegStateParam &prm) {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive? "*** Register:" : "*** Unregister:")
                  << " code=" << prm.code << std::endl;
    }

    virtual void onIncomingCall(OnIncomingCallParam &iprm) {
        std::cout << "*** Incoming Call: " << iprm.callId << std::endl;
        MyCall *call = new MyCall(*this, iprm.callId, ep);
        CallOpParam prm;
        
        // Ring for 3 seconds before answering
        std::this_thread::sleep_for(std::chrono::seconds(2));


        prm.statusCode = (pjsip_status_code)200;
        call->answer(prm);
    }

private:
    Endpoint &ep;
};

int main() {
    Endpoint ep;

    ep.libCreate();

    EpConfig ep_cfg;
    ep.libInit(ep_cfg);

    TransportConfig tcfg;
    tcfg.port = 5060;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    const char* username_sip_cstr = std::getenv("USERNAME_SIP");
    const char* password_cstr = std::getenv("PASSWORD");
    const char* server_cstr = std::getenv("SERVER");

    if (!username_sip_cstr || !password_cstr || !server_cstr) {
    std::cerr << "Environment variables USERNAME, PASSWORD, and SERVER must be set." << std::endl;
    return 1;
    }

    std::string username_sip = username_sip_cstr;
    std::string password = password_cstr;
    std::string server = server_cstr;

    AccountConfig acfg;
    acfg.idUri = "sip:" + username_sip + "@" + server;
    acfg.regConfig.registrarUri = "sip:" + server;
    AuthCredInfo cred("digest", "*", username_sip, 0, password);
    acfg.sipConfig.authCreds.push_back(cred);

    MyAccount *acc = new MyAccount(ep);
    acc->create(acfg);

    // Handle events
    for (;;) {
        ep.libHandleEvents(500);
    }

    return 0;
}
