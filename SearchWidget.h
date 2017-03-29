#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SearchWidget : public QWidget {
  Q_OBJECT
public:
  explicit SearchWidget(QWidget *parent = 0, bool inDirs = false);

public:
  void setReplaceEnabled(bool enable = true);

private:
  QWidget* createSearchPanel();
  QWidget* createReplacePanel();
  void refresh();

private:
  QLineEdit*   mSearchWordLineEdit;
  QLineEdit*   mReplaceWordLineEdit;
  QPushButton* mSearchButton;
  QPushButton* mReplaceButton;
private:
  bool mInDirs;
};

#endif // SEARCHWIDGET_H
