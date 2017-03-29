#include "QuestionMarkFinder.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  //QTextCodec* codec = QTextCodec::("UTF-8");

//  QTextCodec::setCodecForLocale(codec);
  QuestionMarkFinder w;
  w.show();

  return a.exec();
}
