#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QFile>
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include "Finder.h"

#include "SearchWidget.h"

class DocumentWidget : public QWidget {
  Q_OBJECT
public:
  explicit DocumentWidget(QWidget* parent = 0);
  explicit DocumentWidget(const QString& fileName, QWidget *parent = 0);

public:
  void setText(const QString& text = DefaultText());
  QString name() const;
  QString fullPath() const;
  int findText(const QString& text);

private:
  void setup();
  void setupGui();
  void setupShortcuts();

private:
  using SymbolPositions = QList<int>;
  QTextEdit*    mTextEdit;
  QTextBrowser* mBrowser;
  QPushButton*  mSaveButton;
  QLabel*       mErrorStatus;
  QLabel*       mRow;
  QLabel*       mColumn;
  QPushButton*  mSearchButton;
  QLineEdit*    mWantedSymbolLineEdit;
  SearchWidget* mSearchWidget;
private:
  QString mFileName;
private:
  void showText(const QString& text);
  Finder* mFinder;

  void markSymbols(SymbolPositions positions);

private:
  QWidget* createSearchWidget();
  QWidget* createEditorWidget();
private slots:
  void showSearchWidget();

  static QString DefaultText();
private slots:
  void saveFile(bool);
};

#endif
