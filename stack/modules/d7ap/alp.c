/*! \file alp.c
 *

 *  \copyright (C) Copyright 2015 University of Antwerp and others (http://oss-7.cosys.be)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  \author glenn.ergeerts@uantwerpen.be
 *  \author maarten.weyn@uantwerpen.be
 *
 */

#include "debug.h"
#include "ng.h"

#include "alp.h"
#include "packet.h"
#include "fs.h"

static alp_unhandled_action_callback NGDEF(_unhandled_action_cb);
#define unhandled_action_cb NG(_unhandled_action_cb)

static alp_operation_t get_operation(uint8_t* alp_command)
{
    alp_control_t alp_ctrl;
    alp_ctrl.raw = (*alp_command);
    return alp_ctrl.operation;
}

void alp_init(alp_unhandled_action_callback cb)
{
    unhandled_action_cb = cb;
}

void alp_process_command(uint8_t* alp_command, uint8_t alp_command_length, uint8_t* alp_response, uint8_t* alp_response_length)
{
    alp_control_t alp_control = { .raw = (*alp_command) }; alp_command++;

    switch(alp_control.operation)
    {
        case ALP_OP_READ_FILE_DATA:
        {
            alp_operand_file_data_request_t operand;
            operand.file_offset.file_id = (*alp_command); alp_command++;
            operand.file_offset.offset = (*alp_command); alp_command++; // TODO can be 1-4 bytes, assume 1 for now
            operand.requested_data_length = (*alp_command); alp_command++;

            // fill response
            (*alp_response_length) = 0;
            alp_response[(*alp_response_length)] = ALP_OP_RETURN_FILE_DATA; (*alp_response_length)++;
            alp_response[(*alp_response_length)] = operand.file_offset.file_id; (*alp_response_length)++;
            alp_response[(*alp_response_length)] = operand.file_offset.offset; (*alp_response_length)++;
            alp_response[(*alp_response_length)] = operand.requested_data_length; (*alp_response_length)++;
            fs_read_file(operand.file_offset.file_id, operand.file_offset.offset, alp_response + (*alp_response_length), operand.requested_data_length);
            (*alp_response_length) += operand.requested_data_length;

            break;
        }
        case ALP_OP_RETURN_FILE_DATA:
        {
// TODO
//            alp_operand_file_data_t operand;
//            operand.file_offset.file_id = (*alp_command); alp_command++;
//            operand.file_offset.offset = (*alp_command); alp_command++; // TODO can be 1-4 bytes, assume 1 for now
//            operand.provided_data_length = (*alp_command); alp_command++;

//            // fill response
//            (*alp_response_length) = 0;
//            alp_response[(*alp_response_length)] = ALP_OP_RETURN_FILE_DATA; (*alp_response_length)++;
//            alp_response[(*alp_response_length)] = operand.file_offset.file_id; (*alp_response_length)++;
//            alp_response[(*alp_response_length)] = operand.file_offset.offset; (*alp_response_length)++;
//            alp_response[(*alp_response_length)] = operand.provided_data_length; (*alp_response_length)++;
//            memcpy(resp_data_ptr, alp_command_ptr, operand.provided_data_length);
//            packet->payload_length = resp_data_ptr - packet->payload + operand.provided_data_length;
//            break;
        }
        default:
            assert(false); // TODO implement other operations
    }

    // TODO multiple commands in one request
}

// TODO remove?
//bool alp_process_received_request(d7asp_result_t d7asp_result, packet_t* packet)
//{
//    // TODO merge with alp_process_command() ?
//    // TODO split into actions

//    assert(get_operation(packet->payload) == ALP_OP_RETURN_FILE_DATA); // TODO other operations not supported yet

//    if(unhandled_action_cb)
//        unhandled_action_cb(d7asp_result, packet->payload, packet->payload_length); // TODO &packet->hw_radio_packet.rx_meta
//    packet->payload_length = 0;

//    return true;
//}
