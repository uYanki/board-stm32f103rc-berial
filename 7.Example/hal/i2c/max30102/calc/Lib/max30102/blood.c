#include "blood.h"
#include "usart.h"
uint16_t g_fft_index = 0;  // fft��������±�

struct compx s1[FFT_N + 16];  // FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����
struct compx s2[FFT_N + 16];  // FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����

struct {
    float Hp;    // Ѫ�쵰��
    float HpO2;  // ����Ѫ�쵰��
} g_BloodWave;   // ѪҺ��������

BloodData g_blooddata = {0};  // ѪҺ���ݴ洢

#define CORRECTED_VALUE 47  // �궨ѪҺ��������

/*funcation start ------------------------------------------------------------*/
// ѪҺ�����Ϣ����
void blood_data_update(void)
{
    max30102_fifo_t fifo;
    // ��־λ��ʹ��ʱ ��ȡFIFO
    g_fft_index = 0;
    while (g_fft_index < FFT_N) {
        if (MAX30102_CheckFIFO() == HAL_OK) {
            if (MAX30102_ReadFIFO2(&fifo) == HAL_OK)
                // ������д��fft���벢������
                if (g_fft_index < FFT_N) {
                    // ������д��fft���벢������
                    s1[g_fft_index].real = fifo.Red;
                    s1[g_fft_index].imag = 0;
                    s2[g_fft_index].real = fifo.IR;
                    s2[g_fft_index].imag = 0;
                    g_fft_index++;
                }
        }
    }
}

// ѪҺ��Ϣת��
void blood_data_translate(void)
{
    float    n_denom;
    uint16_t i;
    // 4 pt Moving Average

    // ֱ���˲�
    float dc_red = 0;
    float dc_ir  = 0;
    float ac_red = 0;
    float ac_ir  = 0;

    for (i = 0; i < FFT_N; i++) {
        dc_red += s1[i].real;
        dc_ir += s2[i].real;
    }
    dc_red = dc_red / FFT_N;
    dc_ir  = dc_ir / FFT_N;
    for (i = 0; i < FFT_N; i++) {
        s1[i].real = s1[i].real - dc_red;
        s2[i].real = s2[i].real - dc_ir;
    }

    // �ƶ�ƽ���˲�
    for (i = 1; i < FFT_N - 1; i++) {
        n_denom    = (s1[i - 1].real + 2 * s1[i].real + s1[i + 1].real);
        s1[i].real = n_denom / 4.00;

        n_denom    = (s2[i - 1].real + 2 * s2[i].real + s2[i + 1].real);
        s2[i].real = n_denom / 4.00;
    }
    // �˵�ƽ���˲�
    for (i = 0; i < FFT_N - 8; i++) {
        n_denom    = (s1[i].real + s1[i + 1].real + s1[i + 2].real + s1[i + 3].real + s1[i + 4].real + s1[i + 5].real + s1[i + 6].real + s1[i + 7].real);
        s1[i].real = n_denom / 8.00;

        n_denom    = (s2[i].real + s2[i + 1].real + s2[i + 2].real + s2[i + 3].real + s2[i + 4].real + s2[i + 5].real + s2[i + 6].real + s2[i + 7].real);
        s2[i].real = n_denom / 8.00;
    }

    // ��ʼ�任��ʾ
    g_fft_index = 0;
    // ���ٸ���Ҷ�任
    FFT(s1);
    FFT(s2);

    // ��ƽ��
    for (i = 0; i < FFT_N; i++) {
        s1[i].real = sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
        s1[i].real = sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag);
    }
    // ���㽻������
    for (i = 1; i < FFT_N; i++) {
        ac_red += s1[i].real;
        ac_ir += s2[i].real;
    }

    //	for(i = 0;i < 50;i++)
    //	{
    //		if(s1[i].real<=10)
    //			break;
    //	}

    // printf("%d\r\n",(int)i);
    // ��ȡ��ֵ��ĺ�����  �������������Ϊ
    int s1_max_index = find_max_num_index(s1, 30);
    int s2_max_index = find_max_num_index(s2, 30);

    // ���HbO2��Hb�ı仯Ƶ���Ƿ�һ��
    //	if(i>=45)
    //	{
    // ���ʼ���
    //		uint16_t Heart_Rate = 60.00 * SAMPLES_PER_SECOND * s1_max_index / FFT_N;

    float Heart_Rate = 60.00 * ((100.0 * s1_max_index) / 512.00);

    g_blooddata.heart = Heart_Rate;

    // Ѫ����������
    //  double R = (s2[average].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);
    //  sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;

    // double R = (s1[s1_max_index].real/s1[0].real)/(s2[s2_max_index].real/s2[0].real);

    // double sp02_num =-16.666666*R*R+ 8.333333 *R + 100;
    // g_blooddata.SpO2 = sp02_num;
    // Ѫ����������
    // float sp02_num = (s2[s1_max_index].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);

    // sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;
    //( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max)

    float R          = (ac_ir * dc_red) / (ac_red * dc_ir);
    float sp02_num   = -45.060 * R * R + 30.354 * R + 94.845;
    g_blooddata.SpO2 = sp02_num;

    // ״̬����
    //	}
    //	else //���ݷ����쳣
    //	{
    //		g_blooddata.heart = 0;
    //		g_blooddata.SpO2 	= 0;
    //	}
    // �����任��ʾ
}

void blood_Loop(void)
{
    blood_data_update();
    blood_data_translate();
    if (g_blooddata.SpO2 > 99.99) g_blooddata.SpO2 = 99.99;
    printf("HR=%d/min\tSpO2=%.2f\r\n", g_blooddata.heart, g_blooddata.SpO2);
    HAL_Delay(10);
}