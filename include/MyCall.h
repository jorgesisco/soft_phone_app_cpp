// include/MyCall.h
#pragma once
#include <pjsua2.hpp>
#include <pjsua-lib/pjsua.h>

using namespace pj;

class MyCall : public pj::Call {
public:
    MyCall(pj::Account &acc, int call_id, pj::Endpoint &ep);
    virtual void onCallState(pj::OnCallStateParam &prm);
    // virtual void onCallMediaState(pj::OnCallMediaStateParam &prm);

private:
    pj::Account &account;
    pj::Endpoint &ep;
    pj::AudioMediaRecorder *recorder;
    pj::AudioMediaPlayer *player;
};
