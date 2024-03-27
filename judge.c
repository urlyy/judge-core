#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "util/diff.h"
#include "util/runcode.h"
#include "cjson/cJSON.h"

#define EXE "program.exe"
#define SPECIAL_EXE "special.exe"

#define OUT "out.out"
#define SPECIAL_OUT "sout.out"

void res2json(char *destFile,Result *compileResult,Result *runResults,int testCaseNum){
    // 创建 JSON 对象
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        fprintf(stderr, "Failed to create JSON object.\n");
        return;
    }
    //编译结果
    cJSON *compileNode = cJSON_CreateObject();
    cJSON_AddNumberToObject(compileNode, "status", compileResult->status);
    cJSON_AddStringToObject(compileNode, "log", compileResult->log);
    cJSON_AddItemToObject(root, "compile", compileNode);
    // 运行结果
    cJSON * runNodes = cJSON_CreateArray();
    for(int i=0; i<testCaseNum;i++){
        cJSON *runNode = cJSON_CreateObject();
        cJSON_AddNumberToObject(runNode, "status", runResults[i].status);
        cJSON_AddStringToObject(runNode, "log", runResults[i].log);
        cJSON_AddNumberToObject(runNode, "time", runResults[i].timeUsed);
        cJSON_AddNumberToObject(runNode, "memory", runResults[i].memoryUsed);
        cJSON_AddItemToArray(runNodes, runNode);
    }
     cJSON_AddItemToObject(root, "run", runNodes);
    // // 将 JSON 对象转换为 JSON 字符串
    char *jsonStr = cJSON_Print(root);
    if (jsonStr == NULL) {
        fprintf(stderr, "Failed to convert JSON object to string.\n");
        cJSON_Delete(root);
        return;
    }
    cJSON_Delete(root);
    // 打开文件，如果不存在则创建，准备写入  
    FILE *file = fopen(destFile, "w");  
    if (file == NULL) {  
        perror("Error opening file");  
        return;  
    }  
    // 写入字符串到文件  
    fputs(jsonStr, file);  
    fclose(file);  
    free(jsonStr);
}


int judge(const char *codeFilename,const int testCaseNum,const int isSpecial,const char *inputFileSuffix,const char *arg,const int timeLimit,const int memoryLimit,Result *compileRes,Result *runResArr,int *passNum){
    compile(codeFilename,EXE,compileRes);
    if(compileRes->status==COMPILE_ERROR){
        return COMPILE_ERROR;
    }
    for(int i=0;i<testCaseNum;i++){
        char inputFilename[15];
        sprintf(inputFilename, "%d.%s", i,inputFileSuffix);
        runExe(EXE,inputFilename,OUT,timeLimit,memoryLimit,&runResArr[i]);
        // // 看是否运行通过
        if(runResArr[i].status!=ACCECPT){
            return runResArr[i].status;
        } 
        // //看是否答案正确
        if(isSpecial){
            int res=judgeSpecial(arg,OUT,SPECIAL_OUT,timeLimit);
            if(res){
                runResArr[i].status=ACCECPT;
                *passNum+=1;
            }else{
                runResArr[i].status=WRONG_ANSWER;
                return WRONG_ANSWER;
            }
        }else{
            char expectFilename[15];
            sprintf(expectFilename, "%d.%s",i,arg);
            if(contentEqual(OUT,expectFilename)){
                runResArr[i].status=ACCECPT;
                *passNum+=1;
            }else{
                runResArr[i].status=WRONG_ANSWER;
                return WRONG_ANSWER;
            }
        }
        
    }
    return ACCECPT;
}

int main(int argc,char **argv) {
    // 9个入参，其中第一个是当前c代码文件名，后面8个自己输入的
    if(argc!=9){
        printf("参数错误");
        return 1;
    }  
    char * codeFilename = argv[1];
    int isSpecial = atoi(argv[2]);
    int testCaseNum = atoi(argv[3]);
    int timeLimit = atoi(argv[4]);
    int memoryLimit = atoi(argv[5]);
    char * inputFileSuffix = argv[6];
    Result runResArr[testCaseNum];
    Result compileRes;
    int passNum=0;
    for(int i=0;i<testCaseNum;i++){
        runResArr[i].status = -1;
        runResArr[i].log = "";
        runResArr[i].timeUsed = 0;
        runResArr[i].memoryUsed = 0;
    }
    int status = judge(codeFilename,testCaseNum,isSpecial,inputFileSuffix,argv[7],timeLimit,memoryLimit,&compileRes,runResArr,&passNum);
    char * resultFile = argv[8];
    res2json(resultFile,&compileRes,runResArr,testCaseNum);
    remove(EXE);
    remove(OUT);
    remove(SPECIAL_EXE);
    remove(SPECIAL_OUT);
    //把自己的exe删掉
    remove(argv[0]);
    return 0;
}