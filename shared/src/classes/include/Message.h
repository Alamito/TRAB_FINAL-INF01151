
#ifndef MESSAGE__H
#define MESSAGE__H

#include <map>
#include <string>

constexpr std::array<const char*, 3> possibleTypes = {"sum", "ack", "discover"};

class Message {
    private:
        int numberToSum;
        int id;
        
        int type;
        std::map<std::string, int> destination;  // dict with the destination ip and port
        std::map<std::string, int> sender;  // dict with the destination ip and port

    public:
        Message(int destinationIp, int destinationPort, int senderIp, int senderPort) {
            destination["ip"] = destIp;
            destination["port"] = destPort;

            sender["ip"] = senderIp;
            sender["port"] = senderPort;
        }
        void send(int port, std::string ip);
        void waitAck();

        std::map<std::string, int> getDestination() const { return destination; }
        std::map<std::string, int> getSender() const { return sender; }
};