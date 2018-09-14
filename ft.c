/*******************************************************************************
 * @author      Sean Collins
 * Contact:     sgc29@zips.uakron.edu
 * @date        July 17, 2018
 ******************************************************************************/
#include "ft.h"
#include "ft.parser.h"
#include "ft.crc.h"
#include "Convert.h"
#include <stdlib.h>

/*----------------------------- Local Variables ------------------------------*/

static Data_t   s_array [ARRAY_SZ];
static bool     s_flags [ARRAY_SZ];
static void*    s_permissions [ARRAY_SZ];
static uint8_t  s_packet [MAX_PCKT_SZ];
static uint8_t  s_packetIndex = 0;

/*----------------------------------- Port -----------------------------------*/

struct PortRepresentation
{
    PutFunc put;
    GetFunc get;
    EmptyFunc empty;
    Buffer * buffer;
};

Port * FT_NewPort(PutFunc put, GetFunc get, EmptyFunc empty) {
    // Allocate heap storage for port
    Port * port = (Port *)malloc(sizeof(Port));

    // Assign function pointers for rx/tx serial buffer access
    port->put = put;
    port->get = get;
    port->empty = empty;

    // allocate ring buffer for parsing
    port->buffer = createBuffer(MAX_PCKT_SZ + 6);
    return port;
}

void FT_DestroyPort(Port *port) {
    destroyBuffer(port->buffer);
    free(port);
}

/*------------------------------ Initialization ------------------------------*/

void FT_Initialize () {
    // initialize every value in array to zero
    for (int i = 0; i < ARRAY_SZ; ++i) {
        s_array[i] = 0;
        s_flags[i] = false;
        s_permissions[i] = NULL;
    }
}

/*------------------------ Accessing the Local Array -------------------------*/

Data_t FT_ReadArray(Index_t index) {
    s_flags[index] = false;
    return s_array[index];
}

bool FT_ReadFlag(Index_t index) {
    return s_flags[index];
}

/*---------------------------- Receiving Packets -----------------------------*/


/// See "Parsing Helping Functions" section below for definitions
void getBytesFromUART (Port *);

/**
 * @brief   Transfers contents of a receive buffer to the given port's ring
 *          buffer, then scans this buffer for valid packets. Transfers data
 *          from all valid packets to the local FastTransfer array.
 */
void FT_ScanPort(Port *port) {
    getBytesFromUART(port);
    parse(port->buffer, s_array); // TODO: Pass the array
}

void FT_LimitAccess (Index_t index, Port * port) {
    if (index < ARRAY_SZ)
        s_permissions[index] = (void *)port;
}

/*------------------------- Parsing Helper Functions -------------------------*/

/**
 * @brief   Transfers all bytes out of the receive buffer and into the port's
 *          ring buffer.
 */
void getBytesFromUART (Port * port) {
    Buffer * buffer = port->buffer;
    while (!port->empty() && !isFull(buffer))
        push(buffer, port->get());
}

/*--------------------------- Transmitting Packets ---------------------------*/

void FT_ToSend(Index_t index, Data_t data) {
    s_packet[s_packetIndex++] = getMsbFromUnsigned(index);
    s_packet[s_packetIndex++] = getLsbFromUnsigned(index);
    s_packet[s_packetIndex++] = getMsbFromSigned(data);
    s_packet[s_packetIndex++] = getLsbFromSigned(data);
}

void FT_ClearPacket() {
    s_packetIndex = 0;
}

void FT_Send(Port *port, uint8_t address) {
    port->put(0x06);
    port->put(0x85);
    port->put(LOCAL_ADDR);
    port->put(address);
    port->put(s_packetIndex);
    for (int i = 0; i < s_packetIndex; ++i)
        port->put(s_packet[i]);
    port->put(crc(s_packet, s_packetIndex));
    FT_ClearPacket();
}
