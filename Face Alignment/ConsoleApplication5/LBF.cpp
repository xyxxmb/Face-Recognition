//
//  LBF.cpp
//  myopencv
//
//  Created by lequan on 1/24/15.
//  Copyright (c) 2015 lequan. All rights reserved.
//

#include "LBF.h"
#include "LBFRegressor.h"
using namespace std;
using namespace cv;

// parameters
Params global_params;


string modelPath ="./../model/";
string dataPath = "./../datasets/";

string cascadeName = "haarcascade_frontalface_alt.xml";

void InitializeGlobalParam();
void PrintHelp();

int main( int argc, const char** argv ){
//    double rs[3] = {0.4,0.15,0.08};
//    for (int i=0;i<3;i++){
//        Mat image = imread("/Users/lequan/workspace/LBF/Datasets/afw/437595409_1.jpg");
//        string name1("/Users/lequan/workspace/LBF/Datasets/afw/437595409_1.pts");
//        InitializeGlobalParam();
//        Mat_<double> ground_truth_shape = LoadGroundTruthShape(name1);
//        BoundingBox bbx = CalculateBoundingBox(ground_truth_shape);
//        double r = rs[i]*bbx.height/2.0;
//        int a[13]={18,22,37,40,23,27,43,46,31,49,55,52,58};
//        for(int j = 0;j <13;j++){
//            cout <<j<<endl;
//            circle(image,Point2d(ground_truth_shape(a[j]-1,0),ground_truth_shape(a[j]-1,1)),r,Scalar(255,255,255),2,8,0);
//        }
//        imshow("result", image);
//        waitKey(0);
//        string name = "radius" + to_string(i)+".jpg";
//        imwrite(name,image);
//    }
    //initialize parameters
    if (argc > 1 && strcmp(argv[1],"TrainModel")==0){  // 如果是训练模型，就初始化参数，开始训练数据
        InitializeGlobalParam();
    }
    else {
        ReadGlobalParamFromFile(modelPath+"LBF.model");  // 不是训练模型，直接从已经存在的模型中读取参数以及数据
    }

    
    // main process
    if (argc==1){
        PrintHelp();
    }
    else if(strcmp(argv[1],"TrainModel")==0){
        vector<string> trainDataName;
     // you need to modify this section according to your training dataset
     //   trainDataName.push_back("afw");
          trainDataName.push_back("helen");
    //    trainDataName.push_back("lfpw");
        TrainModel(trainDataName);
    }
    else if (strcmp(argv[1], "TestModel")==0){
        vector<string> testDataName;
     // you need to modify this section according to your training dataset
     // testDataName.push_back("ibug");
        testDataName.push_back("helen");
        double MRSE = TestModel(testDataName);
    }
    else if (strcmp(argv[1], "Demo")==0){
        if (argc == 2){
            return FaceDetectionAndAlignment("");
        }
        else if(argc ==3){
            return FaceDetectionAndAlignment(argv[2]);
        }
    }
    else {
        PrintHelp();
    }
	system("pause");
    return 0;
}

// set the parameters when training models.
void InitializeGlobalParam(){
    global_params.bagging_overlap = 0.4;    
    global_params.max_numtrees = 40;   // 每一阶树的个数为10
    global_params.max_depth = 6;    // 树的深度为5
    global_params.landmark_num = 68;  // 共标记多少个landmark
    global_params.initial_num = 5;  
    
    global_params.max_numstage = 8;   // 训练的阶数为7
   // double m_max_radio_radius[10] = {0.4,0.3,0.2,0.15, 0.12, 0.10, 0.08, 0.06, 0.06,0.05};  // 区域半径
	double m_max_radio_radius[10] = { 0.4,0.3,0.2,0.15, 0.10, 0.08, 0.05};  // 区域半径
    //double m_max_numfeats[10] = {500, 500, 500, 300, 300, 200, 200,200,100,100};   // 多少对像素点
	double m_max_numfeats[10] = {  500, 400, 300, 200, 100,80,60 };
    for (int i=0;i<10;i++){
        global_params.max_radio_radius[i] = m_max_radio_radius[i];
    }
    for (int i=0;i<10;i++){
        global_params.max_numfeats[i] = m_max_numfeats[i];
    }
    global_params.max_numthreshs = 500;
}

void ReadGlobalParamFromFile(string path) {
    cout << "Loading GlobalParam..." << endl;
    ifstream fin;
    fin.open(path);
    fin >> global_params.bagging_overlap;   
    fin >> global_params.max_numtrees;
    fin >> global_params.max_depth;
    fin >> global_params.max_numthreshs;
    fin >> global_params.landmark_num;
    fin >> global_params.initial_num;
    fin >> global_params.max_numstage;
    
    for (int i = 0; i< global_params.max_numstage; i++){
        fin >> global_params.max_radio_radius[i];
    }
    
    for (int i = 0; i < global_params.max_numstage; i++){
        fin >> global_params.max_numfeats[i];
    }
    cout << "Loading GlobalParam end"<<endl;
    fin.close();
}

void PrintHelp(){
    cout << "Useage:"<<endl;
    cout << "1. train your own model:    LBF.out  TrainModel "<<endl;
    cout << "2. test model on dataset:   LBF.out  TestModel"<<endl;
    cout << "3. test model via a camera: LBF.out  Demo "<<endl;
    cout << "4. test model on a pic:     LBF.out  Demo xx.jpg"<<endl;
    cout << "5. test model on pic set:   LBF.out  Demo Img_Path.txt"<<endl;
    cout << endl;
}
