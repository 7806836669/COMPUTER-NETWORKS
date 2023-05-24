#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct ARP_Packet {
    unsigned short hardwareType;     
    unsigned short protocolType;     
    unsigned char hardwareLength;    
    unsigned char protocolLength;    
    unsigned short opcode;           
    unsigned char senderMAC[6];      
    unsigned char senderIP[4];       
    unsigned char targetMAC[6];      
    unsigned char targetIP[4];       
};

int main() {
    
    struct ARP_Packet arpPacket;

    arpPacket.hardwareType = htons(1);   
    arpPacket.protocolType = htons(0x0800);   
    arpPacket.hardwareLength = 6;   
    arpPacket.protocolLength = 4;   
    arpPacket.opcode = htons(1);    

    unsigned char senderMAC[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};   
    unsigned char senderIP[] = {192, 168, 0, 1};   
    memcpy(arpPacket.senderMAC, senderMAC, 6);
    memcpy(arpPacket.senderIP, senderIP, 4);

    unsigned char targetIP[] = {192, 168, 0, 2};   
    memcpy(arpPacket.targetIP, targetIP, 4);

    
    return 0;
}
