#include "rnn_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "arm_nnexamples_gru_test_data.h"
#include "arm_math.h"
#include "arm_nnfunctions.h"

#ifdef _RTE_
#include "RTE_Components.h"
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif
#endif

#define DIM_HISTORY 32
#define DIM_INPUT   32
#define DIM_VEC     64

#define USE_X4

#ifndef USE_X4
static q7_t update_gate_weights[DIM_VEC * DIM_HISTORY]  = UPDATE_GATE_WEIGHT_X2;
static q7_t reset_gate_weights[DIM_VEC * DIM_HISTORY]   = RESET_GATE_WEIGHT_X2;
static q7_t hidden_state_weights[DIM_VEC * DIM_HISTORY] = HIDDEN_STATE_WEIGHT_X2;
#else
static q7_t update_gate_weights[DIM_VEC * DIM_HISTORY]  = UPDATE_GATE_WEIGHT_X4;
static q7_t reset_gate_weights[DIM_VEC * DIM_HISTORY]   = RESET_GATE_WEIGHT_X4;
static q7_t hidden_state_weights[DIM_VEC * DIM_HISTORY] = HIDDEN_STATE_WEIGHT_X4;
#endif

static q7_t update_gate_bias[DIM_HISTORY]  = UPDATE_GATE_BIAS;
static q7_t reset_gate_bias[DIM_HISTORY]   = RESET_GATE_BIAS;
static q7_t hidden_state_bias[DIM_HISTORY] = HIDDEN_STATE_BIAS;

static q15_t test_input1[DIM_INPUT]    = INPUT_DATA1;
static q15_t test_input2[DIM_INPUT]    = INPUT_DATA2;
static q15_t test_history[DIM_HISTORY] = HISTORY_DATA;

q15_t scratch_buffer[DIM_HISTORY * 4 + DIM_INPUT];

void gru_example(q15_t* scratch_input, uint16_t input_size, uint16_t history_size, q7_t* weights_update, q7_t* weights_reset, q7_t* weights_hidden_state, q7_t* bias_update, q7_t* bias_reset, q7_t* bias_hidden_state)
{
    q15_t* reset        = scratch_input;
    q15_t* input        = scratch_input + history_size;
    q15_t* history      = scratch_input + history_size + input_size;
    q15_t* update       = scratch_input + 2 * history_size + input_size;
    q15_t* hidden_state = scratch_input + 3 * history_size + input_size;

    // reset gate calculation
    // the range of the output can be adjusted with bias_shift and output_shift
#ifndef USE_X4
    arm_fully_connected_mat_q7_vec_q15(input, weights_reset, input_size + history_size, history_size, 0, 15, bias_reset,
                                       reset, NULL);
#else
    arm_fully_connected_mat_q7_vec_q15_opt(input, weights_reset, input_size + history_size, history_size, 0, 15,
                                           bias_reset, reset, NULL);
#endif
    // sigmoid function, the size of the integer bit-width should be consistent with out_shift
    arm_nn_activations_direct_q15(reset, history_size, 0, ARM_SIGMOID);
    arm_mult_q15(history, reset, reset, history_size);

    // update gate calculation
    // the range of the output can be adjusted with bias_shift and output_shift
#ifndef USE_X4
    arm_fully_connected_mat_q7_vec_q15(input, weights_update, input_size + history_size, history_size, 0, 15,
                                       bias_update, update, NULL);
#else
    arm_fully_connected_mat_q7_vec_q15_opt(input, weights_update, input_size + history_size, history_size, 0, 15,
                                           bias_update, update, NULL);
#endif

    // sigmoid function, the size of the integer bit-width should be consistent with out_shift
    arm_nn_activations_direct_q15(update, history_size, 0, ARM_SIGMOID);

    // hidden state calculation
#ifndef USE_X4
    arm_fully_connected_mat_q7_vec_q15(reset, weights_hidden_state, input_size + history_size, history_size, 0, 15,
                                       bias_hidden_state, hidden_state, NULL);
#else
    arm_fully_connected_mat_q7_vec_q15_opt(reset, weights_hidden_state, input_size + history_size, history_size, 0, 15,
                                           bias_hidden_state, hidden_state, NULL);
#endif

    // tanh function, the size of the integer bit-width should be consistent with out_shift
    arm_nn_activations_direct_q15(hidden_state, history_size, 0, ARM_TANH);
    arm_mult_q15(update, hidden_state, hidden_state, history_size);

    // we calculate z - 1 here
    // so final addition becomes substraction
    arm_offset_q15(update, 0x8000, update, history_size);
    // multiply history
    arm_mult_q15(history, update, update, history_size);
    // calculate history_out
    arm_sub_q15(hidden_state, update, history, history_size);

    return;
}

void rnn_test(void)
{
#ifdef RTE_Compiler_EventRecorder
    EventRecorderInitialize(EventRecordAll, 1);  // initialize and start Event Recorder
#endif

    printf("Start GRU execution\r\n");
    int input_size   = DIM_INPUT;
    int history_size = DIM_HISTORY;

    // copy over the input data
    arm_copy_q15(test_input1, scratch_buffer + history_size, input_size);
    arm_copy_q15(test_history, scratch_buffer + history_size + input_size, history_size);

    gru_example(scratch_buffer, input_size, history_size,
                update_gate_weights, reset_gate_weights, hidden_state_weights,
                update_gate_bias, reset_gate_bias, hidden_state_bias);
    printf("Complete first iteration on GRU\r\n");

    arm_copy_q15(test_input2, scratch_buffer + history_size, input_size);
    gru_example(scratch_buffer, input_size, history_size,
                update_gate_weights, reset_gate_weights, hidden_state_weights,
                update_gate_bias, reset_gate_bias, hidden_state_bias);
    printf("Complete second iteration on GRU\r\n");
}
