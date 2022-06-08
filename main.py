import math

def addnumber(liste):
    index=int(input("in which position do you wish to insert the number"))
    value=int(input("which value do you wish to insert"))
    liste.insert(index-1, value)

def checkvalue(liste):
    value=int(input("which value do you wish to check"))
    if value in liste:
        print("yes")
    else:
        print("no")

def removenumber(liste):
    value=int(input("which value do you wish to remove"))
    if value in liste:
        liste=liste.remove(value)
    else:
        print("value does not exist in liste")

def sliceliste(liste):
    index1=int(input("slicing start : "))
    index2=int(input("slicing stop : "))
    print(liste[index1:index2])


def exo1():
    liste = []
    nb=int(input("entrer nombre de valeur souhaité"))
    for i in range (0,nb,1):
        liste.append(int(input("entrer valeur")))
    sum = sum(liste)
    average = sum/len(liste)
    product=1
    for i in liste:
        product=product*i
    print(sum, product, average)
    sliceliste(liste)


def addperson(liste):
    name=input("name")
    nb2=int(input("how many activity"))
    activities = set()
    for i in range(0,nb2,1):
        activity=input("activity")
        activities.add(activity)
    liste.append((name, activities))

def nbpeople(liste):
    name=input("an activity")
    nb=0
    for i in liste:
        if name in activities:
            nb=nb+1
    return nb

def pointcommun (liste):
    name1=input("a name")
    name2=input("a name")
    for i in liste:
        if name1 == i[0]:
            activities1=i[1]
        if name2 == i[0]:
            activities2=i[1]
    print(activities1 & activities2)

def deleteactivity(liste):
    name=input("name")
    activity =input("activity you wish to remove") 
    for i in liste:
        if name==i[0]:
            i[1].remove(activity)

def removeuser (liste):
    name=input("name")
    for i in liste:
        if name==i[0]:
            liste.remove(i)

def replaceactivity(liste):
    name=input("name")
    activity1 =input("activity you wish to remove") 
    activity2 =input("activity you wish to add") 
    for i in liste:
        if name==i[0]:
            i[1].remove(activity1)
            i[1].add(activity2)

def exo2():
    liste = []
    nb=int(input("how many person"))
    for i in range(0,nb,1):
        name=input("name")
        nb2=int(input("how many activity"))
        activities = set()
        for i in range(0,nb2,1):
            activity=input("activity")
            activities.add(activity)
        liste.append((name, activities))
    print(liste)

Cproject={"demonstration":0, "code":0, "presentation":0, "documentation":0}
pythonproject={"demonstration":0, "code":0, "presentation":0, "documentation":0}
student = []
for i in range(0,10):
    student.append({
        "python": pythonproject,
        "c": Cproject
    })
    student[i]["python"]["demonstration"]=float(input("entrer note"))

 
