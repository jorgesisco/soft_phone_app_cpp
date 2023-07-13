// include/MyAccount.h
#pragma once
#include <pjsua2.hpp>
#include "MyCall.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace pj;

class MyAccount : public Account {
public:
    MyAccount(Endpoint &ep);
    virtual void onRegState(OnRegStateParam &prm);
    virtual void onIncomingCall(OnIncomingCallParam &iprm);

private:
    Endpoint &ep;
};
