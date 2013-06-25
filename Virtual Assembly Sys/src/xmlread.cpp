// xmlread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "tinyxml.h"
#include <string>
#include <list>
#include <Windows.h>
#include "Group.h"
#include "PartInfo.h"
#include "AccessPoints.h"
#include <Inventor/Win/SoWin.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoInput.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h>
using namespace std;
#define MAXLENGTH 1000



SoSeparator *
readFile(const char *filename)
{
	// Open the input file
	SoInput mySceneInput;
	if (!mySceneInput.openFile(filename)) {
		fprintf(stderr, "Cannot open file %s\n", filename);
		return NULL;
	}
	
	// Read the whole file into the database
	SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
	if (myGraph == NULL) {
		fprintf(stderr, "Problem reading file\n");
		return NULL;
	} 
	
	mySceneInput.closeFile();
	return myGraph;
}



float whichIsLess(float a, float b)
{
	if (a > b)
	{
		return b;
	}
	else return a;
}
list<string> readDir(string path)
{
	WIN32_FIND_DATA findData;
	HANDLE hHandle = NULL;
	string filePathName;
	string fullPathName;
	
	filePathName = path;
	filePathName += "\\*.*";
	
	list<string> pathList;
	
	hHandle = FindFirstFile(filePathName.data(), &findData);
	
	if ( hHandle == INVALID_HANDLE_VALUE )
	{
		return 1;
	}
	do 
	{
		if ( "." == (string)findData.cFileName || ".." == (string)findData.cFileName )
		{
			continue;
		}
		
		fullPathName = path;
		fullPathName += "\\";
		fullPathName += findData.cFileName;
		
		string str = fullPathName.substr(fullPathName.length() - 3); //filt the .xml file
		if ( str == "xml")
		{
			pathList.push_back(fullPathName);
		}   
	} while ( FindNextFile(hHandle, &findData) );
	FindClose(hHandle);
	/*
	list<string>::reverse_iterator rpos;
	for ( rpos = pathList.rbegin(); rpos != pathList.rend(); rpos++)
	{
		cout << *rpos << endl;
	}
	*/
	return pathList;
}

bool isInside(point testPoint, point rectX, point rectY)
{
	if (testPoint.x > rectX.x && testPoint.x < rectY.x && testPoint.y < rectX.y && testPoint.y > rectY.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isConflict(point firstUL, point firstDR, point secondUL, point secondDR)
{
	point firstUR,secondUR;
	firstUR.setValue(firstDR.x, firstUL.y);
	secondUR.setValue(secondDR.x, secondUL.y);
	if (isInside(firstUL, secondUL, secondDR)||isInside(firstDR, secondUL, secondDR)||isInside(firstUR, secondUL, secondDR))
	{
		return true;
	}
	if (isInside(secondUL, firstUL, firstDR)||isInside(secondDR, firstUL, firstDR)||isInside(secondUR, firstUL, firstDR))
	{
		return true;
	}
	else return false;
}


int main(int argc, char* argv[])
{
	Group *blankHeader = new Group();
	string path = "c:\\xml";

	list<string> filePath;
	filePath = readDir(path);

	list<string>::reverse_iterator rpos;
	for ( rpos = filePath.rbegin(); rpos != filePath.rend(); rpos++)
	{
		cout << *rpos << endl;
		
		string fileXml = *rpos;
		string fileVrml = fileXml.substr(0,fileXml.length()-3);
		fileVrml += "WRL";
		
		TiXmlDocument doc( fileXml.c_str() );
		doc.LoadFile();
		
		if ( !doc.Parse( fileXml.c_str() ) )
		{
			cout << doc.ErrorDesc() << endl;
		}	
		
		TiXmlElement *root = doc.RootElement();
		
		TiXmlElement *pEle = root;
		for (pEle = root->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
		{
			if (!strcmp(pEle->Value(),"cylinder")) {
				//cout << "current path : " << pEle->Value() <<endl;
				//cout << "radius : " << pEle->FirstChildElement()->GetText() << endl;
				float fileHeight, fileRadius;
				fileRadius = atof(pEle->FirstChildElement()->GetText());
				pEle = pEle->FirstChildElement();
				fileHeight = atof(pEle->NextSiblingElement()->GetText());

				//cout << fileRadius << " " << fileHeight << endl;
				//cout << "height : " << pEle->NextSiblingElement()->GetText() << endl;
				bool isExist = false;
				for (Group *pGp = blankHeader->next; pGp; pGp = pGp->next)
				{
					if (whichIsLess(2*fileRadius, fileHeight) < pGp->getHeight())
					{
						break;
					}
					if (pGp->getPartHeight() == fileHeight && pGp->getPartRadius() == fileRadius)
					{
						PartInfo *newPart = new PartInfo(pGp->getAmount(), fileRadius, fileHeight, (char*)fileXml.c_str(), (char*)fileVrml.c_str());
						pGp->updateGroup(newPart);
						isExist = true;
					}
				}
				if (!isExist)
				{
					PartInfo *newPart = new PartInfo(0, fileRadius, fileHeight, (char*)fileXml.c_str(), (char*)fileVrml.c_str());
					Group *newGroup = new Group(newPart);
					//reArrange
					if (blankHeader->next == NULL)
					{
						blankHeader->next = newGroup;
					}
 					else if (newGroup->getHeight() <= blankHeader->next->getHeight())
					{
						newGroup->next = blankHeader->next;
						blankHeader->next = newGroup;
					}
					else 
					{
						for (Group *pGp = blankHeader->next; pGp; pGp = pGp->next)
						{
							if (newGroup->getHeight() >= pGp->getHeight() && pGp->next == NULL)
							{
								newGroup->next = pGp->next;
								pGp->next = newGroup;
								break;
							}
							else if (newGroup->getHeight() >= pGp->getHeight() && newGroup->getHeight() <= pGp->next->getHeight())
							{
								//insert
								newGroup->next = pGp->next;
								pGp->next = newGroup;
								break;
							}
						}
					}
				}
				break;
			}//end if
		}//end for read one file
		


	}//end for read all files

		//test result
		

/*
				for (Group *pGp = blankHeader->next; pGp; pGp = pGp->next)
				{
					cout << "name: " << pGp->first->xml_dir_path
						<< " H:" << pGp->getHeight() 
						<< " L:" << pGp->getLonger() 
						<< " S:" << pGp->getShorter()
						<< " Amount: " << pGp->getAmount()
						<< " pH:" << pGp->getPartHeight()
						<< " pR:" << pGp->getPartRadius()
						<< endl;
				}*/
		
	//=========================
	//auto arrange

	AccessPoints *origins = new AccessPoints();
	origins->ap.setValue(0,0);
	origins->next = NULL;
	origins->front = NULL;

	int processNo = 1;
	for (Group *pGp = blankHeader->next; pGp; pGp = pGp->next, processNo++)
	{
		//first one
		if (pGp == blankHeader->next)
		{
			pGp->upLeft.setValue(0, pGp->getShorter());
			pGp->downRight.setValue(pGp->getLonger(), 0);
			AccessPoints *ta = new AccessPoints();
			AccessPoints *tb = new AccessPoints();
			ta->ap.setValue(pGp->getLonger(),0);//, pGp->getShorter());
			origins->next = ta;
			ta->front = origins;
			tb->ap.setValue(0,pGp->getShorter());//pGp->getLonger(), 0);
			ta->next = tb;
			tb->front = ta;
			tb->next = NULL;
		}
		//other ones
		else
		{
			for (AccessPoints *pAp = origins->next; pAp; pAp = pAp->next)
			{
				//EVERY AP
				point rangeUL,rangeDR;
				rangeUL.setValue(pAp->ap.x, pAp->ap.y + pGp->getShorter());
				rangeDR.setValue(pAp->ap.x + pGp->getLonger(), pAp->ap.y);
				if (rangeDR.x > MAXLENGTH)
				{
					continue;
				}
				else {
					int j = 1;
					bool conflictFlag = false, isULAP = false, isDRAP = false;
					for (Group *pCT = blankHeader->next; pCT && j < processNo;  pCT = pCT->next, j++)
					{
						//CONFLICT TEST
						if (isConflict(rangeUL,rangeDR,pCT->upLeft,pCT->downRight))
						{
							conflictFlag = true;
							break;
						}
						if ((rangeUL.x == pCT->downRight.x && rangeUL.y < pCT->upLeft.y && rangeUL.y > pCT->downRight.y)|| rangeUL.x == 0 )
						{
							isULAP = true;
						}
						if ((rangeDR.y == pCT->upLeft.y && rangeDR.x < pCT->downRight.x && rangeDR.x > pCT->upLeft.x )||(rangeDR.y == 0 && rangeDR.x < MAXLENGTH))
						{
							isDRAP = true;
						}
					}
					if (!conflictFlag)
					{
						//delete current ap
						if (pAp->next != NULL)
						{
							pAp->front->next = pAp->next;
							pAp->next->front = pAp->front;
						}
						else 
						{
							pAp->front->next = NULL;
						}
						if (isDRAP)
						{
							//insert DR
							for (AccessPoints *pIp = origins->next; pIp; pIp = pIp->next)
							{
								if (rangeDR.y <= pIp->ap.y)
								{
									AccessPoints *ip = new AccessPoints();
									ip->ap.x = rangeDR.x;
									ip->ap.y = rangeDR.y;
									pIp->front->next = ip;
									ip->front = pIp->front;
									ip->next = pIp;
									pIp->front = ip;
									break;
								}
							}
						}
						if (isULAP)
						{
							//insert UL(may have problems, solved)
							for (AccessPoints *pIp = origins->next; pIp; pIp = pIp->next)
							{
								if (pIp->next==NULL&&rangeUL.y>pIp->ap.y)
								{
									AccessPoints *ip = new AccessPoints();
									ip->ap.x = rangeUL.x;
									ip->ap.y = rangeUL.y;
									pIp->next = ip;
									ip->next = NULL;
									ip->front = pIp;
									break;

								}
								if (rangeUL.y < pIp->ap.y)
								{
									AccessPoints *ip = new AccessPoints();
									ip->ap.x = rangeUL.x;
									ip->ap.y = rangeUL.y;
									pIp->front->next = ip;
									ip->front = pIp->front;
									ip->next = pIp;
									pIp->front = ip;
									break;
								}
							}
						}
						
						//update group
						pGp->upLeft.setValue(rangeUL.x, rangeUL.y);
						pGp->downRight.setValue(rangeDR.x, rangeDR.y);
						break;

					}
					
				}

			}

		}
	}

	HWND myWindow = SoWin::init(argv[0]);
	
	SoSeparator *root = new SoSeparator;



	for (pGp = blankHeader->next; pGp; pGp = pGp->next, processNo++)
	{

		for (PartInfo *pPi = pGp->first; pPi; pPi=pPi->next)
		{
		
		SoNode *fileNode;
		SoSeparator *sepNode = new SoSeparator;
		SoTransform *zeroNode = new SoTransform;
		SoTransform *artNode = new SoTransform;
		SoTransform *adjNode = new SoTransform;

		SoInput mySceneInput;
		string tmpvrml = pPi->xml_dir_path;
		tmpvrml = tmpvrml.substr(0,tmpvrml.length()-3) + "wrl";
		if (!mySceneInput.openFile(tmpvrml.c_str())) {
			return NULL;
		}
		cout<<tmpvrml<<endl;
		if(SoDB::read(&mySceneInput, fileNode));
		sepNode->addChild(fileNode);

		SoWinExaminerViewer *tmpViewer = new SoWinExaminerViewer(myWindow);
		
		tmpViewer->setSceneGraph(sepNode);
		
		
		SoSearchAction * searchaction = new SoSearchAction;
		searchaction->setNode(fileNode);
		searchaction->apply(sepNode);
		
		SoPath * path = searchaction->getPath();
		
		const SbViewportRegion tmpViewport = tmpViewer->getViewportRegion();
		SoGetMatrixAction * getmatrixaction = new SoGetMatrixAction(tmpViewport);
		getmatrixaction->apply(path);
		SbMatrix transformation = getmatrixaction->getMatrix();
		SbVec3f tranlation;
		SbRotation rotation;
		SbVec3f scalevector;
		SbRotation scaleorientation;
		transformation.getTransform(tranlation,rotation,scalevector,scaleorientation);
		tmpViewer->~SoWinExaminerViewer();
		float xo,yo,zo;
		tranlation.getValue(xo,yo,zo);
		float xg,yg;
		xg=pGp->upLeft.x;
		yg=pGp->downRight.y;
		float pl = pGp->getLonger()/pGp->getAmount();


/*
				if (pGp->putdown&&pGp->rotate)
				{
					adjNode->rotation.setValue(0.0,0.0,1.0,-90)
				
				}	*/
				if(pGp->putdown&&!pGp->rotate)
				{
					SbRotation rot(SbVec3f(0,1.0,0), SbVec3f(0,0,-1.0));
					artNode->rotation = rot;
				}
				else if (pGp->rotate)
				{
					SbRotation rot(SbVec3f(0,0,-1.0), SbVec3f(1.0,0,0));
					artNode->rotation = rot;
				}
		


		
		zeroNode->translation.setValue(-xo,-yo,-zo);
		adjNode->translation.setValue(xg+(pPi->id-1)*pl+pPi->getRadius(),0,-yg-pPi->getRadius());


		SoSeparator *testcube = new SoSeparator;
		SoDrawStyle *style = new SoDrawStyle;
		SoTransform *cubets = new SoTransform;
		SoMaterial *mat1 = new SoMaterial;
		mat1->diffuseColor.setValue(1,0,0);
		style->style.setValue(SoDrawStyle::LINES);
		cubets->scaleFactor.setValue(10,10,10);
		testcube->addChild(mat1);
		testcube->addChild(cubets);
		testcube->addChild(style);
		//testcube->addChild(new SoCube);


		sepNode->insertChild(adjNode,0);

		sepNode->insertChild(testcube,1);

		sepNode->insertChild(artNode,2);
		sepNode->insertChild(zeroNode,3);
		
		cout<<xg+(pPi->id-1)*pl<<","<<0<<","<<yg<<";"<<xo<<","<<yo<<","<<zo<<endl;



		root->addChild(sepNode);
		}
	}




	SoWinExaminerViewer *myViewer = new SoWinExaminerViewer(myWindow);

	myViewer->setSceneGraph(root);
	myViewer->setTitle("File Reader");
	
	myViewer->show();
	myViewer->viewAll();
	
	SoWin::show(myWindow);
	SoWin::mainLoop();

	system("PAUSE");
	
	return 0 ;
}





















