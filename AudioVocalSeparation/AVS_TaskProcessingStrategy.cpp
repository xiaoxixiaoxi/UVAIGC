//
//  AVS_TaskProcessingStrategy.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#include "AVS_TaskProcessingStrategy.hpp"




// 音频STFT变换函数
std::vector<std::vector<std::complex<double>>> AVS_TaskProcessingStrategy::stft_v2(const std::vector<double>& audio, const int fft_size, const int hop_size)
{
    const int num_frames = (audio.size() - fft_size) / hop_size + 1;
    std::vector<std::vector<std::complex<double>>> stft_data(num_frames, std::vector<std::complex<double>>(fft_size/2+1));

    // 创建FFTW3计算所需的输入、输出、计算计划
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_size);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_size);
    fftw_plan plan = fftw_plan_dft_1d(fft_size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // 初始化汉宁窗函数系数
    double* window = new double[fft_size];
    for (int i = 0; i < fft_size; i++) {
        window[i] = 0.5 * (1 - cos(2 * M_PI * i / (fft_size - 1)));
    }

    // 对音频进行STFT变换
    for (int i = 0; i < num_frames; i++) {
        for (int j = 0; j < fft_size; j++) {
            in[j][0] = audio[i*hop_size+j] * window[j];
            in[j][1] = 0.0;
        }
        fftw_execute(plan);
        for (int j = 0; j < fft_size/2+1; j++) {
            stft_data[i][j] = std::complex<double>(out[j][0], out[j][1]);
        }
    }

    // 释放内存
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    delete[] window;

    return stft_data;
}


std::vector<double> AVS_TaskProcessingStrategy::istft_v2(const std::vector<std::vector<std::complex<double>>>& stft_matrix,int n_fft,int hop_length) {
    
    int n_frames = stft_matrix.size();
    
    
    // 初始化汉宁窗函数系数
    double* window = new double[n_fft];
    for (int i = 0; i < n_fft; i++) {
        window[i] = 0.5 * (1 - cos(2 * M_PI * i / (n_fft - 1)));
    }

    
    
    
    int win_size = n_fft;
    int n_samples = (n_frames - 1) * hop_length + n_fft;
    int half_n_fft = n_fft / 2 + 1;

    // 创建FFTW3的输入、输出以及傅里叶变换计划
//    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * half_n_fft);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n_fft);
    double* in = (double*) fftw_malloc(sizeof(double) * n_fft);
    fftw_plan plan = fftw_plan_dft_c2r_1d(n_fft, out, in, FFTW_ESTIMATE);

    // 分配用于存储每一帧反变换的输出向量
    std::vector<double> y(n_samples);

    // 分配临时变量，用于存储帧的加权后FFT结果和加权窗函数
    std:: vector<std::complex<double>> frame_fft(n_fft);
    std::vector<double> frame_windowed(n_fft);

    // 进行反变换并将每一帧叠加到输出向量y中
    for (int i = 0; i < n_frames; i++) {
        // 将帧的FFT结果赋值给临时变量frame_fft
        for (int j = 0; j < half_n_fft; j++) {
            frame_fft[j] = stft_matrix[i][j];
        }
        for (int j = half_n_fft; j < n_fft; j++) {
            frame_fft[j] = conj(stft_matrix[i][n_fft - j]);
        }

        // 进行反FFT得到帧的时域信号
        fftw_execute_dft_c2r(plan, (fftw_complex*)&frame_fft[0], in);

        // 将帧的加权窗函数赋值给临时变量frame_windowed
        for (int j = 0; j < win_size; j++) {
            frame_windowed[j] = window[j];
        }
        for (int j = win_size; j < n_fft; j++) {
            frame_windowed[j] = 0.0;
        }

        // 将帧的加权窗函数与时域信号相乘得到加权后的时域信号
        for (int j = 0; j < n_fft; j++) {
            y[i * hop_length + j] += frame_windowed[j] * in[j] / n_fft;
        }
    }

    // 释放内存并销毁FFT计划
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    delete[] window;

    return y;
}


void AVS_TaskProcessingStrategy::normalization(std::vector<double>& data){
    
    double max_abs_val_l = 0.0f;
    for (double sample : data) {
        max_abs_val_l = std::max(max_abs_val_l, std::abs(sample));
    }
    double scale = max_abs_val_l > 0.0f ? max_abs_val_l : 1.0f;
    for (double& sample : data) {
        sample /= scale;
    }
}
