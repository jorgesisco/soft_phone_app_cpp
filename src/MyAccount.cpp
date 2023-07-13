// src/MyAccount.cpp
#include "MyAccount.h"

MyAccount::MyAccount(Endpoint &ep) : Account(), ep(ep) {}

void MyAccount::onRegState(OnRegStateParam &prm) {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive? "*** Register:" : "*** Unregister:")
                  << " code=" << prm.code << std::endl;
    }

void MyAccount::onIncomingCall(OnIncomingCallParam &iprm) {
        std::cout << "*** Incoming Call: " << iprm.callId << std::endl;
        MyCall *call = new MyCall(*this, iprm.callId, ep);
        CallOpParam prm;
        
        // Ring for 3 seconds before answering
        std::this_thread::sleep_for(std::chrono::seconds(2));


        prm.statusCode = (pjsip_status_code)200;
        call->answer(prm);
    }
