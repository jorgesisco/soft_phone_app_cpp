#include <pjsua2.hpp>
#include <iostream>
#include <cstdlib>  // for getenv
#include <thread>
#include <chrono>

using namespace pj;

class MyCall : public Call {
public:
    MyCall(Account &acc, int call_id = PJSUA_INVALID_ID) 
        : Call(acc, call_id)
    { }

    virtual void onCallState(OnCallStateParam &prm) {
        CallInfo ci = getInfo();
        std::cout << "*** Call: " << ci.remoteUri << ", State: " << ci.stateText << std::endl;
    }
};

class MyAccount : public Account {
public:
    virtual void onRegState(OnRegStateParam &prm) {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive? "*** Register:" : "*** Unregister:")
                  << " code=" << prm.code << std::endl;
    }

    virtual void onIncomingCall(OnIncomingCallParam &iprm) {
        std::cout << "*** Incoming Call: " << iprm.callId << std::endl;
        MyCall *call = new MyCall(*this, iprm.callId);
        CallOpParam prm;
        
        // Ring for 3 seconds before answering
        std::this_thread::sleep_for(std::chrono::seconds(2));


        prm.statusCode = (pjsip_status_code)200;
        call->answer(prm);
    }
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

    MyAccount *acc = new MyAccount;
    acc->create(acfg);

    // Handle events
    for (;;) {
        ep.libHandleEvents(500);
    }

    return 0;
}
