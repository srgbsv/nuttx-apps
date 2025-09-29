#pragma once

// system includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

// include the canard C++ APIs
#include <canard/publisher.h>
#include <canard/subscriber.h>
#include <canard/service_client.h>
#include <canard/service_server.h>
#include <canard/handler_list.h>
#include <canard/transfer_object.h>

// include the base canard API
#include <canard.h>

// we are using the socketcan driver
#include <drivers/socketcan/socketcan.h>

// include the headers for the generated DroneCAN messages from the
// dronecan_dsdlc compiler

#include <dronecan_msgs.h>

class CanardInterface : public Canard::Interface 
{
    
public:
    CanardInterface(uint8_t iface_index) :
        Interface(iface_index) {}
        
    void init(const char *interface_name);

    // implement required interface functions
    bool broadcast(const Canard::Transfer &bcast_transfer) override;
    bool request(uint8_t destination_node_id, const Canard::Transfer &req_transfer) override;
    bool respond(uint8_t destination_node_id, const Canard::Transfer &res_transfer) override;

    void process(uint32_t duration_ms);

    static void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
    static bool shouldAcceptTransfer(const CanardInstance* ins,
                                    uint64_t* out_data_type_signature,
                                    uint16_t data_type_id,
                                    CanardTransferType transfer_type,
                                    uint8_t source_node_id);

    uint8_t get_node_id() const override { return canard.node_id; }
    void setNodeId(uint8_t node_id) {
        canardSetLocalNodeID(&canard, node_id);
    }

private:
    uint8_t memory_pool[2048];
    CanardInstance canard;
    CanardTxTransfer tx_transfer;

    // we will use socketcan driver for this example
    SocketCANInstance socketcan;

};
