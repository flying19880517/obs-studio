#pragma once

#include <QObject>
#include "IAgoraRtcEngine.h"

class XimalayaAgoraRtcEngineEventHandler : public agora::rtc::IRtcEngineEventHandler
{
public:
    XimalayaAgoraRtcEngineEventHandler();

    // IRtcEngineEventHandler interface
public:
   /* virtual void onJoinChannelSuccess(const char *channel, agora::rtc::uid_t uid, int elapsed);
    virtual void onRejoinChannelSuccess(const char *channel, agora::rtc::uid_t uid, int elapsed);
    virtual void onWarning(int warn, const char *msg);
    virtual void onError(int err, const char *msg);
    virtual void onAudioQuality(agora::rtc::uid_t uid, int quality, unsigned short delay, unsigned short lost);
    virtual void onAudioVolumeIndication(const agora::rtc::AudioVolumeInfo *speakers, unsigned int speakerNumber, int totalVolume);
    virtual void onLeaveChannel(const agora::rtc::RtcStats &stats);
    virtual void onRtcStats(const agora::rtc::RtcStats &stats);
    virtual void onAudioDeviceStateChanged(const char *deviceId, int deviceType, int deviceState);
    virtual void onAudioMixingFinished();
    virtual void onVideoDeviceStateChanged(const char *deviceId, int deviceType, int deviceState);
    virtual void onNetworkQuality(agora::rtc::uid_t uid, int txQuality, int rxQuality);
    virtual void onLastmileQuality(int quality);
    virtual void onFirstLocalVideoFrame(int width, int height, int elapsed);
    virtual void onFirstRemoteVideoDecoded(agora::rtc::uid_t uid, int width, int height, int elapsed);
    virtual void onFirstRemoteVideoFrame(agora::rtc::uid_t uid, int width, int height, int elapsed);
    virtual void onUserJoined(agora::rtc::uid_t uid, int elapsed);
    virtual void onUserOffline(agora::rtc::uid_t uid, agora::rtc::USER_OFFLINE_REASON_TYPE reason);
    virtual void onUserMuteAudio(agora::rtc::uid_t uid, bool muted);
    virtual void onUserMuteVideo(agora::rtc::uid_t uid, bool muted);
    virtual void onUserEnableVideo(agora::rtc::uid_t uid, bool enabled);
    virtual void onApiCallExecuted(const char *api, int error);
    virtual void onLocalVideoStats(const agora::rtc::LocalVideoStats &stats);
    virtual void onRemoteVideoStats(const agora::rtc::RemoteVideoStats &stats);
    virtual void onCameraReady();
    virtual void onVideoStopped();
    virtual void onConnectionLost();
    virtual void onConnectionInterrupted();
    virtual void onRefreshRecordingServiceStatus(int status);
    virtual void onStreamMessage(agora::rtc::uid_t uid, int streamId, const char *data, size_t length);
    virtual void onStreamMessageError(agora::rtc::uid_t uid, int streamId, int code, int missed, int cached);
    virtual void onMediaEngineLoadSuccess();
    virtual void onMediaEngineStartCallSuccess();
    virtual void onRequestChannelKey();*/
};
