# introduction

- 自制的 c 语言判题程序，将与编译器一起打包成 docker 容器，供上层 web 判题服务使用。目前只支持 gcc 编译，可以手动修改源码`util/runcode.c`中的 `compile` 和 `_runExe` 函数，以判题 java/python 等语言
- 支持特判(special judge，即允许 1 个测试用例有多种正确答案,如输出是偶数即 AC 的情况)
- 判题结果通过 cjson 库转为 json 字符串并写入本地，基于 docker volume 让容器外的 web 服务获取
- `useless`是我一开始用 shell 写了一半的，发现 shell 又臭又烂，后改换 c，但还是留作纪念
- 在子进程中通过`setrlimit`限制进程的使用内存和运行时间，父线程等待子线程运行完(或异常终止)后查看其信号量，判断异常情况是 TLE 还是 MLE 还是 RE
- 获取进程内存峰值和运行时间用的是`rusage`，不知道准不准确
- 比较`期望输出`和`实际输出`文件时，实现了忽略行尾空格和最后额外的换行
- 作为一名 java/go/rust 选手，第一次拿 c 写这么相对底层的东西(unix 编程)，如有不合理之处欢迎批评与交流

# run

1. my os:`ubuntu 20.04`
2. install gcc 和 python3(python 是用来编写特判代码的)
3. put the code and input files and expected files or special file into root dir , at the same level as `judge.c`
4. just run the shell file `./run.sh`

the second line params means

1. the executable file
2. code that user just uploaded
3. isSpecialJudge ? 0:1
4. testCaseNum (2 testCaseNum example: 0.in,1.in -> 0.expect 1.expect,the suffix can be comtomized)
5. time limt(ms)
6. memory limit(kb)
7. inputFileSuffix
8. isSpecialJudge ? specialCodeFilename : expectFileSuffix
9. a json file represents the judge result

# TODO(只是写写，不会真的做的)

- 多线程一起判所有用例？
- 更准确的获取占用内存峰值
- 尽可能容易改写成其他语言的判题机
- 减少内存泄露(真心不会手动释放内存)
- 能判断出：段错误/输出超限/格式错误
- 多加注释
