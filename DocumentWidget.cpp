#include <QDebug>
#include <QLayout>
#include <QSplitter>
#include <QShortcut>
#include <QStackedLayout>
#include "HtmlHighlighter.h"

#include "DocumentWidget.h"


DocumentWidget::DocumentWidget(const QString& fileName, QWidget *parent)
  : QWidget(parent)
  , mTextEdit             (new QTextEdit())
  , mBrowser              (new QTextBrowser())
  , mSaveButton           (new QPushButton("Сохранить"))
  , mErrorStatus          (new QLabel("Найдено: "))
  , mSearchButton         (new QPushButton(QIcon(":/search"),"Find"))
  , mWantedSymbolLineEdit (new QLineEdit(""))
  , mSearchWidget         (new SearchWidget(this))
  , mFileName             (fileName)
  , mFinder               (new Finder())
{
  setup();
  QFile file(fileName);
  bool ok = (!fileName.isEmpty()) && file.open(QIODevice::ReadOnly | QIODevice::Text);
  setText(ok ? file.readAll() : DefaultText());
  new HtmlHighlighter (mTextEdit->document());

}

void DocumentWidget::setText(const QString& text) {
  mTextEdit->setPlainText(text);
  mBrowser->setHtml(text);
  mFinder->setText(text);
}

QString DocumentWidget::name() const {
  return mFileName.section('/', -1);
}

QString DocumentWidget::fullPath() const {
  return mFileName;
}

int DocumentWidget::findText(const QString& text) {
  int result = mFinder->find(text);
  markSymbols(mFinder->symbolPositions());
  return result;
}

void DocumentWidget::setup() {
  setupGui();
  setupShortcuts();
}

void DocumentWidget::setupGui() {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(createEditorWidget());
  layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum));
  layout->addWidget(mSearchWidget);
  this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  connect(mSaveButton, SIGNAL(clicked(bool)),  this, SLOT(saveFile(bool)));
  connect(mTextEdit, &QTextEdit::textChanged, [this]{ mBrowser->setHtml(mTextEdit->toPlainText()); } );
}

void DocumentWidget::setupShortcuts() {
  QShortcut* findShortcut = new QShortcut(QKeySequence("Ctrl+f"), this);
  connect(findShortcut, &QShortcut::activated, this, &DocumentWidget::showSearchWidget);
}

DocumentWidget::DocumentWidget(QWidget* parent) : DocumentWidget("", parent) { }


void DocumentWidget::showText(const QString& text) {
  setText(text);
  mTextEdit->setFocus();
}

void DocumentWidget::markSymbols(DocumentWidget::SymbolPositions positions) {
  if (positions.isEmpty()) return;
  QTextCursor cursor = mTextEdit->textCursor();
  cursor.setPosition(positions.front(), QTextCursor::MoveAnchor);
  cursor.setPosition(cursor.position() + 1, QTextCursor::KeepAnchor);
  mTextEdit->setTextCursor(cursor);
}

QWidget* DocumentWidget::createSearchWidget() {
  QWidget* widget = new QWidget(this);
  QHBoxLayout* hBoxLayout = new QHBoxLayout(widget);
  hBoxLayout->addWidget(mWantedSymbolLineEdit);
  hBoxLayout->addWidget(mSearchButton);
  hBoxLayout->setMargin(0);

  return widget;
}

QWidget* DocumentWidget::createEditorWidget() {
  QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
  splitter->addWidget(mTextEdit);
  splitter->addWidget(mBrowser);
  QFont* font = new QFont("Courier New");
  mTextEdit->setFont(*font);
  splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  return splitter;
}

void DocumentWidget::showSearchWidget() {
  if (mSearchWidget->isHidden()) mSearchWidget->show();
  else                           mSearchWidget->hide();
}

QString DocumentWidget::DefaultText() {
  return "<h1> Welcome! </h1>For usual opening files press <b>Ctrl+O</b>.<br>For smart opening <i>(opening only files that contains needed substring)</i> press <b>Ctrl+Shift+O</b>.<br><br> To find substring in files press<b> Ctrl+F</b><br><br>Press <b>F1</b> to show this tab again.<br><br>Good luck!";
  return "";
}

void DocumentWidget::saveFile(bool) {
  QFile file(mFileName);
  if (file.open(QIODevice::WriteOnly )) {
    QByteArray byteArray;
    byteArray.append(mTextEdit->toPlainText());
    file.write(byteArray);
  }
}


