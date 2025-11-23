#include "tools/SystemTools.hpp"
#include "providers/CanardInterface.hpp"
#include "Config.hpp"
#include "Logging.hpp"

/*
  in this example we will use dynamic node allocation if MY_NODE_ID is zero
 */
#define MY_NODE_ID 22

/*
Transmits all frames from the TX queue, receives up to one frame.
*/
void CanardInterface::process(uint32_t timeout_msec) {
    // Transmitting
    for (const CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;) {
        const int16_t tx_res = socketcanTransmit(&socketcan, txf, 0);
        if (tx_res != 0) {
            canardPopTxQueue(&canard);
        }
    }

    // Receiving
    const uint32_t start_ms = SystemTools::millis32();
    while (SystemTools::millis32() - start_ms < timeout_msec) {
        CanardCANFrame rx_frame;
        const int16_t rx_res = socketcanReceive(&socketcan, &rx_frame, timeout_msec);
        if (rx_res > 0) {
            int16_t ret = canardHandleRxFrame(&canard, &rx_frame, SystemTools::micros64());
            if (ret < 0) {
                snowerror("canardHandleRxFrame failed: %d\n", ret);
            }
        }
    }
}
/*
handle an incoming message
*/
void CanardInterface::onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
    CanardInterface* iface = (CanardInterface*) ins->user_reference;
    iface->handle_message(*transfer);
}

/*
check if we want the message. This is based on what we have subscribed to
*/
bool CanardInterface::shouldAcceptTransfer(const CanardInstance* ins,
                                uint64_t* out_data_type_signature,
                                uint16_t data_type_id,
                                CanardTransferType transfer_type,
                                uint8_t source_node_id)
{
    CanardInterface* iface = (CanardInterface*)ins->user_reference;
    return iface->accept_message(data_type_id, transfer_type, *out_data_type_signature);
}

/*
  Initializing the Libcanard instance.
*/
void CanardInterface::init(const char *interface_name)
{
    int16_t res = socketcanInit(&socketcan, interface_name);
    if (res < 0) {
        (void)fprintf(stderr, "Failed to open CAN iface '%s'\n", interface_name);
        exit(1);
    }

    // init canard object
    canardInit(&canard,
               memory_pool,
               sizeof(memory_pool),
               onTransferReceived,
               shouldAcceptTransfer,
               this);

    // set node ID if not doing DNA
    if (MY_NODE_ID > 0) {
        canardSetLocalNodeID(&canard, MY_NODE_ID);
    } else {
        printf("Waiting for DNA node ID allocation\n");
    }
}

bool CanardInterface::broadcast(const Canard::Transfer &bcast_transfer) {
    tx_transfer = {
        .transfer_type = bcast_transfer.transfer_type,
        .data_type_signature = bcast_transfer.data_type_signature,
        .data_type_id = bcast_transfer.data_type_id,
        .inout_transfer_id = bcast_transfer.inout_transfer_id,
        .priority = bcast_transfer.priority,
        .payload = (const uint8_t*)bcast_transfer.payload,
        .payload_len = uint16_t(bcast_transfer.payload_len),
#if CANARD_ENABLE_CANFD
        .canfd = bcast_transfer.canfd,
#endif
#if CANARD_ENABLE_DEADLINE
        .deadline_usec = micros64() + (bcast_transfer.timeout_ms * 1000),
#endif
#if CANARD_MULTI_IFACE
        .iface_mask = uint8_t((1<<num_ifaces) - 1),
#endif
    };
    // do canard broadcast
    bool success = canardBroadcastObj(&canard, &tx_transfer) > 0;
    return success;
}

bool CanardInterface::request(uint8_t destination_node_id, const Canard::Transfer &req_transfer) {
    printf("Requesting node %d\n", destination_node_id);
    tx_transfer = {
        .transfer_type = req_transfer.transfer_type,
        .data_type_signature = req_transfer.data_type_signature,
        .data_type_id = req_transfer.data_type_id,
        .inout_transfer_id = req_transfer.inout_transfer_id,
        .priority = req_transfer.priority,
        .payload = (const uint8_t*)req_transfer.payload,
        .payload_len = uint16_t(req_transfer.payload_len),
#if CANARD_ENABLE_CANFD
        .canfd = req_transfer.canfd,
#endif
#if CANARD_ENABLE_DEADLINE
        .deadline_usec = micros64() + (req_transfer.timeout_ms * 1000),
#endif
#if CANARD_MULTI_IFACE
        .iface_mask = uint8_t((1<<num_ifaces) - 1),
#endif
    };
    // do canard request
    return canardRequestOrRespondObj(&canard, destination_node_id, &tx_transfer) > 0;
}

bool CanardInterface::respond(uint8_t destination_node_id, const Canard::Transfer &res_transfer) {
    printf("Responding to node %d\n", destination_node_id);
    tx_transfer = {
        .transfer_type = res_transfer.transfer_type,
        .data_type_signature = res_transfer.data_type_signature,
        .data_type_id = res_transfer.data_type_id,
        .inout_transfer_id = res_transfer.inout_transfer_id,
        .priority = res_transfer.priority,
        .payload = (const uint8_t*)res_transfer.payload,
        .payload_len = uint16_t(res_transfer.payload_len),
#if CANARD_ENABLE_CANFD
        .canfd = res_transfer.canfd,
#endif
#if CANARD_ENABLE_DEADLINE
        .deadline_usec = micros64() + (res_transfer.timeout_ms * 1000),
#endif
#if CANARD_MULTI_IFACE
        .iface_mask = uint8_t((1<<num_ifaces) - 1),
#endif
    };
    // do canard respond
    return canardRequestOrRespondObj(&canard, destination_node_id, &tx_transfer) > 0;
}