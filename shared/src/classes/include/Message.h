#ifndef MESSAGE__H
#define MESSAGE__H

#include <map>
#include <string>
#include <array>
#include <Socket.h>

constexpr std::array<const char*, 3> possibleTypes = {"sum", "ack", "discover"};

struct ServerResponse {
    char serverIp[INET_ADDRSTRLEN];
    double totalSum;
};

class Message {
    private:
        int numberToSum;
        int id;
        int type;

        Socket recipient;
        Socket sender;

    public:
        Message(int numberToSum, const std::string& recipientIp, int recipientPort, const std::string& senderIp, int senderPort);

        void send();
        ServerResponse waitAck();  // Alteração aqui

        //getters
        int getNumberToSum() const { return numberToSum; }
        int getSenderPort() const { return sender.getPort(); }  
        std::string getSenderIp() const { return sender.getIp(); }
        int getRecipientPort() const { return recipient.getPort(); }
        std::string getRecipientIp() const { return recipient.getIp(); }

};

#endif // MESSAGE__H