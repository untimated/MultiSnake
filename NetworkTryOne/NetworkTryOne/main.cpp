//
//  main.cpp
//  NetworkTryOne
//
//  Created by Michael Herman on 10/8/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//
#define SERVER_PORT 7001
#define SERVER_PORT2 7002


#include <iostream>
#include <vector>
#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakPeer.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"



using namespace RakNet;
using namespace std;


struct __attribute__((packed))message{
    unsigned char typeId;
    unsigned char posX;
    unsigned char posY;
}mStruct;


struct __attribute__((packed))message2{
    unsigned char typeId;
    unsigned char info[4];
}broadcastInfo;




unsigned char GetPacketIdentifier(Packet *p);
bool CheckClient(vector<SystemAddress> &arr,SystemAddress target);
void AddClient(vector<SystemAddress> &arr,SystemAddress target);

int main(int argc, const char * argv[]) {

    //INIT()
    unsigned char packetIdentifier;

    RakNet::RakPeerInterface *server=RakNet::RakPeerInterface::GetInstance();
    RakNet::Packet *packet;
    SocketDescriptor sdArray[1];
    
    
    sdArray[0].port = SERVER_PORT;
    sdArray[0].socketFamily =AF_INET;
    bool b = server->Startup(4, sdArray, 1)==RAKNET_STARTED;
    server->SetMaximumIncomingConnections(5);
    server->SetOccasionalPing(true);
    cout<<server->GetSystemAddressFromGuid(server->GetMyGUID()).ToString()<<endl;
    cout<<"GUID :: "<< server->GetMyGUID().ToString()<<endl;
    if(!b){
        puts("re-Attempt\n");
        b = server->Startup(4,sdArray,1)==RAKNET_STARTED;
        cout<<server->GetSystemAddressFromGuid(server->GetMyGUID()).ToString()<<endl;
        cout<<b;
        if(!b){
            puts("SERVER FAIL");
            exit(1);
        }else{
            puts("SERVER SUCCESS");
            //server->SetOccasionalPing(true);
        }
        
    }else{
        cout<<"Server Started"<<endl;

    }

    cout << "wait..." <<endl;

    while(1){
        RakSleep(10);

        for (packet=server->Receive(); packet; server->DeallocatePacket(packet), packet=server->Receive()) {
    
            packetIdentifier = GetPacketIdentifier(packet);

            switch (packetIdentifier)
            {
                {
                case 100:
                    //Client send Updates
                    
                    cout<<"Update from Client!"<<endl;
                    struct message* tmp = (struct message*) packet->data;
                    
                    if(packet->systemAddress.GetPort()==9000){
                        cout<<"port 9000 in"<<endl;
                        broadcastInfo.info[0] = tmp->posX;
                        broadcastInfo.info[1] = tmp->posY;
                    }
                    if(packet->systemAddress.GetPort()==9001){
                        cout<<"port 9001 in"<<endl;
                        broadcastInfo.info[2] = tmp->posX;
                        broadcastInfo.info[3] = tmp->posY;
                    }

                    
                    broadcastInfo.typeId = 101;
                    void* ptr = (void*)&broadcastInfo;
                    char *text = (char*)ptr;

                    server->Send(text, sizeof(text), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_RAKNET_GUID, true);
                    break;
                    
                }
                case ID_DISCONNECTION_NOTIFICATION:
                    // Connection lost normally
                    printf("ID_DISCONNECTION_NOTIFICATION\n");
                    break;
                case ID_ALREADY_CONNECTED:
                    // Connection lost normally
                    //printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid);
                    break;
                case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                    printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
                    break;
                case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
                    printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
                    break;
                case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
                    printf("ID_REMOTE_CONNECTION_LOST\n");
                    break;
                case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
                    printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
                    break;
                case ID_CONNECTION_BANNED: // Banned from this server
                    printf("We are banned from this server.\n");
                    break;
                case ID_CONNECTION_ATTEMPT_FAILED:
                    printf("Connection attempt failed\n");
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    // Sorry, the server is full.  I don't do anything here but
                    // A real app should tell the user
                    printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
                    break;
                    
                case ID_INVALID_PASSWORD:
                    printf("ID_INVALID_PASSWORD\n");
                    break;
                    
                case ID_CONNECTION_LOST:
                    // Couldn't deliver a reliable packet - i.e. the other system was abnormally
                    // terminated
                    printf("ID_CONNECTION_LOST\n");
                    break;
                    
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    // This tells the client they have connected
                    printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", packet->systemAddress.ToString(true), packet->guid.ToString());
                    printf("My external address is %s\n", server->GetExternalID(packet->systemAddress).ToString(true));
                    break;
                case ID_CONNECTED_PING:
                case ID_UNCONNECTED_PING:
                    printf("Ping from %s\n", packet->systemAddress.ToString(true));
                    break;

                    
                default:
                    // It's a client, so just show the message
                    printf("%s\n", packet->data);
                    break;
            }
        }
    }

    return 0;

}

unsigned char GetPacketIdentifier(Packet *p)
{
    
    if ((unsigned char)p->data[0] == ID_TIMESTAMP)
        return (unsigned char) p->data[sizeof(MessageID) + sizeof(RakNet::Time)];
    else
        return (unsigned char) p->data[0];
}

//return true if user is already Exist
//return false if user is not exist
bool CheckClient(vector<SystemAddress> &arr, SystemAddress target)
{
    int j;
    for(j=0; j<3; j++)
    {
        if(arr.at(j) == target)
        {
            return true;
        }
    }
    return false;
}

void AddClient(vector<SystemAddress> &arr, SystemAddress target)
{
    int i;
    arr.push_back(target);
}
