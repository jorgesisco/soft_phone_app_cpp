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
                pj::AudioMedia audioMedia = getAudioMedia(mi.index);
                

                // Create a reference to playback device media
                const pj::AudioMedia& playbackMedia = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();

                // Transmit the call's audio media to the capture device (for you to hear the caller)
                audioMedia.startTransmit(playbackMedia);

                // Transmit from the local machine's microphone to the call's audio media
                pj::Endpoint::instance().audDevManager().getCaptureDevMedia().startTransmit(audioMedia);
            }
        }
    }

// void MyCall::onCallMediaState(pj::OnCallMediaStateParam &prm)
// {
//         pj::CallInfo ci = getInfo();

//         for (auto &mi : ci.media)
//         {
//             if (mi.type == PJMEDIA_TYPE_AUDIO && mi.status == PJSUA_CALL_MEDIA_ACTIVE)
//             {
//                 pj::AudioMedia audioMedia = getAudioMedia(mi.index);
                

//                 // Create a reference to playback device media
//                 const pj::AudioMedia& playbackMedia = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();

//                 // Transmit the call's audio media to the capture device (for you to hear the caller)
//                 audioMedia.startTransmit(playbackMedia);

//                 // Transmit from the local machine's microphone to the call's audio media
//                 pj::Endpoint::instance().audDevManager().getCaptureDevMedia().startTransmit(audioMedia);
//             }
//         }
//     }


