//
//  main.cpp
//  NetworkTryTwo
//
//  Created by Michael Herman on 10/10/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
// Client

#include <iostream>
#include <ncurses.h>
#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakPeer.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"




#define SERVER_PORT 7001
#define CLIENT_PORT 9001

using namespace RakNet;
using namespace std;

//INIT VAR
unsigned char playerOneGlobalX;
unsigned char playerOneGlobalY;
unsigned char playerTwoGlobalX;
unsigned char playerTwoGlobalY;
unsigned char packetIdentifier;
SystemAddress serverID;
char ipAddr[15];

struct __attribute__((packed)) message{
    unsigned char typeId;
    unsigned char posX;
    unsigned char posY;
}clientMessage;


struct __attribute__((packed)) message2{
    unsigned char typeId;
    unsigned char info[4];
};


//Functions Prototype
void IntroText();
void Render();
void initVar();
unsigned char GetPacketIdentifier(RakNet::Packet *p);


int main(int argc, const char * argv[]) {
    initVar();
    initscr();
    keypad(stdscr, TRUE);
    RakNet::Packet* p;
    
    /*Setup Client - Server Connection*/
    RakNet::RakPeerInterface *client=RakNet::RakPeerInterface::GetInstance();
    addstr("ENTER IP TO CONNECT : ");
    refresh();
    getnstr(ipAddr, sizeof(ipAddr));
    printw("%s",ipAddr);
    refresh();
    clear();
    
    
    RakNet::SocketDescriptor socket;
    socket.port = CLIENT_PORT;
    strcpy(socket.hostAddress, ipAddr);
    
    bool isClientStarted = (client->Startup(1, &socket, 1)==RAKNET_STARTED);
    if(!isClientStarted)
    {
        isClientStarted = client->Startup(1, &socket, 1)==RAKNET_STARTED;
        if(!isClientStarted)printf("Connection Fail");
    }
    
    printw("Client Started");
    refresh();
    ConnectionAttemptResult attemptResult = client->Connect(ipAddr, SERVER_PORT, 0, 0);
    
    if(attemptResult == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        printw( "Connect Attempt Succesful");refresh();
    }
    else printw("Connect Attempt fail");refresh();
    
    /*Print IP address used by the raknet*/
    for (int i=0; i < client->GetNumberOfAddresses(); i++)
    {
        printw("%i. %s\n", i+1, client->GetLocalIP(i));refresh();
    }
    
    //Setup server target address
    serverID.FromString(ipAddr);
    serverID.SetPortHostOrder(SERVER_PORT);
    
    {
        printw("Server Address : %s\n",serverID.ToString());refresh();
        printw("Client GUID : %s\n",client->GetGuidFromSystemAddress(serverID).ToString());refresh();
        printw("Client Address : %s\n",client->GetSystemAddressFromGuid(client->GetMyGUID()).ToString());refresh();
    }
    
    
    IntroText();
    refresh();
    clientMessage.typeId = 100;
    
    
    int ch = getch();
    clear();
    
    while(1)
    {
        
        for(p=client->Receive(); p; client->DeallocatePacket(p), p=client->Receive())
        {
            // We got a packet, get the identifier with our handy function
            packetIdentifier = GetPacketIdentifier(p);
            
            // Check if this is a network message packet
            switch (packetIdentifier)
            {
                {
                case 101:
                    Render();
                    printw("BroadCast from Server !\n");
                    struct message2 *serverMessage = (struct message2*) p->data;
                    playerOneGlobalX = serverMessage->info[0];
                    playerOneGlobalY = serverMessage->info[1];
                    playerTwoGlobalX = serverMessage->info[2];
                    playerTwoGlobalY = serverMessage->info[3];
                    
                    printw("info 3: %i, info 4 :%i\n", playerTwoGlobalX,playerTwoGlobalY);
                    printw("%i, %i, %i, %i",serverMessage->info[0],serverMessage->info[1],serverMessage->info[2],serverMessage->info[3]);
                    refresh();
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
                    printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
                    printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
                    break;
                case ID_CONNECTED_PING:
                case ID_UNCONNECTED_PING:
                    printf("Ping from %s\n", p->systemAddress.ToString(true));
                    break;
                default:
                    // It's a client, so just show the message
                    printw("%s\n", p->data);
                    //refresh();
                    break;
            }
        }
        
        
        char c  = getch();
        clear();
        
        switch (ch)
        {
            case KEY_DOWN:
                clientMessage.posY++;
                printw("%s\n","KEY_UP");
                //printw("Pos X :%i , Pos Y : %i\n",playerOneGlobalX, playerOneGlobalY);
                printw("Pos X :%i , Pos Y : %i\n",clientMessage.posX, clientMessage.posY);
                refresh();
                break;
            case KEY_RIGHT:
                printw("%s\n","KEY_RIGHT");
                clientMessage.posX++;
                //printw("Pos X :%i , Pos Y : %i\n",playerOneGlobalX, playerOneGlobalY);
                printw("Pos X :%i , Pos Y : %i\n",clientMessage.posX, clientMessage.posY);
                refresh();
                break;
            default:
                printw("nothing");
                refresh();
                break;
        }
        void* ptr = (void*)&clientMessage;
        char *text = (char*)ptr;
        client->Send(text, (const int)sizeof(text), HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverID, false);
        //RakSleep(10);
        }
        
    
    client->Shutdown(0);
    return 0;
}
void initVar()
{
    playerOneGlobalX = 0;
    playerOneGlobalY = 0;
    playerTwoGlobalX = 0;
    playerTwoGlobalY = 0;
    clientMessage.posX = 0;
    clientMessage.posY = 0;
    
    //Id for client packet is 100
    clientMessage.typeId = 100;
}
void Render()
{
    for(int i = 0; i<= 20; i++)
    {
        for(int j = 0; j<=30; j++)
        {
            if((i == playerOneGlobalY)&&(j==playerOneGlobalX))
            {
                printw("%s",">");
            }else{
                printw("%s","_");
            }
            
            if((i == playerTwoGlobalY)&&(j==playerTwoGlobalX))
            {
                printw("%s","<");
            }else{
                printw("%s","_");
            }
        }
        printw("\n");
    }
    refresh();
}

void IntroText()
{
    printw("%s\n","      /$$$$$$  /$$   /$$  /$$$$$$  /$$   /$$ /$$$$$$$$");
    printw("%s\n","     /$$__  $$| $$$ | $$ /$$__  $$| $$  /$$/| $$_____/");
    printw("%s\n","    | $$  \__/| $$$$| $$| $$  \ $$| $$ /$$/ | $$      ");
    printw("%s\n","    |  $$$$$$ | $$ $$ $$| $$$$$$$$| $$$$$/  | $$$$$   ");
    printw("%s\n","     \____  $$| $$  $$$$| $$__  $$| $$  $$  | $$__/   ");
    printw("%s\n","     /$$  \ $$| $$\  $$$| $$  | $$| $$\  $$ | $$      ");
    printw("%s\n","    |  $$$$$$/| $$ \  $$| $$  | $$| $$ \  $$| $$$$$$$$");
    printw("%s\n","     \______/ |__/  \__/|__/  |__/|__/  \__/|________/");
}

unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
    if (p==0)
        return 255;
    
    if ((unsigned char)p->data[0] == ID_TIMESTAMP){
        return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
    }else{
        return (unsigned char) p->data[0];
    }
}
