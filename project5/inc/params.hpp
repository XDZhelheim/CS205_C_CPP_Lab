#ifndef PARAMS
#define PARAMS

typedef struct conv_param {
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float* p_weight;
    float* p_bias;
} conv_param;

typedef struct fc_param {
    int in_features;
    int out_features;
    float* p_weight;
    float* p_bias;
} fc_param;

extern float conv0_weight[16 * 3 * 3 * 3];
extern float conv0_bias[16];
extern float conv1_weight[32 * 16 * 3 * 3];
extern float conv1_bias[32];
extern float conv2_weight[32 * 32 * 3 * 3];
extern float conv2_bias[32];
extern float fc0_weight[2 * 2048];
extern float fc0_bias[2];
extern conv_param conv_params[3];
extern fc_param fc_params[1];

#endif