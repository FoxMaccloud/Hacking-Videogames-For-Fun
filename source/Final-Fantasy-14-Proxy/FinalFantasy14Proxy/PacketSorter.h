#pragma once
#include "HelperFunctions.h"


/**
* 
* All these structs are just from the Sapphire emulator.
* 
*
* The actuall packet sorting is still to be made.
* The header seems to be correct, but there are some weird stuff going on and bytes not ligning up propperly from what I could understand.
**/



struct FFXIVARR_PACKET_HEADER
{
    /**
    * Structure representing the common header for all FFXIVARR packets.
    *
    * 0               4               8              12      14      16
    * +-------------------------------+---------------+-------+-------+
    * | unknown_0                     | unknown_8                     |
    * +-------------------------------+---------------+-------+-------+
    * | timestamp                     | size          | cType | count |
    * +---+---+-------+---------------+---------------+-------+-------+
    * | ? |CMP|   ?   | ?             |
    * +---+---+-------+---------------+
    * (followed by /count/ FFXIVARR_PACKET_SEGMENTs)    
    */
    /** Unknown data, no actual use has been determined */
    uint64_t unknown_0;
    uint64_t unknown_8;
    /** Represents the number of milliseconds since epoch that the packet was sent. */
    uint64_t timestamp;
    /** The size of the packet header and its payload */
    uint32_t size;
    /** The type of this connection - 1 zone, 2 chat*/
    uint16_t connectionType;
    /** The number of packet segments that follow. */
    uint16_t count;
    uint8_t unknown_20;
    /** Indicates if the data segments of this packet are compressed. */
    uint8_t isCompressed;
    uint32_t unknown_24;
};

//struct FFXIVARR_PACKET_SEGMENT_HEADER
//{
//    /** The size of the segment header and its data. */
//    uint32_t size;
//    /** The session ID this segment describes. */
//    uint32_t source_actor;
//    /** The session ID this packet is being delivered to. */
//    uint32_t target_actor;
//    /** The segment type. (1, 2, 3, 7, 8, 9, 10) */
//    uint16_t type;
//    uint16_t padding;
//};
//
//enum packets {
//    unknown,
//    stance
//};
//
//packets sort(const char* buffer, int size)
//{
//    if (size < sizeof(FFXIVARR_PACKET_HEADER))
//        return;
//
//    if (buffer[sizeof(FFXIVARR_PACKET_HEADER)] == 0x000000) // some stance signature
//        return stance;
//    
//    // 126 bytes until 4 byte counter?  
//}