# BUAA-OS-2022 源码

Author : saltyfishyjk

## 作者简介

我是saltyfishyjk，欢迎访问我的个人站[saltyfishyjk'sBlog](https://saltyfishyjk.github.io)。

如果想了解更多关于我的信息，可以访问[saltyfishyjk简介](https://saltyfishyjk.github.io/about/)

## 课程简介

《操作系统》是北航计算机学院大二下学期核心专业课之一。除了理论课程外，还通过课程设计引导同学们实现一个小操作系统——MOS。本仓库即为笔者在2022春季学期的MOS课设源码。在笔者参加的学期中，一共有lab0-lab6七个lab，其中lab0为一次上机，lab1-lab5分别有2次上机，lab6没有上机。额外地，课程最后会有**挑战性任务**，笔者参加的学期有四种挑战性任务：lab2操作系统移植；lab4实现POSIX标准线程与信号量；lab5实现FAT文件系统；lab6实现更丰富的shell。

## 仓库简介

### 实验完成度

本仓库的MOS通过了lab0-lab6的所有课下评测；通过了lab1-lab5的所有强测；通过了除lab2-2之外的所有课上上机exam；通过了lab0,lab1-2,lab2-1,lab3-1,lab5-1一共5个Extra。

笔者的水平有限，代码也仅供参考。

### 分支目录

#### `lab[a]`

如：`lab3`

名为`lab[a]`的分支为课下分支。

#### `lab[a]-result`

如：`lab3-result`

`lab[a]`课下测试结果分支。

#### `lab[a]-[b]-exam`

如：`lab3-1-exam`

`lab[a]`的第`[b]`次上机的exam分支。

#### `lab[a]-[b]-Exam-result`

如：`lab3-1-Exam-result`

`lab[a]`第`[b]`次上机的exam评测结果分支。通过这一分支，你可以更具体地了解到笔者每一次提交的反馈信息。

#### `lab[a]-[b]-Extra`

如：`lab3-1-Extra`

`lab[a]`第`[b]`次上机的Extra分支。

#### `lab[a]-[b]-Extra-result`

如：`lab3-1-Extra-result`

`lab[a]`第`[b]`次上机的Extra评测结果分支。通过这一分支，你可以更具体地了解到笔者每一次提交的反馈信息。

#### `lab[a]-strong`

`lab[a]`强测分支。

#### `lab[a]-strong-result`

`lab[a]`强测结果分支。

#### `lab4-chcallenge-***`

笔者的lab4挑战性任务相关分支，具体含义如下：

- `lab4-challenge`：一次失败的尝试，倒在了错综复杂的OS迷宫里（哭
- `lab4-challenge-forkv1`：复刻学长代码，并加以优化，最终完成了该挑战性任务

#### `job-scheduling`

任务调度理论作业，本质上是用大模拟实现的。

## 博客

笔者在学习的过程中，也总结了一些个人心得和部分上机复盘，可以参考[saltyfishyjk的OS博客](https://saltyfishyjk.github.io/tags/#BUAA-OS)

## 后记

这份代码的完成，离不开老师、助教、学长、同学们的帮助，笔者代码能力和系统能力都算不上强，在课程的最后将源码上传GitHub是我能为这门课做出的最后贡献了。
