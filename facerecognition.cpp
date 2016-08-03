#include "facerecognition.h"

FaceRecognition::FaceRecognition()
{

}

bool FaceRecognition::Classification(AlignDlib *align, TorchWrap *tw,char * pathNameImage)
{

    //Align face
    Mat alignedFace;
    if(!align->alignDlib(pathNameImage,&alignedFace))
        return false;

    //feature Extraction Torch
    vector<double> features;
    if(!tw->forwardImage(alignedFace,&features))
        return false;

    struct svm_model *model;
    model = svm_load_model("svm_FaceRecognition_v1.xml");
    unsigned int c = 0;
    svm_node* svmNodeProbe = (struct svm_node *)malloc((features.size())*sizeof(struct svm_node));
    for (c=0; c<features.size(); c++)
    {
        svmNodeProbe[c].index = c+1;  // Index starts from 1; Pre-computed kernel starts from 0
        svmNodeProbe[c].value = features[c];
    }
    svmNodeProbe[c].index = -1;   // End of line

    double prob_est[2];  // Probability estimation
    double retval1 = svm_predict_probability(model,svmNodeProbe,prob_est);

    cout << retval1 << endl;
}

#include "dirent.h"
DIR *dir;
struct dirent *ent;

int FaceRecognition::Training(AlignDlib *align, TorchWrap *tw,char * trainingImagesPath){

    struct svm_node** svmVec;
    svmVec = (struct svm_node **)malloc((0)*sizeof(struct svm_node));

    struct svm_problem svmProblem;
    svmProblem.y = (double *)malloc((0)*sizeof(double));

    int Class=1;
    int r=0, c;
    if ((dir = opendir (trainingImagesPath)) != NULL) {
      /* get all the files and directories within directory */

      while ((ent = readdir (dir)) != NULL) {
        DIR *dir2;
        struct dirent *ent2;
        if(*ent->d_name=='.' || *ent->d_name=='..')
            continue;

        char dest[200];
        strcpy(dest,trainingImagesPath);
        char * pathImages = strcat(dest , ent->d_name);
        dir2 = opendir(pathImages);
        while((ent2 = readdir (dir2)) != NULL){
            if(*ent2->d_name=='.' || *ent2->d_name=='..')
                continue;

            char pathNameImage[500];
            strcpy(pathNameImage,pathImages);
            strcat(pathNameImage , "/");
            strcat(pathNameImage , ent2->d_name);

            cout << "name image: " << pathNameImage << endl;

            //Align face
            Mat alignedFace;
            if(!align->alignDlib(pathNameImage,&alignedFace))
                continue;

            //feature Extraction Torch
            vector<double> features;
            if(!tw->forwardImage(alignedFace,&features))
                continue;

            svm_node* svmNode = (struct svm_node *)malloc((features.size())*sizeof(struct svm_node));
            for (c=0; c<features.size(); c++)
            {
                svmNode[c].index = c+1;  // Index starts from 1; Pre-computed kernel starts from 0
                svmNode[c].value = features[c];
            }
            svmNode[c].index = -1;   // End of line
            svmVec = (struct svm_node **)realloc(svmVec,(r+1)*sizeof(struct svm_node));
            svmProblem.y = (double *)realloc(svmProblem.y,(r+1)*sizeof(double));
            svmVec[r] = svmNode;
            svmProblem.y[r++] = Class;

        }
        ++Class;
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }

    svmProblem.x = svmVec;
    svmProblem.l = r;

    struct svm_parameter param;

    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = 0.5;
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    struct svm_model *model;
    model=svm_train(&svmProblem,&param);
    svm_save_model("svm_FaceRecognition_v1.xml",model);
    cout << "model trained" << endl;

}
