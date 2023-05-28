//
//  AVS_MDXNet_ONNX_Strategy.cpp
//  UVAIGC
//
//  Created by 林溪明 on 2023/5/10.
//

#include "AVS_MDXNet_ONNX_Strategy.hpp"
#include "AudioFile.h"



AVS_MDXNet_ONNX_Strategy::AVS_MDXNet_ONNX_Strategy(AVS_LoaderFile* m_fileLoader)
{
//    this->type = AVS_MDXNet;
    
    //加载
    // 配置加载模块
    // 配置保存模块
    
    //加载模块
    if(m_fileLoader != nullptr){
        this->setLoaderFile(m_fileLoader);
        
    }
   
    
    

    
}
AVS_MDXNet_ONNX_Strategy::~AVS_MDXNet_ONNX_Strategy()
{
    delete session_Instrumental;
    delete session_Vocals;
    session_Instrumental = NULL;
    
    
//    delete m_fileLoader;
    
//    delete wavSaveStrategy;
    
    
    
    std::cout<<"删除AVS_MDXNet_ONNX 对象"<<"\n";
    
}


bool AVS_MDXNet_ONNX_Strategy::loadOxxnModel(Ort::Session*& session,std::string modelPath){
    
    
    if (session!=nullptr){
        delete session;
    }
   
    //std::string weightFile = "/Users/linximing/Desktop/models/MDX_Net_Models/UVR-MDX-NET-Inst_HQ_1.onnx";
    std::string weightFile = modelPath;
    

    std::wstring weightFile_w(weightFile.begin(), weightFile.end());
    
    
    try {
        
        //加载ONNX模型
        session = new Ort::Session(env, weightFile_w.c_str(), Ort::SessionOptions{ nullptr });
//        getInPutGetShape(session);

     } catch (const Ort::Exception& exception) {
         std::cout << "错误" << exception.what() << std::endl;
         
         
         //信息输出
         out_info = exception.what();
         //标记错误
//         status = AVS_S_failed;
         m_state = Error;
//         setName("设置名称");
//         m_name;
         
        
        
         return false;

//       exit(-1);
     }
   
    
    //获取输入的接口 和是否可以加载
    

    return true;
    
}

bool AVS_MDXNet_ONNX_Strategy::isLoadModel(AVS_MDXNet_Models_Type type){
    bool is_load = false;
    switch (type) {
        case AVS_MDXNet_Models_Type::MMType_Instrumental:
            
            if(this->session_Instrumental != nullptr){
                is_load = true;
            }
            
            break;
        case AVS_MDXNet_Models_Type::MMType_Vocals:
            
            if(this->session_Vocals != nullptr){
                is_load = true;
            }
            
            break;
            
        default:
            break;
    }
    
    
    return is_load;
    
}

bool AVS_MDXNet_ONNX_Strategy::verifyModel(std::string modelPath){
      
    Ort::Env test_env;

    std::wstring weightFile_w(modelPath.begin(), modelPath.end());

    //加载ONNX模型
    Ort::Session  test_session_Instrumental(test_env, weightFile_w.c_str(), Ort::SessionOptions{ nullptr });
    return true;
    
}


//加载不同模型
bool AVS_MDXNet_ONNX_Strategy::loadOxxnModel_Instrumental(std::string modelPath){
//    int a =0;
    

    
    
    
    return loadOxxnModel(this->session_Instrumental, modelPath);

    
}
bool AVS_MDXNet_ONNX_Strategy::loadOxxnModel_Vocals(std::string modelPath){
//    int a =0;
    return loadOxxnModel(this->session_Vocals, modelPath);
   
    
}



std::vector<int64_t> AVS_MDXNet_ONNX_Strategy::getInPutGetShape(Ort::Session* session){
    
    //输出模型输入节点的数量
    size_t num_input_nodes = session->GetInputCount();
    
    std::vector<int64_t> out{1,-1,-1,-1 };
    
    //获取输入输出维度
    for(auto i = 0; i<num_input_nodes;i++)
    {
        std::vector<int64_t> input_dims = session->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        
        if (input_dims.size() == 4){
            
            
            out[1] = input_dims[01];
            out[2] = input_dims[02];
            out[3] = input_dims[03];
//            input_Shape_01 = input_dims[01];
//            input_Shape_02 = input_dims[02];
//            input_Shape_03 = input_dims[03];
        }
        
//
//        for(auto j=0; j<input_dims.size();j++){
//            //根据模型自定义
//            std::cout <<input_dims[j]<<" ";
//        }
            
    }
    
    return out;
    
    
    
}


void AVS_MDXNet_ONNX_Strategy::run_oxxn(Ort::Session* session, std::vector<std::vector<std::complex<double>>>& spectrogram_l,std::vector<std::vector<std::complex<double>>>& spectrogram_r,int offset){
    
    //TODO: 数据错误处理 尺寸问题
    //数据准备
    std::vector<const char*> inputNames = {"input"};
    std::vector<const char*>  outputNames = {"output"};
    
    //std::vector<int64_t> input_shape_{1,4,3072, 256};
    std::vector<int64_t> input_shape_ = getInPutGetShape(session);
    
    int input_size_num = input_shape_[1]*input_shape_[2]*input_shape_[3];
    std::vector<float> input_tensor_values(input_size_num);
    
    std::vector<float> results_(input_size_num);
    
    memset(input_tensor_values.data(),0,sizeof(float)*(input_tensor_values.size()));
    memset(results_.data(),0,sizeof(float)*(results_.size()));
    
    
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
//    auto stft_data = spectrogram;
    
    // 数据准备。需要按照模型的数据排序 由
    int aa = (input_shape_[2]*input_shape_[3]);
    for(int i = 0; i < input_shape_[3]; i++){
        for (int j = 0;  j < input_shape_[2] ;  j++){
            //int bb =  i * 3072 + j;
            int bb =  i + j*input_shape_[3];
            
            //  如果大于输入边缘 就不再写入
            if ((i + offset + 1) > spectrogram_l.size() ){
                break;
            }
            
            input_tensor_values[bb] = float(spectrogram_l[i + offset][j].real()); // 实部1spectrogram[i][j].real();
            input_tensor_values[bb+aa] = float(spectrogram_l[i + offset][j].imag());
            
            input_tensor_values[bb+(2*aa)] = float(spectrogram_r[i + offset][j].real());
            input_tensor_values[bb+(3*aa)] = float(spectrogram_r[i + offset][j].imag());
            
        }
        
        
    }
 
    Ort::Value input_tensor_ = Ort::Value::CreateTensor<float>(memory_info,  input_tensor_values.data(),  input_tensor_values.size(),
                                                       input_shape_.data(), input_shape_.size());
    
    Ort::Value output_tensor_ = Ort::Value::CreateTensor<float>(memory_info, results_.data(), results_.size(),
                                                                input_shape_.data(), input_shape_.size());
    
//    assert(input_tensor_.IsTensor());
//    Ort::Env env;
//    std::string weightFile =  "/Users/linximing/Desktop/models/MDX_Net_Models/UVR-MDX-NET-Inst_HQ_1.onnx";
//    //加载ONNX模型
//    session = new Ort::Session(env, weightFile.c_str(), Ort::SessionOptions{ nullptr });

    
  
     session->Run(Ort::RunOptions{nullptr},
                 inputNames.data(),
                 &input_tensor_,
                inputNames.size(),
                outputNames.data(),
                 &output_tensor_,
                outputNames.size());
    
//    把数据再次填充回去
    for(int i = 0; i < input_shape_[3]; i++){
        for (int j = 0;  j < input_shape_[2] ;  j++){
            //int bb = i * 3072 + j;
            int bb =  i + j*input_shape_[3];
            
            if ((i + offset + 1) > spectrogram_l.size() ){
                break;
            }
            
            
            double  real_ = results_[bb];
            double  imag_ = results_[bb+aa];
            
            std::complex<double> fft_value(real_,imag_);
            spectrogram_l[i + offset][j] = fft_value;
            
            
            
            double  real_r = results_[bb+2*aa];
            double  imag_r = results_[bb+3*aa];
            
            std::complex<double> fft_value_r(real_r,imag_r);
            spectrogram_r[i + offset][j] = fft_value_r;
            
            
            
            
            
            
            
        }
    }
    
    input_tensor_values.clear(); ;
    results_.clear();
    
    
}



void AVS_MDXNet_ONNX_Strategy::run( AVS_Task& avsTask,Ort::Session* session, std::vector<std::vector<double>>& in_data,std::vector<std::vector<double>>& out_data){
    //数据拆分 准备 分帧 分完了应该为每一份256帧
    //变换 推理 还原
    //数据还原成音频
    
    
    //进度条分配 当前/总轨道数*(stft*0.1 + 推理*0.8 + istft*0.1) = 1.0
//    用一个函数表示
    
    //
    for(std::vector<double>& i:in_data){
        //归一化
        this->normalization(i);
    }
    
    
    std::vector<int64_t> input_shape = getInPutGetShape(session);
    
    //参数从模型读出
    int window_size =  input_shape[2]*2;
//    int window_size =  2048*2;
    int hop_size =  window_size/6;
    
    
    int num_frames =  input_shape[3];
//    int num_frames = 512;
    
    
//    std::vector<int64_t> input_shape{1,4,2048,512};
    
    //每一帧几个采样点
    int num_frames_sub = hop_size*num_frames;
    
    //判断轨道数
    int raw_track = in_data.size();
    
    //假就结束 真继续
    while (in_data.size()%2 != 0) {
        // 填充轨道
        std::vector<double> ls_track(in_data[0].size(),0.0);
        in_data.push_back(ls_track);
    }
    std::cout << "轨道数:" <<in_data.size() << "\n";
    
//    std::vector<std::vector<std::complex<double>>> ls_spectrogram_l;
    
    for (int i = 0; i < in_data.size()/2; i++) {
        
        float gd_bl = (i+1)/(in_data.size()/2);
        float stft_jd = 0;
        float onnxrun_jd = 0;
        float istft_jd = 0;
      
        
//        std::vector<std::vector<std::complex<double>>> spectrogram;
        std::vector<std::vector<std::complex<double>>> spectrogram_l  = this->stft_v2(in_data[i], window_size, hop_size);
        std::vector<std::vector<std::complex<double>>> spectrogram_r  = this->stft_v2(in_data[i+1], window_size, hop_size);
        
       
        //进度
        stft_jd = 1;
       
        this->setProgress( avsTask ,gd_bl, stft_jd,onnxrun_jd,istft_jd );

        
        std::cout << "stft尺寸 shape: (" << spectrogram_l.size() << ", " << spectrogram_l[0].size() << ")" << std::endl;
        std::cout << "stft02尺寸 shape: (" << spectrogram_r.size() << ", " << spectrogram_r[0].size() << ")" << std::endl;
//        分帧处理 256 不够用0 填充 最后再删除
        
        int raw_spectrogram_num = spectrogram_l.size();

        for (int j = 0; j <= spectrogram_l.size()/num_frames; j++) {
            int start = j*num_frames;
            
            
            
            
            try {
                this->run_oxxn(session ,spectrogram_l,spectrogram_r,start);
             } catch (const Ort::Exception& exception) {
                 //信息输出
                 out_info = exception.what();
                 //标记错误
//                 status = AVS_S_failed;
                 m_state = Finished;
                 
                 
                 //标志失败
                 avsTask.set_stateError();
                 avsTask.setOutInfo(exception.what());
                 break;
             }
            
            
            //进度
            onnxrun_jd = (start*1.0)/raw_spectrogram_num;
            this->setProgress( avsTask ,gd_bl, stft_jd,onnxrun_jd,istft_jd );
//
            std::cout << j<<"推理开始位置:" << start << "进度" << avsTask.getProgress()  << "\n";
        }
        
       
        //填充的去掉
        while (spectrogram_l.size() > raw_spectrogram_num) {

            spectrogram_l.pop_back();
            spectrogram_r.pop_back();
        }
        
        this->setProgress( avsTask ,gd_bl, stft_jd,onnxrun_jd,istft_jd );
        
        std::cout << "元素:" << spectrogram_l.size() <<" " << raw_spectrogram_num << "\n";
        
        std::vector<double> out_signal_l = this->istft_v2(spectrogram_l, window_size, hop_size);
        std::vector<double> out_signal_r = this->istft_v2(spectrogram_r, window_size, hop_size);
        
        
        //判断是否归一
        uint64_t yy = 0;
        for(double& y:out_signal_l){
            if (y > 1.0 || y < -1.0) {
                yy +=1;
            }
        }
        double bl = (yy*1.0)/out_signal_l.size();
        if (bl > 0.4){
            this->normalization(out_signal_l);
            this->normalization(out_signal_r);
        }
          
        

        out_data.push_back(out_signal_l);
        out_data.push_back(out_signal_r);
        
        
        //进度
        istft_jd = 1;
        this->setProgress(avsTask ,gd_bl, stft_jd,onnxrun_jd,istft_jd );

    }
    
    
    
    //轨道数回归原本
    while (out_data.size() > raw_track) {
        out_data.pop_back();
    }
    

    
   
    
}



//进度条分配 当前/总轨道数*(stft*0.1 + 推理*0.8 + istft*0.1) = 1.0
//设置进度条 单任务
void AVS_MDXNet_ONNX_Strategy::setProgress( AVS_Task& avsTask,float gd_bl,float stft_jd,float onnxrun_jd,float istft_jd){
    
    //判断是否取消。取消就引发异常
    if(this->m_is_cancelled != nullptr){
        if(*this->m_is_cancelled == true){
            
            // 状态修改
            avsTask.set_stateCancel();
            
            //取消引起的异常
            throw OperationCanceledException("Operation canceled.");
            
            
        }
    }
    
    //再添加一个因素
    double progress = gd_bl *(stft_jd*0.1 + onnxrun_jd*0.8 +  istft_jd*0.1) /avsTask.m_mdxnet_type_list.size() + myprogress;
    avsTask.setProgress(progress);
    
}



//实现任务处理统一接口
void AVS_MDXNet_ONNX_Strategy::processTask( TaskBasis& task) {
    
    // 实现音频分离的具体处理逻辑
    
    // 使用 AVS_task 对象进行音频处理的具体实现
    AVS_Task& avsTask = static_cast< AVS_Task&>(task);
   // 进行 AVS_task 特定的处理逻辑
//    avsTask.setName("yyyy");
//    avsTask.setProgress(<#double progress#>);
    //获取音频数据
    //设置状态
    avsTask.set_stateStart();
    avsTask.setProgress(0.01);
    myprogress = 0;
    
    std::vector<std::vector<double>> loadedAudio = m_fileLoader->loadAudioFromFile(avsTask.getUrl(),avsTask.timeRange_in,avsTask.timeRange_out);
    
    
    // 配置输出模块
    AVS_WAVFileSaveStrategy wavSaveStrategy;
    setSaveStrategy(&wavSaveStrategy);
    
    //有选中几个输出
//    分成不同处理
  
    
    std::vector<AVS_MDXNet_Models_Type> avs_maxnet_models = avsTask.m_mdxnet_type_list;
    
    //计算进度条用
    double ls_myprogress = 1.0/avs_maxnet_models.size();
    
    for (AVS_MDXNet_Models_Type i: avs_maxnet_models) {
       
        
        if(i == MMType_Instrumental){
            std::vector<std::vector<double>> Instrumental_signal;
            
            //开始处理
            run(avsTask, session_Instrumental, loadedAudio, Instrumental_signal);
            
            std::string name2 = "_Instrumental";
            savefile(avsTask,Instrumental_signal,name2);
            
            
        }
        
        if (i == MMType_Vocals) {
            
            std::vector<std::vector<double>> Vocals_signal;
            run(avsTask, session_Vocals, loadedAudio,  Vocals_signal);
            
            std::string name2 = "_Vocals";
            savefile(avsTask,Vocals_signal,name2);
            
        }
        //添加其他的
        
        
        //计算进度条用
        myprogress +=ls_myprogress;
            
            
            
            
            
    }
    
    
    
    
    
    
    

    //状态设置完成
    avsTask.set_stateFinished();

    

    
    
    
    
    
    
    
    
    
    
    
}



//分装一个文件保存
bool AVS_MDXNet_ONNX_Strategy::savefile(AVS_Task& avsTask,std::vector<std::vector<double>>& signal,std::string& name2){
    
    bool is_save = false;
    if(avsTask.getState() == TaskBasis::Running){
        AVS_SaveParams saveParams;
        std::string outpath = avsTask.getAvailableFilePath(avsTask.out_path_dir, avsTask.getName() + name2, "wav");
        saveParams.filePath = outpath;
        saveParams.audioData = &signal;
        saveParams.sampleRate = 44100;
        
        is_save = m_fileSaveModule.saveFile(saveParams);
        if (is_save) {
            //保存下来后面可以
            avsTask.out_path_list.push_back(saveParams.filePath);
        }else{
            avsTask.set_stateError();
            avsTask.setOutInfo("File save failed");
            
        }
        
    }
    
    return is_save;
}

