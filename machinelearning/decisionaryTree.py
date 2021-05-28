from math import log
import operator

def createDataSet1():    # 数据
    dataSet =  [[64, '青', '高', '否', '良', '否'],
                [64, '青', '高', '否', '优', '否'],
                [128, '中', '高', '否', '良', '买'],
                [60, '老', '中', '否', '良', '买'],
                [64, '老', '低', '是', '良', '买'],
                [64, '老', '低', '是', '优', '否'],
                [64, '中', '低', '是', '优', '买'],
                [128, '青', '中', '否', '良', '否'],
                [64, '青', '低', '是', '良', '买'],
                [132, '老', '中', '是', '良', '买'],
                [64, '青', '中', '是', '优', '买'],
                [32, '中', '中', '否', '优', '买'],
                [32, '中', '高', '是', '良', '买'],
                [64, '老', '中', '否', '优', '否'],
               ]
    labels = ['计数', '年龄', '收入', '学生', '信用', '是否购买']
    return dataSet, labels

# 计算数据的熵(entropy)
def calcEntropy(dataSet):
    # numEntries = len(dataSet)  # 数据条数
    num = len(dataSet)  # 数据条数
    numEntries = 0 # 数据总数
    labelCounts = {}
    for featVec in dataSet:
        numEntries += featVec[0]
        currentLabel = featVec[-1]  # 标签
        if currentLabel not in labelCounts.keys():
            labelCounts[currentLabel] = 0
        labelCounts[currentLabel] += featVec[0]  # 统计有多少个类以及每个类的数量
    Entropy = 0
    for key in labelCounts:
        prob = float(labelCounts[key])/float(numEntries)  # 计算权重
        Entropy -= prob*log(prob, 2)  # 计算每个类的熵值
    return Entropy

def calcConditionalEntropy(dataSet,i,featList,uniqueVals):
    ce = 0.0
    for value in uniqueVals:
        subDataSet = splitDataSet(dataSet,i,value)
        prob = len(subDataSet) / float(len(dataSet)) # 极大似然估计概率
        ce += prob * calcEntropy(subDataSet) #∑pH(Y|X=xi) 条件熵的计算
    print(labels[i],"的条件熵",ce)
    return ce

def calcInformationGain(dataSet,baseEntropy,i):
    featList = [example[i] for example in dataSet] # 第i维特征列表
    uniqueVals = set(featList) # 换成集合 - 集合中的每个元素不重复
    newEntropy = calcConditionalEntropy(dataSet,i,featList,uniqueVals)#计算条件熵，
    infoGain = baseEntropy - newEntropy # 信息增益 = 信息熵 - 条件熵
    print(labels[i],"的信息增益",infoGain)
    return infoGain

def splitDataSet(dataSet,axis,value): # 按某个特征分类后的数据
    retDataSet = []
    for featVec in dataSet:
        if featVec[axis] == value:
            reducedFeatVec = featVec[:axis]
            reducedFeatVec.extend(featVec[axis+1:])
            retDataSet.append(reducedFeatVec)
    return retDataSet

# 选择最优的分类特征
def chooseBestFeatureToSplit(dataSet):
    numFeatures = len(dataSet[0])-1
    baseEntropy = calcEntropy(dataSet)  # 原始的熵
    bestInfoGain = 0
    bestFeature = -1
    for i in range(1,numFeatures):
        infoGain = calcInformationGain(dataSet,baseEntropy,i)
        if(infoGain > bestInfoGain):   # 若按某特征划分后，熵值减少的最大，则次特征为最优分类特征
            bestInfoGain = infoGain
            bestFeature = i
    return bestFeature

# 按分类后类别数量排序；
def majorityCnt(classList):
    classCount = {}
    for vote in classList:
        if vote not in classCount.keys():
            classCount[vote] = 0
        classCount[vote] += 1

    print(classCount)
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]

# def majorityCnt(classList):
#     classCount = {}
#     print(classList)
#     for vote in classList:
#         if vote[1] not in classCount.keys():
#             classCount[vote[1]] = 0
#         classCount[vote[1]] += vote[0]
#     sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
#     print("sorted",sortedClassCount)
#     return sortedClassCount[0][0]

def createTree(dataSet,labels):

    classList = [example[-1] for example in dataSet]
    classList2 = [[example[0],example[-1]] for example in dataSet]
    # print(classList)
    print(classList.count(classList[0]))
    print(len(classList))
    print("----------")
    # print(classList2)
    lenll = 0
    i = 0
    for classL in classList2:
        if classL[1] == classList2[0][1]:
            lenll +=1
        i +=1
    print(lenll)
    # print(classList2.count(classList2[0][1]))
    print(len(classList2))

    if classList2.count(classList2[0][1]) == len(classList2[0]):
        print("1success")

    if classList.count(classList[0]) == len(classList):
        print("fanhui1")
        return classList[0]
    if len(dataSet[0]) == 1:
        print("fanghui222")
        return majorityCnt(classList)

    # classList2 = [[example[0],example[-1]] for example in dataSet]
    
    # if classList.count(classList[0][1]) == len(classList[0]):
    #     print(22)
    #     return classList[0][1]
    # if len(dataSet[0]) == 1:
    #     print("major")
    #     return majorityCnt(classList[0])  

    print("*******************")
    bestFeat = chooseBestFeatureToSplit(dataSet)  # 选择最优特征
    bestFeatLabel = labels[bestFeat]
    print("信息增益最大的特征是",bestFeatLabel)
    myTree = {bestFeatLabel: {}}  # 分类结果以字典形式保存
    del(labels[bestFeat])
    featValues = [example[bestFeat] for example in dataSet]
    uniqueVals = set(featValues)
    for value in uniqueVals:
        subLabels = labels[:]
        myTree[bestFeatLabel][value]=createTree(splitDataSet\
                            (dataSet,bestFeat,value),subLabels)
    return myTree



if __name__=='__main__':
    dataSet, labels=createDataSet1()  # 创造示列数据
    print(createTree(dataSet, labels))  # 输出决策树模型结果
