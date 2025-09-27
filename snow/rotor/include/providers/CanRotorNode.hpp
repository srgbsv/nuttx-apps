#pragma once

// system includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

// include the base canard API
#include <canard.h>

// we are using the socketcan driver
#include <drivers/socketcan/socketcan.h>

// include the headers for the generated DroneCAN messages from the
// dronecan_dsdlc compiler
//TODO Write correct path
#include <dronecan_msgs.h>

#include "CanardInterface.hpp"
//#include "state/State.hpp"

#include <stdatomic.h>
#include <memory>

namespace Snow {

    class CanRotorNode  {

    public:

        void startNode(const char *interface_name);
        
    private:
        CanardInterface canard_iface{0};

        // declare publishers for outgoing messages
        Canard::Publisher<uavcan_protocol_NodeStatus> nodeStatus{canard_iface};
        Canard::Publisher<uavcan_equipment_esc_Status> escStatus{canard_iface};

        // incoming messages
        void handleRawCommand(const CanardRxTransfer& transfer, const uavcan_equipment_esc_RawCommand& cmd);
        Canard::ObjCallback<Snow::CanRotorNode, uavcan_equipment_esc_RawCommand> raw_command_cb{this, &handleRawCommand};
        Canard::Subscriber<uavcan_equipment_esc_RawCommand> raw_command_listener{raw_command_cb, 0};

        // Node Info Server
        void handleGetNodeInfo(const CanardRxTransfer& transfer, const uavcan_protocol_GetNodeInfoRequest& req);
        Canard::ObjCallback<CanRotorNode, uavcan_protocol_GetNodeInfoRequest> node_info_req_cb{this, &handleGetNodeInfo};
        Canard::Server<uavcan_protocol_GetNodeInfoRequest> node_info_server{canard_iface, node_info_req_cb};

        // parameter server
        void handleParamGetSet(const CanardRxTransfer& transfer, const uavcan_protocol_param_GetSetRequest& req);
        Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_GetSetRequest> param_get_set_req_cb{this, &handleParamGetSet};
        Canard::Server<uavcan_protocol_param_GetSetRequest> param_server{canard_iface, param_get_set_req_cb};
        void handleParamExecuteOpcode(const CanardRxTransfer& transfer, const uavcan_protocol_param_ExecuteOpcodeRequest& req);
        Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_ExecuteOpcodeRequest> param_executeopcode_req_cb{this, &handleParamExecuteOpcode};
        Canard::Server<uavcan_protocol_param_ExecuteOpcodeRequest> param_opcode_server{canard_iface, param_executeopcode_req_cb};


        // DNA request call
        void requestDNA();

        void sendNodeStatus(void);
        void process1HzTasks(uint64_t timestamp_usec);
        void sendRotorStatus(void);


        // keep node_status around for updating status
        uavcan_protocol_NodeStatus node_status_msg;

        /*
        data for dynamic node allocation process
        */
        struct {
            uint32_t send_next_node_id_allocation_request_at_ms;
            uint32_t node_id_allocation_unique_id_offset;
        } DNA;
    };

}