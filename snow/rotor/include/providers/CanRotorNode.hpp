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

class CanRotorNode  {

public:

    static int taskSpawn(const char *interface_name);
    
private:
    CanardInterface _canardIface{0};
    static std::unique_ptr<CanRotorNode> _instance;
    int _task_id{0};

    // declare publishers for outgoing messages
    Canard::Publisher<uavcan_protocol_NodeStatus> nodeStatus{_canardIface};
    Canard::Publisher<uavcan_equipment_esc_Status> escStatus{_canardIface};

    // incoming messages
    void handleRawCommand(const CanardRxTransfer& transfer, const uavcan_equipment_esc_RawCommand& cmd);
    Canard::ObjCallback<CanRotorNode, uavcan_equipment_esc_RawCommand> raw_command_cb{this, &CanRotorNode::handleRawCommand};
    Canard::Subscriber<uavcan_equipment_esc_RawCommand> raw_command_listener{raw_command_cb, 0};

    // Node Info Server
    void handleGetNodeInfo(const CanardRxTransfer& transfer, const uavcan_protocol_GetNodeInfoRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_GetNodeInfoRequest> node_info_req_cb{this, &CanRotorNode::handleGetNodeInfo};
    Canard::Server<uavcan_protocol_GetNodeInfoRequest> node_info_server{_canardIface, node_info_req_cb};

    // parameter server
    void handleParamGetSet(const CanardRxTransfer& transfer, const uavcan_protocol_param_GetSetRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_GetSetRequest> param_get_set_req_cb{this, &CanRotorNode::handleParamGetSet};
    Canard::Server<uavcan_protocol_param_GetSetRequest> param_server{_canardIface, param_get_set_req_cb};

    void handleParamExecuteOpcode(const CanardRxTransfer& transfer, const uavcan_protocol_param_ExecuteOpcodeRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_ExecuteOpcodeRequest> param_executeopcode_req_cb{this, &CanRotorNode::handleParamExecuteOpcode};
    Canard::Server<uavcan_protocol_param_ExecuteOpcodeRequest> param_opcode_server{_canardIface, param_executeopcode_req_cb};

    // handlers for dynamic node allocation (DNA)
    Canard::Publisher<uavcan_protocol_dynamic_node_id_Allocation> allocation_pub{_canardIface};
    void handleDNAAllocation(const CanardRxTransfer& transfer, const uavcan_protocol_dynamic_node_id_Allocation& msg);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_dynamic_node_id_Allocation> allocation_cb{this, &CanRotorNode::handleDNAAllocation};
    Canard::Subscriber<uavcan_protocol_dynamic_node_id_Allocation> allocation_listener{allocation_cb, 0};


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