#pragma once

// system includes
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#include <stdatomic.h>
#include <pthread.h>
#include <memory>
#include <sys/types.h> 


#include "Config.hpp"
#include "CanardInterface.hpp"
#include "state/State.hpp"


class CanRotorNode  {

public:

    static int taskSpawn(int argc, char** argv);
    //static int startNode(const char* iface_name, std::shared_ptr<State> state);
    static int startNode(const char* iface_name, std::shared_ptr<State> state);
    static void stopNode();

    void run();
    int init(const char * iface_name);
    
private:
    static pid_t _task_id;
    static bool  _should_exit;
    static std::unique_ptr<CanRotorNode> _instance;
    static std::shared_ptr<State> _state;

    CanardInterface _canard_iface{0};

    // declare publishers for outgoing messages
    Canard::Publisher<uavcan_protocol_NodeStatus> node_status{_canard_iface};

    /*void handleGetNodeStatus(const CanardRxTransfer& transfer, const uavcan_protocol_NodeStatus& cmd);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_NodeStatus> node_status_req_cb{this, &CanRotorNode::handleGetNodeStatus};
    Canard::Subscriber<uavcan_protocol_NodeStatus> node_status_listener{node_status_req_cb, 0};*/

    void handleActuatorListCommand(const CanardRxTransfer& transfer, const uavcan_equipment_actuator_ArrayCommand& cmd);
    Canard::ObjCallback<CanRotorNode, uavcan_equipment_actuator_ArrayCommand> actuator_list_req_cb{this, &CanRotorNode::handleActuatorListCommand};
    Canard::Subscriber<uavcan_equipment_actuator_ArrayCommand> actuator_list_listener{actuator_list_req_cb, 0};

    // incoming messages
    /*void handleRawCommand(const CanardRxTransfer& transfer, const uavcan_equipment_esc_RawCommand& cmd);
    Canard::ObjCallback<CanRotorNode, uavcan_equipment_esc_RawCommand> raw_command_cb{this, &CanRotorNode::handleRawCommand};
    Canard::Subscriber<uavcan_equipment_esc_RawCommand> raw_command_listener{raw_command_cb, 0};*/

    // Node Info Server
    void handleGetNodeInfo(const CanardRxTransfer& transfer, const uavcan_protocol_GetNodeInfoRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_GetNodeInfoRequest> node_info_req_cb{this, &CanRotorNode::handleGetNodeInfo};
    Canard::Server<uavcan_protocol_GetNodeInfoRequest> node_info_server{_canard_iface, node_info_req_cb};



    // parameter server
    /*void handleParamGetSet(const CanardRxTransfer& transfer, const uavcan_protocol_param_GetSetRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_GetSetRequest> param_get_set_req_cb{this, &CanRotorNode::handleParamGetSet};
    Canard::Server<uavcan_protocol_param_GetSetRequest> param_server{_canard_iface, param_get_set_req_cb};*/

    /*void handleParamExecuteOpcode(const CanardRxTransfer& transfer, const uavcan_protocol_param_ExecuteOpcodeRequest& req);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_param_ExecuteOpcodeRequest> param_executeopcode_req_cb{this, &CanRotorNode::handleParamExecuteOpcode};
    Canard::Server<uavcan_protocol_param_ExecuteOpcodeRequest> param_opcode_server{_canard_iface, param_executeopcode_req_cb};*/

    // handlers for dynamic node allocation (DNA)
    /*Canard::Publisher<uavcan_protocol_dynamic_node_id_Allocation> allocation_pub{_canard_iface};
    void handleDNAAllocation(const CanardRxTransfer& transfer, const uavcan_protocol_dynamic_node_id_Allocation& msg);
    Canard::ObjCallback<CanRotorNode, uavcan_protocol_dynamic_node_id_Allocation> allocation_cb{this, &CanRotorNode::handleDNAAllocation};
    Canard::Subscriber<uavcan_protocol_dynamic_node_id_Allocation> allocation_listener{allocation_cb, 0};*/


    // DNA request call
    void requestDNA();

    void sendNodeStatus(void);
    void process1HzTasks(uint64_t timestamp_usec);
    void sendRotorStatus(void);

    bool shouldExit() { return _should_exit; }

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