#include "QuestionMarkFinder.h"
#include <QLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include <QString>
#include <QByteArray>
#include "DocumentWidget.h"
#include <QShortcut>
#include <QInputDialog>
#include <Global.h>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QSyntaxHighlighter>

QuestionMarkFinder::QuestionMarkFinder(QWidget *parent)
: QWidget(parent)
, mOpenButton           (new QPushButton(QIcon(":/openInBrowser.png"),""))
, mOpenFolderButton     (new QPushButton(QIcon(":/openFolder.png"),""))
, mTabs                 (new QTabWidget())
{  
  setup();
}

void QuestionMarkFinder::setup() {
  setupGui();
  setupShortcuts();
}

void QuestionMarkFinder::setupGui() {
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(createToolButtons());
  mainLayout->addWidget(createTabWidget());
//  mainLayout->addWidget(createFileSystemView());
  mainLayout->setMargin(0);

  setupTabBehavior();
}

void QuestionMarkFinder::setupShortcuts() {
  QShortcut* openShortcut = new QShortcut(QKeySequence("Ctrl+o"), this);
  QShortcut* openIfExistShortcut = new QShortcut(QKeySequence("Ctrl+Shift+o"), this);

  connect(openShortcut,        &QShortcut::activated, this, &QuestionMarkFinder::openDocuments);
  connect(openIfExistShortcut, &QShortcut::activated, this, &QuestionMarkFinder::openIfContain);
}

void QuestionMarkFinder::setupTabBehavior() {
  mTabs->setTabsClosable(true);
  connect(mTabs, &QTabWidget::tabCloseRequested, [this](int index) { mTabs->removeTab(index);} );
  connect(mTabs, &QTabWidget::tabCloseRequested, this, &QuestionMarkFinder::addDefaultTab );
}

void QuestionMarkFinder::openDocuments() {
  DocumentWidgets documents = load();
  for (auto doc : documents) mTabs->addTab(doc, doc->name());
}

void QuestionMarkFinder::openIfContain() {
  bool ok;
  QInputDialog* inputWidget = new QInputDialog();
  this->layout()->addWidget(inputWidget);
  QString neededText = inputWidget->getText(this, tr("Smart open")
                                          ,tr("Open files that containing this substring:"), QLineEdit::Normal,"your text", &ok);
  if (ok && !neededText.isEmpty()) {
    auto documents = load();
    for (auto doc : documents) {
    int lastIndex = doc->findText(neededText);
    qDebug() << lastIndex;
      if (lastIndex != Search::NotFound) {
        mTabs->addTab(doc, doc->name());
      }
      else delete doc;
    }
  }
}

QuestionMarkFinder::DocumentWidgets QuestionMarkFinder::load() {
  QStringList fileNames = QFileDialog::getOpenFileNames(this, "", "", "HTML files (*.html *.htm)");
  DocumentWidgets result;
  for (const QString& fileName : fileNames) {
    result.push_back(new DocumentWidget(fileName, this));
  }
  return result;
}

QWidget* QuestionMarkFinder::createToolButtons(){
  QWidget* widget = new QWidget(this);
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->addWidget(mOpenButton);
  layout->addWidget(mOpenFolderButton);
  layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
  return widget;
}

QWidget* QuestionMarkFinder::createTabWidget() {
  QWidget* widget = new QWidget(this);
  QVBoxLayout* vBoxLayout = new QVBoxLayout(widget);
  vBoxLayout->addWidget(mTabs);  
  addDefaultTab();
  vBoxLayout->setMargin(0);  
  return widget;
}

QWidget* QuestionMarkFinder::createFileSystemView() {
  QListView* view = new QListView(this);
  QFileSystemModel* model = new QFileSystemModel();
  QStringList filters;
  filters << "*.html" << "*.htm";
  model->setRootPath(QDir::currentPath());

  model->setNameFilterDisables(false);
  model->setNameFilters(filters);

  view->setModel(model);
  view->setRootIndex(model->index(QDir::currentPath()));

  connect(view, &QListView::doubleClicked, [view] (QModelIndex index) { view->setRootIndex(index); } );
  //connect(view, &QListView::, [view] (QModelIndex index) { view->setRootIndex(index);} );

  return view;
}

void QuestionMarkFinder::addDefaultTab() {
  if (mTabs->count() < 1) mTabs->addTab(new DocumentWidget(this), "blank:info");
}

