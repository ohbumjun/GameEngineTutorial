#pragma once
#include "Hazel/Core/Thread/Thread.h"
#include "Hazel/Core/Thread/ThreadUtil.h"
#include "Hazel/Core/ID/UUID.h"
#include "ServerInfo.h"
#include <Hazel.h>

static const int BUF_SIZE = 1024;

class OverlappedServerLayer : public Hazel::Layer
{
public:

    struct ClientInfo
    {
        SOCKET clientSock;
        // �ش� io �� �Ϸ�Ǹ� overlapped �� ��ϵ� event �� signaled ���°� �ȴ�.
        WSAOVERLAPPED overlapped;
        WSAEVENT evObj;
        Hazel::UUID id;
        WSABUF dataBuf;
        // ���ŵ� ������ ������ ����� ������ ������ ���� WSABUF ����ü
        char buf[BUF_SIZE];
        Hazel::Thread communicateThread;
        std::vector<std::string> sentMessage;
        std::vector<std::string> receivedMessage;
    };

    OverlappedServerLayer() : Layer("OverlappedServerLayer")
    {
    }
    ~OverlappedServerLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    void OnEvent(Hazel::Event &event) override;
    virtual void OnImGuiRender() override;
    void ImGuiChatWindow();

private:
    void initialize();
    void receiveMessage(Hazel::UUID threadId);
    void receiveConnection();

    static const int BUF_SIZE = 1024;

    WSADATA m_WsaData;
    int receiveLen = 0;
    SOCKET m_ReceiverSock;
    SOCKADDR_IN m_ServAddr;
    std::unordered_map<Hazel::UUID, ClientInfo*> m_ConnectedClients;
    Hazel::Thread m_AcceptThread;
    Hazel::SpinLock m_SpinLock;
};