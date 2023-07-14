// src/MyCall.cpp
#include "MyCall.h"



MyCall::MyCall(pj::Account &acc, int call_id, pj::Endpoint &ep) 
    : Call(acc, call_id), account(acc), ep(ep), recorder(nullptr), player(nullptr) {}

void MyCall::onCallState(pj::OnCallStateParam &prm)
{
        pj::CallInfo ci = getInfo();

        for (auto &mi : ci.media)
        {
            if (mi.type == PJMEDIA_TYPE_AUDIO && mi.status == PJSUA_CALL_MEDIA_ACTIVE)
            {
        pj::CallInfo ci = getInfo();

        for (auto &mi : ci.media)
        {
            if (mi.type == PJMEDIA_TYPE_AUDIO && mi.status == PJSUA_CALL_MEDIA_ACTIVE)
            
            {
                pj::AudioMedia audioMedia = getAudioMedia(mi.index);

                // Get the virtual speaker index (replace this with the correct index)
                int virtual_speaker_index = 3;  // replace with the correct index for VB-Cable

                // Set the default playback device to the virtual speaker
                pj::Endpoint::instance().audDevManager().setPlaybackDev(virtual_speaker_index);

                // Create a reference to the virtual speaker media
                const pj::AudioMedia& virtualSpeakerMedia = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();

                // Transmit the call's audio media to the virtual speaker
                audioMedia.startTransmit(virtualSpeakerMedia);

                // Transmit from the virtual microphone to the call's audio media
                virtualSpeakerMedia.startTransmit(audioMedia);
            }
        }
    }
        }
    }


