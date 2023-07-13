// #include <pjsua2.hpp>
// #include <iostream>
// #include <cstdlib>  // for getenv
// #include <thread>
// #include <chrono>
// #include <pjsua-lib/pjsua.h>


#include "MyCall.h"
#include "MyAccount.h"
#include <iostream>
#include <cstdlib>  // for getenv

using namespace pj;

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