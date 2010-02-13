#include <iostream>
using namespace std;

#include <QtTest/QtTest>
#include <QList>
#include "utility/unit_tests/Test_MiscFuncs.h"
#include "utility/unit_tests/Test_Point2D.h"
#include "utility/unit_tests/Test_Vector2D.h"

int main(int argc, char** argv) {
  QObject* tmpObj;
  QList<QObject*> testObjs;

  // Add test objects to the list.
  tmpObj = new Test_MiscFuncs();
  if(tmpObj != NULL)
    testObjs.append(tmpObj);

  tmpObj = new Test_Point2D();
  if(tmpObj != NULL)
    testObjs.append(tmpObj);

  tmpObj = new Test_Vector2D();
  if(tmpObj != NULL)
    testObjs.append(tmpObj);
  
  // Iterate through tests, incrementing
  // retVal for every failure
  int retVal = 0;
  for(int i=0; i<testObjs.size(); i++) {
    tmpObj = testObjs.at(i);
    if(tmpObj != NULL) {
      retVal += QTest::qExec(tmpObj, argc, argv);
    }
  }

  cout<<"=---> Total Tests Failed: "<<retVal<<endl;
  return retVal;
}
