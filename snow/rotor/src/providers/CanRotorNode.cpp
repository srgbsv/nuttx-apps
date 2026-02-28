#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>

#include "providers/CanRotorNode.hpp"
#include "tools/SystemTools.hpp"
#include "Logging.hpp"

/*
  declare heads of handler and transfer lists
 */
DEFINE_HANDLER_LIST_HEADS();
DEFINE_TRANSFER_OBJECT_HEADS();

// convenience macros
#define MIN(a,b) ((a)<(b)?(a):(b))
#define C_TO_KELVIN(temp) (temp + 273.15f)
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


pid_t CanRotorNode::_task_id = -1;
bool CanRotorNode::_should_exit = false;
std::unique_ptr<CanRotorNode> CanRotorNode::_instance;
std::shared_ptr<State> CanRotorNode::_state;

/*
handle a GetNodeInfo request
*/
void CanRotorNode::handleGetNodeInfo(const CanardRxTransfer& transfer, const uavcan_protocol_GetNodeInfoRequest& req)
{
    snowinfo("Handled GetNodeInfo request\n");
    uavcan_protocol_GetNodeInfoResponse node_info_rsp {};

    // fill in node name
    node_info_rsp.name.len = snprintf((char*)node_info_rsp.name.data, sizeof(node_info_rsp.name.data), "CanRotorNode");

    // fill in software and hardware versions
    // TODO: get real versions
    node_info_rsp.software_version.major = 0;
    node_info_rsp.software_version.minor = 1;
    node_info_rsp.hardware_version.major = 0;
    node_info_rsp.hardware_version.minor = 1;
    SystemTools::getUniqueID(node_info_rsp.hardware_version.unique_id);
    node_info_rsp.status = node_status_msg;
    node_info_rsp.status.uptime_sec = SystemTools::millis32() / 1000UL;

    node_info_server.respond(transfer, node_info_rsp);
}

/*
handle a ESC RawCommand request
*/
/*void CanRotorNode::handleRawCommand(const CanardRxTransfer& transfer, const uavcan_equipment_esc_RawCommand& cmd)
{
    printf("Handled RAW command\n");
}*/

/*
handle a Actuators RAWCommand
*/
void CanRotorNode::handleActuatorListCommand(const CanardRxTransfer& transfer, const uavcan_equipment_actuator_ArrayCommand& cmd)
{
    //snowinfo("Handle actuator list command\n");
    for (size_t i = 0; i < cmd.commands.len; i++) {
        int index = cmd.commands.data[i].actuator_id;
        float value = cmd.commands.data[i].command_value;
        //snowinfo("Actuator %d command: %f\n", index, value);
        _state->lock();
        _state->setActuatorValue(index, (int) (value * 1000.0f)); // scale to int and 
        _state->unlock();
    }
}

/*
handle parameter GetSet request
*/
/*void CanRotorNode::handleParamGetSet(const CanardRxTransfer& transfer, const uavcan_protocol_param_GetSetRequest& req)
{
    printf("Handle param GetSet\n");
    return;
}*/

/*
handle parameter execute opcode request
*/
/*void CanRotorNode::handleParamExecuteOpcode(const CanardRxTransfer& transfer, const uavcan_protocol_param_ExecuteOpcodeRequest& req)
{
    printf("Handle param ExecuteOpcode\n");
    if (req.opcode == UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQUEST_OPCODE_ERASE) {
        // here is where you would reset all parameters to defaults
    }
    if (req.opcode == UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_REQUEST_OPCODE_SAVE) {
        // here is where you would save all the changed parameters to permanent storage
    }

    uavcan_protocol_param_ExecuteOpcodeResponse pkt {};
    pkt.ok = true;
    param_opcode_server.respond(transfer, pkt);
}*/

/*
send the 1Hz NodeStatus message. This is what allows a node to show
up in the DroneCAN GUI tool and in the flight controller logs
*/
void CanRotorNode::sendNodeStatus(void)
{
    snowinfo("Send node status\n");

    node_status_msg.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    node_status_msg.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    node_status_msg.sub_mode = 0;
    node_status_msg.uptime_sec = SystemTools::millis32() / 1000UL;

    node_status.broadcast(node_status_msg);
}

/*
This function is called at 1 Hz rate from the main loop.
*/
void CanRotorNode::process1HzTasks(uint64_t timestamp_usec)
{
    /*
    Transmit the node status message
    */
    sendNodeStatus();
}

void CanRotorNode::sendRotorStatus(void) {

}

int CanRotorNode::startNode(const char* iface_name, std::shared_ptr<State> state)
{
    if (_task_id != -1) {
        snowerror("CanNode already started. Task Id: %d", _task_id);
        return -1;
    }
    _state = state;

    char** argv = new char*[2];
    argv[0] = strdup(iface_name);
    argv[1] = nullptr;
    
    int ret = task_create("ROTOR_CAN", ROTOR_CAN_TASK_PRIORITY, ROTOR_CAN_TASK_STACK_SIZE, CanRotorNode::taskSpawn, argv);

    delete[] argv[0];
    delete[] argv[1];
    delete[] argv;

    return ret;
}

/*
Initializing the CAN backend driver; in this example we're using SocketCAN
*/
void CanRotorNode::run()
{
    /*
    Run the main loop.
    */
    uint64_t next_1hz_service_at = SystemTools::micros64();
    uint64_t next_50hz_service_at = SystemTools::micros64();
    uint64_t next_test_service_at = SystemTools::micros64();


    while (!shouldExit()) {
        _canard_iface.process(10);

        const uint64_t ts = SystemTools::micros64();


        if (ts >= next_1hz_service_at) {
            next_1hz_service_at += 1000000ULL;
            process1HzTasks(ts);
        }
        if (ts >= next_50hz_service_at) {
            next_50hz_service_at += 1000000ULL/50U;

            sendRotorStatus();
        }
        if (ts >= next_test_service_at) {
            next_test_service_at += 50000000ULL;
            if (testMode) {
                if (testNodeStep == 1) {
                    testRelease();
                    testNodeStep = 0;
                } else {
                    testStep();
                    testNodeStep = 1;
                }
            }
        }
    }
}

void CanRotorNode::stopNode() {
    if (_instance) {
        snowinfo("CanRotorNode stopping\n");
        _should_exit = true;
        // wait for task to exit
        while (_task_id != -1) {
            usleep(10000);
        }
        snowinfo("CanRotorNode stopped\n");
    }
}

/*
Initializing the Socket CAN backend driver
*/
int CanRotorNode::init(const char * iface_name) {
    snowinfo("Canard Iface init %s\n", iface_name);
    // init the interface
    #ifdef ROTOR_TEST_MODE
        testMode = ROTOR_TEST_MODE;
    #else
        testMode = false;
    #endif
    if (!testMode) {
        _canard_iface.init(iface_name);
        _canard_iface.set_node_id(ROTOR_CAN_NODE_ID);
    } else {
        snowinfo("Canard Iface init test mode\n");
    }

    return 0;
}

int CanRotorNode::taskSpawn(int argc, char** argv) {
    //TODO Make constructor
    _instance = std::make_unique<CanRotorNode>();
    
    if (_instance) {
        snowinfo("Can task spawn\n");

        _task_id = getpid();
        _instance->init(argv[1]);
        _instance->run();

        _instance.reset();
        _task_id = -1;
        return 0;
    } else {
        snowerror("alloc failed");
    }

    _instance.reset();
    _task_id = -1;

    return -1;
}

void CanRotorNode::testStep() {
    snowinfo("Test step\n");
    for(int i = 0; i < 4; i++) {
        int res = _state->setActuatorValue(i, 500);
    }
}

void CanRotorNode::testRelease() {
    snowinfo("Test release\n");
    for(int i = 0; i < 4; i++) {
        int res = _state->setActuatorValue(i, 0.0f);
    }
}