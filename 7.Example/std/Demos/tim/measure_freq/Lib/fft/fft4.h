// 基4快速傅里叶变换(序列长度N=2^k)

void fft(const float* src_x, const float* src_y, float* dst_x, float* dst_y, int k) {
    int    i, n;
    float  temp, temp_x, temp_y;
    float* buffer;
    if (0 == k) {
        *dst_x = *src_x;
        *dst_y = *src_y;
        return;
    }
    n      = 1 << (k - 1);
    buffer = (float*)malloc(4 * n * sizeof(float));
    for (i = 0; i < n; i++) {
        buffer[i]         = src_x[i * 2];
        buffer[i + n]     = src_y[i * 2];
        buffer[i + 2 * n] = src_x[i * 2 + 1];
        buffer[i + 3 * n] = src_y[i * 2 + 1];
    }
    fft((const float*)buffer + 2 * n, (const float*)buffer + 3 * n, dst_x, dst_y, k - 1);
    fft((const float*)buffer, (const float*)buffer + n, (float*)buffer + 2 * n, (float*)buffer + 3 * n, k - 1);
    for (i = 0; i < n; i++) {
        temp          = i * M_PI / n;
        temp_x        = cos(temp);
        temp_y        = sin(temp);
        buffer[i]     = dst_y[i] * temp_y + dst_x[i] * temp_x;
        buffer[i + n] = dst_y[i] * temp_x - dst_x[i] * temp_y;
        dst_x[i]      = buffer[i + 2 * n] + buffer[i];
        dst_y[i]      = buffer[i + 3 * n] + buffer[i + n];
        dst_x[i + n]  = buffer[i + 2 * n] - buffer[i];
        dst_y[i + n]  = buffer[i + 3 * n] - buffer[i + n];
    }
    free(buffer);
}
