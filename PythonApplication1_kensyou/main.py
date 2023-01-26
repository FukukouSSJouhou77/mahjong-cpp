import os
import sys
import time
import copy
import numpy as np
import random
from MahjongCpp import mahjongcpp
os.system("")




def ImageCreation(x):
    # 2Dのデータ
    TwoDimensionsdata=[]
    OneDisposeData=x
    # 累積データ
    AccumulationData=[]
    data=[[0 for i in range(38)]for i2 in range(38)]
    AccumulationData.append(copy.deepcopy(OneDisposeData[0]))
    # 初回はスキップ
    FirstTime=0

    for i in OneDisposeData:
        if  not FirstTime == 0:
            for i2 in AccumulationData:
                data[i2+1][i+1] = data[i2+1][i+1]+1
                TwoDimensionsdata.append(copy.deepcopy(data))
            AccumulationData.append(i)
        else:
            data[0][i+1] = data[0][i+1] + 1
            TwoDimensionsdata.append(copy.deepcopy(data))
            FirstTime=1
    return TwoDimensionsdata


def LabelCalculation(x):
    hai=x
    lavel = [0 for i in range(37)]
    for i2 in hai:
        lavel[i2] = lavel[i2] + 1
    return lavel

GameNumber=1
DatasetPathNumber=0


data=[]
label=[]
for i in range(GameNumber):
    yama=[]
    for i2 in range(4):
        for i3 in range(34):
           yama.append(i3)
    yama.remove(4)
    yama.append(34)
    yama.remove(13)
    yama.append(35)
    yama.remove(22)
    yama.append(36)
    tehai=[]
    for i2 in range(4):
        Onetehai=[]
        for i3 in range(13):
            choice=random.choices(yama)
            choice=np.squeeze(choice)
            Onetehai.append(choice)
            yama.remove(choice)
        tehai.append(Onetehai)
    sutehai=[[] for i in range(4)]
    for i2 in range(18):
        for i3 in range(4):
            #if not tehai[i3] =agari

            hiki=random.choices(yama)
            hiki = np.squeeze(hiki)
            tehai[i3].append(hiki)
            yama.remove(hiki)
            # choice=mahjong-cpp
            choice=mahjongcpp(tehai[i3],i2+1)
            if(choice == -1):
                print("ERR")
                exit()
            choice=np.squeeze(choice)

            tehai[i3].remove(choice)
            sutehai[i3].append(choice)
            # np.save('D:\\CNNmahjongImageData\\' + str(DatasetPathNumber), np.array(ImageCreation(sutehai[i3])))
            # np.save('D:\\CNNmahjongLabels\\' + str(DatasetPathNumber), np.array(LabelCalculation(tehai[i3])))
            data.extend(ImageCreation(sutehai[i3]))
            label.append(LabelCalculation(tehai[i3]))

            DatasetPathNumber=DatasetPathNumber+1
for i in data[10]:
    print(i)
print("")
print(label[10])
