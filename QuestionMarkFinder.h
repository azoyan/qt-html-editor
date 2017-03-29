#ifndef QUESTIONMARKFINDER_H
#define QUESTIONMARKFINDER_H

#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QTextEdit>
#include <QTextBrowser>
#include <QStringList>
#include <QLineEdit>
#include <QTabBar>
#include <QTabWidget>
#include "DocumentWidget.h"
#include <QList>

class QuestionMarkFinder : public QWidget {
  Q_OBJECT
public:
  QuestionMarkFinder(QWidget* parent = 0);
private:
  using DocumentWidgets = QList<DocumentWidget*>;
  void setup();
  void setupGui();
  void setupShortcuts();
  void setupTabBehavior();

private slots:
  void openDocuments();
  void openIfContain();
  DocumentWidgets load();
private:
  QWidget* createToolButtons();
  QWidget* createTabWidget();
  QWidget* createFileSystemView();
private slots:
  void addDefaultTab();



private:
  QPushButton*  mOpenButton;  
  QPushButton*  mOpenFolderButton;
  QTabWidget*   mTabs;
};

#endif // QUESTIONMARKFINDER_H
