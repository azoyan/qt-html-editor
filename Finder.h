#ifndef FINDER_H
#define FINDER_H
#include <QList>
#include <QString>

class Finder {
public:
using SymbolPositions = QList<int>;
  explicit Finder(const QString& text = QString(), const QString& target = QString());
public:
  void setText(const QString& text);
  int find(const QString& target);
  bool hasFounded() const;
public:
  SymbolPositions symbolPositions() const;
private:
  QString mText;
  SymbolPositions mSymbolPositions;
  bool mHasFounded;
};

#endif // FINDER_H
