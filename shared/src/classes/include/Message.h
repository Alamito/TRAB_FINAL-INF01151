#ifndef MESSAGE__H
#define MESSAGE__H

#include <map>
#include <string>
#include <array>
#include <Socket.h>

constexpr std::array<const char*, 3> possibleTypes = {"sum", "ack", "discover"};

class Message {
    private:
        int numberToSum;
        int id;
        int type;
        // std::map<std::string, int> destination;  // dict with the destination ip and port
        // std::map<std::string, int> sender;       // dict with the sender ip and port

        Socket recipient;
        Socket sender;

    public:
        Message(int numberToSum, const std::string& recipientIp, int recipientPort, const std::string& senderIp, int senderPort);

        void send();
        void waitAck();

        //getters
        int getNumberToSum() const { return numberToSum; }
        int getSenderPort() const { return sender.getPort(); }  
        std::string getSenderIp() const { return sender.getIp(); }
        int getRecipientPort() const { return recipient.getPort(); }
        std::string getRecipientIp() const { return recipient.getIp(); }

};

#endif // MESSAGE__H
